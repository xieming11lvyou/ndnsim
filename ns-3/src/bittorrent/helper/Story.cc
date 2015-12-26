/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2014 ComSys, RWTH Aachen University
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
 * Authors: Rene Glebke (principal author), Alexander Hocks
 */

#include "Story.h"

#include "brite-topology-helper.h"

#include "ns3/BitTorrentClient.h"
#include "ns3/BitTorrentVideoClient.h"
#include "ns3/BitTorrentTracker.h"
#include "ns3/BitTorrentUtilities.h"
#include "ns3/StorageManager.h"

#include "ns3/abort.h"
#include "ns3/double.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/mpi-interface.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/nstime.h"
#include "ns3/on-off-helper.h"
#include "ns3/pointer.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/random-variable.h"
#include "ns3/random-variable-stream.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"

#include <cstdarg>
#include <cmath>
#include <ctime>
#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>

namespace ns3 {
namespace bittorrent {

#ifdef NS3_MPI
#define SCHEDULE_CHAPTER_NOARGS(function, type) \
  { \
    UniformVariable uv; \
    for (NodeContainer::Iterator it = affectedNodes.Begin (); it != affectedNodes.End (); ++it) \
      { \
        if (PeekPointer (*it)->GetSystemId () == MpiInterface::GetSystemId ()) \
          { \
            Simulator::Schedule (MilliSeconds (time.GetMilliSeconds () + uv.GetInteger (0, time2.GetMilliSeconds () - time.GetMilliSeconds ())), function, dynamic_cast<type*> (PeekPointer ((*it)->GetApplication (0)))); \
          } \
      } \
  }
#else
#define SCHEDULE_CHAPTER_NOARGS(function, type) \
  { \
    UniformVariable uv; \
    for (NodeContainer::Iterator it = affectedNodes.Begin (); it != affectedNodes.End (); ++it) \
      { \
        Simulator::Schedule (MilliSeconds (time.GetMilliSeconds () + uv.GetInteger (0, time2.GetMilliSeconds () - time.GetMilliSeconds ())), function, dynamic_cast<type*> (PeekPointer ((*it)->GetApplication (0)))); \
      } \
  }
#endif

#ifdef NS3_MPI
#define SCHEDULE_CHAPTER(function, type, ...) \
  { \
    UniformVariable uv; \
    for (NodeContainer::Iterator it = affectedNodes.Begin (); it != affectedNodes.End (); ++it) \
      { \
        if (PeekPointer (*it)->GetSystemId () == MpiInterface::GetSystemId ()) \
          { \
            Simulator::Schedule (MilliSeconds (time.GetMilliSeconds () + uv.GetInteger (0, time2.GetMilliSeconds () - time.GetMilliSeconds ())), function, dynamic_cast<type*> (PeekPointer ((*it)->GetApplication (0))), __VA_ARGS__); \
          } \
      } \
  }
#else
#define SCHEDULE_CHAPTER(function, type, ...) \
  { \
    UniformVariable uv; \
    for (NodeContainer::Iterator it = affectedNodes.Begin (); it != affectedNodes.End (); ++it) \
      { \
        Simulator::Schedule (MilliSeconds (time.GetMilliSeconds () + uv.GetInteger (0, time2.GetMilliSeconds () - time.GetMilliSeconds ())), function, dynamic_cast<type*> (PeekPointer ((*it)->GetApplication (0))), __VA_ARGS__); \
      } \
  }
#endif

#ifdef NS3_MPI
#define CALL_FUNCTION(function, type, ...) \
  { \
    for (NodeContainer::Iterator it = affectedNodes.Begin (); it != affectedNodes.End (); ++it) \
      { \
        if (PeekPointer (*it)->GetSystemId () == MpiInterface::GetSystemId ()) \
          { \
            dynamic_cast<type*> (PeekPointer ((*it)->GetApplication (0)))->function (__VA_ARGS__); \
          } \
      } \
  }
#else
#define CALL_FUNCTION(function, type, ...) \
  { \
    for (NodeContainer::Iterator it = affectedNodes.Begin (); it != affectedNodes.End (); ++it) \
      { \
        dynamic_cast<type*> (PeekPointer ((*it)->GetApplication (0)))->function (__VA_ARGS__); \
      } \
  }
#endif

Story::Story ()
{
  m_trackerAdded = false;
  m_randomSeedSet = false;
  m_btNodeCount = 0;
  m_otherNodeCount = 0;
  m_loggingToFile = false;
  m_checkData = false;
}

Story::~Story ()
{
}

std::string Story::GetSimulationId () const
{
  return m_simulationId;
}

bool Story::GetLoggingToFile () const
{
  return m_loggingToFile;
}

bool Story::GetUseFakeData () const
{
  return m_useFakeData;
}

bool Story::GetCheckData () const
{
  return m_checkData;
}

uint32_t Story::GetBTNodeCount () const
{
  return m_btNodeCount;
}

uint32_t Story::GetOtherNodeCount () const
{
  return m_otherNodeCount;
}

std::string Story::GetTorrentFolder ()   const
{
  return m_torrentFolder;
}

std::string Story::GetTorrentFile () const
{
  return m_torrentFile;
}

std::string Story::GetBandwidthSamplesFile () const
{
  return m_bandwidthSamplesFile;
}

std::string Story::GetReplacement (std::string variableName)
{
  return m_variables[variableName];
}

void Story::SetReplacement (std::string variableName, std::string value)
{
  m_variables[variableName] = value;
}

void Story::ParseReplacements (std::string toParse)
{
  toParse += "/";

  size_t colonPos = toParse.find (":");
  size_t slashPos = 0;

  while (colonPos != std::string::npos)
    {
      size_t newslashPos = toParse.find ("/", slashPos + 1);
      if (newslashPos != std::string::npos)
        {
          m_variables[toParse.substr (slashPos, colonPos - slashPos)] = toParse.substr (colonPos + 1, newslashPos - colonPos - 1);
        }
      slashPos = newslashPos + 1;
      colonPos = toParse.find (":", slashPos + 1);
    }
}

bool Story::GetTrackerAdded () const
{
  return m_trackerAdded;
}

bool Story::GetRandomSeedSet () const
{
  return m_randomSeedSet;
}

void Story::ParseVideoTime (std::istringstream& lineBuffer, int32_t& result, bool& isRelative, uint32_t currentLine = 0)
{
  std::string buffer;
  lineBuffer >> buffer;

  int8_t prefix = 1;
  isRelative = false;

  if (buffer[0] == '+')
    {
      prefix = 1;
      isRelative = true;
    }
  else if (buffer[0] == '-')
    {
      prefix = -1;
      isRelative = true;
    }

  size_t hPos, mPos, sPos, msPos;
  hPos = buffer.find ('h');
  mPos = buffer.find ('m');
  sPos = buffer.find ('s');
  msPos = buffer.find ('i');

  if (hPos == std::string::npos || mPos == std::string::npos || sPos == std::string::npos)
    {
      NS_ABORT_MSG ("[line " << currentLine << "] Error: Missing either \"h\" or \"m\" or \"s\" in time arguments.");
    }

  uint32_t h, m, s, ms;

  if (isRelative)
    {
      h = lexical_cast<uint32_t> (buffer.substr (1, hPos - 1));
    }
  else
    {
      h = lexical_cast<uint32_t> (buffer.substr (0, hPos));
    }

  m = lexical_cast<uint32_t> (buffer.substr (hPos + 1, mPos - hPos - 1));
  s = lexical_cast<uint32_t> (buffer.substr (mPos + 1, sPos - mPos - 1));

  if (msPos != std::string::npos)
    {
      ms = lexical_cast<uint32_t> (buffer.substr (sPos + 1, msPos - sPos - 1));
    }
  else
    {
      ms = 0;
    }

  result = prefix * (ms + 1000 * s + 60000 * m + 3600000 * h);
}

void Story::ReadAndScheduleStory (std::string filePath, uint32_t simulationDuration)
{
  std::ifstream storyFile;
  std::istringstream lineBuffer;
  std::string line;

  storyFile.open (filePath.c_str ());
  if (!storyFile.is_open ())
    {
      NS_ABORT_MSG ("Error: Story file not found.");
    }

  std::cout << "This is the VODSim Story reader. Processing file " << filePath << "." << std::endl;

  std::string buffer, buffer2, buffer3;

  Time oldTime = FemtoSeconds (0);
  uint32_t currentLine = 0;

  m_randomSeedSet = false;
  m_trackerAdded = (m_btTracker->GetN () > 0);

  while (!storyFile.eof ())
    {
      // Step 0: Read in next line and replace variables -->
      ++currentLine;
      getline (storyFile, line);      

      for (std::map<std::string, std::string>::const_iterator it = m_variables.begin (); it != m_variables.end (); ++it)
        {
          std::string variable = "$" + (*it).first + "$";
          size_t position;
          do
            {
              position = line.find (variable);
              if (position != std::string::npos)
                {
                  line.erase (position, variable.length ());
                  line.insert (position, m_variables[(*it).first]);
                }
            }
          while (position != std::string::npos);
        }

      lineBuffer.clear ();
      lineBuffer.str (line);
      if (lineBuffer.str () == "" || (lineBuffer.str ()[0] == '/' && lineBuffer.str ()[1] == '/'))
        {
          continue;
        }

      // <-- Step 0: Read in next line and replace variables

      // Step 1: Process the time of the event -->

      Time time, time2;
      bool poisson = false;
      Time poissonInterarrivalTime;

      if (lineBuffer.str ()[0] == 'p')
        {
          poisson = true;
        }

      if (poisson || lineBuffer.str ().substr (0, 4) == "from")
        {
          lineBuffer >> buffer;
          if (poisson && buffer != "poisson")
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: A poisson process declaration must begin with the \"poisson from\" keyword.");
            }
          if (poisson)
            {
              lineBuffer >> buffer;
            }
          if (buffer != "from")
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: A time range declaration must begin with the \"from\" keyword.");
            }

          int32_t eventTime;
          bool dummy;

          ParseVideoTime (lineBuffer, eventTime, dummy, currentLine);
          time = MilliSeconds (eventTime);

          if (time < oldTime)
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: You are scheduling an event that takes place before already scheduled events. That's not allowed.");
            }

          lineBuffer >> buffer;

          if (buffer != "till" && buffer != "until")
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: When defining a time range, you must provide the end of the range following the statements \"till\" or \"until\".");
            }

          ParseVideoTime (lineBuffer, eventTime, dummy, currentLine);
          time2 = MilliSeconds (eventTime);

          if (time2 < time)
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: When defining a time range, the upper bound of the range must actually be an UPPER bound.");
            }

          if (poisson)
            {
              lineBuffer >> buffer;

              if (buffer != "interarrival")
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: When defining a poisson process, you must provide the mean interarrival time following the statement \"interarrival\".");
                }

              ParseVideoTime (lineBuffer, eventTime, dummy, currentLine);
              poissonInterarrivalTime = MilliSeconds (eventTime);

              if (poissonInterarrivalTime > (time2 - time) || poissonInterarrivalTime.IsNegative ())
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: When defining a poisson process, the interarrival time must be positive and not be larger than the difference between the start and end time of the process.");
                }
            }
        }
      else
        {
          int32_t eventTime;
          bool dummy;
          ParseVideoTime (lineBuffer, eventTime, dummy, currentLine);

          time = MilliSeconds (eventTime);
          time2 = time;

          if (time < oldTime)
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: You are scheduling an event that takes place before already scheduled events. That's not allowed.");
            }
        }

      oldTime = time;

      if (time.GetMilliSeconds () == time2.GetMilliSeconds ())
        {
          std::cout << "At " << time.GetMilliSeconds () << " msec:" << std::endl;
        }
      else if (!poisson)
        {
          std::cout << "Between " << time.GetMilliSeconds () << " and " << time2.GetMilliSeconds () << " msec at a random point in time: " << std::endl;
        }
      else if (poisson)
        {
          std::cout << "Between " << time.GetMilliSeconds () << " and " << time2.GetMilliSeconds () << " msec following a poisson process with an interarrival time of " << poissonInterarrivalTime.GetMilliSeconds () << " msec: " << std::endl;
        }

      // <-- Step 1: Process the time of the event

      // Step 2: Process the target of the event -->

      std::string target;
      uint32_t clientNrStart = 0;
      uint32_t clientNrEnd = 0;
      bool tracker = false;
      bool client = false;
      bool topology = false;
      bool simulation = false;
      bool random = false;
      NodeContainer affectedNodes;

      lineBuffer >> target;

      if (target == "simulation")
        {
          simulation = true;
        }
      if (target == "topology")
        {
          topology = true;
        }
      else if (target == "tracker")
        {
          tracker = true;
        }
      else if (target == "client")
        {
          client = true;
          lineBuffer >> clientNrStart;
          clientNrEnd = clientNrStart;
          if (clientNrEnd > m_btNodes->GetN ())
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: A node with such an index is not available.");
            }
          std::cout << "	For client "<< clientNrStart << " (" << m_btNodes->Get (clientNrStart - 1)->GetId () << "):" << std::endl;
        }
      else if (target == "clients")
        {
          client = true;
          lineBuffer >> buffer;
          if (buffer == "max")
            {
              clientNrStart = m_btNodes->GetN ();
            }
          else
            {
              clientNrStart = lexical_cast<uint32_t> (buffer);
            }
          lineBuffer >> buffer;
          lineBuffer >> buffer;
          if (buffer == "max")
            {
              clientNrEnd = m_btNodes->GetN ();
            }
          else
            {
              clientNrEnd = lexical_cast<uint32_t> (buffer);
              if (clientNrEnd > m_btNodes->GetN ())
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: More nodes requested than available.");
                }
            }
          std::cout << "	For clients "<< clientNrStart << " (" << m_btNodes->Get (clientNrStart - 1)->GetId () << ") - " << clientNrEnd << " (" << m_btNodes->Get (clientNrEnd - 1)->GetId () << "):" << std::endl;

        }
      else if (target == "all")
        {
          lineBuffer >> target;
          if (target == "clients")
            {
              client = true;
              clientNrStart = 1;
              clientNrEnd = m_btNodes->GetN ();
            }
          else
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: Missing \"clients\" statement behind \"all\".");
            }
          std::cout << "	For all clients:"<< std::endl;

        }
      else if (target == "random")
        {
          random = true;
          lineBuffer >> buffer;
          if (buffer != "clients")
            {
              client = true;
              clientNrStart = 1;
              clientNrEnd = lexical_cast<uint32_t> (buffer);
              if (clientNrEnd > m_btNodes->GetN ())
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: More random nodes requested than available.");
                }
              lineBuffer >> buffer;
              if (buffer != "clients")
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Wrong format for number of random clients.");
                }

              std::cout << "	For "<< clientNrEnd << " random clients:" << std::endl;
            }
        }
      else if (target == "group")
        {
          client = true;
          lineBuffer >> buffer;
          if (m_groups.find (buffer) != m_groups.end ())
            {
              affectedNodes = (*m_groups.find (buffer)).second;
            }
          else
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: Cannot find group.");
            }

          std::cout << "	For group "<< buffer << " (" << affectedNodes.GetN () << " nodes):" << std::endl;
        }

      if (client)
        {
          if (random)
            {
              std::set<uint32_t> indices = Utilities::GetRandomSampleF2 (m_btNodes->GetN (), clientNrEnd);

              for (std::set<uint32_t>::const_iterator indexIt = indices.begin (); indexIt != indices.end (); ++indexIt)
                {
                  affectedNodes.Add (m_btNodes->Get ((*indexIt) - 1));
                }
            }
          else
            {
              for (uint32_t i = clientNrStart - 1; i < clientNrEnd; ++i)
                {
                  affectedNodes.Add (m_btNodes->Get (i));
                }
            }
        }
      else if (tracker)
        {
          affectedNodes.Add (m_btTracker->Get (0));
        }
      // <-- Step 2

      // Step 3: Get and schedule the action to perform

      lineBuffer >> buffer;

      if (client)
        {
          if (buffer == "join")
            {
              lineBuffer >> buffer;
              if (buffer == "group")
                {
                  lineBuffer >> buffer;

                  if (m_groups.find (buffer) == m_groups.end ())
                    {
                      m_groups[buffer] = affectedNodes;
                    }
                  else
                    {
                      m_groups[buffer].Add (affectedNodes);
                    }

                  std::cout << "		Nodes joined group "<< buffer << " which now has " << m_groups[buffer].GetN () << " nodes" << std::endl;
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to join right now...");
                }
            }

          else if (buffer == "leave")
            {
              lineBuffer >> buffer;

              if (buffer == "group")
                {
                  lineBuffer >> buffer;

                  if (m_groups.find (buffer) == m_groups.end ())
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Group to leave does not exist!");
                    }
                  else
                    {
                      NodeContainer newNodes;
                      NodeContainer toDeleteFrom = (*m_groups.find (buffer)).second;
                      for (NodeContainer::Iterator it = toDeleteFrom.Begin (); it != toDeleteFrom.End (); ++it)
                        {
                          bool inNodeContainer = false;
                          for (NodeContainer::Iterator it2 = affectedNodes.Begin (); it2 != affectedNodes.End (); ++it2)
                            {
                              if ((*it2) == (*it))
                                {
                                  inNodeContainer = true;
                                }
                            }
                          if (!inNodeContainer)
                            {
                              newNodes.Add (*it);
                            }
                        }
                      m_groups[buffer] = newNodes;

                      std::cout << "		Scheduled leaving of group "<< buffer << " which now has " << m_groups[buffer].GetN () << " nodes" << std::endl;
                    }
                }
              else if (buffer == "cloud")
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentVideoClient::DisconnectFromCloud, BitTorrentVideoClient)
                  std::cout << "		Scheduled leaving of cloud"<< std::endl;
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to leave right now...");
                }
            }
          else if (buffer == "clean")
            {
              lineBuffer >> buffer;

              if (buffer == "group")
                {
                  lineBuffer >> buffer;

                  if (m_groups.find (buffer) == m_groups.end ())
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Group to clean does not exist!");
                    }
                  else
                    {
                      NodeContainer emptyGroup;
                      m_groups[buffer] = emptyGroup;
                      std::cout << "		Cleared group "<< buffer << std::endl;
                    }
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to clean right now...");
                }
            }
          else if (buffer == "init")
            {
              if (client)
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentVideoClient::StartApplication, BitTorrentVideoClient)
                }
              else
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentTracker::StartApplication, BitTorrentTracker)
                }
              std::cout << "		Scheduled init."<< std::endl;
            }
          else if (buffer == "set")
            {
              lineBuffer >> buffer;

              if (buffer == "peers")
                {
                  uint32_t numwant;
                  lineBuffer >> buffer;
                  if (buffer == "all")
                    {
                      numwant = m_btNodes->GetN ();
                    }
                  else
                    {
                      numwant = lexical_cast<uint32_t> (buffer);
                    }

                  SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetDesiredPeers, BitTorrentVideoClient, numwant)

                  std::cout << "		Set desired peers to "<< numwant << std::endl;
                }
              else if (buffer == "max")
                {
                  lineBuffer >> buffer;

                  if (buffer == "peers")
                    {
                      uint32_t numwant;

                      lineBuffer >> buffer;

                      if (buffer == "all")
                        {
                          numwant = m_btNodes->GetN ();
                        }
                      else
                        {
                          numwant = lexical_cast<uint32_t> (buffer);
                        }

                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetMaxPeers, BitTorrentVideoClient, numwant)

                      std::cout << "		Set maximum allowed peers to "<< buffer << std::endl;
                    }
                }
              else if (buffer == "unchoked")
                {
                  lineBuffer >> buffer;

                  if (buffer == "peers")
                    {
                      uint16_t concurrentUnchokes;

                      lineBuffer >> buffer;

                      if (buffer == "all")
                        {
                          concurrentUnchokes = 0;
                        }
                      else
                        {
                          concurrentUnchokes = lexical_cast<uint16_t> (buffer);
                        }

                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetMaxUnchokedPeers, BitTorrentVideoClient, concurrentUnchokes)

                      std::cout << "		Set maximum unchoked peers to "<< buffer << std::endl;
                    }
                }
              else if (buffer == "autoconnect")
                {
                  lineBuffer >> buffer;

                  bool autoConnect = (buffer == "1");

                  SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetAutoConnect, BitTorrentVideoClient, autoConnect)
                  std::cout << "		Scheduled autoconnect ("<< (buffer == "1") << ")." << std::endl;
                }
              else if (buffer == "autoplay")
                {
                  lineBuffer >> buffer;

                  bool autoPlay = false, autoPlayFromRight = false;

                  if (buffer == "from")
                    {
                      lineBuffer >> buffer;
                      if (buffer == "right")
                        {
                          lineBuffer >> buffer;
                          autoPlayFromRight = (buffer == "1");
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetAutoPlayFromRight, BitTorrentVideoClient, autoPlayFromRight)
                          if (autoPlayFromRight)
                            {
                              SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetAutoPlay, BitTorrentVideoClient, autoPlayFromRight)
                              std::cout << "		Scheduled playback from the rightmost continously reachable position from the beginning when the client starts ("<< (buffer == "1") << ")." << std::endl;
                            }
                        }
                      else
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: Missing \"right\" statement after \"set autoplay from\".");
                        }
                    }
                  else if (buffer == "1" || buffer == "0")
                    {
                      autoPlay = (buffer == "1");
                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetAutoPlay, BitTorrentVideoClient, autoPlay)
                      std::cout << "		Scheduled automatic start of playback when the client starts ("<< (buffer == "1") << ")." << std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know how to set that \"autoplay\" option.");
                    }
                }
              else if (buffer == "leave")
                {
                  lineBuffer >> buffer;

                  if (buffer == "after")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "completed")
                        {
                          int32_t seedingTime;
                          bool voidDummy;
                          ParseVideoTime (lineBuffer, seedingTime, voidDummy, currentLine);

                          CALL_FUNCTION (SetSeedingDuration, BitTorrentVideoClient, MilliSeconds (seedingTime))
                        }
                      else
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know after what I should leave.");
                        }

                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Can only leave \"after\" something happens.");
                    }
                }
              else if (buffer == "strategy" || buffer == "protocol")
                {
                  lineBuffer >> buffer;

                  if (buffer == "options")
                    {
                      std::map<std::string, std::string> arguments;

                      while (!lineBuffer.eof ())
                        {
                          lineBuffer >> buffer;
                          size_t equalSignPos = buffer.find ('=');
                          arguments[buffer.substr (0, equalSignPos)] = buffer.substr (equalSignPos + 1, buffer.size () - equalSignPos - 1);
                        }

                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetStrategyOptions, BitTorrentVideoClient, arguments)

                      std::cout << "		Set strategy options to set of "<< arguments.size () << " options." << std::endl;
                    }
                  else
                    {
                      lineBuffer >> buffer;

                      CALL_FUNCTION (SetProtocol, BitTorrentVideoClient, buffer)

                      std::cout << "		Set protocol to "<< buffer << std::endl;
                    }
                }
              else if (buffer == "initial")
                {
                  lineBuffer >> buffer;

                  if (buffer == "bitfield")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "empty")
                        {
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetInitialBitfield, BitTorrentVideoClient, "empty")
                          std::cout << "		Set initial bitfield to empty."<< std::endl;
                        }
                      else if (buffer == "full")
                        {
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetInitialBitfield, BitTorrentVideoClient, "full")
                          std::cout << "		Set initial bitfield to full."<< std::endl;
                        }
			//KIRIL - for partially complete file at start
                      else if (buffer == "part")
                        {
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetInitialBitfield, BitTorrentVideoClient, "part")
                          std::cout << "		Set initial bitfield to full."<< std::endl;
                        }
                      else if (buffer == "gaussian")
                        {
                          lineBuffer >> buffer;

                          if (buffer != "mean")
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: You must first define the mean (\"mean\") of the gaussian distribution.");
                            }

                          lineBuffer >> buffer;

                          double mean = lexical_cast<double> (buffer);

                          lineBuffer >> buffer;

                          if (buffer != "stddev")
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: After the mean (\"mean\") you must define the standard deviation (\"stddev\") of the gaussian distribution. (NOT the variance!)");
                            }

                          lineBuffer >> buffer;

                          double stdDev = lexical_cast<double> (buffer);

                          bool randomTail = false;

                          if (!lineBuffer.eof ())
                            {
                              lineBuffer >> buffer;

                              if (buffer == "random")
                                {
                                  lineBuffer >> buffer;

                                  if (buffer == "tail")
                                    {
                                      randomTail = true;
                                    }
                                  else
                                    {
                                      NS_ABORT_MSG ("[line " << currentLine << "] Error: The gaussian distribution only allows a \"random tail\".");
                                    }
                                }
                              else
                                {
                                  NS_ABORT_MSG ("[line " << currentLine << "] Error: The gaussian distribution only allows a \"random tail\".");
                                }
                            }

                          NormalVariable gaussians (mean, stdDev * stdDev);

                          for (NodeContainer::Iterator it = affectedNodes.Begin (); it != affectedNodes.End (); ++it)
                            {
                              double randomValue = -1;
                              while (randomValue < 0 || randomValue > 1)
                                {
                                  randomValue = gaussians.GetValue ();
                                }

                              if (!randomTail)
                                {
                                  Simulator::Schedule (time, &BitTorrentVideoClient::SetInitialBitfield, dynamic_cast<BitTorrentVideoClient*> (PeekPointer ((*it)->GetApplication (0))), "p" + lexical_cast<std::string> (randomValue));
                                  std::cout << "		Node "<< (*it)->GetId () << " has " << randomValue * 100 << "% bitfield filling from the beginning." << std::endl;
                                }
                              else
                                {
                                  Simulator::Schedule (time, &BitTorrentVideoClient::SetInitialBitfield, dynamic_cast<BitTorrentVideoClient*> (PeekPointer ((*it)->GetApplication (0))), "q" + lexical_cast<std::string> (randomValue));
                                  std::cout << "		Node "<< (*it)->GetId () << " has " << randomValue * 100 << "% bitfield filling from the beginning, with a random tail for the following pieces." << std::endl;
                                }
                            }
                        }
                      else if (buffer == "from")
                        {
                          lineBuffer >> buffer;

                          if (buffer != "left")
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: Missing \"left\" after \"from\".");
                            }

                          lineBuffer >> buffer;

                          double percentage = lexical_cast<double> (buffer);

                          if (percentage < 0 || percentage > 1)
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: The filling percentage must be given as a value in the range [0.0, 1.0].");
                            }

                          bool randomTail = false;

                          if (!lineBuffer.eof ())
                            {
                              lineBuffer >> buffer;

                              if (buffer == "random")
                                {
                                  lineBuffer >> buffer;

                                  if (buffer == "tail")
                                    {
                                      randomTail = true;
                                    }
                                  else
                                    {
                                      NS_ABORT_MSG ("[line " << currentLine << "] Error: A filling from the left only allows a \"random tail\".");
                                    }
                                }
                              else
                                {
                                  NS_ABORT_MSG ("[line " << currentLine << "] Error: A filling from the left only allows a \"random tail\".");
                                }
                            }

                          if (!randomTail)
                            {
                              SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetInitialBitfield, BitTorrentVideoClient, "p" + lexical_cast<std::string> (percentage))
                              std::cout << "		Set initial bitfield to "<< percentage * 100 << "% filling from the beginning." << std::endl;
                            }
                          else
                            {
                              SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetInitialBitfield, BitTorrentVideoClient, "q" + lexical_cast<std::string> (percentage))
                              std::cout << "		Set initial bitfield to "<< percentage * 100 << "% filling from the beginning, with a random tail for the following pieces." << std::endl;
                            }
                        }
                      else if (buffer == "debug")
                        {
                          lineBuffer >> buffer;
                          uint32_t bitfieldIndex = lexical_cast<uint32_t> (buffer);
                          lineBuffer >> buffer;
                          uint32_t value = lexical_cast<uint32_t> (buffer);
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::ManipulateInitialBitfield, BitTorrentVideoClient, bitfieldIndex, value)
                          std::cout << "		Set bitfield index "<< bitfieldIndex << " to value " << value << " for debugging purposes." << std::endl;
                        }
                      else
                        {
                          UniformVariable uv;
                          
                          buffer = lexical_cast<std::string> (uv.GetValue (0, 1) );
                          
                          if(!lineBuffer.eof ())
                            lineBuffer >> buffer;
                            
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetInitialBitfield, BitTorrentVideoClient, "random " + buffer)
                          
                          std::cout << "		Set initial bitfield to random, percentage: " << buffer << "." << std::endl;
                        }
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what initial thing to set for peers.");
                    }
                }
              else if (buffer == "block")
                {
                  lineBuffer >> buffer;

                  if (buffer == "size")
                    {
                      lineBuffer >> buffer;

                      bool send = false;
                      if (buffer == "send")
                        {
                          send = true;
                        }
                      else if (buffer != "request")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to set for block sizes.");
                        }

                      uint32_t blockSize;
                      lineBuffer >> buffer;
                      blockSize = lexical_cast<uint32_t> (buffer);

                      if (send)
                        {
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetSendBlockSize, BitTorrentVideoClient, blockSize)
                          std::cout << "		Set transmission block size to "<< blockSize << " bytes." << std::endl;
                        }
                      else
                        {
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetRequestBlockSize, BitTorrentVideoClient, blockSize)
                          std::cout << "		Set request block size to "<< blockSize << " bytes." << std::endl;
                        }
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to set for block requests.");
                    }
                }
              else if (buffer == "piece")
                {
                  lineBuffer >> buffer;

                  if (buffer == "timeout")
                    {
                      int32_t blockTimeout;
                      bool voidDummy;
                      ParseVideoTime (lineBuffer, blockTimeout, voidDummy, currentLine);

                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetPieceTimeout, BitTorrentVideoClient, MilliSeconds (blockTimeout))
                      std::cout << "		Set piece timeout to "<< blockTimeout << " milliseconds." << std::endl;
                    }
                  else if (buffer == "max")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "requests")
                        {
                          lineBuffer >> buffer;

                          uint32_t maxPieceRequests;
                          maxPieceRequests = lexical_cast<uint32_t> (buffer);

                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetMaxRequestsPerPiece, BitTorrentVideoClient, maxPieceRequests)
                          std::cout << "		Set number of maximum concurrent requests per piece to "<< maxPieceRequests << "." << std::endl;
                        }
                      else
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what max thing to set for piece requests.");
                        }
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to set for piece requests.");
                    }
                }
              else if (buffer == "concurrent")
                {
                  lineBuffer >> buffer;

                  if (buffer == "requests")
                    {
                      lineBuffer >> buffer;

                      uint32_t maxPeerRequests;
                      maxPeerRequests = lexical_cast<uint32_t> (buffer);

                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetMaxRequestsPerPeer, BitTorrentVideoClient, maxPeerRequests)
                      std::cout << "		Set number of maximum concurrent requests per peer (in pieces) to "<< maxPeerRequests << "." << std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to set for concurrent things.");
                    }
                }
              else if (buffer == "video")
                {
                  lineBuffer >> buffer;

                  if (buffer == "prebuffering")
                    {
                      int32_t preBufferingTime;
                      bool voidDummy;
                      ParseVideoTime (lineBuffer, preBufferingTime, voidDummy, currentLine);

                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetPreBufferingTime, BitTorrentVideoClient, MilliSeconds (preBufferingTime))

                      std::cout << "		Set pre-buffering time to "<< preBufferingTime << " milliseconds." << std::endl;
                    }
                  else if (buffer == "skip")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "active")
                        {
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetPiecesMissable, BitTorrentVideoClient, true)

                          std::cout << "		Set pieces missable during playback, subject to the further settings (\"tolerance\" and \"afterwards\")."<< std::endl;

                        }
                      else if (buffer == "inactive")
                        {
                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetPiecesMissable, BitTorrentVideoClient, false)

                          std::cout << "		Set pieces missable during playback, subject to the further settings (\"tolerance\" and \"afterwards\")."<< std::endl;
                        }
                      else if (buffer == "tolerance")
                        {
                          int32_t skipTolerance;
                          bool voidDummy;
                          ParseVideoTime (lineBuffer, skipTolerance, voidDummy, currentLine);

                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetSkipTolerance, BitTorrentVideoClient, MilliSeconds (skipTolerance))

                          std::cout << "		Set maximum tolerance for missing pieces to skip before onset of a buffering period to "<< skipTolerance << " milliseconds." << std::endl;
                        }
                      else if (buffer == "afterwards")
                        {
                          int32_t skipTolerance;
                          bool voidDummy;
                          ParseVideoTime (lineBuffer, skipTolerance, voidDummy, currentLine);

                          SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetRequiredContinousPlaybackAfterSkip, BitTorrentVideoClient, MilliSeconds (skipTolerance))

                          std::cout << "		Set minimum length of required uninterrupted possible playback after a buffering period because of missing pieces to "<< skipTolerance << " milliseconds." << std::endl;
                        }
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know how to set that for the video plaback.");
                    }
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know how to set that for the client.");
                }
            }
          else if (buffer == "connect")
            {
              uint32_t connectCount;
              lineBuffer >> buffer;

              if (buffer == "max")
                {
                  connectCount = 0;                       // Will force PeerConnectorStrategyBase to connect to max. numwant peers
                }
              else
                {
                  connectCount = lexical_cast<uint32_t> (buffer);
                }

              SCHEDULE_CHAPTER (&BitTorrentVideoClient::TriggerCallbackConnectToPeers, BitTorrentVideoClient, connectCount)

              if (buffer == "max")
                {
                  std::cout << "		Scheduled connections to the maximum possible clients."<< std::endl;
                }
              else
                {
                  std::cout << "		Scheduled connections to "<< connectCount << "clients." << std::endl;
                }
            }
          else if (buffer == "directconnect")
            {
              std::string address_str;
              std::string port;

              lineBuffer >> buffer;
              lineBuffer >> buffer2;

              if (buffer != "to" || buffer2 != "ip")
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Can only \"directconnect to\" ip <IPAddress> port <Port>.");
                }

              lineBuffer >> address_str;

              lineBuffer >> buffer;

              if (buffer != "port")
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Can only \"directconnect to\" ip <IPAddress>.");
                }

              lineBuffer >> port;

              Ipv4Address address (address_str.c_str ());

              SCHEDULE_CHAPTER (&BitTorrentVideoClient::TriggerCallbackConnectToPeer, BitTorrentVideoClient, address, lexical_cast<uint16_t> (port));

              std::cout << "		Scheduled connection to peer with address "<< address_str << ", port " << port << "." << std::endl;
            }
          else if (buffer == "disconnect")
            {
              int32_t disconnectCount;
              lineBuffer >> buffer;

              if (buffer == "all")
                {
                  disconnectCount = -1;                       // TODO: Make a function that disconnects from 0 <= x <= (Connection count) peers
                }
              else
                {
                  disconnectCount = lexical_cast<uint32_t> (buffer);
                }

              SCHEDULE_CHAPTER (&BitTorrentVideoClient::TriggerCallbackDisconnectPeers, BitTorrentVideoClient, disconnectCount)

              std::cout << "		Scheduled disconnection of"<< disconnectCount << "clients" << std::endl;

            }
          else if (buffer == "rejoin")
            {
              lineBuffer >> buffer;

              if (buffer == "cloud")
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentVideoClient::JoinCloud, BitTorrentVideoClient)
                  std::cout << "		Scheduled rejoining of cloud"<< std::endl;
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to join right now...");
                }
            }
          else if (buffer == "video")
            {
              lineBuffer >> buffer;

              if (buffer == "play")
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentVideoClient::Play, BitTorrentVideoClient)
                  std::cout << "		Scheduled video play."<< std::endl;
                }
              else if (buffer == "pause")
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentVideoClient::Pause, BitTorrentVideoClient)
                  std::cout << "		Scheduled pausing of video if no buffering takes place at this moment."<< std::endl;
                }
              else if (buffer == "unpause")
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentVideoClient::Pause, BitTorrentVideoClient)
                  std::cout << "		Scheduled unpausing of video if no buffering takes place at this moment."<< std::endl;
                }
              else if (buffer == "stop")
                {
                  SCHEDULE_CHAPTER_NOARGS (&BitTorrentVideoClient::Stop, BitTorrentVideoClient)
                  std::cout << "		Scheduled video stop."<< std::endl;
                }
              else if (buffer == "ff")
                {
                  int32_t newTime;
                  bool isRelative;
                  ParseVideoTime (lineBuffer, newTime, isRelative, currentLine);

                  if (isRelative)
                    {
                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetPlaybackPositionRelative, BitTorrentVideoClient, MilliSeconds (newTime))
                    }
                  else
                    {
                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetPlaybackPosition, BitTorrentVideoClient, MilliSeconds (newTime))
                    }

                  std::cout << "		Scheduled video fast forward "<< (isRelative ? "for " : "to ") << newTime << "milliseconds" << std::endl;
                }
              else if (buffer == "skip")
                {
                  lineBuffer >> buffer;

                  if (buffer == "left")
                    {
                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetPlaybackPositionToTheRight, BitTorrentVideoClient, true)

                      std::cout << "		Scheduled video fast forward to rightmost continously reachable position from the beginning."<< std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to skip.");
                    }
                }
              else if (buffer == "buffer")
                {
                  lineBuffer >> buffer;

                  uint8_t bufferingType = 0;
                  if (buffer == "for")
                    {
                      bufferingType = 0;
                    }
                  else if (buffer == "next")
                    {
                      bufferingType = 1;
                    }
                  else if (buffer == "until")
                    {
                      bufferingType = 2;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know how else to manipulate client's buffer behavior.");
                    }

                  int32_t bufferTime;
                  bool voidDummy;
                  ParseVideoTime (lineBuffer, bufferTime, voidDummy, currentLine);

                  if (bufferingType == 0)                      // for
                    {
                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::BufferFor, BitTorrentVideoClient, MilliSeconds (bufferTime))

                      std::cout << "            Scheduled video buffering for the next " << buffer << " ms in \"real time\". " << std::endl;
                    }
                  else if (bufferingType == 1)                      // next
                    {
                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::BufferFor, BitTorrentVideoClient, MilliSeconds (bufferTime))

                      std::cout << "            Scheduled video buffering until the next " << buffer << " ms in the video are available. " << std::endl;
                    }
                  else                       // until
                    {
                      SCHEDULE_CHAPTER (&BitTorrentVideoClient::BufferFor, BitTorrentVideoClient, MilliSeconds (bufferTime))

                      std::cout << "            Scheduled video buffering until the part at " << buffer << " ms in the video is available. " << std::endl;
                    }
                }
              else if (buffer == "metrics")
                {
                  lineBuffer >> buffer;

                  if (buffer != "interval")
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Can only set \"metrics interval\".");
                    }

                  int32_t metricInterval;
                  bool dummy;
                  ParseVideoTime (lineBuffer, metricInterval, dummy, currentLine);

                  SCHEDULE_CHAPTER (&BitTorrentVideoClient::SetGatherMetricsEventPeriodicity, BitTorrentVideoClient, MilliSeconds (metricInterval))

                  std::cout << "                Scheduled metrics to be taken every " << time << " ms." << std::endl;
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to do with the video plaback.");
                }
            }
          else
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to do with the client.");
            }
        }
      else if (tracker)
        {
          if (buffer == "set")
            {
              lineBuffer >> buffer;
              if (buffer == "update")
                {
                  lineBuffer >> buffer;
                  if (buffer == "interval")
                    {
                      int32_t updateInterval;
                      bool voidDummy;
                      ParseVideoTime (lineBuffer, updateInterval, voidDummy, currentLine);

                      SCHEDULE_CHAPTER (&BitTorrentTracker::SetUpdateInterval, BitTorrentTracker, MilliSeconds (updateInterval))
                      std::cout << "		Set tracker update interval to "<< updateInterval << " ms." << std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Missing \"interval\" after \"update\".");
                    }
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know how to set that for the tracker.");
                }
            }
          else
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what else to do with the tracker.");
            }
        }
      else if (topology)
        {
          if (buffer == "set")
            {
              lineBuffer >> buffer;

              if (buffer == "file")
                {
                  lineBuffer >> buffer;

                  m_topologyHelper->SetFileName (buffer);
                  m_routerNodes->Add (m_topologyHelper->Read ());

                  std::cout << "		Read and established topology in file "<< buffer << "." << std::endl;
                }
              else if (buffer == "bandwidth")
                {
                  lineBuffer >> buffer;

                  if (buffer == "samples")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "file")
                        {
                          lineBuffer >> buffer;

                          if (buffer == "none")
                            {
                              m_bandwidthSamplesFile = "";
                              m_topologyHelper->SetBandwidthSamplesFile (m_bandwidthSamplesFile);

                              std::cout << "		Will not use a bandwidth samples file. Using simple DSL distribution as fallback."<< std::endl;
                            }
                          else
                            {
                              m_bandwidthSamplesFile = buffer;
                              m_topologyHelper->SetBandwidthSamplesFile (m_bandwidthSamplesFile);

                              std::cout << "		Set bandwidth samples file to "<< m_bandwidthSamplesFile << "." << std::endl;
                            }
                        }
                    }
                }
              else if (buffer == "delays")
                {
                  uint32_t min = 0;
                  uint32_t max = 0;

                  lineBuffer >> buffer;
                  if (buffer != "min")
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Minimum delay (\"min\") must be provided first.");
                    }
                  lineBuffer >> buffer;
                  min = lexical_cast<uint32_t> (buffer);
                  lineBuffer >> buffer;
                  if (buffer != "max")
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Maximum delay (\"max\") must be provided after minimum delay.");
                    }
                  lineBuffer >> buffer;
                  max = lexical_cast<uint32_t> (buffer);

                  m_topologyHelper->SetClientLinkDelays (min, max);

                  std::cout << "		Set client link delays to a random value between "<< min << "ms and " << max << "ms." << std::endl;
                }
              else if (buffer == "node")
                {
                  lineBuffer >> buffer;

                  if (buffer == "count")
                    {
                      lineBuffer >> buffer;

                      uint32_t newNodes = lexical_cast<uint32_t> (buffer);

                      NodeContainer newNodeContainer = m_topologyHelper->AddClientNodesToLastTopology (newNodes, "point-to-point");

                      for (NodeContainer::Iterator it = newNodeContainer.Begin (); it != newNodeContainer.End (); ++it)
                        {
                          Ptr<Node> node = *it;
                          Ptr<BitTorrentVideoClient> btclient = Create<BitTorrentVideoClient> ();
                          btclient->SetStartTime (Seconds (simulationDuration));                             // So the applications don't start when not ordered to
                          btclient->SetStopTime (Seconds (simulationDuration - 1));
                          btclient->SetCheckDownloadedData (m_checkData);

#ifdef NS3_MPI
                          if (PeekPointer (*it)->GetSystemId () == MpiInterface::GetSystemId ())
#endif
                            {
                              node->AddApplication (btclient);
                              m_clientAppContainer->Add (btclient);
                            }
                        }

                      m_btNodes->Add (newNodeContainer);
                      m_btNodeCount += newNodes;

                      std::cout << "		Added "<< newNodes << " client nodes to the topology." << std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to do with the nodes in the topology.");
                    }
                }
              else if (buffer == "other")
                {
                  lineBuffer >> buffer;

                  if (buffer == "node")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "count")
                        {
                          lineBuffer >> buffer;

                          uint32_t newNodes = lexical_cast<uint32_t> (buffer);

                          NodeContainer newNodeContainer = m_topologyHelper->AddClientNodesToLastTopology (newNodes, "point-to-point");

                          m_otherNodes->Add (newNodeContainer);
                          m_otherNodeCount += newNodes;

                          std::cout << "		Added "<< newNodes << " other nodes to the topology." << std::endl;
                        }
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Can only \"set other node count\" for the topology.");
                    }
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to do with the topology.");
                }
            }
          else if (buffer == "add")
            {
              std::string uplinkSpeed = "", downlinkSpeed, delay;
              lineBuffer >> buffer;

              if (buffer == "tapnode")
                {
                  // Step 1: Get a random router
                  std::map<std::string, Ptr<Node> >::const_iterator routerIt = m_topologyHelper->GetLastRouters ();
                  UniformVariable uv;
                  uint32_t its = uv.GetInteger (0, m_topologyHelper->GetLastRouterCount () - 1);
                  for (uint32_t i = 0; i < its; ++i)
                    {
                      ++routerIt;
                    }

                  lineBuffer >> buffer;

                  // Step 2: Check if a TapDevice name has been specified
                  if (buffer != "tapname")
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding a tapnode, please specify the name of the TapDevice as a first parameter.");
                    }

                  lineBuffer >> buffer;
                  std::string tapDevice = buffer;

                  // Step 3: Check if uplink and downlink are specified
                  lineBuffer >> buffer;
                  if (buffer == "uplink")
                    {
                      lineBuffer >> uplinkSpeed;

                      lineBuffer >> buffer;
                      if (buffer != "downlink")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, please specify the downlink speed of the clients (\"downlink\") as a second parameter.");
                        }

                      lineBuffer >> downlinkSpeed;
                    }
                  else
                    {
                      // Alternative: Use default values
                      uplinkSpeed = "100Mbps";
                      downlinkSpeed = "100Mbps";
                    }

                  m_tapNodes->Add (m_topologyHelper->AddTapNodeToLastTopology (tapDevice, (*routerIt).second, uplinkSpeed, downlinkSpeed, "0ms", false, false));

                  std::cout << "		Added TapNode attached to TapDevice "<< tapDevice << std::endl;
                }
              else if (buffer == "tracker")
                {
                  if (!m_trackerAdded)
                    {
                      std::map<std::string, Ptr<Node> >::const_iterator routerIt = m_topologyHelper->GetLastRouters ();
                      UniformVariable uv;
                      uint32_t its = uv.GetInteger (0, m_topologyHelper->GetLastRouterCount () - 1);
                      for (uint32_t i = 0; i < its; ++i)
                        {
                          ++routerIt;
                        }

                      Ptr<BitTorrentTracker> btTrackerApp = Create<BitTorrentTracker> ();
                      btTrackerApp->SetAnnouncePath ("/announce");
                      btTrackerApp->SetScrapePath ("/scrape");
                      btTrackerApp->SetUpdateInterval (Seconds (60));
                      btTrackerApp->SetStartTime (Seconds (0));

                      m_btTracker->Add (m_topologyHelper->AddNodeToLastTopology ((*routerIt).second, "point-to-point", "100Mbps", "100Mbps", "0ms", false, false, 0, true, ""));
                      m_btTracker->Get (0)->AddApplication (btTrackerApp);
                      m_trackerAppContainer->Add (btTrackerApp);

                      std::cout << "		Added the tracker application to a new node attached to a random router."<< std::endl;

                      m_trackerAdded = true;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Tracker already existent in the topology. Only one tracker is allowed.");
                    }
                }
              else if (buffer == "clients")
                {
                  uint32_t newNodeCount;
                  std::string deviceType, uplinkSpeed = "", downlinkSpeed, delay, routerId = "";
                  NodeContainer newNodeContainer;

                  lineBuffer >> buffer;

                  if (buffer != "count")
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, please specify the number of the clients to add (\"count\") as a first parameter.");
                    }

                  lineBuffer >> buffer;

                  newNodeCount = lexical_cast<uint32_t> (buffer);

                  lineBuffer >> buffer;

                  if (buffer != "type")
                  {
                	  NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, please specify the connection type of the clients to add (\"type\") as a second parameter.");
                  }

                  lineBuffer >> deviceType;

                  lineBuffer >> buffer;

                  if (buffer == "uplink")
                    {
                      lineBuffer >> uplinkSpeed;

                      lineBuffer >> buffer;

                      if (buffer != "downlink")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, please specify the downlink speed of the clients (\"downlink\") as a third parameter.");
                        }

                      lineBuffer >> downlinkSpeed;
                    }
                  else if (buffer == "using")
                    {
                      lineBuffer >> buffer;

                      if (buffer != "samples")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you can either specify up- and downlink bandwidths or use the \"samples file\".");
                        }

                      lineBuffer >> buffer;

                      if (buffer != "file")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you can either specify up- and downlink bandwidths or use the \"samples file\".");
                        }

                      uplinkSpeed = "samples";
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you can either specify up- and downlink bandwidths or use the \"samples file\".");
                    }

                  lineBuffer >> buffer;

                  if (buffer == "delay")
                    {
                      lineBuffer >> delay;
                    }
                  else if (uplinkSpeed != "samples")
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients not using the \"samples file\", please specify the channel delay of the connection of the clients (\"delay\") as a third parameter.");
                    }

                  if (!lineBuffer.eof ())
                    {
                      lineBuffer >> buffer >> buffer2;

                      if (buffer != "to" || buffer2 != "router")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you may specify to which router to add your nodes using the \"to router\" argument.");
                        }
                      else
                        {
                          lineBuffer >> routerId;
                        }
                    }

                  if (uplinkSpeed == "samples")
                    {
                      newNodeContainer = m_topologyHelper->AddClientNodesToLastTopology (newNodeCount, deviceType);
                    }
                  else
                    {
                      UniformVariable uv;
                      for (uint32_t i = 0; i < newNodeCount; ++i)
                        {
                          std::map<std::string, Ptr<Node> >::const_iterator routerIt = m_topologyHelper->GetLastRouters ();
                          uint32_t its = uv.GetInteger (0, m_topologyHelper->GetLastRouterCount () - 1);

                          if(routerId == "")
                            {
                              for (uint32_t j = 0; j < its; ++j)
                                {
                                  ++routerIt;
                                }
                            }
                          else
                            {
                              its = m_topologyHelper->GetLastRouterCount ();
                              bool found = false;

                              for (uint32_t j = 0; j < its; ++j)
                                {
                                  if((*routerIt).first == routerId)
                                    {
                                      found = true;
                                      break;
                                    }

                                  ++routerIt;
                                }

                              if (!found)
                                {
                                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Could not find the router to attach the new clients to. Make sure that IDs are sequential, starting at 0.");
                                }
                            }

                          newNodeContainer.Add (m_topologyHelper->AddNodeToLastTopology ((*routerIt).second, deviceType, uplinkSpeed, downlinkSpeed, delay, false, false, 0, false, ""));
                        }
                    }

                  for (NodeContainer::Iterator it = newNodeContainer.Begin (); it != newNodeContainer.End (); ++it)
                    {
                      Ptr<Node> node = *it;
                      Ptr<BitTorrentVideoClient> btclient = Create<BitTorrentVideoClient> ();
                      btclient->SetStartTime (Seconds (simulationDuration - 1)); // So the applications don't start when not ordered to
                      btclient->SetStopTime (Seconds (simulationDuration));
                      btclient->SetCheckDownloadedData (m_checkData);

#ifdef NS3_MPI
                      if (PeekPointer (*it)->GetSystemId () == MpiInterface::GetSystemId ())
#endif
                        {
                          node->AddApplication (btclient);
	                      m_clientAppContainer->Add (btclient);
                        }
                    }

                  m_btNodes->Add (newNodeContainer);
                  m_btNodeCount += newNodeCount;

                  std::cout << "		Added "<< newNodeCount << " client nodes to the topology." << std::endl;
                }
              else if (buffer == "other" || buffer == "background" || buffer == "sink")
                {
                  bool background = (buffer == "background");
                  bool backgroundSink = (buffer == "sink");
                  
                  lineBuffer >> buffer;

                  if (buffer == "nodes")
                    {
                      uint32_t newNodeCount;
                      std::string deviceType, uplinkSpeed = "", downlinkSpeed, delay, routerId, ip, destination;
                      NodeContainer newNodeContainer;
                      NodeContainer newNodeContainer2;

                      lineBuffer >> buffer;

                      if (buffer != "count")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, please specify the number of the clients to add (\"count\") as a first parameter.");
                        }

                      lineBuffer >> buffer;

                      newNodeCount = lexical_cast<uint32_t> (buffer);

                      lineBuffer >> buffer;

                      if (buffer != "type")
                      {
                        NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, please specify the connection type of the clients to add (\"type\") as a second parameter.");
                      }

                      lineBuffer >> deviceType;

                      lineBuffer >> buffer;

                      if (buffer == "uplink")
                        {
                          lineBuffer >> uplinkSpeed;

                          lineBuffer >> buffer;

                          if (buffer != "downlink")
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, please specify the downlink speed of the clients (\"downlink\") as a second parameter.");
                            }

                          lineBuffer >> downlinkSpeed;
                        }
                      else if (buffer == "using")
                        {
                          lineBuffer >> buffer;

                          if (buffer != "samples")
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you can either specify up- and downlink bandwidths or use the \"samples file\".");
                            }

                          lineBuffer >> buffer;

                          if (buffer != "file")
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you can either specify up- and downlink bandwidths or use the \"samples file\".");
                            }

                          uplinkSpeed = "samples";
                        }
                      else
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you can either specify up- and downlink bandwidths or use the \"samples file\".");
                        }

                      lineBuffer >> buffer;

                      if (buffer == "delay")
                        {
                          lineBuffer >> delay;
                        }
                      else if (uplinkSpeed != "samples")
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients not using the \"samples file\", please specify the channel delay of the connection of the clients (\"delay\") as a third parameter.");
                        }
                        
                      if (!lineBuffer. eof())
                      {
                        lineBuffer >> buffer;
                        
                        if (buffer == "ip" && (!background || backgroundSink))
                        {
                            lineBuffer >> ip;
                        }
                        else if (buffer == "destination" && background)
                        {
                            lineBuffer >> destination;
                        }
                        else
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: You may either specify an \"ip\" address for a default empty node or a \"destination\" for a background traffic node.");
                        }
                      }
                      
                      if (!lineBuffer.eof ())
                        {
                          lineBuffer >> buffer >> buffer2;

                          if (buffer != "to" || buffer2 != "router")
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: When adding clients, you may specify to which router to add your nodes using the \"to router\" argument.");
                            }
                          else
                            {
                              lineBuffer >> routerId;
                            }
                        }                        
                                              
                                            
                      if (uplinkSpeed == "samples")
                        {
                          newNodeContainer = m_topologyHelper->AddClientNodesToLastTopology (newNodeCount, deviceType);
                        }
                      else
                        {
                          UniformVariable uv;
                          Ipv4Address ipAddress (ip.c_str ());
                          
                          for (uint32_t i = 0; i < newNodeCount; ++i)
                            {
                              std::map<std::string, Ptr<Node> >::const_iterator routerIt = m_topologyHelper->GetLastRouters ();
                              uint32_t its = uv.GetInteger (0, m_topologyHelper->GetLastRouterCount () - 1);
                              
                              if(routerId == "")
                                {
                                  for (uint32_t j = 0; j < its; ++j)
                                    {
                                      ++routerIt;
                                    }
                                }
                              else
                                {
                                  its = m_topologyHelper->GetLastRouterCount ();
                                  bool found = false;

                                  for (uint32_t j = 0; j < its; ++j)
                                    {
                                      if((*routerIt).first == routerId)
                                        {
                                          found = true;
                                          break;
                                        }

                                      ++routerIt;
                                    }

                                  if (!found)
                                    {
                                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Could not find the router to attach the new nodes to. Make sure that IDs are sequential, starting at 0.");
                                    }
                                }
                              
                              if(ip != "")
                                {                             
                                    std::ostringstream ipAddressStream;
                                    ipAddress.Print (ipAddressStream);
                                    
                                    newNodeContainer.Add (m_topologyHelper->AddNodeToLastTopology ((*routerIt).second, deviceType, uplinkSpeed, downlinkSpeed, delay, false, false, 0, false, ipAddressStream.str ())); 
                                    
                                    ipAddress.Set(ipAddress.Get() + 1);    
                                  }
                                else
                                  {
                                    newNodeContainer.Add (m_topologyHelper->AddNodeToLastTopology ((*routerIt).second, deviceType, uplinkSpeed, downlinkSpeed, delay, false, false, 0, false, "")); 
                                  }
                            }
                        }
                   
                      if (background)
                      {
                         if (destination == "")
                         {
                           NS_ABORT_MSG ("[line " << currentLine << "} Error: When defining a \"background\" node, you must specify a \"destination\" ip for the traffic.");
                         }
                         
                         OnOffHelper ooh ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address (destination.c_str ()), 33333));
                         
                         ooh.SetConstantRate (DataRate (uplinkSpeed));
                        
                         ooh.SetAttribute ("StartTime", TimeValue(Seconds(0)));                        
                         ooh.SetAttribute ("StopTime", TimeValue(Seconds (simulationDuration - 1)));

#ifdef NS3_MPI                         
                         if (MpiInterface::GetSystemId () == 0)
#endif
                         {
                             ooh.Install (newNodeContainer);
                         }
                      }
                      else if (backgroundSink)
                      {
                        PacketSinkHelper psh ("ns3::TcpSocketFactory", Ipv4Address ("0.0.0.0")); //Ipv4Address (ip.c_str()));
                        psh.SetAttribute ("Local", AddressValue (InetSocketAddress ("0.0.0.0", 33333)));
                        
                        psh.SetAttribute ("StartTime", TimeValue(Seconds(0)));
                        psh.SetAttribute ("StopTime", TimeValue(Seconds (simulationDuration - 1)));

#ifdef NS3_MPI
                        if (MpiInterface::GetSystemId () == 0)
#endif
                        {
                          psh.Install (newNodeContainer);
                        }
                      }
                      
                      m_otherNodes->Add (newNodeContainer);
                      m_otherNodeCount += newNodeCount;

                      std::cout << "		Added " << newNodeCount << " " << (background ? "background" : (backgroundSink ? "sink" : "other")) << " nodes to the topology." << std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Can only add \"{background, sink, other} nodes\" to the topology.");
                    }
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what else to add to the topology.");
                }
            }
          else
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to do with the topology.");
            }
        }
      else if (simulation)
        {
          if (buffer == "set")
            {
              lineBuffer >> buffer;

              if (buffer == "id")
                {
                  if (m_simulationId == "")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "random")
                        {
                          UniformVariable uv;
                          m_simulationId = lexical_cast<std::string> (uv.GetInteger (0, std::numeric_limits<uint32_t>::max () - 1));
                        }
                      else if (buffer == "time")
                        {
                          time_t currentTime;
                          struct tm* currentTimeInfo;
                          char simulationIdBuffer[20];

                          std::time (&currentTime);
                          currentTimeInfo = localtime (&currentTime);
                          strftime (simulationIdBuffer, 20, "%Y-%m-%d-%H-%M-%S", currentTimeInfo);

                          m_simulationId.assign (simulationIdBuffer, 19);
                        }
                      else
                        {
                          m_simulationId = buffer;
                        }

                      std::cout << "                The ID of this simulation is " << m_simulationId << "." << std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Simulation ID already set in a previous line.");
                    }
                }
              else if (buffer == "logging")
                {
                  lineBuffer >> buffer;

                  if (buffer == "file")
                    {
                      m_loggingToFile = true;
                    }

                  std::cout << "		Enabled logging to file."<< std::endl;
                }
              else if (buffer == "folder")
                {
                  lineBuffer >> buffer;

                  m_torrentFolder = buffer;

                  std::cout << "		Set torrent data folder to "<< buffer << "." << std::endl;
                }
              else if (buffer == "file")
                {
                  if (m_torrentFolder == "")
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: You must set the torrent data folder first.");
                    }

                  lineBuffer >> buffer;

                  m_torrentFile = buffer;

                  bool useFakeData = false;
                  if (!lineBuffer.eof ())
                    {
                      lineBuffer >> buffer;

                      if (buffer == "fake")
                        {
                          lineBuffer >> buffer;

                          if (buffer == "data")
                            {
                              StorageManager::GetInstance ()->SetUseFakeData (true);
                              useFakeData = true;
                              m_useFakeData = true;
                            }
                          else
                            {
                              NS_ABORT_MSG ("[line " << currentLine << "] Error: Can only fake \"data\" for the torrent.");
                            }
                        }
                      else
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: The path to the torrent file must not include whitespace characters.");
                        }
                    }

                  if (!useFakeData)
                    {
                      std::cout << "		Set shared torrent file to "<< m_torrentFile << "." << std::endl;
                    }
                  else
                    {
                      std::cout << "		Set shared torrent file to "<< m_torrentFile << " but using fake data for speedup." << std::endl;
                    }
                }
              else if (buffer == "random")
                {
                  lineBuffer >> buffer;

                  if (buffer == "seed")
                    {
                      lineBuffer >> buffer;

                      uint32_t randomSeed = 0;                           // assignment to mute compiler

                      if (buffer == "time")
                        {
                          randomSeed = std::time (0);
                        }
                      else
                        {
                          randomSeed = lexical_cast<uint32_t> (buffer);
                        }

                      srand (randomSeed);
                      SeedManager::SetSeed (randomSeed);

                      std::cout << "		Set random seed to "<< randomSeed << "." << std::endl;

                      m_randomSeedSet = true;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to do with the random seed.");
                    }
                }
              else if (buffer == "variable")
                {
                  lineBuffer >> buffer;
                  lineBuffer >> buffer2;
                  buffer3 = buffer2;
                  while (!lineBuffer.eof ())
                    {
                      lineBuffer >> buffer2;
                      buffer3.append (" " + buffer2);
                    }

                  m_variables[buffer] = buffer3;

                  std::cout << "		Set variable "<< buffer << " to value \"" << buffer3 << "\"." << std::endl;
                }
              else if (buffer == "pcap")
                {
                  lineBuffer >> buffer;

                  if (buffer == "enabled")
                    {
                      lineBuffer >> buffer;

                      if (buffer == "prefix")
                        {
                          lineBuffer >> buffer;

                          buffer = buffer + "." + lexical_cast<std::string> (rand () % static_cast<uint32_t> (std::pow ((double)2, (double)31)));

                          m_topologyHelper->EnablePcaps (buffer);

                          std::cout << "		PCAP enabled. Files will be stored under "<< buffer << ".*.pcap." << std::endl;
                        }
                      else
                        {
                          NS_ABORT_MSG ("[line " << currentLine << "] Error: To enable PCAP tracing, specify the file name prefix (may include paths).");
                        }
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: PCAP can only be \"enable\"d.");
                    }
                }
              else if (buffer == "checkdata")
                {
                  lineBuffer >> buffer;

                  if (buffer == "1")
                    {
                      m_checkData = true;
                      std::cout << "		Downloaded data of clients will be checked for correctness."<< std::endl;
                    }
                  else if (buffer == "0")
                    {
                      m_checkData = false;
                      std::cout << "		Downloaded data of clients will not be checked for correctness."<< std::endl;
                    }
                  else
                    {
                      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know whether to check downloaded Data or not.");
                    }
                }
              else
                {
                  NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to set for the simulation.");
                }
            }
          else
            {
              NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what to do with the simulation.");
            }
        }
      else
        {
          NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know that entity.");
        }
    }     // while(!storyFile.eof())

  if (m_simulationId == "")
    {
      NS_ABORT_MSG ("[line " << currentLine << "] Error: No Simulation ID was set. This, however, is a required setting.");
    }

  if (!m_randomSeedSet)
    {
      NS_ABORT_MSG ("[line " << currentLine << "] Error: Don't know what seed to use for random numbers. This, however, is a required setting.");
    }
}

} // ns bittorrent
} // ns ns3
