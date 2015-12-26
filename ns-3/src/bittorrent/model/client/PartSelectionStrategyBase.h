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

#ifndef PARTSELECTIONSTRATEGY_H_
#define PARTSELECTIONSTRATEGY_H_

#include "AbstractStrategy.h"

#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include <map>
#include <list>
#include <set>

namespace ns3 {
namespace bittorrent {

class BitTorrentClient;
class Peer;

/**
 * \ingroup BitTorrent
 *
 * \brief Implements the basic strategy used to the select the parts of the file to download next.
 *
 * This class provides a basis for the implementation of part selection strategies in BitTorrent-based protocols. It keeps track of all
 * possible part (= block) requests and currently-running requests, accessible by-block as well as by-peer, and manages concurrent requests.
 *
 * The general procedure for the creation a new part selection strategy, as illustrated by the RarestFirstPartSelectionStrategy class, is the
 * following:\n
 * 1. Derive a class from the PartSelectionStrategyBase class (or any derived classes)\n
 * 2. Override the DoInitialize method and register all events handled by the base class with the BitTorrentClient class:\n
 *  2a. For all events that you do not wish to alter the behavior for, create a callback to the base classes event handler\n
 *  2b. For all events that you DO want to alter the behavior for, create (or override) an event handler and create a callback to that method.
 *  Chain up the base class' reaction at the end of your implementation by calling BaseClass::EventHandlerMethod().\n
 * 3. Override the GetHighestPriorityBlockForPeer and / or GetPeerOrderForScheduler methods to implement the desired new part selection behavior.
 */
class PartSelectionStrategyBase : public AbstractStrategy
{
// Internal classes and types used
protected:
  /**
   * \ingroup BitTorrent
   *
   * \brief Lightweight class holding information about a block requested by (derivates of) the PartSelectionStrategyBase class.
   *
   * This class is used to centrally store all information available about a currently-pending request for a block sent to a peer.
   */
  class BlockRequested
  {
  public:
    uint32_t m_pieceIndex;
    uint32_t m_blockOffset;
    uint32_t m_blockLength;

    Time m_timeoutTime;
    EventId m_timeoutEvent;

    Ptr<Peer> m_requestedFrom;

  public:
    /// @cond HIDDEN
    BlockRequested ()
    {
    }
    /// @endcond HIDDEN

    /**
     * \brief Construct a general requested block.
     *
     * @param pieceIndex the index of the piece.
     * @param blockOffset the offset (in bytes) of the requested block within the piece.
     * @param blockLength the length (in bytes) of the requested block within the piece.
     */
    BlockRequested (uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
    {
      m_pieceIndex = pieceIndex;
      m_blockOffset = blockOffset;
      m_blockLength = blockLength;
    }

    /**
     * \brief Main constructor for the BlockRequested class.
     *
     * @param requestedFrom pointer to the local Peer class instance representing the peer from which the block was requested.
     * @param pieceIndex the index of the piece.
     * @param blockOffset the offset (in bytes) of the requested block within the piece.
     * @param blockLength the length (in bytes) of the requested block within the piece.
     */
    BlockRequested (Ptr<Peer> requestedFrom, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
    {
      m_pieceIndex = pieceIndex;
      m_blockOffset = blockOffset;
      m_blockLength = blockLength;
      m_requestedFrom = requestedFrom;
    }

  public:
    /**
     * @returns true, if requested piece, block offset and block length match.
     */
    bool requestEqualTo (const BlockRequested& other) const
    {
      return
        m_pieceIndex == other.m_pieceIndex
        && m_blockOffset == other.m_blockOffset
        && m_blockLength == other.m_blockLength;

    }

    /**
     * @returns true, if requested piece, block offset, block length <b>and</b> the peer receiving the request match.
     */
    bool operator == (const BlockRequested& other) const
    {
      return
        requestEqualTo (other)
        && m_requestedFrom == other.m_requestedFrom;
    }
    bool operator != (const BlockRequested& other) const
    {
      return !(*this == other);
    }
  };

  /**
   * \ingroup BitTorrent
   *
   * \brief Lightweight class holding information about a piece needed by the local client., together with possible blocks to download and pending blocks.
   *
   * This class centrally stores all information about a piece that an instance of the BitTorrent client needs to download, or is currently downloading.
   * These information include the blocks possible to download (depending on the settings for request size in the BitTorrentClient class), the pending
   * blocks and all peers currently asked for some part of the respective piece.
   */
  class PieceNeeded
  {
  public:
    uint32_t m_pieceIndex;

    std::list<std::pair<uint32_t, uint32_t> > m_possibleBlocks;
    std::list<BlockRequested*> m_pendingBlocks;
    std::multiset<Ptr<Peer> > m_requestedFrom;

    Time m_timeoutTime;
    EventId m_timeoutEvent;

  public:
    /// @cond HIDDEN
    PieceNeeded ()
    {
      m_pieceIndex = 0;
    }
    /// @endcond HIDDEN

    /**
     * \brief Create an instance of the PieceNeeded class for a specific piece.
     *
     * Note: Members other than the index of the piece for which the instance stands are not filled by this constructor.
     * Use specific methods provided by the PartSelectionStrategyBase class or its derivatives to generate properly-initialized
     * versions of this class.
     */
    PieceNeeded (uint32_t pieceIndex)
    {
      m_pieceIndex = pieceIndex;
    }
  };

  typedef std::map<uint32_t, PieceNeeded>                       NeededPiecesMap;         // Stores information about pieces that still need to be downloaded
  typedef std::map<Ptr<Peer>, std::list<BlockRequested*> >      RequestedBlocksMap;      // Stores information about blocks currently pending, sorted by peer
  typedef std::map<Ptr<Peer>, std::set<uint32_t> >              RequestedPiecesMap;      // Stores information about pieces currently pending, sorted by peer

// Fields
protected:
  // Main data structures
  NeededPiecesMap            m_neededPieces;               // Holds information for all blocks (by piece) which have not yet been downloaded
  RequestedBlocksMap         m_requestedBlocks;            // The blocks which are currently being downloaded, by peer
  RequestedPiecesMap         m_requestedPieces;            // The pieces which are currently being downloaded, by peer

  // Settings
  Time                       m_periodicInterval;           // The time span between trying to assign piece REQUESTs to peers, if no other event (like HAVE messages) occur in-between
  EventId                    m_nextPeriodicEvent;          // The next scheduled assignment event
  uint32_t                   m_blocksPerPiece;             // The number of blocks that each piece is divided into; for easier calculation of timeouts (see constructor)

  // Heuristics (not used as of now)
  /// @cond HIDDEN
  Time                       m_requestPatience;
  Time                       m_chokingPatience;
  Time                       m_downloadPatience;
  /// @endcond HIDDEN

// Constructors etc.
public:
  PartSelectionStrategyBase (Ptr<BitTorrentClient> myClient);
  virtual ~PartSelectionStrategyBase ();

  /**
   * \brief Initialze the strategy. Register the needed event listeners with the associated client.
   */
  virtual void DoInitialize ();

  /**
   * \brief React to in-operation changes of strategy parameters.
   *
   * Note: When overriding this method, make sure to first call the base implementation to re-initialize the basic data structures accordingly.
   */
  virtual void ProcessStrategyOptionsChangedEvent ();

// Getters, setters
public:
  /**
   * @returns the interval at which the client should try sending requests for missing blocks to peers.
   */
  Time GetPeriodicInterval () const;

  /**
   * \brief Set the interval at which the client should try to send requests for missing blocks to peers.
   *
   * This setting controls the periodicity with which the block downloading scheduling function is called.
   *
   * Note: This setting controls the maximum interval at which the scheduling function is called. Appropriate
   * events (such as the reception of a HAVE message for a needed piece from a peer) also cause the scheduling
   * function to be triggered.
   *
   * The setting is applied at the end of the current scheduling round, i.e., it takes at most GetPeriodicInterval () until the new value is adopted.
   *
   * @param periodicInterval the desired time that shall elapse between the client's tries to send requests to peers. Ignored, if not strictly positive.
   */
  void SetPeriodicInterval (Time periodicInterval);

// Internal methods
protected:
  /**
   * \brief Generate possible requests for a missing piece to send out to peers.
   *
   * This method subdivides the given piece into blocks according to the settings in the (derivatives of the) BitTorrentClient class regarding
   * piece request size and returns a list of possible requests to send out to peers.
   *
   * You can override this method to implement differing "chunkings" of certain pieces.
   *
   * Note: The logic within the PartSelectionStrategyBase class regards a piece to be downloaded when no further requests are available, i.e.,
   * when the list of possible blocks in the PieceNeeded object returned by this function is empty. You should make sure that the chunks returned
   * by this method completely cover a piece. Disjointness of the requested parts of the file is, however, not necessary.
   *
   * Also note: This function will always generate a possible chunkization of the given piece, even if it was already downloaded.
   *
   * @param pieceIndex the piece for which the list of possible requests shall be generated.
   *
   * @returns a PieceNeeded object containing possible requests for the given piece in its m_possibleBlocks member.
   */
  virtual PieceNeeded GeneratePieceNeeded (uint32_t pieceIndex);

  /**
   * \brief Initialze the map of needed pieces.
   *
   * This method checks for each piece that the shared file consists of whether it was already downloaded and creates possible blocks to download
   * by calling the GeneratePieceNeeded member function for non-downloaded pieces.
   */
  void InitializeNeededPiecesMap ();

  /**
   * \brief Request a block and mark it as pending.
   *
   * @param block a pointer to the block to be requested. Usually a pointer to a BlockRequested filled by the GetHighestPriorityBlockForPeer member.
   */
  void RequestBlock (BlockRequested* block);

  /**
   * \brief Cancel a pending block and mark it as needed again.
   *
   * This method searches the internal data structures for the existence of the given block and issues a CANCEL message to the associated peer.
   *
   * @param block the block to cancel.
   */
  void CancelRequest (BlockRequested block);

  /**
   * \brief Internal method. Save a requested block in the internal data structures to mark it as a pending download.
   *
   * This method also updates the data structures holding requests sent to a specifc peer.
   *
   * @param block pointer to the block to be inserted.
   */
  void SaveRequest (BlockRequested* block);

  /**
   * \brief Internal method. Remove a block requested from one peer from the internal data structures.
   *
   * This method also updates the data structures holding requests sent to a specifc peer.
   *
   * @param block the block to removed.
   * @param cancel if set to true, a CANCEL message for the given block is issued when it is found.
   */
  void RemoveRequest (BlockRequested block, bool cancel);

  /**
   * \brief Internal method. Remove a requested block from the internal data structures for all peers.
   *
   * This method works similarly to the RemoveRequest method but ignores the peer from which the block was requested,
   * removing all found instances of all block requests.
   *
   * This method is usually called upon the reception of a block from a peer to cancel other, concurrent requests for
   * that block to avoid downloading it multiple times.
   *
   * @param block the block to removed. The m_requestedFrom member of the block is ignored.
   * @param cancel if set to true, a CANCEL message for the given block is issued when it is found.
   */
  void RemoveAllRequests (BlockRequested block, bool cancel);

// Event listeners
public:
  // BitTorrent event listeners (PeerWireProtocol, other events)

  /**
   * \brief This method starts the periodic piece sheduling upon the reception of a bitfield.
   *
   * It is safe to call this method "off schedule".
   */
  virtual void                    ProcessBitfieldReceivedEvent (Ptr<Peer> peer);

  /**
   * \brief Process a changed choking status.
   *
   * This method reacts to a peer choking the client by immediately cancelling all pending block requests
   * and marking them as non-pending.
   */
  virtual void ProcessPeerChokeChangingEvent (Ptr<Peer> peer);

  /**
   * \brief This method starts scheduling of pieces when information about a needed piece arrived.
   */
  virtual void ProcessPeerHaveEvent (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief Process a completed piece.
   *
   * This method is called upon the completion of a block transfer from a peer. It checks whether with the respective block transfer caused the
   * piece to be completely transferred. It then performs a check of the received piece (via a direct memory comparison with the shared file)
   * and issues a PieceCompleteEvent with the associated BitTorrentClient class in case the piece passed the check. If the piece was found to
   * be corrupted, it re-initiates the piece download by calling the GeneratePieceNeeded method and inserting the resulting object into the
   * internal data structures for needed pieces.
   *
   * Note: For reasons of simplicity, only the peer sending the block successfully completing the download of a piece is announced as the
   * transferring peer in the PieceCompleteEvent. Derived classes may override this method to implement more advanced behavior.
   *
   * @param peer the peer from which the block was received.
   * @param pieceIndex the index of the piece in the bitfield that the received block belongs to.
   * @param blockOffset the offset (in bytes) of the block to request within the piece.
   * @param blockLength the length of the block to request.
   */
  virtual void ProcessPeerBlockCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  /**
   * \brief Process a closed connection.
   *
   * This method reacts to the closure of a connection with a peer by removing all currently-pending requests from the internal data structures
   * to avoid unnecessary time-outs.
   */
  virtual void ProcessPeerConnectionCloseEvent (Ptr<Peer> peer);

  // Client-internal listeners

  /**
   * \brief React to block requests that timed out.
   *
   * The default behavior of this method is to simply remove the timed-out block and mark the block as non-pending. You may override this
   * method to implement more advanced behavior.
   */
  virtual void ProcessRequestTimeout (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  // Semi-listener methods

  /**
   * \brief React to a completed piece.
   *
   This is done by marking the piece as downloaded in all internal data structures, sending out HAVE messages to all
   * connected clients and calling the CheckDownloadCompleted method to check for the completion of the download of the whole shared file.
   *
   * @param pieceIndex the piece just downloaded.
   */
  virtual void ProcessCompletedPiece (uint32_t pieceIndex);

  /**
   * \brief Check the bitfield of the client for completeness of the file and issue a DownloadCompleted event in that case.
   *
   * This method additionally sends not interested messages to all associated peers.
   */
  virtual void CheckDownloadCompleted ();

// Strategy implementation methods
protected:

  /**
   * \brief Check whether a request for a block is "allowed" according to the strategy's heuristics and the client's settings.
   *
   * This method is intended to perform checks on a block download suggestion according to checks defined within the strategy.
   * Such checks may include the number of concurrent requests from a single peer, overall number of pending requests, time (delay)-related
   * heuristics etc. The standard implementation checks for any violations of the client's general settings that a request might cause. If
   * any violation is detected, the request is forbidden (return value false).
   *
   * Note: Depending on the part selection strategy implementation, ignoring the result of this method may lead to inconsistencies within
   * the data structures of the strategy when issuing the request. This is indeed the case for the implementation in the PartSelectionStrategyBase class,
   * which is why any derived implementations should first check for the decision of the base classes and only proceed if the base classes allowed the request.
   *
   * @param peer the peer from which the block should be requested.
   * @param pieceIndex the index of the piece in the bitfield that the block belongs to.
   * @param blockOffset the offset (in bytes) of the block to request within the piece.
   * @param blockLength the length of the block to request.
   *
   * @returns true, if the checks performed allow the block to be requested from the given peer.
   */
  virtual bool RequestAllowedForBlock (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  /**
   * \brief Get the next block to download from a peer.
   *
   * This method implements the part selection heuristics of a specific strategy. For a given peer, the method returns the block of highest
   * priority (and, hence, the natural choice for a request). In the standard implementation, this method returns all needed blocks in a
   * sequential manner, i.e., the first fitting non-downloaded block is returned. You can override this method to provide any desired behavior.
   *
   * In case of a failure (e.g., if no suitable block is found for the given peer), the method is expected to set the
   * m_blockLength member of the BlockRequested object to 0 (zero).
   *
   * @param peer pointer to the Peer class instance representing the peer for which the block of highest priority shall be computed.
   * @param blockPtr pointer to a BlockRequested instance in which the block of highest priority shall be stored.
   */
  virtual void GetHighestPriorityBlockForPeer (Ptr<Peer> peer, BlockRequested& blockPtr);

// Helper methods
protected:

  /**
   * \brief Assign block requests to peers.
   *
   * This method randomly iterates through the list of available peers and for each peer that has unchoked the client
   * requests the blocks of highest priority (according to the GetHighestPriorityBlockForPeer member method), up to the
   * maximum number of requests per peer set via the BitTorrentClient::SetMaxRequestsPerPeer method. If a peer was assigned
   * a block but was not unchoked, the methods sends out an INTERESTED message to the peer to ask for an unchoking of the client.
   *
   * You may override this method to change the order of peers in the process.
   */
  virtual void Scheduler ();

  /**
   * \brief Generate a permutation of the peers to use in the block request scheduling process.
   *
   * You may override this method to change the order of peers in the request scheduling process.
   *
   * @returns a list of indices used to traverse the list of available peers in the scheduling process.
   *  The inclueded values must be in the range [0..BitTorrentClient::GetConnectedPeerCount()].
   */
  virtual std::list<uint32_t> GetPeerOrderForScheduler ();
  // Periodically-called function that may check include certain heuristics and then calles Scheduler() if needed

  /**
   * \brief This method checks whether the download has already finished and if not, call the scheduling function to issue further requests to peers.
   *
   * The periodicity of this method is determined by the SetPeriodicInterval member function.
   *
   * It is safe to call this function "out of schedule".
   */
  virtual void ProcessPeriodicSchedule ();

// Debug
public:
  /// @cond HIDDEN
  void DebugPrint ();
  /// @endcond HIDDEN
};

} // ns bittorrent
} // ns ns3

#endif /* PARTSELECTIONSTRATEGY_H_ */
