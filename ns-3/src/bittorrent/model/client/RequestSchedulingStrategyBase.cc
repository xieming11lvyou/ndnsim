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
 * Authors: Rene Glebke, Martin Lang
 */

#include "RequestSchedulingStrategyBase.h"

#include "BitTorrentClient.h"
#include "BitTorrentPeer.h"

#include "ns3/log.h"

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::RequestSchedulingStrategyBase");
NS_OBJECT_ENSURE_REGISTERED (RequestSchedulingStrategyBase);


RequestSchedulingStrategyBase::RequestSchedulingStrategyBase (Ptr<BitTorrentClient> myClient) : AbstractStrategy (myClient)
{
  NS_LOG_FUNCTION_NOARGS ();
}

RequestSchedulingStrategyBase::~RequestSchedulingStrategyBase ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void RequestSchedulingStrategyBase::DoInitialize ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_myClient->RegisterCallbackRequestEvent (MakeCallback (&RequestSchedulingStrategyBase::ProcessPeerRequestEvent, this));
}

void RequestSchedulingStrategyBase::ProcessPeerRequestEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  // Only send out if we are not choking the peer...
  if (!peer->GetAmChoking ())
    {
      // and if the requested piece (and hence, the block) is available
      if ((*m_myClient->GetBitfield ())[pieceIndex / 8] & (0x01 << (7 - pieceIndex % 8)))
        {
          peer->SendBlock (pieceIndex, blockOffset, blockLength);
        }
    }
}

} // ns bittorrent
} // ns ns3
