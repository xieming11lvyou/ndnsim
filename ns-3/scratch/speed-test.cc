/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012-2013 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"

#include "ns3/ndnSIM-module.h"

#include "a-ndn-app.h"
//#include "a-ndn-consumer.h"
#include "a-ndn-fw.h"
#include "a-ndn-simple-test.h"

#include "a-ndn-consumerbatches.h"

#include "ns3/ns2-mobility-helper.h"

#include "a-ndn-ndnfw.h"

#include "a-ndn-consumer.h"

#include "a-ndn-producer.h"


using namespace std;
using namespace ns3;
uint32_t nodeNumber = 10;
 const double RANGE = 300;
 NodeContainer nodes;
Ptr<ndn::NetDeviceFace>
MyNetDeviceFaceCallback (Ptr<Node> node, Ptr<ndn::L3Protocol> ndn, Ptr<NetDevice> device)
{
  // NS_LOG_DEBUG ("Create custom network device " << node->GetId ());
 Ptr<ndn::NetDeviceFace> face = CreateObject<MyNetDeviceFace> (node, device,nodeNumber);
  //Ptr<ndn::NetDeviceFace> face = CreateObject<MyNetDeviceFace> (node, device);
 //cout<<nodeNumber<<endl;
  ndn->AddFace (face);
  return face;
}
void speed()
{
  for(uint32_t i=0;i<nodeNumber;i++)
    {
    cout<<abs(nodes.Get(i)->GetObject<MobilityModel> ()->GetVelocity().x)<<endl;
    }
  Simulator::Schedule (Seconds(5), &speed);



}


int
main (int argc, char *argv[])
{
  double T=10;
  nodeNumber = 10;


  // disable fragmentation
  // Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue ("OfdmRate24Mbps"));

  CommandLine cmd;
  cmd.AddValue ("T", "T", T);
  cmd.AddValue ("num", "num",nodeNumber );
  cmd.Parse (argc,argv);


  stringstream ss,ss1;
  ss<<nodeNumber;
  ss1<<(nodeNumber-2);
  string b = ss.str();
  string a = ss1.str();
  //cout<<b<<endl;
  string url = string("scratch/")+b+string(".tcl");
//  cout<<url<<endl;
  //////////////////////
  //////////////////////
  //////////////////////
  WifiHelper wifi = WifiHelper::Default ();
  // wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate54Mbps"));

  YansWifiChannelHelper wifiChannel;// = YansWifiChannelHelper::Default ();
  //wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  //wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");

  //wifiChannel.AddPropagationLoss ("ns3::ThreeLogDistancePropagationLossModel");
  wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel","MaxRange", DoubleValue(RANGE));
  //wifiChannel.AddPropagationLoss ("ns3::NakagamiPropagationLossModel");

  //YansWifiPhy wifiPhy = YansWifiPhy::Default();
  YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default ();
  wifiPhyHelper.SetChannel (wifiChannel.Create ());
  // wifiPhyHelper.Set("TxPowerStart", DoubleValue(5));
  // wifiPhyHelper.Set("TxPowerEnd", DoubleValue(5));


  NqosWifiMacHelper wifiMacHelper = NqosWifiMacHelper::Default ();
  wifiMacHelper.SetType("ns3::AdhocWifiMac");

  // Ptr<UniformRandomVariable> randomizer = CreateObject<UniformRandomVariable> ();
  // randomizer->SetAttribute ("Min", DoubleValue (10));
  // randomizer->SetAttribute ("Max", DoubleValue (100));

  // MobilityHelper mobility;
  // // mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
  // //                                "X", PointerValue (randomizer),
  // //                                "Y", PointerValue (randomizer),
  // //                                "Z", PointerValue (randomizer));
  //  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();


//cout<<nodeNumber<<endl;

  nodes.Create (nodeNumber);

  // //for (uint32_t i = 0; i<nodeNumber+2; i++)
  // for (uint32_t i = 0; i<nodeNumber+2; i++)
  // {

  //    if (i==5)
  //      i=i+2;
  //   positionAlloc->Add (Vector (-80+40*i, 80.0, 0.0));
  // }


  // mobility.SetPositionAllocator (positionAlloc);
  // mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");





  // for (uint32_t i=0; i<nodeNumber; i++)
  // {
  //   if (i!=5)
  //     mobility.Install (nodes.Get(i));
  // }
  //  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  //  mobility.Install (nodes.Get(5));

  //  nodes.Get(5)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(-25.0, 0.0, 0.0));
  Ns2MobilityHelper ns2 = Ns2MobilityHelper (url);//5km_50node.tcl");//

  ns2.Install ();

  Simulator::Schedule (Seconds(5), &speed);



//  ////////////////
//  // 1. Install Wifi
//  NetDeviceContainer wifiNetDevices = wifi.Install (wifiPhyHelper, wifiMacHelper, nodes);
//
//  // 2. Install Mobility model
//  //mobility.Install (nodes);
//
//  // 3. Install NDN stack
// // NS_LOG_INFO ("Installing NDN stack");
//  ndn::StackHelper ndnHelper;
//  ndnHelper.AddNetDeviceFaceCreateCallback (WifiNetDevice::GetTypeId (), MakeCallback (MyNetDeviceFaceCallback));
//  ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::ndnFw","ID",a);
//  ndnHelper.SetContentStore ("ns3::ndn::cs::Lru", "MaxSize", "1000");
//  ndnHelper.SetDefaultRoutes (true);
//  ndnHelper.Install (nodes);

  // 4. Set up applications
 // NS_LOG_INFO ("Installing Applications");

//  ndn::AppHelper consumerHelper ("ns3::ndn::MyConsumer");
//  consumerHelper.SetPrefix ("/test/prefix");
//  consumerHelper.SetAttribute("MaxSeq", IntegerValue(0));
//  //consumerHelper.SetAttribute ("Frequency", DoubleValue (1.0));
//  consumerHelper.SetAttribute ("LifeTime", StringValue("4000s"));
//  consumerHelper.SetAttribute ("StartTime", TimeValue (Seconds (T)));
//  consumerHelper.Install (nodes.Get (nodeNumber-2));
//
//  //  consumerHelper.SetAttribute ("StopTime", StringValue("30s"));
//  //
//
//
//  ndn::AppHelper producerHelper ("ns3::ndn::V2VProducer");
//  producerHelper.SetPrefix ("/");
//  producerHelper.SetAttribute ("PayloadSize", StringValue("0"));
//  producerHelper.Install (nodes.Get (nodeNumber-1));

 ////////////////


  Simulator::Stop (Seconds (4000));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
