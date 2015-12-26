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

#ifndef BRITE_TAP_HELPER_H__
#define BRITE_TAP_HELPER_H__

#include "ns3/node-container.h"
#include "ns3/string.h"

#include <list>
#include <map>
#include <string>

namespace ns3 {
namespace bittorrent {

/// @cond HIDDEN
class BriteTapHelper
{
// Fields
private:
  BriteTapHelper (const BriteTapHelper&);
  BriteTapHelper& operator= (const BriteTapHelper&);

// Constructors etc.
public:
  static TypeId GetTypeId (void);

  BriteTapHelper ();
  virtual ~BriteTapHelper ();

// Main interaction methods
public:
  static void InstallTapDevices (NodeContainer *tapNodes, std::list<std::string> *tapDeviceNames);
};
/// @endcond HIDDEN

} // ns bittorrent
} // ns ns3

#endif // BRITE_TAP_HELPER_H__
