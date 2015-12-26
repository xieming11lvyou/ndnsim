/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 ComSys, RWTH Aachen University
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
 * Authors: Alexander Hocks
 */

#include "brite-tap-helper.h"

#include "ns3/BitTorrentUtilities.h"

#include "ns3/abort.h"
#include "ns3/channel.h"
#include "ns3/csma-channel.h"
#include "ns3/csma-helper.h"
#include "ns3/csma-net-device.h"
#include "ns3/global-value.h"
#include "ns3/integer.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-nix-vector-routing.h"
#include "ns3/ipv4-nix-vector-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/log.h"
#include "ns3/mpi-interface.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/random-variable.h"
#include "ns3/uinteger.h"

#include <fstream>
#include <list>
#include <sstream>
#include <string>

using namespace std;

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("BriteTapHelper");
NS_OBJECT_ENSURE_REGISTERED (BriteTapHelper);

TypeId BriteTapHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BriteTapHelper").SetParent<Object> ();
  return tid;
}

BriteTapHelper::BriteTapHelper ()
{
  NS_LOG_FUNCTION (this);
}

BriteTapHelper::~BriteTapHelper ()
{
  NS_LOG_FUNCTION (this);
}

void BriteTapHelper::InstallTapDevices (NodeContainer *tapNodes, std::list<std::string> *tapDeviceNames)
{
  NS_LOG_ERROR("Brite Tap Helper: Tap device functionality is not supported on this system. Aborting.");
}


} // ns bittorrent
} // ns ns3
