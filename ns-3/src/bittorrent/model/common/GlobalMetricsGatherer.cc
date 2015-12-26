/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2013 ComSys, RWTH Aachen University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Rene Glebke
 */

#include "GlobalMetricsGatherer.h"

#include "ns3/BitTorrentClient.h"
#include "ns3/BitTorrentPeer.h"

#include "ns3/application-container.h"
#include "ns3/mpi-interface.h"
#include "ns3/simulator.h"

#include <sys/stat.h>
#include <fstream>
#include <ios>

namespace ns3 {
namespace bittorrent {

GlobalMetricsGatherer::GlobalMetricsGatherer ()
{
  m_fileNamePrefix = "undefined";
  m_fileOutputEnabled = false;
  m_finishedAppCount = 0;
  m_finishedExternalAppCount = 0;
  m_stopFraction = -1; // -1 indicatates that the GMG shall not stop the simulation at a specific threshold
  m_externalStopFraction = 0;
}

GlobalMetricsGatherer::~GlobalMetricsGatherer ()
{
  m_pieceCount.clear ();
}

void GlobalMetricsGatherer::SetFileNamePrefix (const std::string fileNamePrefix, bool enableLogging)
{
  m_fileNamePrefix = fileNamePrefix;
  m_fileOutputEnabled = enableLogging;
}

std::string GlobalMetricsGatherer::GetFileNamePrefix () const
{
  return m_fileNamePrefix;
}

bool GlobalMetricsGatherer::GetLoggingToFileEnabled () const
{
  return m_fileOutputEnabled;
}

void GlobalMetricsGatherer::RegisterWithApplications (ApplicationContainer appContainer)
{
  for (ApplicationContainer::Iterator it = appContainer.Begin (); it != appContainer.End (); ++it)
    {
      bool alreadyRegistered = false;
      for (ApplicationContainer::Iterator it2 = m_registeredWith.Begin (); it2 != m_registeredWith.End (); ++it2)
        {
          if ((*it) == (*it2))
            {
              alreadyRegistered = true;
              break;
            }
        }

      if (!alreadyRegistered)
        {
          DynamicCast<BitTorrentClient> (*it)->RegisterCallbackCloudConnectionEstablishedEvent (MakeCallback (&GlobalMetricsGatherer::UpdateHealthIndexAppStart, this));
          DynamicCast<BitTorrentClient> (*it)->RegisterCallbackCloudConnectionSuspendedEvent (MakeCallback (&GlobalMetricsGatherer::UpdateHealthIndexAppStop, this));

          DynamicCast<BitTorrentClient> (*it)->RegisterCallbackDownloadCompleteEvent (MakeCallback (&GlobalMetricsGatherer::UpdateFinishedApps, this));
          DynamicCast<BitTorrentClient> (*it)->RegisterCallbackPieceCompleteEvent (MakeCallback (&GlobalMetricsGatherer::UpdateHealthIndexPieceCompleted, this));
          DynamicCast<BitTorrentClient> (*it)->RegisterCallbackPieceRequestedEvent (MakeCallback (&GlobalMetricsGatherer::UpdateDemandUp, this));
          DynamicCast<BitTorrentClient> (*it)->RegisterCallbackPieceCompleteEvent (MakeCallback (&GlobalMetricsGatherer::UpdateDemandDown, this));
          DynamicCast<BitTorrentClient> (*it)->RegisterCallbackPieceCancelledEvent (MakeCallback (&GlobalMetricsGatherer::UpdateDemandDown, this));

          m_registeredWith.Add ((*it));
        }
    }
}

void GlobalMetricsGatherer::AnnounceExternalClients (uint32_t count)
{
  m_externalAppCount += count;
}

void GlobalMetricsGatherer::SetStopFraction (double stopFraction, double externalStopFraction)
{
  if ((stopFraction >= 0 && stopFraction <= 1) || stopFraction == -1)
    {
      m_stopFraction = stopFraction;
    }

  if ((externalStopFraction >= 0 && externalStopFraction <= 1) || externalStopFraction == -1)
    {
      m_externalStopFraction = externalStopFraction;
    }
}

void GlobalMetricsGatherer::WriteToFile (const std::string metricName, const std::string metricString, bool timestamp) const
{
  if (!m_fileOutputEnabled)
    {
      std::cout << metricName;
      if (timestamp)
        {
          std::cout << ": " << Simulator::Now ().GetMilliSeconds () << "ms";
        }
      std::cout << ": " << metricString << std::endl;
      return;
    }
  else
    {
      std::ofstream fileStream;

      fileStream.open (std::string (GetFileNamePrefix () + "-" + metricName + ".dat").c_str (), std::ios_base::app);

      // Fallback for a non-ready stream
      if (!fileStream.is_open () || !fileStream.good ())
        {
          std::cout << metricName;
          if (timestamp)
            {
              std::cout << ": " << Simulator::Now ().GetMilliSeconds () << "ms";
            }
          std::cout << ": " << metricString << std::endl;
          return;
        }

      // Default output
      if (timestamp)
        {
          fileStream << Simulator::Now ().GetMilliSeconds () << "ms: ";
        }
      fileStream << metricString << "\n";

      fileStream.close ();
    }
}

std::string GlobalMetricsGatherer::GetWallclockTime ()
{
  time_t timeStamp;
  timeStamp = time (0);

  tm *currentTime;
  currentTime = localtime (&timeStamp);

  std::stringstream ss;
  ss << currentTime->tm_mday << '.' << currentTime->tm_mon + 1 << '.'
     << currentTime->tm_year + 1900 << " - " << currentTime->tm_hour
     << ':' << currentTime->tm_min << ':' << currentTime->tm_sec;

  return ss.str ();
}


void GlobalMetricsGatherer::UpdateFinishedApps ()
{
  if (m_stopFraction >= 0)
    {
      if (++m_finishedAppCount >= m_stopFraction * m_registeredWith.GetN ())
        {
#ifdef NS3_MPI
          /*
           * In case we use MPI, we have no global state in our simulation.
           * Hence, we create a file for each finished sub process and check
           * if files for all for all processes exist. If they do, we can
           * terminate the simulation after removing all files again.
           */
          std::string firstPart = std::string (GetFileNamePrefix () + ".");
          std::string lastPart = ".finished.dat";

          FILE* pFinishedFile = fopen ((firstPart + lexical_cast <std::string> (MpiInterface::GetSystemId ()) + lastPart).c_str (), "w");
          fclose (pFinishedFile);

          struct stat fileStat;
          for (uint32_t i = 0; i < MpiInterface::GetSize (); ++i)
            {
              if (stat ((firstPart + lexical_cast <std::string> (i) + lastPart).c_str (), &fileStat) != 0)
              {
                  return;
              }
            }

          for (uint32_t i = 0; i < MpiInterface::GetSize (); ++i)
            {
              remove ((firstPart + lexical_cast <std::string> (i) + lastPart).c_str ());
            }
#endif

          if (m_externalStopFraction == -1 || (m_finishedExternalAppCount >= m_externalStopFraction * m_externalAppCount))
            {
              WriteToFile ("simulation-stopped", GetWallclockTime (), false);
              Simulator::Stop (Seconds (1));
            }
        }
    }
}

void GlobalMetricsGatherer::AnnounceFinishedExternalClient ()
{
  if (m_externalStopFraction >= 0)
    {
      if (++m_finishedExternalAppCount >= m_externalStopFraction * m_externalAppCount)
        {
          if (m_stopFraction == -1 || (m_finishedAppCount >= m_stopFraction * m_registeredWith.GetN ()))
            {
              WriteToFile ("simulation-stopped", GetWallclockTime (), false);
              Simulator::Stop (Seconds (1));
            }
        }
    }
}

void GlobalMetricsGatherer::UpdateHealthIndexAppStart (Ptr<BitTorrentClient> client)
{
  const std::vector<uint8_t> clientBitfield = *(client->GetBitfield ());

  for (uint32_t current = 0; current < client->GetTorrent ()->GetBitfieldSize (); ++current)
    {
      uint8_t currentByte = clientBitfield[current];
      for (uint8_t bit = 0; bit < 8; ++bit)
        {
          if (currentByte & (1 << bit))
            {
              m_pieceCount[static_cast<uint32_t> ((current << 3) + bit)] = m_pieceCount[static_cast<uint32_t> ((current << 3) + bit)] + 1;
            }
        }
    }
}

void GlobalMetricsGatherer::UpdateHealthIndexAppStop (Ptr<BitTorrentClient> client)
{
  const std::vector<uint8_t> clientBitfield = *(client->GetBitfield ());

  for (uint32_t current = 0; current < client->GetTorrent ()->GetBitfieldSize (); ++current)
    {
      uint8_t currentByte = clientBitfield[current];
      for (uint8_t bit = 0; bit < 8; ++bit)
        {
          if (currentByte & (1 << bit))
            {
              m_pieceCount[static_cast<uint32_t> ((current << 3) + bit)] = std::max (static_cast<uint32_t> (0), m_pieceCount[static_cast<uint32_t> ((current << 3) + bit)] - 1);
            }
        }
    }
}

void GlobalMetricsGatherer::UpdateHealthIndexPieceCompleted (Ptr<Peer> peer, uint32_t pieceIndex)
{
  m_pieceCount[pieceIndex] = m_pieceCount[pieceIndex] + 1;
}

void GlobalMetricsGatherer::UpdateDemandUp (Ptr<Peer> peer, uint32_t pieceIndex)
{
  m_pieceDemand[pieceIndex] = m_pieceDemand[pieceIndex] + 1;
}

void GlobalMetricsGatherer::UpdateDemandDown (Ptr<Peer> peer, uint32_t pieceIndex)
{
  m_pieceDemand[pieceIndex] = std::max (static_cast<uint32_t> (0), m_pieceDemand[pieceIndex] - 1);
}

const std::map<uint32_t, uint32_t>& GlobalMetricsGatherer::GetPieceCount () const
{
  return m_pieceCount;
}

const std::map<uint32_t, uint32_t>& GlobalMetricsGatherer::GetPieceDemand () const
{
  return m_pieceDemand;
}

} // ns bittorrent
} // ns ns3
