/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 ComSys, RWTH Aachen University
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

#include "BitTorrentVideoMetricsBase.h"

#include "BitTorrentVideoClient.h"
#include "BitTorrentPeer.h"

#include "ns3/pointer.h"
#include "ns3/simulator.h"

#include <map>

namespace ns3 {
namespace bittorrent {

BitTorrentVideoMetricsBase::BitTorrentVideoMetricsBase (Ptr<BitTorrentClient> myClient) : AbstractStrategy (myClient)
{
  m_myVideoClient = DynamicCast<BitTorrentVideoClient> (myClient);
}

BitTorrentVideoMetricsBase::~BitTorrentVideoMetricsBase ()
{

}

void BitTorrentVideoMetricsBase::DoInitialize ()
{
  // Register for the different events the client issues
  m_myVideoClient->RegisterCallbackGatherMetricsEvent (MakeCallback (&BitTorrentVideoMetricsBase::ReturnPeriodicMetrics, this));

  m_myVideoClient->RegisterCallbackPlaybackPositionWillChangePeriodicallyEvent (MakeCallback (&BitTorrentVideoMetricsBase::ProcessPlaybackPositionWillChangePeriodicallyEvent, this));
  m_myVideoClient->RegisterCallbackPlaybackPositionChangedEvent (MakeCallback (&BitTorrentVideoMetricsBase::ProcessPlaybackPositionChangedEvent, this));
  m_myVideoClient->RegisterCallbackPlaybackStateChangedEvent (MakeCallback (&BitTorrentVideoMetricsBase::ProcessPlaybackStateChangedEvent, this));

  // Initialize the fluency metric
  m_bufferStart = m_bufferEnd = Simulator::Now ();
  m_playbackStart = m_playbackEnd = MilliSeconds (0);
  m_fluencyNumerator = m_fluencyDenominator = MilliSeconds (0);
}

void BitTorrentVideoMetricsBase::ProcessPlaybackPositionWillChangePeriodicallyEvent ()
{
  m_periodicPositionChangeAnnounced = true;
}

void BitTorrentVideoMetricsBase::ProcessPlaybackPositionChangedEvent (Time newPosition)
{
  // Process the fluency metric
  if (!m_periodicPositionChangeAnnounced)
    {
      m_playbackStart = m_myVideoClient->GetPlaybackPosition ();
    }

  // Reset the announcement of a periodic position change, if so announced (since this method handled that change)
  m_periodicPositionChangeAnnounced = false;
}

void BitTorrentVideoMetricsBase::ProcessPlaybackStateChangedEvent ()
{
  if (m_myVideoClient->IsPlaying ())
    {
      // Process the fluency metric
      if (!m_myVideoClient->IsPaused ())
        {
          m_bufferEnd = Simulator::Now ();
          m_playbackStart = m_myVideoClient->GetPlaybackPosition ();
        }
      else
        {
          m_bufferStart = Simulator::Now ();
          m_playbackEnd = m_myVideoClient->GetPlaybackPosition ();

          m_fluencyNumerator += ((m_playbackEnd - m_playbackStart) - (m_bufferEnd - m_bufferStart));
          m_fluencyDenominator += (m_playbackEnd - m_playbackStart);
        }
    }
}

std::map<std::string, std::string> BitTorrentVideoMetricsBase::ReturnPeriodicMetrics ()
{
  // Step 1: Prepare the result
  std::map<std::string, std::string> result;

  // Step 2: Add the fluency metric
  if (!m_fluencyDenominator.IsZero ())
    {
      result["fluency"] = lexical_cast<std::string> (m_fluencyNumerator.GetMilliSeconds () / m_fluencyDenominator.GetMilliSeconds ());
    }
  else
    {
      result["fluency"] = "div/0";
    }

  // Last step: Return the result
  return result;
}

} // ns bittorrent
} // ns ns3
