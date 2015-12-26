/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2012 ComSys, RWTH Aachen University
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

#include "RarestFirstPartSelectionStrategy.h"

#include "ns3/BitTorrentClient.h"
#include "ns3/BitTorrentPeer.h"

#include "ns3/log.h"
#include "ns3/random-variable.h"

#include <list>
#include <set>
#include <utility>
#include <vector>

namespace ns3 {
namespace bittorrent  {

NS_LOG_COMPONENT_DEFINE ("bittorrent::RarestFirstPartSelectionStrategy");
NS_OBJECT_ENSURE_REGISTERED (RarestFirstPartSelectionStrategy);

RarestFirstPartSelectionStrategy::RarestFirstPartSelectionStrategy (Ptr<BitTorrentClient> myClient) : PartSelectionStrategyBase (myClient)
{
  // Step 1: Initialize the data structures used to determine the entropy of the pieces in the swarm
  m_piecesByRarity = new std::set<uint32_t>[m_myClient->GetMaxPeers () + 2];
  m_raritiesByPiece.resize (m_myClient->GetTorrent ()->GetNumberOfPieces ());

  // Step 2: Iterate through the pieces and see whether they are available locally (i.e., finished) or not
  for (uint32_t i = 0; i < m_myClient->GetTorrent ()->GetNumberOfPieces (); ++i)
    {
      // Step 2a: If the piece is in the list of needed pieces, it is obviously not available locally and, hence, needed
      if (m_neededPieces.find (i) != m_neededPieces.end ())
        {
          m_piecesByRarity[0].insert (i);
          m_raritiesByPiece[i] = 0;
        }
      // Step 2b: Else, it is available locally and nothing else has to be done
      else
        {
          m_piecesByRarity[m_myClient->GetMaxPeers () + 1].insert (i);
          m_raritiesByPiece[i] = m_myClient->GetMaxPeers () + 1;
        }
    }
}

RarestFirstPartSelectionStrategy::~RarestFirstPartSelectionStrategy ()
{
  for (uint16_t i = 0; i < m_myClient->GetMaxPeers () + 2; ++i)
    {
      m_piecesByRarity[i].clear ();
    }
  delete[] m_piecesByRarity;
  m_raritiesByPiece.clear ();
}

void RarestFirstPartSelectionStrategy::DoInitialize ()
{
  // Step 1: Register events handled by the base class only
  m_myClient->RegisterCallbackChokeChangingEvent (MakeCallback (&PartSelectionStrategyBase::ProcessPeerChokeChangingEvent,this));
  m_myClient->RegisterCallbackBlockCompleteEvent (MakeCallback (&PartSelectionStrategyBase::ProcessPeerBlockCompleteEvent,this));

  // Step 2: Register our own handlers
  m_myClient->RegisterCallbackBitfieldReceivedEvent (MakeCallback (&RarestFirstPartSelectionStrategy::ProcessPeerBitfieldReceivedEvent,this));
  m_myClient->RegisterCallbackPeerHaveEvent (MakeCallback (&RarestFirstPartSelectionStrategy::ProcessPeerHaveEvent,this));
  m_myClient->RegisterCallbackConnectionCloseEvent (MakeCallback (&RarestFirstPartSelectionStrategy::ProcessPeerConnectionCloseEvent,this));
  m_myClient->RegisterCallbackStrategyOptionsChangedEvent (MakeCallback (&RarestFirstPartSelectionStrategy::ProcessStrategyOptionsChangedEvent, this));
}

void RarestFirstPartSelectionStrategy::ProcessPeerBitfieldReceivedEvent (Ptr<Peer> peer)
{
  // Step 1: Gather all the pieces we have to move because the newly-connected peer has them
  std::list<std::pair<uint16_t, std::set<uint32_t>::iterator > > *toMove = new std::list<std::pair<uint16_t, std::set<uint32_t>::iterator > >[m_myClient->GetMaxPeers ()];
  for (uint16_t i = 0; i <= m_myClient->GetMaxPeers () - 1; ++i)
    {
      std::set<uint32_t>::iterator it = m_piecesByRarity[i].begin ();
      for (; it != m_piecesByRarity[i].end (); ++it)
        {
          if (peer->HasPiece (*it))
            {
              toMove[i].push_back (std::pair<uint16_t, std::set<uint32_t>::iterator > (i, it));
            }
        }
    }

  // Step 2: Now, move the pieces to move
  for (uint16_t i = 0; i <= m_myClient->GetMaxPeers () - 1; ++i)
    {
      std::list<std::pair<uint16_t, std::set<uint32_t>::iterator > >::const_iterator it2 = toMove[i].begin ();
      for (; it2 != toMove[i].end (); ++it2)
        {
          std::pair<uint16_t, std::set<uint32_t>::iterator > it3 = *it2;
          m_piecesByRarity[it3.first + 1].insert (*(it3.second));
          m_raritiesByPiece[*(it3.second)]++;
          m_piecesByRarity[it3.first].erase (it3.second);
        }
      toMove[i].clear ();
    }
  delete[] toMove;

  // Step 3: Call the base class event handler (Note: This needs to be done at the end because it may cause a call to Schedule()!)
  PartSelectionStrategyBase::ProcessBitfieldReceivedEvent (peer);
}

void RarestFirstPartSelectionStrategy::ProcessPeerHaveEvent (Ptr<Peer> peer, uint32_t pieceIndex)
{
  // Step 0: If we have already completed our download, there will be no data structures left to operate in => return immediately
  if (m_myClient->GetDownloadCompleted ())
    {
      return;
    }

  // Step 1: Get the rarity of the newly-announced piece, return if an error (= already announced by each peer, e.g., double announce) occurred
  uint16_t currentRarity = m_raritiesByPiece[pieceIndex];
  if (currentRarity >= m_myClient->GetMaxPeers ())
    {
      return;
    }
  // Step 1a: Else, shift the availability of the respective piece
  else
    {
      m_piecesByRarity[currentRarity].erase (pieceIndex);
      m_piecesByRarity[currentRarity + 1].insert (pieceIndex);
      m_raritiesByPiece[pieceIndex] = currentRarity + 1;           // m_raritiesByPiece[pieceIndex] + 1;
    }

  // Step 3: Call the base class event handler, e.g., for invoking the scheduler
  PartSelectionStrategyBase::ProcessPeerHaveEvent (peer, pieceIndex);
}

void RarestFirstPartSelectionStrategy::ProcessPeerConnectionCloseEvent (Ptr<Peer> peer)
{
  /*
   * NOTE: This function is more or less the "inverse" to ProcessBitfieldReceivedEvent, so no further annotations here
   */
  std::list<std::pair<uint16_t, std::set<uint32_t>::iterator > > *toMove = new std::list<std::pair<uint16_t, std::set<uint32_t>::iterator > >[m_myClient->GetMaxPeers ()];
  for (uint16_t i = 1; i <= m_myClient->GetMaxPeers (); ++i)
    {
      std::set<uint32_t>::iterator it = m_piecesByRarity[i].begin ();
      for (; it != m_piecesByRarity[i].end (); ++it)
        {
          if (peer->HasPiece (*it))
            {
              toMove[i - 1].push_back (std::pair<uint16_t, std::set<uint32_t>::iterator > (i, it));
            }
        }
    }

  for (uint16_t i = 1; i <= m_myClient->GetMaxPeers (); ++i)
    {
      std::list<std::pair<uint16_t, std::set<uint32_t>::iterator > >::const_iterator it2 = toMove[i - 1].begin ();
      for (; it2 != toMove[i - 1].end (); ++it2)
        {
          std::pair<uint16_t, std::set<uint32_t>::iterator > it3 = *it2;
          m_piecesByRarity[it3.first - 1].insert (*(it3.second));
          m_raritiesByPiece[*(it3.second)]--;
          m_piecesByRarity[it3.first].erase (it3.second);
        }
      toMove[i - 1].clear ();
    }
  delete[] toMove;

  PartSelectionStrategyBase::ProcessPeerConnectionCloseEvent (peer);
}

void RarestFirstPartSelectionStrategy::ProcessStrategyOptionsChangedEvent ()
{
  // Step 1: We must react to a changed maximum number of peers so we can sort into our buckets correctly
  if (m_myClient->GetLastChangedStrategyOptionName () == "max_peers")
    {
      // Step 1: We retrieve the old and new values of the maximum peers
      std::pair<std::string, std::string> maxPeers = m_myClient->GetStrategyOptionChangePair ("max_peers");
      uint16_t oldMaxPeers = lexical_cast<uint16_t> (maxPeers.first);
      uint16_t newMaxPeers = lexical_cast<uint16_t> (maxPeers.second);

      // Step 2: Now, we create a new array, assign the values accordingly, delete the old array and set the pointer to the new array start
      // Step 2a: The easy case: newMaxPeers >= oldMaxPeers: Just enlarge the array
      if (newMaxPeers >= oldMaxPeers)
        {
          std::set<uint32_t>* newPiecesByRarity = new std::set<uint32_t>[newMaxPeers + 2];
          for (uint32_t i = 0; i < oldMaxPeers; ++i)
            {
              newPiecesByRarity[i] = m_piecesByRarity[i];
              m_piecesByRarity[i].clear ();
            }
          delete[] m_piecesByRarity;
          m_piecesByRarity = newPiecesByRarity;
        }
      /*
       * Step 2b: The not-so-easy case: newMaxPeers < oldMaxPeers: Resize the array and re-fill it
       * NOTE: This implementation DEMANDS that the surplus peers have already been correctly de-registered from the client!
       *       Otherwise, the strategy is unable to determine the number of correctly-registered clients.
       *       Note that this is normally guaranteed if any strategy responsible for peer connections is the first strategy
       *       informed about this event. The standard implementation of the ProtocolFactory guarantees this.
       */
      else
        {
          // Step 2b1: Delete the array of old sets and create a new one
          for (uint32_t i = 0; i < oldMaxPeers; ++i)
            {
              m_piecesByRarity[i].clear ();
            }
          delete[] m_piecesByRarity;
          m_piecesByRarity = new std::set<uint32_t>[newMaxPeers + 2];

          // Step 2b2: Now, refill the buckets
          for (uint32_t i = 0; i < m_myClient->GetTorrent ()->GetNumberOfPieces (); ++i)
            {
              // Step 2b2a: The easy subcase: The piece is available locally
              if (m_neededPieces.find (i) == m_neededPieces.end ())
                {
                  m_piecesByRarity[newMaxPeers + 1].insert (i);
                  m_raritiesByPiece[i] = newMaxPeers + 1;
                }
              // Step 2b2b: The not-so.easy subcase: The piece is NOT available locally, so we have to recount its rarity
              else
                {
                  uint16_t rarity = 0;
                  for (std::vector<Ptr<Peer> >::const_iterator it = m_myClient->GetPeerListIterator (); it != m_myClient->GetPeerListEnd (); ++it)
                    {
                      if ((*it)->HasPiece (i))
                        {
                          ++rarity;
                        }
                    }

                  m_piecesByRarity[rarity].insert (i);
                  m_raritiesByPiece[i] = rarity;
                }
            }
        }
    }

  // Step 2: Call the base class event handler
  PartSelectionStrategyBase::ProcessStrategyOptionsChangedEvent ();
}

void RarestFirstPartSelectionStrategy::ProcessCompletedPiece (uint32_t pieceIndex)
{
  // Step 1: Get the current rarity of the newly-completed piece
  uint16_t currentRarity = m_raritiesByPiece[pieceIndex];

  // Step 2: If the rarity was not the "maximum rarity" (i.e., it was missing), mark it as completed
  if (currentRarity <= m_myClient->GetMaxPeers ())
    {
      m_piecesByRarity[currentRarity].erase (pieceIndex);

      // OLD
      // m_piecesByRarity[m_myClient->GetMaxPeers () + 1].insert (pieceIndex);

      // MEMORY
      uint32_t newRarity = m_myClient->GetMaxPeers () + 1;
      m_piecesByRarity[newRarity].insert (pieceIndex);
      // /MEMORY

      m_raritiesByPiece[pieceIndex] = m_myClient->GetMaxPeers () + 1;
    }

  // Step 3: Call the base class event handler
  PartSelectionStrategyBase::ProcessCompletedPiece (pieceIndex);
}

void RarestFirstPartSelectionStrategy::GetHighestPriorityBlockForPeer (Ptr<Peer> peer, BlockRequested& blockPtr)
{
  // Step 1: Prepare the passed block so far we can so that accessing it after this function does not result in undesired behavior
  blockPtr.m_requestedFrom = peer;
  blockPtr.m_blockLength = 0;

  // Step 2: Calculate the timeout of a piece by taking into account how many requests are already running for this peer
  blockPtr.m_timeoutTime =
    Simulator::Now () +
    MilliSeconds ((1 + m_requestedBlocks[peer].size ()) * m_myClient->GetPieceTimeout ().GetMilliSeconds () / m_blocksPerPiece);

  // Step 3: Find a block that we may need to download; with completetion of alredy-requested pieces preceding new rarest-first choices
  bool blockFound = false;

  // Step 3a: Take an "educated guess" by looking at blocks currently requested from this peer
  std::list<BlockRequested*>::reverse_iterator currentBlock = m_requestedBlocks[peer].rbegin ();  // Backward iteration for increased efficiency
  while (!blockFound && currentBlock != m_requestedBlocks[peer].rend ())
    {
      NeededPiecesMap::iterator npmIt = m_neededPieces.find ((**currentBlock).m_pieceIndex);
      std::list<std::pair<uint32_t, uint32_t> >::iterator blockIt = (*npmIt).second.m_possibleBlocks.begin ();
      while (blockIt != (*npmIt).second.m_possibleBlocks.end ())
        {
          if (RequestAllowedForBlock (peer, (*npmIt).second.m_pieceIndex, (*blockIt).first, (*blockIt).second - (*blockIt).first))
            {
              blockPtr.m_pieceIndex = (*npmIt).second.m_pieceIndex;
              blockPtr.m_blockOffset = (*blockIt).first;
              blockPtr.m_blockLength = (*blockIt).second - (*blockIt).first;

              NS_LOG_INFO ("Rarest First educated guess chose piece " << (*npmIt).second.m_pieceIndex << "@" << (*blockIt).first << "->" << (*blockIt).second - (*blockIt).first << " (rarity " <<  m_raritiesByPiece[(*npmIt).second.m_pieceIndex] << ").");

              blockFound = true;
              break;
            }
          else
            {
              ++blockIt;
            }
        }

      ++currentBlock;
    }
  if (blockFound)
    {
      return;
    }

  // Step 3b: If the heuristic did not find a suitable block, apply a rarest-first scheme
  // Step 3b1: Iterate through the needed pieces in the order given by our rarity list
  uint16_t i = 1;
  while (!blockFound && i <= m_myClient->GetMaxPeers ())
    {
      // Step 3b1a: Only if this bucket is nonempty, enter it
      if (m_piecesByRarity[i].size () > 0)
        {
          // Step 3b1a1: Now, prepare a data structure which contains all pieces of the current availability bucket AT THE GIVEN PEER
          std::vector<std::pair<uint32_t, std::pair<uint32_t, uint32_t> > > possibleBlocks;
          possibleBlocks.reserve (m_piecesByRarity[i].size ());
          for (std::set<uint32_t>::const_iterator it = m_piecesByRarity[i].begin (); it != m_piecesByRarity[i].end (); ++it)
            {
              // Step 3b1a1b: Find the first fitting block for each piece
              if (peer->HasPiece (*it))
                {
                  NeededPiecesMap::iterator npmIt = m_neededPieces.find (*it);
                  std::list<std::pair<uint32_t, uint32_t> >::iterator blockIt = (*npmIt).second.m_possibleBlocks.begin ();
                  while (blockIt != (*npmIt).second.m_possibleBlocks.end ())
                    {
                      if (RequestAllowedForBlock (peer, (*npmIt).second.m_pieceIndex, (*blockIt).first, (*blockIt).second - (*blockIt).first))
                        {
                          possibleBlocks.push_back (std::pair<uint32_t, std::pair<uint32_t, uint32_t> > ((*npmIt).second.m_pieceIndex, std::pair<uint32_t, uint32_t> ((*blockIt).first, (*blockIt).second)));
                          break;
                        }
                      else
                        {
                          ++blockIt;
                        }
                    }
                }
            }

          // Step 3b1a2: If we have found any blocks, randomly choose one of them
          if (possibleBlocks.size () != 0)
            {
              UniformVariable uv;
              uint32_t index = uv.GetInteger (0, possibleBlocks.size () - 1);
              blockPtr.m_pieceIndex = possibleBlocks[index].first;
              blockPtr.m_blockOffset = possibleBlocks[index].second.first;
              blockPtr.m_blockLength = possibleBlocks[index].second.second - possibleBlocks[index].second.first;

              NS_LOG_INFO ("Rarest First heuristic chose piece " << blockPtr.m_pieceIndex << "@" << blockPtr.m_blockOffset << "->" << blockPtr.m_blockOffset + blockPtr.m_blockLength << " (rarity " <<  m_raritiesByPiece[blockPtr.m_pieceIndex] << ").");

              possibleBlocks.clear ();
              blockFound = true;
              break;
            }
        }

      // Step 3b1b: Increase the "iterator" through the rarity buckets
      ++i;
    }
}

} // ns bittorrent
} // ns ns3
