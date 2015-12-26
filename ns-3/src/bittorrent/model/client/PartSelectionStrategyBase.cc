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

#include "PartSelectionStrategyBase.h"

#include "BitTorrentClient.h"
#include "ns3/BitTorrentUtilities.h"
#include "BitTorrentPeer.h"

#include "ns3/log.h"

#include <list>
#include <map>
#include <set>
#include <vector>

namespace ns3 {
namespace bittorrent  {

NS_LOG_COMPONENT_DEFINE ("bittorrent::PartSelectionStrategyBase");

PartSelectionStrategyBase::PartSelectionStrategyBase (Ptr<BitTorrentClient> myClient) : AbstractStrategy (myClient)
{
  // Step 1: Check if the download is completed already completed since then, no other work has to be done here
  CheckDownloadCompleted ();

  // Step 2: Set up the basic working intervals of the strategy
  m_periodicInterval = Seconds (1);

  // Step 3: Set up the timeouts for connection failure/... heuristics
  m_requestPatience = Seconds (45);
  m_chokingPatience = Seconds (180);
  m_downloadPatience = Seconds (120);

  /*
   * Step 4: Initialize the data structures representing still needed pieces / blocks
   *
   * NOTE: For the sake of simplicity, this implementation will not store whether parts of a pice have been downloaded
   * over the course of multiple client instantiations; i.e., while the download progress of a single piece is stored during
   * a single session (i.e., an ns-3 simulation run), storing this sub-piece download state is not possible.
   */
  InitializeNeededPiecesMap ();

  // Step 5: Retrieve the number of blocks per piece so we can more correctly determine the time-outs of block requests
  NeededPiecesMap::iterator it = m_neededPieces.begin ();
  if (it != m_neededPieces.end ())
    {
      m_blocksPerPiece = (*it).second.m_possibleBlocks.size ();
    }
}

PartSelectionStrategyBase::~PartSelectionStrategyBase ()
{
}

void PartSelectionStrategyBase::DoInitialize ()
{
  // Step 1: Register with the client class instance
  m_myClient->RegisterCallbackBitfieldReceivedEvent (MakeCallback (&PartSelectionStrategyBase::ProcessBitfieldReceivedEvent, this));
  m_myClient->RegisterCallbackChokeChangingEvent (MakeCallback (&PartSelectionStrategyBase::ProcessPeerChokeChangingEvent,this));
  m_myClient->RegisterCallbackPeerHaveEvent (MakeCallback (&PartSelectionStrategyBase::ProcessPeerHaveEvent,this));
  m_myClient->RegisterCallbackBlockCompleteEvent (MakeCallback (&PartSelectionStrategyBase::ProcessPeerBlockCompleteEvent,this));
  m_myClient->RegisterCallbackConnectionCloseEvent (MakeCallback (&PartSelectionStrategyBase::ProcessPeerConnectionCloseEvent,this));
  m_myClient->RegisterCallbackStrategyOptionsChangedEvent (MakeCallback (&PartSelectionStrategyBase::ProcessStrategyOptionsChangedEvent, this));

  /*
   * Note: DO NOT START THE SCHEDULER RIGHT NOW! This will be done automatically when connection to a peer has been fully established!
   */
}

void PartSelectionStrategyBase::ProcessStrategyOptionsChangedEvent ()
{
  /*
   * This is a little tricky a change: We have to immediately cancel ALL requests sent out to any peer and
   * re-initialize the list of possible requests.
   * NOTE: This will result in a multitude of requests being not fulfilled, especially: Any already-begun piece
   * will be emptied again since keeping track of all half-finished requests in a non-uniform-size fashion is too complicated!
   */
  if (m_myClient->GetLastChangedStrategyOptionName () == "request_block_size")
    {
      // Step 1: Iterate through the needed pieces and cancel all requests
      NeededPiecesMap::iterator npmIt = m_neededPieces.begin ();
      while (npmIt != m_neededPieces.end ())
        {
          // Step 1a: Cancel all block requests
          while (!(*npmIt).second.m_pendingBlocks.empty ())
            {
              BlockRequested* block = (*npmIt).second.m_pendingBlocks.front ();

              // Step 1a1: Cancel the timeout event for this request
              (*block).m_timeoutEvent.Cancel ();

              // Step 1a2: Cancel the associated request
              (*block).m_requestedFrom->CancelRequest ((*block).m_pieceIndex, (*block).m_blockOffset, (*block).m_blockLength);

              // Step 1a3: Remove the reference to the peer for this request
              std::multiset<Ptr<Peer> >::iterator it = (*npmIt).second.m_requestedFrom.find ((*block).m_requestedFrom);
              (*npmIt).second.m_requestedFrom.erase (it);

              // Step 1a3: Destroy the related request object
              delete block;

              // Step 1a4: Remove the reference to this request from the list
              (*npmIt).second.m_pendingBlocks.pop_front ();
            }

          // Step 1b: Cancel the timeout event related to the whole piece
          (*npmIt).second.m_timeoutEvent.Cancel ();

          // Step 1c: Remove the piece
          NeededPiecesMap::iterator npmIt2 = npmIt;
          ++npmIt2;
          m_neededPieces.erase (npmIt);
          npmIt = npmIt2;
        }

      // Step 2: Clean the requested blocks map (should be in sync with the needed pieces map and hence now only contain invalid pointers
      RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.begin ();
      while (rbmIt != m_requestedBlocks.end ())
        {
          (*rbmIt).second.clear ();
          ++rbmIt;
        }

      // Step 3: Re-initialize the needed pieces
      InitializeNeededPiecesMap ();

      // Step 4: Retrieve the number of blocks per piece so we can more correctly determine the time-outs of block requests
      NeededPiecesMap::iterator it = m_neededPieces.begin ();
      if (it != m_neededPieces.end ())
        {
          m_blocksPerPiece = (*it).second.m_possibleBlocks.size ();
        }

      // Step 5: Run the scheduler again for faster assignment of new requests to the peers
      Scheduler ();
    }
}

Time PartSelectionStrategyBase::GetPeriodicInterval() const
{
  return m_periodicInterval;
}

void PartSelectionStrategyBase::SetPeriodicInterval(Time periodicInterval)
{
  if(periodicInterval.IsStrictlyPositive())
    {
      m_periodicInterval = periodicInterval;
    }
}

PartSelectionStrategyBase::PieceNeeded PartSelectionStrategyBase::GeneratePieceNeeded (uint32_t pieceNeeded)
{
  PartSelectionStrategyBase::PieceNeeded result;

  // Step 1: Generate the needed blocks according to the client's current block size setting
  std::list<std::pair<uint32_t, uint32_t> > possibleBlocks;
  uint32_t currentOffset = 0;
  uint32_t pieceLength =
    (m_myClient->GetTorrent ()->HasTrailingPiece () && pieceNeeded == m_myClient->GetTorrent ()->GetNumberOfPieces () - 1) ?
    m_myClient->GetTorrent ()->GetTrailingPieceLength () :
    m_myClient->GetTorrent ()->GetPieceLength ();

  while (currentOffset + m_myClient->GetRequestBlockSize () - 1 <= pieceLength)
    {
      possibleBlocks.push_back (std::pair<uint32_t, uint32_t> (currentOffset, currentOffset + m_myClient->GetRequestBlockSize ()));
      currentOffset += m_myClient->GetRequestBlockSize ();
    }
  if(currentOffset < pieceLength)
    {
      possibleBlocks.push_back (std::pair<uint32_t, uint32_t> (currentOffset, pieceLength));
    }

  // Step 2: Assign the piece ID and the computed possible blocks to the resulting data structure
  result.m_pieceIndex = pieceNeeded;
  result.m_possibleBlocks = possibleBlocks;

  return result;
}

void PartSelectionStrategyBase::InitializeNeededPiecesMap ()
{
  // Step 0: Clear our list of needed pieces
  m_neededPieces.clear ();

  // Step 1: Iterate through our client's bitfield and check each entry for missing pieces, create an entry in the map if missing
  uint32_t currentPiece = 0;
  for (uint32_t currentBitfieldByte = 0; currentBitfieldByte < m_myClient->GetTorrent ()->GetBitfieldSize (); ++currentBitfieldByte)
    {
      // We invert the current bitfield entry and then check bitwise; using a left shift operation after each check
      uint8_t currentByte = ~(*m_myClient->GetBitfield ())[currentBitfieldByte];
      for (uint8_t bit = 0; bit < 8 && currentPiece < m_myClient->GetTorrent ()->GetNumberOfPieces (); ++bit, ++currentPiece)
        {
          if (currentByte & 0x80)
            {
              PieceNeeded p = GeneratePieceNeeded (currentPiece);
              m_neededPieces.insert (std::pair<uint32_t, PieceNeeded> (currentPiece, p));
            }

          currentByte = static_cast<uint8_t> (currentByte << 1);
        }
    }
}

void PartSelectionStrategyBase::RequestBlock (BlockRequested* block)
{
  NS_LOG_INFO ("Requesting block " << (*block).m_pieceIndex << "@" << (*block).m_blockOffset << "->" << (*block).m_blockOffset + (*block).m_blockLength << " from peer " << (*block).m_requestedFrom->GetRemoteIp () << "; timeout at " << (*block).m_timeoutTime.GetMilliSeconds () << "ms.");

  // Step 1: Send out a request message to the remote peer
  (*block).m_requestedFrom->RequestPiece ((*block).m_pieceIndex, (*block).m_blockOffset, (*block).m_blockLength);

  // Step 2: Insert the request information for this block into the data structures
  SaveRequest (block);

  // Step 3: If the block has not yet been requested from any peer, issue a PieceRequestedEvent
  if (m_neededPieces[(*block).m_pieceIndex].m_pendingBlocks.size () == 1)
    {
      m_myClient->PieceRequestedEvent((*block).m_requestedFrom, (*block).m_pieceIndex);
    }
}

void PartSelectionStrategyBase::CancelRequest (BlockRequested block)
{
  NS_LOG_INFO ("Cancelling block " << block.m_pieceIndex << "@" << block.m_blockOffset << "->" << block.m_blockOffset + block.m_blockLength << " from peer " << block.m_requestedFrom->GetRemoteIp () << ".");

  // Step 1: Issue a cancel message to the remote peer
  block.m_requestedFrom->CancelRequest (block.m_pieceIndex, block.m_blockOffset, block.m_blockLength);

  // Step 2: Remove the request information for this block from the data structures
  RemoveRequest (block, false); // false since we have already canceled it in step 1 (more intuitive)

  // Step 3: If no other block of this piece is wanted anymore, issue a PieceCancelledEvent
  if (m_neededPieces[block.m_pieceIndex].m_pendingBlocks.size () == 0)
    {
      m_myClient->PieceCancelledEvent(block.m_requestedFrom, block.m_pieceIndex);
    }
}

void PartSelectionStrategyBase::SaveRequest (BlockRequested* block)
{
  // Step 1: Store it into the data structure for currently-pending requests
  m_requestedBlocks[(*block).m_requestedFrom].push_back (block);

  // Step 2: Also store it into the data structure for needed pieces
  m_neededPieces[(*block).m_pieceIndex].m_pendingBlocks.push_back (block);
  m_neededPieces[(*block).m_pieceIndex].m_requestedFrom.insert ((*block).m_requestedFrom);
}

void PartSelectionStrategyBase::RemoveRequest (BlockRequested block, bool cancel)
{
  // Step 1: Get a reference to the piece this block belongs to
  NeededPiecesMap::iterator npmIt = m_neededPieces.find (block.m_pieceIndex);
  if (npmIt == m_neededPieces.end ())
    {
      return;
    }

  // Step 2: Now retrieve an iterator through the pending blocks for this piece
  std::list<BlockRequested*>& pendingBlocks = (*npmIt).second.m_pendingBlocks;
  std::list<BlockRequested*>::iterator pbIt = pendingBlocks.begin ();

  // Step 3: Iterate through the pending blocks and remove the reference to the block given as the argument
  while (pbIt != pendingBlocks.end ())
    {
      // Only remove the one request which represents the request from OUR peer
      if (block == (**pbIt))
        {
          // Step 3a: Delete the reference to the object in the list of open requests for the peer it was assigned to
          RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.find (block.m_requestedFrom);
          if (rbmIt != m_requestedBlocks.end ())
            {
              std::list<BlockRequested*>::iterator pbIt2 = (*rbmIt).second.begin ();
              while (pbIt2 != (*rbmIt).second.end () && (*pbIt2) != (*pbIt))
                {
                  ++pbIt2;
                }
              if (pbIt2 != (*rbmIt).second.end ())
                {
                  (*rbmIt).second.erase (pbIt2);
                }
            }

          // Step 3b: If asked to, cancel the request with the remote peer
          if (cancel)
            {
              (**pbIt).m_requestedFrom->CancelRequest ((**pbIt).m_pieceIndex, (**pbIt).m_blockOffset, (**pbIt).m_blockLength);
            }

          // Step 3c: Remove the canceled request's peer from the data structure keeping track of them
          std::multiset<Ptr<Peer> >::iterator it = (*npmIt).second.m_requestedFrom.find ((**pbIt).m_requestedFrom);
          (*npmIt).second.m_requestedFrom.erase (it);

          // Step 3d: Delete the request
          delete (*pbIt);

          // Step 3e: Remove the initially found reference to this request
          pendingBlocks.erase (pbIt);

          // Step 3f: Break the loop since we have found the respective request for this piece
          break;
        }
      else
        {
          ++pbIt;
        }
    }
}

void PartSelectionStrategyBase::RemoveAllRequests (BlockRequested block, bool cancel)
{
  // Step 1: Get a reference to the piece this block belongs to
  NeededPiecesMap::iterator npmIt = m_neededPieces.find (block.m_pieceIndex);
  if (npmIt == m_neededPieces.end ())
    {
      return;
    }

  // Step 2: Now retrieve an iterator through the pending blocks for this piece
  std::list<BlockRequested*>& pendingBlocks = (*npmIt).second.m_pendingBlocks;
  std::list<BlockRequested*>::iterator pbIt = pendingBlocks.begin ();

  // Step 3: Iterate through the pending blocks and remove all references to the block given as the argument
  while (pbIt != pendingBlocks.end ())
    {
      if (block.requestEqualTo (**pbIt))
        {
          // Step 3a: Get an iterator to the next block requested for easy removal
          std::list<BlockRequested*>::iterator pbItNext = pbIt;
          pbItNext++;

          // Step 3b: Cancel the timeout event for this request
          (**pbIt).m_timeoutEvent.Cancel ();

          // Step 3c: Delete the reference to the object in the list of open requests for the peer it was assigned to
          RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.find (block.m_requestedFrom);
          if (rbmIt != m_requestedBlocks.end ())
            {
              std::list<BlockRequested*>::iterator pbIt2 = (*rbmIt).second.begin ();
              while (pbIt2 != (*rbmIt).second.end () && (*pbIt2) != (*pbIt))
                {
                  ++pbIt2;
                }
              if (pbIt2 != (*rbmIt).second.end ())
                {
                  (*rbmIt).second.erase (pbIt2);
                }
            }

          // Step 3d: If asked to, cancel the request with the remote peer
          if (cancel && (**pbIt).m_requestedFrom != block.m_requestedFrom)
            {
              (**pbIt).m_requestedFrom->CancelRequest ((**pbIt).m_pieceIndex, (**pbIt).m_blockOffset, (**pbIt).m_blockLength);
            }

          // Step 3e: Remove the cancelled request's peer from the data structure keeping track of them
          std::multiset<Ptr<Peer> >::iterator it = (*npmIt).second.m_requestedFrom.find ((**pbIt).m_requestedFrom);
          (*npmIt).second.m_requestedFrom.erase (it);

          // Step 3f: Delete the request
          delete (*pbIt);

          // Step 3g: Remove the initially found reference to this request
          pendingBlocks.erase (pbIt);

          // Step 3h: Set the loop's iterator to the correct new value
          pbIt = pbItNext;
        }
      else
        {
          ++pbIt;
        }
    }
}

void PartSelectionStrategyBase::ProcessBitfieldReceivedEvent (Ptr<Peer> peer)
{
  // Step 1 (and only): Reception of a bitfield kicks off the periodic piece scheduling
  if (!m_myClient->GetDownloadCompleted ())
    {
      m_requestedBlocks[peer] = std::list<BlockRequested*> ();
      ProcessPeriodicSchedule ();
    }
}

void PartSelectionStrategyBase::ProcessPeerChokeChangingEvent (Ptr<Peer> peer)
{
  // Step 1: If the peer is choking us, immediately remove all currently running requests
  if (peer->IsChoking ())
    {
      RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.find (peer);
      if (rbmIt == m_requestedBlocks.end ())
        {
          return;
        }

      std::list<BlockRequested*>& requestedBlocks = (*rbmIt).second;
      while (!requestedBlocks.empty ())
        {
          // Step 1: Get a pointer to the object representing the requested block
          BlockRequested* block = requestedBlocks.front ();

          // Step 2: Cancel the timeout event for this piece
          (*block).m_timeoutEvent.Cancel ();

          // Step 3: Delete the reference to this block request in the list of needed pieces
          NeededPiecesMap::iterator npmIt = m_neededPieces.find ((*block).m_pieceIndex);
          if (npmIt != m_neededPieces.end ())
            {
              (*npmIt).second.m_pendingBlocks.remove (block);

              // Step 3a: Also remove the reference to the requesting peer
              std::multiset<Ptr<Peer> >::iterator it = (*npmIt).second.m_requestedFrom.find (peer);
              (*npmIt).second.m_requestedFrom.erase (it);
            }

          // Step 4: Delete the object
          delete block;

          // Step 5: Remove the object reference
          requestedBlocks.pop_front ();
        }
    }

  // Step 2: Immediately trigger the scheduler
  Scheduler ();
}

void PartSelectionStrategyBase::ProcessPeerHaveEvent (Ptr<Peer> peer, uint32_t pieceIndex)
{
  // Trigger the scheduler if we were informed about the availability of a piece we need
  if (m_neededPieces.find (pieceIndex) != m_neededPieces.end ())
    {
      Scheduler ();
    }
}

void PartSelectionStrategyBase::ProcessPeerBlockCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  NS_LOG_INFO ("Peer " << peer->GetRemoteIp () << " sent me block " << pieceIndex << "@" << blockOffset << "->" << blockOffset + blockLength << ".");

  // Step 1: Get an iterator to the actual corresponding data structure entry for this block
  RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.find (peer);
  if (rbmIt == m_requestedBlocks.end ())
    {
      return;
    }
  std::list<BlockRequested*>::iterator it = (*rbmIt).second.begin ();
  BlockRequested block (peer, pieceIndex, blockOffset, blockLength);
  while ((it != (*rbmIt).second.end ()) && !((**it) == block))
    {
      ++it;
    }

  // Step 1a: If we have not found the block, we have not requested it (from this peer) and drop the message
  if (it == (*rbmIt).second.end ())
    {
      return;
    }

  // Step 2: Remove all instances to the block in all our data structures and cancel all pending requests
  RemoveAllRequests (block, true);

  // Step 3: Now, mark this block as received by removing its entry from the list of "possible blocks"
  NeededPiecesMap::iterator npmIt = m_neededPieces.find (pieceIndex);
  if (npmIt == m_neededPieces.end ())
    {
      return;
    }
  (*npmIt).second.m_possibleBlocks.remove (std::pair<uint32_t, uint32_t> (blockOffset, blockOffset + blockLength));

  // Step 4: If our request information is empty and the piece has been successfully downloaded (checksum), mark the whole piece as finished
  if ((*npmIt).second.m_possibleBlocks.empty ())
    {
      // Step 4.1: If checksums are enabled, check the downloaded piece
      bool pieceOk = true;
      if (m_myClient->GetCheckDownloadedData ())
        {
          const uint8_t* pieceCorruptionMap = peer->GetPieceCorruptionMap ();
          pieceOk = (pieceCorruptionMap[pieceIndex] == BT_PEER_PIECE_RECEPTION_CHECKSUM_OK);
        }

      // Step 4.2a: React to the successful download of the piece
      if (pieceOk)
        {
          NS_LOG_INFO ("Piece " << pieceIndex << " successfully downloaded.");

          // Step 4.2a1: Remove the piece from the data structure containing needed pieces
          m_neededPieces.erase (npmIt);

          // Step 4.2a2: Process the completed piece
          ProcessCompletedPiece (pieceIndex);

          // Step 4.2a3: Issue an event indicating the completion of this piece by the sending peer
          m_myClient->PieceCompleteEvent (peer, pieceIndex);
        }
      else              // Step 4.2b: In case the piece was not successfully downloaded, re-enter all blocks into the data structure for neeeded blocks
        {
          NS_LOG_INFO ("Piece " << pieceIndex << " was corrupted. Re-entering into needed pieces.");

          m_neededPieces.erase (npmIt);
          PieceNeeded p = GeneratePieceNeeded (pieceIndex);
          m_neededPieces.insert (std::pair<uint32_t, PieceNeeded> (pieceIndex, p));
        }
    }

  // Step 5: Call the scheduler to assign a new request to the peer
  Scheduler ();
}

void PartSelectionStrategyBase::ProcessPeerConnectionCloseEvent (Ptr<Peer> peer)
{
  // A closed connection more or less is the same as a choked connection. Plus: We delete the references to this peer totally
  ProcessPeerChokeChangingEvent (peer);
  RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.find (peer);
  if (rbmIt != m_requestedBlocks.end ())
    {
      m_requestedBlocks.erase (rbmIt);
    }
  RequestedPiecesMap::iterator rpmIt = m_requestedPieces.find (peer);
  if (rpmIt != m_requestedPieces.end ())
    {
      m_requestedPieces.erase (rpmIt);
    }
}

void PartSelectionStrategyBase::ProcessRequestTimeout (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  // Step 1: Remove the request information for this block, cancelling the piece with the remote peer
  NS_LOG_INFO ("Request " << pieceIndex << "@" << blockOffset << "->" << blockOffset + blockLength << " timed out from peer " << peer->GetRemoteIp () << ".");

  // Step 2: Remvove the request from the data structures
  RemoveRequest (BlockRequested (peer, pieceIndex, blockOffset, blockLength), true);

  // Step 3: If no other block of this piece is wanted anymore, issue a PieceCancelledEvent
  if (m_neededPieces[pieceIndex].m_pendingBlocks.size () == 0)
    {
      m_myClient->PieceTimeoutEvent (peer, pieceIndex);
      m_myClient->PieceCancelledEvent(peer, pieceIndex);
    }
}

void PartSelectionStrategyBase::ProcessCompletedPiece (uint32_t pieceIndex)
{
  // Step 1: Update the client's bitfield
  m_myClient->SetPieceComplete (pieceIndex);

  // Step 2: Send out have messages to all associated clients
  std::vector<Ptr<Peer> >::const_iterator it = m_myClient->GetPeerListIterator ();
  for (; it != m_myClient->GetPeerListEnd (); ++it)
    {
      (*it)->SendHaveMessage (pieceIndex);
    }

  // Step 3: Check if the download is completed (possibly the most important function call in the whole simulation ;=))
  CheckDownloadCompleted ();
}

void PartSelectionStrategyBase::CheckDownloadCompleted ()
{
  if (m_myClient->GetPiecesCompleted () == m_myClient->GetTorrent ()->GetNumberOfPieces ())
    {
      NS_LOG_INFO ("WOHOOOO! " << m_myClient->GetNode ()->GetId () << " (PeerID " << m_myClient->GetPeerId () << ") has completed download!");

      for (std::vector<Ptr<Peer> >::const_iterator it = m_myClient->GetPeerListIterator (); it != m_myClient->GetPeerListEnd (); ++it)
        {
          (*it)->SetAmInterested (false);
        }

      m_myClient->DownloadCompleteEvent ();
    }
}

bool PartSelectionStrategyBase::RequestAllowedForBlock (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  /*
   * We must check for several criteria:
   * 1) The piece must be wanted
   * 2) The number of already-pending blocks for this piece must not be exceeded
   * 3) The number of already-pending blocks for this peer must not be exceeded
   * 4) The number of already-pending requests for this piece for the given peer must not be exceeded
   * 5) The number of already-pending requests for this particular block must not be exceeded (!= criterion 2!)
   */

  // Criterion 1: The piece must be wanted
  NeededPiecesMap::iterator npmIt = m_neededPieces.find (pieceIndex);
  if (npmIt == m_neededPieces.end ())
    {
      return false;
    }
  // Criterion 2: Not more than the allowed number of block requests per piece
  else if ((*npmIt).second.m_pendingBlocks.size () >= m_myClient->GetMaxRequestsPerPiece ())
    {
      return false;
    }
  // Criterion 3: Not more than the allowed number of requests for this peer
  else if (m_requestedBlocks[peer].size () >= m_myClient->GetMaxRequestsPerPeer ())
    {
      return false;
    }
  // Criterion 4: Not more than the allowed number of requests for this piece for the given peer
  else if ((*npmIt).second.m_requestedFrom.count (peer) >= m_myClient->GetMaxRequestsPerPeerPerPiece ())
    {
      return false;
    }
  // Criterion 5: Not more than the allowed number of requests for this block
  else
    {
      BlockRequested toFind (pieceIndex, blockOffset, blockLength);
      uint16_t leftRequests = m_myClient->GetMaxRequestsPerBlock ();

      std::list<BlockRequested*>::iterator it = (*npmIt).second.m_pendingBlocks.begin ();
      while (leftRequests > 0 && it != (*npmIt).second.m_pendingBlocks.end ())
        {
          if (toFind.requestEqualTo (**it))
            {
              --leftRequests;
              if ((**it).m_requestedFrom == peer)
                {
                  return false;
                }
            }

          ++it;
        }

      return leftRequests > 0;
    }
}

void PartSelectionStrategyBase::GetHighestPriorityBlockForPeer (Ptr<Peer> peer, BlockRequested& blockPtr)
{
  // Step 1: Prepare the passed block so far we can so that accessing it after this function does not result in undesired behavior
  blockPtr.m_requestedFrom = peer;
  blockPtr.m_blockLength = 0;

  // Step 2: Calculate the timeout of a piece by taking into account how many requests are already running for this peer
  blockPtr.m_timeoutTime =
    Simulator::Now () +
    MilliSeconds ((1 + m_requestedBlocks[peer].size ()) * m_myClient->GetPieceTimeout ().GetMilliSeconds () / m_blocksPerPiece);

  // Step 3: Return needed blocks for piece in-order
  NeededPiecesMap::iterator npmIt = m_neededPieces.begin ();
  while (npmIt != m_neededPieces.end ())
    {
      std::list<std::pair<uint32_t, uint32_t> >::iterator blockIt = (*npmIt).second.m_possibleBlocks.begin ();
      while (blockIt != (*npmIt).second.m_possibleBlocks.end ())
        {
          if (RequestAllowedForBlock (peer, (*npmIt).second.m_pieceIndex, (*blockIt).first, (*blockIt).second - (*blockIt).first))
            {
              blockPtr.m_pieceIndex = (*npmIt).second.m_pieceIndex;
              blockPtr.m_blockOffset = (*blockIt).first;
              blockPtr.m_blockLength = (*blockIt).second - (*blockIt).first;
              npmIt = m_neededPieces.end ();
              --npmIt;
              break;
            }
          else
            {
              ++blockIt;
            }
        }

      ++npmIt;
    }
}

void PartSelectionStrategyBase::Scheduler ()
{
  // Step 1: Get the list of available peers
  const std::vector<Ptr<Peer> > &peerlist = m_myClient->GetActivePeers ();

  // Step 2: If we have already finished downloading or if we have no active peers, do nothing
  if (m_myClient->GetDownloadCompleted () || peerlist.size () == 0)
    {
      return;
    }

  // Step 3: Randomly iterate through the list of peers and assign requests to them if applicable
  std::list<uint32_t> requestOrder = GetPeerOrderForScheduler ();
  for (std::list<uint32_t>::const_iterator it = requestOrder.begin (); it != requestOrder.end (); ++it)
    {
      Ptr<Peer> currentPeer = peerlist[(*it) - 1];
      RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.find (currentPeer);
      if (rbmIt == m_requestedBlocks.end ())         // In case one of our data structures went off the line,
        {
          continue;
        }
      std::list<BlockRequested*>& requestedBlocks = (*rbmIt).second;

      // Step 3a: We send out as many requests per peer as our client's setting allows us to
      while (requestedBlocks.size () < m_myClient->GetMaxRequestsPerPeer ())
        {
          BlockRequested* block = new BlockRequested ();
          GetHighestPriorityBlockForPeer (currentPeer, *block);

          // Step 3a1: If the blockLength is > 0, we found a valid request for this peer
          if ((*block).m_blockLength > 0)
            {
              // Step 3a1a: If the peer is choking us, the only thing we can do is announce our interest in the peer
              if (currentPeer->IsChoking ())
                {
                  currentPeer->SetAmInterested (true);
                  
                  // OLD
                  // break;
                  
                  // MEMORY
                  delete block;
                  break;
                  // /MEMORY
                }

              // Step 3a1b: Set up timeouts for this block
              (*block).m_timeoutEvent = Simulator::Schedule ((*block).m_timeoutTime - Simulator::Now (), &PartSelectionStrategyBase::ProcessRequestTimeout, this, currentPeer, (*block).m_pieceIndex, (*block).m_blockOffset, (*block).m_blockLength);

              // Step 3a1c: Send out and save the request
              RequestBlock (block);
            }
          // Step 3a2: Else, we break looking for further available blocks and, if needed, signalize no interest to the peer
          else
            {
              // Step 3a2a: Delete the block
              delete block;

              // Step 3a2b: If there are no reuqested blocks for this peer, set our status to uninterested (we have no requests left)
              if (requestedBlocks.size () == 0)
                {
                  currentPeer->SetAmInterested (false);
                }
              break;
            }
        }
    }
}

inline std::list<uint32_t> PartSelectionStrategyBase::GetPeerOrderForScheduler ()
{
  return Utilities::GetPermutationP (m_myClient->GetActivePeers ().size (), m_myClient->GetActivePeers ().size ());
}

void PartSelectionStrategyBase::ProcessPeriodicSchedule ()
{
  if (!m_myClient->GetDownloadCompleted ())
    {
      bool goOn = true;

      // If we haven't scheduled the disconnection of a peer, we can start our piece selection scheduler
      if (goOn)
        {
          Scheduler ();
        }
      else // else, we delay the execution of the scheduler until the disconnections have been completed
        {
          /*
           * Note: ScheduleNow won't work since disconnecting peers also trigger events,
           * which would inevitably be AFTER this scheduling here, meaning that requests
           * may be scheduled for peers that will immediately close connection. So we delay
           * the scheduler for a minimum amount of time (ALL now-scheduled events are always
           * executed before any non-now scheduled one)
           */
          Simulator::Schedule (MilliSeconds (1), &PartSelectionStrategyBase::Scheduler, this);
        }

      m_nextPeriodicEvent = Simulator::Schedule (m_periodicInterval, &PartSelectionStrategyBase::ProcessPeriodicSchedule, this);
    }
}

void PartSelectionStrategyBase::DebugPrint ()
{
  std::cout << "Needed pieces -->" << std::endl;
  NeededPiecesMap::iterator npmIt = m_neededPieces.begin ();
  while (npmIt != m_neededPieces.end ())
    {
      std::cout << "		Piece "<< (*npmIt).second.m_pieceIndex << std::endl;

      std::cout << "			Possible blocks: "<< std::endl;
      std::list<std::pair<uint32_t, uint32_t> >::iterator it = (*npmIt).second.m_possibleBlocks.begin ();
      while (it != (*npmIt).second.m_possibleBlocks.end ())
        {
          std::cout << "				"<< (*it).first << "->" << (*it).second << std::endl;
          ++it;
        }

      std::cout << "			Pending blocks: "<< std::endl;
      std::list<BlockRequested*>::iterator it2 = (*npmIt).second.m_pendingBlocks.begin ();
      while (it2 != (*npmIt).second.m_pendingBlocks.end ())
        {
          std::cout << "				"<< (**it2).m_pieceIndex << "@" << (**it2).m_blockOffset << "->" << (**it2).m_blockOffset + (**it2).m_blockLength << " from " << (**it2).m_requestedFrom->GetRemoteIp () << std::endl;
          ++it2;
        }

      ++npmIt;
    }
  std::cout << "<-- Needed pieces" << std::endl;

  std::cout << "Requested blocks -->" << std::endl;
  RequestedBlocksMap::iterator rbmIt = m_requestedBlocks.begin ();
  while (rbmIt != m_requestedBlocks.end ())
    {
      std::cout << "		Requests for "<< (*rbmIt).first->GetRemoteIp () << std::endl;
      std::list<BlockRequested*>::iterator it = (*rbmIt).second.begin ();
      while (it != (*rbmIt).second.end ())
        {
          std::cout << "				"<< (**it).m_pieceIndex << "@" << (**it).m_blockOffset << "->" << (**it).m_blockOffset + (**it).m_blockLength << " from " << (**it).m_requestedFrom->GetRemoteIp () << std::endl;
          ++it;
        }

      ++rbmIt;
    }
}

} // ns bittorrent
} // ns ns3
