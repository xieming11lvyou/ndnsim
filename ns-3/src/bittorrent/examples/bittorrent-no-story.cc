/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2012 ComSys, RWTH Aachen University
 * Derived from the sixth tutorial setup.
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

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/trace-helper.h"

#include "ns3/BitTorrentTracker.h"
#include "ns3/BitTorrentVideoClient.h"
#include "ns3/GlobalMetricsGatherer.h"

using namespace ns3;
using namespace bittorrent;

int main (int argc, char *argv[])
{
  /*
   * This example establishes an 802.11 network and installs a BitTorrent tracker on one of the nodes
   * and BitTorrent clients on all others. One of the nodes is a leecher (i.e., it has the whole file
   * already available locally) while all other nodes have no parts of the file available and will
   * attempt to donwload it via WiFi.
   *
   * The topology builds upon the setup found in examples/third.cc.
   */

  NodeContainer nodes;
  nodes.Create (12);
  NodeContainer apNode;
  apNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer stationDevices;
  stationDevices = wifi.Install (phy, mac, nodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, apNode);

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobility.Install (nodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (apNode);

  InternetStackHelper stack;
  stack.Install (nodes);
  stack.Install (apNode);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  address.Assign (stationDevices);
  address.Assign (apDevices);

  /*
   * Setting up a BitTorrent simulation is done as follows:
   *
   * 1) Install and configure BitTorrentTracker application on one of the nodes.
   * 2) Load a torrent file via the tracker application.
   * 3) Install BitTorrentClient applications on the desired number of nodes.
   * 4) Set up the BitTorrent metrics gatherer for output handling.
   * 5) Start the simulation.
   */

  // 1) Install a BitTorrentTracker application (with default values) on one of the nodes
  Ptr<BitTorrentTracker> bitTorrentTracker = Create<BitTorrentTracker> ();
  nodes.Get (0)->AddApplication (bitTorrentTracker);

  // 2) Load a torrent file via the tracker application
  Ptr<Torrent> sharedTorrent = bitTorrentTracker->AddTorrent ("input/bittorrent/torrent-data", "input/bittorrent/torrent-data/100MB-full.dat.torrent");

  // 3) Install BitTorrentClient applications on the desired number of nodes
  ApplicationContainer bitTorrentClients;
  for (unsigned int i = 1; i < nodes.GetN (); ++i)
    {
      Ptr<BitTorrentClient> client = Create<BitTorrentClient> ();
      client->SetTorrent (sharedTorrent);
      nodes.Get (i)->AddApplication (client);
      bitTorrentClients.Add (client);
    }
  // Make the first application node a seeder
  DynamicCast<BitTorrentClient> (nodes.Get (1)->GetApplication (0))->SetInitialBitfield ("full");

  // 4) Set up the BitTorrent metrics gatherer for output handling (here, we just log to the screen)
  GlobalMetricsGatherer* gatherer = GlobalMetricsGatherer::GetInstance ();
  gatherer->SetFileNamePrefix ("This will be ignored while logging to the screen", false);
  gatherer->RegisterWithApplications (bitTorrentClients);
  gatherer->SetStopFraction (1.0, 1.0); // Stops the simulation when all nodes have finished downloading

  // 5a) Start the simulation
  Simulator::Run ();

  Simulator::Destroy ();
  return 0;
}
