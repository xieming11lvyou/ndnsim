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

#ifndef RFPARTSELECTIONSTRATEGY_H_
#define RFPARTSELECTIONSTRATEGY_H_

#include "ns3/PartSelectionStrategyBase.h"

#include <set>
#include <vector>

namespace ns3 {
namespace bittorrent {

class BitTorrentClient;
class Peer;

/**
 * \ingroup BitTorrent
 *
 * \brief Implements BitTorrent's default rarest-piece-first part selection strategy.
 *
 * This class provides an implementation of BitTorrent's default rarest-piece-first part selection strategy.
 * For this purpose, the class keeps track of all announcements for pieces sent by the client's peers and sorts
 * the pieces the shared file consists of by rarity. It then, for each peer, selects among the rarest (i.e.,
 * least often announced) pieces for download. It employs a heuristic scheme for faster selection of blocks
 * that first tries to download missing blocks of a piece already requested from a peer (to complete that piece)
 * and only then selects the pieces for download according to the rarest-first scheme.
 *
 */
class RarestFirstPartSelectionStrategy : public PartSelectionStrategyBase {
// Fields
protected:
	std::set<uint32_t> *m_piecesByRarity;
	std::vector<uint16_t> m_raritiesByPiece;

// Constructors etc.
public:
	RarestFirstPartSelectionStrategy(Ptr<BitTorrentClient> myClient);
	virtual ~RarestFirstPartSelectionStrategy();

  /**
   * \brief Initialze the strategy. Register the needed event listeners with the associated client.
   *
   * Especially, this method registers event listeners for member functions provided by the base
   * PartSelectionStrategyBase class.
   */
	virtual void DoInitialize();
	virtual void ProcessStrategyOptionsChangedEvent();

// Event listeners
public:

	/**
	 * \brief Reacts to an announced bitfield by shifting the availability of all pieces announced by the peer up by one.
	 */
	virtual void ProcessPeerBitfieldReceivedEvent(Ptr<Peer> peer);

	/**
	 * \brief Reacts to the closure of a connection by shifting the availability of all pieces announced by the peer down by one.
	 */
	virtual void ProcessPeerConnectionCloseEvent(Ptr<Peer> peer);

	/**
	 * \brief Reacts to a HAVE message by a peer by shifting the availability of the announced piece up by one.
	 */
	virtual void ProcessPeerHaveEvent(Ptr<Peer> peer, uint32_t pieceIndex);

// Semi-listener methods
protected:
	virtual void ProcessCompletedPiece(uint32_t pieceIndex);

// Strategy implementation methods
protected:

	/**
	 * \brief Implements a rarest-first part selection strategy.
	 *
	 * This strategy first tries to complete any piece which is already being requested from the given peer
	 * and only then randomly selects among the rarest pieces available at the specific peer.
	 */
	virtual void GetHighestPriorityBlockForPeer(Ptr<Peer> peer, BlockRequested& blockPtr);
};

} // ns bittorrent
} // ns ns3

#endif /* RFPARTSELECTIONSTRATEGY_H_ */
