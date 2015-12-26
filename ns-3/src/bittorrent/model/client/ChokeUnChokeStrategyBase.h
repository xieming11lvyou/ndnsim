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

#ifndef CHOKEUNCHOKESTRATEGYBASE_H_
#define CHOKEUNCHOKESTRATEGYBASE_H_

#include "AbstractStrategy.h"
#include "BitTorrentPeer.h"

#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include <list>
#include <map>
#include <set>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Implements the standard BitTorrent choking/unchoking mechanism, including optimistic unchoking.
 *
 * This class provides a basic implementation of BitTorrent's choking/unchoking mechanism as described in the
 * <a href="http://wiki.theory.org/BitTorrentSpecification#Choking_and_Optimistic_Unchoking" target="_blank">BitTorrent Protocol Specification v1.0</a>.
 *
 * While there are less overall connections available than the allowed maximum of unchoked peers, the mechanism automatically unchokes
 * every newly-interested peer instantaneously. After this period, it switches to the standard, periodic choking/unchoking process including
 * optimistic unchoking of exactly one peer each round. Both the periodicity of the whole choking/unchoking process and the number of rounds that
 * each optimistically unchoked peer can "survive" can be controlled. When the client looses connection to a peer, the strategy instantaneously unchokes
 * another peer, if available. For settings regarding the number of desired and maximum unchoked peers, see the BitTorrentClient class.
 *
 * Precisely, the choking/unchoking mechanism in period:\n
 * 1. Virtually chokes all available peers\n
 * 2. Sorts all peers according to their last known state (interested / uninterested, choked / unchoked)\n
 * 3. Gets the best peers to unchoke (GetCandidateForInsertion method)
 * 4. Finds the set of currently-unchoked peers that may be replaced by the peers found in 3. (GetCandidateForRemoval method)\n
 * 5. Finds uninterested peers to unchoke (according to a heuristic also described in the <a href="http://wiki.theory.org/BitTorrentSpecification#Choking_and_Optimistic_Unchoking" target="_blank">BitTorrent Protocol Specification v1.0</a>\n
 * 6. Chooses the peer to unchoke optimistically, if needed (with a bias towards newly-connected peers as described in the specification)\n
 * 7. Sends out CHOKE / UNCHOKE messages to only those peers which have actually a change in their status (i.e., e.g., peers staying unchoked do not receive another UNCHOKE message)
 */
class ChokeUnChokeStrategyBase : public AbstractStrategy
{
// Fields
protected:
  // General settings
  Time                       m_periodicInterval;      // The interval at which the choking/unchoking mechanism should be triggered

  // Fields indicating which peers belong to which group in the choking/unchoking mechanism
  std::set<Ptr<Peer> >       m_unchokedPeers;         // Currently unchoked peers
  std::set<Ptr<Peer> >       m_downloadingPeers;      // Currently downloading peers (subset of m_unchokedPeers)
  std::list<Ptr<Peer> >      m_newlyConnectedPeers;   // Peers connected after the last triggering of the choking/unchoking mechanism
  std::set<Ptr<Peer> >       m_interestedPeers;       // Peers who have last sent an INTERESTED message
  std::set<Ptr<Peer> >       m_uninterestedPeers;     // Peers who have last sent an UNINTERESTED message

  // Fields related to optimistic unchoking
  uint32_t                   m_optimisticMultiple;    // Optimistic unchoking should only appear every m_optimisticMultiple of all triggerings
  uint8_t                    m_periodicScheduleRound; // Corresponsds to the number of rounds taken since the last optimistic unchoke (see m_optimisticMultiple)

  uint16_t                   m_newlyConnectedPeersSinceLastPeriod;   // The number of newly connected peers since the last optimistic unchoking of peers

  Ptr<Peer>                  m_optimisticallyUnchokedPeer;           // The currently optimistically unchoked peers

  // The next periodic choking/unchoking event
  EventId                    m_nextPeriodicEvent;     // The next periodic choking/unchoking event

// Constructors etc.
public:
  ChokeUnChokeStrategyBase (Ptr<BitTorrentClient> myClient);
  virtual ~ChokeUnChokeStrategyBase ();

  virtual void DoInitialize ();

// Getters, setters
public:
  /**
   * \brief Get the choking/unchoking interval.
   *
   * @returns the interval at which the choking/unchoking of connected peers is executed.
   */
  Time GetPeriodicInterval () const;

  /**
   * \brief Set the choking/unchoking interval.
   *
   * The choking/unchoking of connected peers is executed at least once within this interval. If a currently-unchoked peer departs, the
   * choking/unchoking mechanism is triggered independently from this setting; this also applies in the situation in which less than the
   * desired number of peers is unchoked.
   *
   * @param periodicInterval the desired interval. Must be strictly positive; else, it is ignored.
   */
  void SetPeriodicInterval (Time periodicInterval);

  /**
   * \brief Get the multiple at which optimistic unchoking is performed wrt. the normal choking/unchoking process.
   *
   * @returns the number of periodic choking/unchoking events that have to take place before the optimistically-unchoked peer is rotated.
   */
  uint32_t GetOptimisticMultiple () const;

  /**
   * \brief Set the multiple at which optimistic unchoking is performed wrt. the normal choking/unchoking process.
   *
   * @param optimisticMultiple the number of periodic choking/unchoking events that have to take place before the optimistically-unchoked peer is rotated.
   *  Must be strictly positive; else, it is ignored.
   *
   */
  void SetOptimisticMultiple (uint32_t optimisticMultiple);

// Event listeners
public:
  // BitTorrent event listeners (PeerWireProtocol, other events)

  /**
   * \brief Add a newly-connected peer to the associated list.
   */
  virtual void ProcessBitfieldReceivedEvent (Ptr<Peer> peer);

  /**
   * \brief Directly react to changes in the interested / uninterested state of a peer.
   *
   * A peer may be unchoked off-schedule if free slots are available
   */
  virtual void ProcessPeerInterestedChangingEvent (Ptr<Peer> peer);

  /**
   * \brief Immediately call the choking/unchoking mechanism in case an unchoked peer has closed the connection.
   */
  virtual void ProcessPeerConnectionCloseEvent (Ptr<Peer> peer);

  // The periodic event listener

  /**
   * \brief The main choking/unchoking method.
   */
  virtual void ProcessPeriodicSchedule ();

// Helper methods
protected:
  /**
   * \brief Determine the peer which best fits the unchoking criteria at this moment.
   *
   * The default criterion is download speed from the peer (upload speed in seeder mode). You may override this method to implement different criteria.
   *
   * @returns a pointer to the (interested) peer which best fits the unchoking criteria.
   */
  virtual std::pair<double, Ptr<Peer> > GetCandidateForRemoval () const;

  /**
   * \brief Determine the currently unchoked peer which should best be removed (choked) in the current choking round.
   *
   * The default criterion is download speed from the peer (upload speed in seeder mode). You may override this method to implement different criteria.
   *
   * @returns a pointer to the (currently downloading) peer which brest fits the choking criteria.
   */
  virtual std::pair<double, Ptr<Peer> > GetCandidateForInsertion () const;                                                                            // Returns the best candidate for unchoking from  m_interestedPeers according to some metric (standard: upload to this client)

// Statistics
protected:
  /**
   * \brief React to the metrics gathering event.
   *
   * The default implementation calculates the cumulative up- and download speeds of all peers.
   *
   * @retunrs the calculated metrics. Default supplied keys are "upload-speed" and "download-speed".
   */
  std::map<std::string, std::string> ProcessGatherMetricsEvent ();
};

} // ns bittorrent
} // ns ns3

#endif /* CHOKEUNCHOKESTRATEGYBASE_H_ */
