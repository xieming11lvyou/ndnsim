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

#include "a-ndn-simple-test.h"
 
using namespace std;
using namespace ns3;

Ptr<ndn::NetDeviceFace>
MyNetDeviceFaceCallback (Ptr<Node> node, Ptr<ndn::L3Protocol> ndn, Ptr<NetDevice> device)
{
  // NS_LOG_DEBUG ("Create custom network device " << node->GetId ());
  Ptr<ndn::NetDeviceFace> face = CreateObject<MyNetDeviceFace> (node, device);
  ndn->AddFace (face);
  return face;
}

void RandSeq (uint32_t n,uint32_t arr[])
{
   uint32_t i;
   uint32_t p;
   uint32_t tmp;
   for (i=0;i<=n;i++)
   {
    arr[i] = i;
   }

   for (i=n;i>0;i--)
   {
      UniformVariable m_rand1;
      p = m_rand1.GetInteger(1,i);
      //cout<<"rand: "<<" "<<0<<" "<<i<<endl;
      //cout<<p<<endl;
      tmp=arr[p];
      arr[p]=arr[i];
      arr[i]=tmp;
      //cout<<endl;
   }
}


//
// DISCLAIMER:  Note that this is an extremely simple example, containing just 2 wifi nodes communicating
//              directly over AdHoc channel.
//

// Ptr<ndn::NetDeviceFace>
// MyNetDeviceFaceCallback (Ptr<Node> node, Ptr<ndn::L3Protocol> ndn, Ptr<NetDevice> device)
// {
//   // NS_LOG_DEBUG ("Create custom network device " << node->GetId ());
//   Ptr<ndn::NetDeviceFace> face = CreateObject<ndn::MyNetDeviceFace> (node, device);
//   ndn->AddFace (face);
//   return face;
// }

int
main (int argc, char *argv[])
{
  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  //Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  //Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue ("OfdmRate24Mbps"));
  CommandLine cmd;
  double n;
  double m;
  cmd.AddValue ("n","n",n);
  cmd.AddValue ("m","m",m);
  cmd.Parse (argc,argv);

  RngSeedManager::SetSeed (n);
  //////////////////////
  //////////////////////
  //////////////////////
  WifiHelper wifi = WifiHelper::Default ();
  // wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate24Mbps"));

  YansWifiChannelHelper wifiChannel;// = YansWifiChannelHelper::Default ();
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  //wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  
  //wifiChannel.AddPropagationLoss ("ns3::ThreeLogDistancePropagationLossModel");
  wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel","MaxRange", DoubleValue(200));
  // wifiChannel.SetAttribute ("MaxRange", DoubleValue(200));
 //wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  //swifiChannel.AddPropagationLoss ("ns3::NakagamiPropagationLossModel");

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
  // mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
  //                                "X", PointerValue (randomizer),
  //                                "Y", PointerValue (randomizer),
  //                                "Z", PointerValue (randomizer));

  // mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  NodeContainer nodes;
  nodes.Create (100);


 //  double distance = 1000.0;
 //  MobilityHelper mobility;
 //  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
 //  //positionAlloc->Add (Vector (distance*4, 80.0, 0.0));
 //  //positionAlloc->Add (Vector (distance*3, 80.0, 0.0));
 //  //positionAlloc->Add (Vector (distance*2.0, 80.0, 0.0));
 //  positionAlloc->Add (Vector (distance, 80.0, 0.0));
 //  positionAlloc->Add (Vector (0.0, 80.0, 0.0));
 //  mobility.SetPositionAllocator (positionAlloc);

 //  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

 //  mobility.Install (nodes.Get(1));
 // //  mobility.Install (nodes.Get(2));
 // //  mobility.Install (nodes.Get(3));
 // // mobility.Install (nodes.Get(4));


 //  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");

 //  mobility.Install (nodes.Get(0));
  
 //  nodes.Get(0)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(5.0, 0.0, 0.0));


//----------------------------------------------------------------------------------------
  // nodes.Get(1)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(10.0, 0.0, 0.0));
  // nodes.Get(2)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(10.0, 0.0, 0.0));

//---------------------------------------------------------------------------------------

 

  ////////////////
  // 1. Install Wifi
  NetDeviceContainer wifiNetDevices = wifi.Install (wifiPhyHelper, wifiMacHelper, nodes);
  Ns2MobilityHelper ns2 = Ns2MobilityHelper ("scratch/My_mobility100.tcl");
  ns2.Install ();
  // 2. Install Mobility model
 // mobility.Install (nodes);

  // 3. Install NDN stack
  //NS_LOG_INFO ("Installing NDN stack");
  ndn::StackHelper ndnHelper;
  ndnHelper.AddNetDeviceFaceCreateCallback (WifiNetDevice::GetTypeId (), MakeCallback (MyNetDeviceFaceCallback));
  // ndnHelper.AddNetDeviceFaceCreateCallback (WifiNetDevice::GetTypeId (), MakeCallback (MyNetDeviceFaceCallback));
  ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::MyFw");
  ndnHelper.SetContentStore ("ns3::ndn::cs::Lru", "MaxSize", "1000");
  ndnHelper.SetDefaultRoutes (true);
  ndnHelper.Install (nodes);

  // 4. Set up applications
  //NS_LOG_INFO ("Installing Applications");
  //ndn::AppHelper consumerHelper ("ns3::ndn::MyConsumer");
  ndn::AppHelper consumerHelper ("ns3::ndn::MyConsumerBatches");
  //ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
  //consumerHelper.SetPrefix ("/very-long-prefix-requested-by-client/this-interest-hundred-bytes-long-interest");
  


  //consumerHelper.SetAttribute ("Batches", StringValue ("1s 1"));
  consumerHelper.SetAttribute ("MyBatches", StringValue ("1s 1"));
  consumerHelper.SetPrefix ("/prefix");
  consumerHelper.SetAttribute ("LifeTime", StringValue ("10000s"));
  consumerHelper.SetAttribute ("StartTime", TimeValue (Seconds (1500.0)));

  // Ptr<Application> app = nodes.Get(0)->GetApplication();
  // cout<<"Delay: "<<app->delay<<endl;
  //consumerHelper.SetAttribute("MaxSeq", IntegerValue(100));
  //consumerHelper.SetAttribute ("Frequency", DoubleValuse (10.0));
  consumerHelper.Install (nodes.Get (0));


  // Ptr<MyProducer> myProducer = CreateObject<MyProducer>();
  // nodes.Get(1)->AddApplication (myProducer);

  //nodes.Get(2)->AddApplication (myProducer);
  
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  producerHelper.SetPrefix ("/");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.SetAttribute ("StartTime", StringValue("5"));
 
 
  //const int num = 3;
  uint32_t array[100];
  RandSeq (99, array);

 //  for(int i=0; i<10; i++)
 //  {
 //    cout<<array[i]<<" ";
 // }

  for (int i=1; i<m; i++)
  {
    producerHelper.Install (nodes.Get (array[i]));
  }


  ////////////////

  //Simulator::Stop (Seconds (5000.0));
  Simulator::Stop (Seconds (10000.0));



  Simulator::Run ();
  Simulator::Destroy ();



  return 0;
}
