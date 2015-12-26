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
 * Authors: Martin Lang
 */

#ifndef ABSTRACTSTRATEGY_H_
#define ABSTRACTSTRATEGY_H_

#include "BitTorrentClient.h"

#include "ns3/object.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace bittorrent {

class BitTorrentClient;

/**
 * \ingroup BitTorrent
 *
 * \brief The base class for strategy implementations in the BitTorrent client model.
 *
 * To implement new client logic, derive a new class either from this base class or the default derived classes.
 *
 * Use the ProtocolFactory class to create a "strategy bundle", which can be attached to a BitTorrentClient class instance upon its creation.
 */
class AbstractStrategy : public Object
{
public:
  AbstractStrategy (Ptr<BitTorrentClient> myClient);
  virtual ~AbstractStrategy ();

  virtual void DoInitialize () = 0;

protected:
  Ptr<BitTorrentClient> m_myClient;    // The client that this strategy is associated with
};

} // ns bittorrent
} // ns ns3

#endif // ABTRACTSTRATEGY_H_
