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
#include "another-ndn-netdeviceface.h"
#include "ndn-netdeviceface.h"
#include "a-ndn-consumerbatches.h"

#include "ns3/ns2-mobility-helper.h"

#include "a-ndn-ndnfw.h"

#include "a-ndn-consumer.h"

#include "a-ndn-producer.h"


using namespace std;
using namespace ns3;
double Dec = 1;
uint32_t nodeNumber = 50;
uint32_t numProducer = 1;
uint32_t array[100] ;
double p = 2;
double origin = 0;

const double RANGE = 300;
Ptr<ndn::NetDeviceFace>
NdnNetDeviceFaceCallback (Ptr<Node> node, Ptr<ndn::L3Protocol> ndn, Ptr<NetDevice> device)
{
  // NS_LOG_DEBUG ("Create custom network device " << node->GetId ());
 Ptr<ndn::NetDeviceFace> face = CreateObject<NewNetDeviceFace> (node, device,array,numProducer,p/10.0,Dec);
 //Ptr<ndn::NetDeviceFace> face = CreateObject<NewNetDeviceFace> (node, device,nodeNumber);
  //Ptr<ndn::NetDeviceFace> face = CreateObject<NewNetDeviceFace> (node, device);
 // cout<<nodeNumber<<endl;
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
      p = m_rand1.GetInteger(0,i);
      //cout<<"rand: "<<" "<<0<<" "<<i<<endl;
      //cout<<p<<endl;
      tmp=arr[p];
      arr[p]=arr[i];
      arr[i]=tmp;
      //cout<<endl;
   }
}

int
main (int argc, char *argv[])
{
  double T=100;
  int seed = 1;
  numProducer = 1;
  nodeNumber = 20;
  Dec =2;


  // disable fragmentation
  // Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue ("OfdmRate24Mbps"));

  CommandLine cmd;
  cmd.AddValue("seed","seed",seed);
  cmd.AddValue("numProducer","numProducer",numProducer);
  cmd.AddValue ("T", "T", T);
  cmd.AddValue ("p", "p", p);
  cmd.AddValue ("consumer", "consumer", array[0]);
  cmd.AddValue ("num", "num",nodeNumber );
  cmd.AddValue ("dec", "dec", Dec);
  cmd.Parse (argc,argv);
  RngSeedManager::SetSeed (seed);
  RandSeq((nodeNumber-3),array);

  array[0] = nodeNumber-2;
  array[1] =nodeNumber-1;



  stringstream ss,ss2;
  ss<<nodeNumber;
  ss2<<array[0];
  string b = ss.str();
  string a = ss2.str();
  //cout<<b<<endl;
  string url = string("scratch/")+b+string(".tcl");
// cout<<url<<endl;
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


  NodeContainer nodes;
  nodes.Create (nodeNumber);




 // Install constant model
//     MobilityHelper mobility;
////     double distance =20.0;
//     Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
//     for (uint32_t i = 0; i<50; i++)
//    {
//       if(i == 48)
//         {
//         positionAlloc->Add (Vector (1020, 80.0, 0.0));
//         continue;
//         }
//       if(i == 49)
//         {
//         positionAlloc->Add (Vector (2040, 80.0, 0.0));
//         continue;
//         }
//       positionAlloc->Add (Vector (40+40*i, 80.0, 0.0));
//     }
//
//
//     mobility.SetPositionAllocator (positionAlloc);
//     mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
//
//     mobility.Install (nodes);
//     mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
//     mobility.Install (nodes.Get(48));
//     nodes.Get(48)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(10.0, 0.0, 0.0));



  // for (uint32_t i=0; i<nodeNumber; i++)
  // {
  //   if (i!=5)
  //     mobility.Install (nodes.Get(i));
  // }
  //  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  //  mobility.Install (nodes.Get(5));
  
  //
  Ns2MobilityHelper ns2 = Ns2MobilityHelper (url);//5km_50node.tcl");//
  
  ns2.Install ();


  

  

  ////////////////
  // 1. Install Wifi
  NetDeviceContainer wifiNetDevices = wifi.Install (wifiPhyHelper, wifiMacHelper, nodes);

  // 2. Install Mobility model
  //mobility.Install (nodes);

  // 3. Install NDN stack
 // NS_LOG_INFO ("Installing NDN stack");
  ndn::StackHelper ndnHelper;
  ndnHelper.AddNetDeviceFaceCreateCallback (WifiNetDevice::GetTypeId (), MakeCallback (NdnNetDeviceFaceCallback));
  ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::ndnFw","ID",a);

  ndnHelper.SetContentStore ("ns3::ndn::cs::Lru", "MaxSize", "1000");
  ndnHelper.SetDefaultRoutes (true);
  ndnHelper.Install (nodes);

  // 4. Set up applications
 // NS_LOG_INFO ("Installing Applications");

  ndn::AppHelper consumerHelper ("ns3::ndn::MyConsumer");
  consumerHelper.SetPrefix ("/test/prefix");
  consumerHelper.SetAttribute("MaxSeq", IntegerValue(0));
  //consumerHelper.SetAttribute ("Frequency", DoubleValue (1.0));
  consumerHelper.SetAttribute ("LifeTime", StringValue("4000s"));
  consumerHelper.SetAttribute ("StartTime", TimeValue (Seconds (T)));
//  cout<<"consumerId"<<array[0]<<endl;
  consumerHelper.Install (nodes.Get (array[0]));

  //  consumerHelper.SetAttribute ("StopTime", StringValue("30s"));
  //
 

  ndn::AppHelper producerHelper ("ns3::ndn::V2VProducer");
  producerHelper.SetPrefix ("/");
  producerHelper.SetAttribute ("PayloadSize", StringValue("0"));

  for(uint32_t i =1;i<numProducer+1;i++)
    {
//    cout<<"producerId"<<array[i]<<endl;
    producerHelper.Install (nodes.Get (array[i]));
    }

 ////////////////


  Simulator::Stop (Seconds (4000));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
