/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/random-variable-stream.h"
#include "ns3/mobility-module.h"
#include "ns3/object-factory.h"
#include "../ip/print.h"
#include "../ip/udp-echo-client1024.h"
#include "../ip/udp-echo-client1024.cc"
#include "../ip/udp-echo-server1024.h"
#include "../ip/udp-echo-server1024.cc"


#include "ns3/yans-wifi-helper.h"
#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/ns2-mobility-helper.h"

using namespace ns3;

//NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

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

int
main (int argc, char *argv[])
{
  int port =10;

  CommandLine cmd;
  double n;
  double m;
  cmd.AddValue ("n","n",n);
  cmd.AddValue ("m","m",m);
  cmd.Parse (argc,argv);



  int numPacket = 5000;

  Time::SetResolution (Time::NS);

 //LogComponentEnable ("UdpEchoClient1024Application", LOG_LEVEL_INFO);
//  LogComponentEnable ("UdpEchoServer1024Application", LOG_LEVEL_INFO);s
  NodeContainer nodes;
  nodes.Create (100);

  RngSeedManager::SetSeed (n);
  //////////////////////

  WifiHelper wifi = WifiHelper::Default ();
  // wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate24Mbps"));
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel","MaxRange", DoubleValue(200));
  YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default ();
  wifiPhyHelper.SetChannel (wifiChannel.Create ());
  NqosWifiMacHelper wifiMacHelper = NqosWifiMacHelper::Default ();
  wifiMacHelper.SetType("ns3::AdhocWifiMac");


//// Install constant model
//  MobilityHelper mobility;
//  double distance =20.0;
//  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
//  positionAlloc->Add (Vector (50.0, 50, 0.0));
//  positionAlloc->Add (Vector (50+distance, 50, 0.0));
//  positionAlloc->Add (Vector (50, 50+distance, 0.0));
//  positionAlloc->Add (Vector (20-distance, 50, 0.0));
//  mobility.SetPositionAllocator (positionAlloc);
//  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
//  mobility.Install (nodes);

// Install SUMO model
//10 jiedian 50s
//100 jiedian  1500s

  NetDeviceContainer wifiNetDevices = wifi.Install (wifiPhyHelper, wifiMacHelper, nodes);

  Ns2MobilityHelper ns2 = Ns2MobilityHelper ("scratch/My_mobility100.tcl");
  ns2.Install ();

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (wifiNetDevices);


//Create by Factory
  ObjectFactory m_factory_server;
  m_factory_server.SetTypeId("ns3::UdpEchoServer1024");
  m_factory_server.Set("Port",UintegerValue(port));
  m_factory_server.Set("Number",UintegerValue(numPacket+20));

  uint32_t array[100];
  RandSeq (99, array);

 //Must  Create num_server app
  for (int i=0;i<m;i++)
    {
        Ptr<Application> app_server = m_factory_server.Create<Application>();
        nodes.Get(array[i])->AddApplication(app_server);
        app_server->SetStartTime(Seconds (1500.0));
        app_server->SetStopTime(Seconds(2100.0));

    }

  ObjectFactory m_factory_client;
  m_factory_client.SetTypeId("ns3::UdpEchoClient1024");
  m_factory_client.Set ("MaxPackets", UintegerValue (1));
  m_factory_client.Set ("Interval", TimeValue (Seconds (1.0)));
  m_factory_client.Set  ("PacketSize", UintegerValue (1024));
  m_factory_client.Set("Number",UintegerValue(numPacket));
  m_factory_client.Set ("RemoteAddress", AddressValue(Ipv4Address("255.255.255.255")));
  m_factory_client.Set ("RemotePort", UintegerValue (port));
  Ptr<Application> app_client = m_factory_client.Create<Application>();
  nodes.Get(0)->AddApplication(app_client);
  app_client->SetStartTime(Seconds (1500.0));
  app_client->SetStopTime(Seconds(2100.0));

//  wifiPhy.EnablePcapAll ("ndn-ip");

  Simulator::Stop (Seconds (2100));
  Simulator::Run ();

  Simulator::Destroy ();
  return 0;
}
