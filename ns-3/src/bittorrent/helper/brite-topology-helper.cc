/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2014 ComSys, RWTH Aachen University
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
 * Authors: Rene Glebke (principal author), Alexander Hocks
 */

#include "brite-topology-helper.h"

#include "ns3/BitTorrentUtilities.h"
#include "ns3/brite-tap-helper.h"
// #include "ns3/sync-tunnel-bridge-helper.h"

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
#include "ns3/trace-helper.h"
#include "ns3/uinteger.h"

#include <fstream>
#include <list>
#include <sstream>
#include <string>

using namespace std;

namespace ns3 {
namespace bittorrent {

#define ADD_ADDRESS_TAG(object, valueOrigin)\
  {\
    std::string value;\
    valueOrigin.GetAttributeFailSafe ("Device-IP", value);\
    m_specialIPs.insert (std::pair<Ptr<NetDevice>, std::string>(object, value));\
  }

#define ASSIGN_ADDRESSES(containerName, ipsStart, netmask) \
  { \
    ipv4Helper.SetBase (ipsStart, netmask);\
    ipv4Helper.Assign (containerName);\
    for(uint32_t i = 0; i < containerName.GetN(); ++i)\
    {\
      if (m_specialIPs.find (containerName.Get (i)) != m_specialIPs.end ())\
      {\
        std::string ip = m_specialIPs.find (containerName.Get (i))->second;\
        if(ip != "")\
        {\
          NetDeviceContainer dummy; dummy.Add(containerName.Get(i));\
          ipv4Helper.SetBase ("0.0.0.0", netmask, ip.c_str ());\
          ipv4Helper.Assign (dummy);\
        }\
      }\
    }\
  }

NS_LOG_COMPONENT_DEFINE ("bittorrent::BriteTopologyHelper");
NS_OBJECT_ENSURE_REGISTERED (BriteTopologyHelper);

TypeId BriteTopologyHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BriteTopologyHelper").SetParent<Object> ();
  return tid;
}

BriteTopologyHelper::BriteTopologyHelper ()
{
  NS_LOG_FUNCTION (this);

  m_bandwidthSamplesFile = "";
  m_minDelay = 10;
  m_maxDelay = 32;
  m_hasTapNode = false;
  m_pcapFilePrefix = "";
}

BriteTopologyHelper::~BriteTopologyHelper ()
{
  NS_LOG_FUNCTION (this);
}

NodeContainer BriteTopologyHelper::AddClientNodesToLastTopology (uint32_t numberOfClientNodes, std::string deviceType)
{
  NodeContainer clientNodes;

  std::list<std::pair<std::string, std::string> > bandwidths;
  std::list<std::pair<std::string, std::string> >::iterator bandwidthsIt;
  UniformVariable routerChooser, latencyChooser;
  std::string delay;

  if (m_bandwidthSamplesFile == "")
    {
      bandwidths = GetSimpleDSLDistribution (numberOfClientNodes);
    }
  else
    {
      bandwidths = GetBandwidthDistributionFromFile (numberOfClientNodes);
    }
  bandwidthsIt = bandwidths.begin ();

  for (uint32_t i = 0; i < numberOfClientNodes; ++i)
    {
      Ptr<Node> parent = m_routers[lexical_cast < std::string > (routerChooser.GetInteger (0, m_routers.size () - 1))];
      delay = lexical_cast<std::string> (latencyChooser.GetInteger (m_minDelay, m_maxDelay));
      clientNodes.Add (AddNodeToLastTopology (
                         parent,
                         deviceType,
                         (*bandwidthsIt).first,
                         (*bandwidthsIt).second,
                         delay,
                         false,
                         false,
                         m_autonomousSystems[lexical_cast < std::string > (parent->GetId ())],
                         false,
                         ""
                         ));
      ++bandwidthsIt;
    }

  DistributeRouterNodesToMPISystems();
  DistributeClientNodesToMPISystems();

  return clientNodes;
}

NodeContainer BriteTopologyHelper::AddNodeToLastTopology (Ptr<Node> parent, std::string deviceType, std::string upstreamBandwidth, std::string downstreamBandwidth, std::string delay, bool isRouterNode, bool isBorderRouterNode, int32_t AS, bool isTrackerNode, std::string specialIP)
{
  Ptr<Node> newNode = CreateObject<Node> ();
  NodeContainer nodeContainer;
  nodeContainer.Add (newNode);
  m_nodes.Add (newNode);
  if (isRouterNode)
    {
      if (isBorderRouterNode)
        {
          m_borderRouters[lexical_cast < std::string > (newNode->GetId ())] = newNode;
        }
      else
        {
          m_routers[lexical_cast < std::string > (newNode->GetId ())] = newNode;
        }
    }
  else
    {
      m_clients[lexical_cast < std::string > (newNode->GetId ())] = newNode;
      m_clientsToRouters[lexical_cast < std::string > (newNode->GetId ())] = parent;
    }

  m_autonomousSystems[lexical_cast < std::string > (newNode->GetId ())] = AS;

  Link link (newNode, lexical_cast<std::string> (newNode->GetId ()), parent, lexical_cast<std::string> (parent->GetId ()));
  link.SetAttribute ("Device-Type", isTrackerNode ? "tracker" : deviceType);
  link.SetAttribute ("Device-DataRate0", upstreamBandwidth);
  link.SetAttribute ("Device-DataRate1", downstreamBandwidth);
  link.SetAttribute ("Device-IP", specialIP);
  link.SetAttribute ("Channel-Delay", delay);

  AddLink (link);

  DistributeRouterNodesToMPISystems();
  DistributeClientNodesToMPISystems();

  return nodeContainer;
}

NodeContainer BriteTopologyHelper::AddTapNodeToLastTopology (std::string tapDeviceName, Ptr<Node> parent, std::string upstreamBandwidth, std::string downstreamBandwidth, std::string delay, bool isRouterNode, bool isBorderRouterNode, int32_t AS)
{
  Ptr<Node> newNode = CreateObject<Node> ();
  NodeContainer nodeContainer;
  nodeContainer.Add (newNode);
  m_tapNodes.Add (newNode);
  m_nodes.Add (newNode);

  m_clients[lexical_cast < std::string > (newNode->GetId ())] = newNode;
  m_clientsToRouters[lexical_cast < std::string > (newNode->GetId ())] = parent;

  Link link (newNode, lexical_cast<std::string> (newNode->GetId ()), parent, lexical_cast<std::string> (parent->GetId ()));
  link.SetAttribute ("Device-Type", "csmaTap");
  link.SetAttribute ("Device-DataRate0", upstreamBandwidth);
  link.SetAttribute ("Device-DataRate1", downstreamBandwidth);
  link.SetAttribute ("Device-IP", "");
  link.SetAttribute ("Channel-Delay", delay);  

  AddLink (link);

  m_tapDeviceNames.push_back (tapDeviceName);
  m_hasTapNode = true;

  DistributeRouterNodesToMPISystems();
  DistributeClientNodesToMPISystems();

  return nodeContainer;
}

bool BriteTopologyHelper::HasTapNode () const
{
  return m_hasTapNode;
}

bool BriteTopologyHelper::EstablishNetworkOnLastTopology ()
{
  TopologyReader::ConstLinksIterator linksIt;
  PointToPointHelper ptpHelper;
  CsmaHelper csmaHelper;
  NetDeviceContainer ptpInterfaces;
  NetDeviceContainer csmaInterfaces;
  NetDeviceContainer csmaTapInterfaces;
  NetDeviceContainer trackerInterfaces;
  InternetStackHelper isHelper;
  Ipv4AddressHelper ipv4Helper;
  Ptr<PointToPointNetDevice> fromNodeDevice, toNodeDevice;

  NS_LOG_DEBUG ("Establishing links between the nodes...");

  int i = 0;
  for (linksIt = LinksBegin (); linksIt != LinksEnd (); ++linksIt, ++i)
    {
      Link link = *linksIt;
      Ptr<Node> fromNode = link.GetFromNode ();
      Ptr<Node> toNode = link.GetToNode ();

      if (link.GetAttribute ("Device-Type") == "point-to-point" || link.GetAttribute ("Device-Type") == "topology" || link.GetAttribute ("Device-Type") == "tracker")
        {
          ptpHelper.SetChannelAttribute ("Delay", StringValue (link.GetAttribute ("Channel-Delay")));

          ptpHelper.SetDeviceAttribute ("DataRate", StringValue (link.GetAttribute ("Device-DataRate0")));

          if (link.GetAttribute ("Device-Type") == "tracker")
           {
             trackerInterfaces.Add (ptpHelper.Install (fromNode, toNode));
             
             ADD_ADDRESS_TAG(trackerInterfaces.Get (trackerInterfaces.GetN () - 2), link);
           }
          else
           {
             ptpInterfaces.Add (ptpHelper.Install (fromNode, toNode));
             
             ADD_ADDRESS_TAG(ptpInterfaces.Get (ptpInterfaces.GetN () - 2), link);             
           }

          fromNodeDevice = fromNode->GetDevice (fromNode->GetNDevices () - 1)->GetObject<PointToPointNetDevice> ();
          toNodeDevice = toNode->GetDevice (toNode->GetNDevices () - 1)->GetObject<PointToPointNetDevice> ();

          fromNodeDevice->SetDataRate (DataRate (link.GetAttribute ("Device-DataRate0")));
          toNodeDevice->SetDataRate (DataRate (link.GetAttribute ("Device-DataRate1")));
        }
      else if (link.GetAttribute ("Device-Type") == "csmaTap")
        {
          NodeContainer csmaContainer;
          csmaContainer.Add (toNode);
          csmaContainer.Add (fromNode);

          csmaHelper.SetChannelAttribute ("DataRate", StringValue (link.GetAttribute ("Device-DataRate0")));
          csmaHelper.SetChannelAttribute ("Delay", StringValue (link.GetAttribute ("Channel-Delay")));
          csmaTapInterfaces.Add (csmaHelper.Install (csmaContainer));
          
          ADD_ADDRESS_TAG(csmaTapInterfaces.Get (csmaTapInterfaces.GetN () - 1), link);
        }
      else if (link.GetAttribute ("Device-Type") == "csma")
        {
          Ptr<CsmaNetDevice> routerDevice;
          bool csmaDeviceFound = false;
          for (uint32_t i = 0; i < toNode->GetNDevices () && !csmaDeviceFound; i++)
            {
              routerDevice = toNode->GetDevice (i)->GetObject<CsmaNetDevice> (CsmaNetDevice::GetTypeId ());
              if ( routerDevice != 0 )
                {
                  csmaDeviceFound = true;
                }
            }

          if (routerDevice == 0)
            {
              csmaHelper.SetChannelAttribute ("DataRate", StringValue (link.GetAttribute ("Device-DataRate0")));
              csmaHelper.SetChannelAttribute ("Delay", StringValue (link.GetAttribute ("Channel-Delay")));
              routerDevice = DynamicCast<CsmaNetDevice> (csmaHelper.Install (toNode).Get (0));
              csmaInterfaces.Add (routerDevice);
            }

            csmaInterfaces.Add (csmaHelper.Install (fromNode, DynamicCast<CsmaChannel> (routerDevice->GetChannel ())));
            
            ADD_ADDRESS_TAG(csmaInterfaces.Get (csmaInterfaces.GetN () - 2), link);
        }
      else
        {
          NS_ABORT_MSG ("Error: Unknown interface device type " << link.GetAttribute ("Device-Type") << " for a node in the topology");
        }
    }

  if(!m_hasTapNode)
    {
      Ipv4NixVectorHelper nixRouting;
      Ipv4StaticRoutingHelper staticRouting;
      Ipv4ListRoutingHelper list;
      list.Add (staticRouting, 0);
      list.Add (nixRouting, 1);
      isHelper.SetRoutingHelper(list);
    }

  isHelper.Install (m_nodes);
  
  ASSIGN_ADDRESSES(trackerInterfaces, "10.1.0.0", "255.255.255.255");
  ASSIGN_ADDRESSES(ptpInterfaces, "10.2.0.0", "255.255.255.255");
  ASSIGN_ADDRESSES(csmaInterfaces, "10.100.0.0", "255.255.0.0");
  ASSIGN_ADDRESSES(csmaTapInterfaces, "192.168.0.0", "255.255.255.0");

  if (m_hasTapNode)
    {
	BriteTapHelper::InstallTapDevices (&m_tapNodes, &m_tapDeviceNames);

        /* 
        SyncTunnelBridgeHelper syncBridge;
        syncBridge.SetAttribute("TunnelFlowId", IntegerValue(42));
        syncBridge.SetAttribute("TunnelDestinationAddress", Ipv4AddressValue("10.0.0.3"));
        syncBridge.SetAttribute("TunnelDestinationPort", UintegerValue(7544));
        GlobalValue::Bind("SyncTunnelReceiveAddress", Ipv4AddressValue("10.0.0.1"));
        GlobalValue::Bind("SyncTunnelReceivePort", UintegerValue(7545));
        // install the bridge on node 0
        syncBridge.Install(m_tapNodes.Get(0), csmaTapInterfaces.Get(1));
        */
        Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    }

  if (m_pcapFilePrefix != "")
    {
      std::string outputPath = m_pcapFilePrefix + ".node";
      
      ptpHelper.EnablePcapAll (outputPath);
      csmaHelper.EnablePcapAll (outputPath);
    }

  return true;
}

std::list<std::pair<std::string, std::string> > BriteTopologyHelper::GetBandwidthDistributionFromFile (uint32_t numberOfNodes)
{
  std::list<std::pair<std::string, std::string> > result;
  uint32_t numberOfBuckets = 0;
  std::vector<uint32_t> bucketSizes;
  uint32_t numberOfSamples = 0;
  std::map<uint32_t, std::vector<std::pair<std::string, std::string> > > buckets;
  IntEmpiricalVariable bucketChooser;

  ifstream file;
  istringstream lineBuffer;
  string line;
  uint32_t bucketSize;
  string rx, tx;

  // First, we open the file...
  file.open (m_bandwidthSamplesFile.c_str ());
  if (!file.is_open ())
    {
      NS_ABORT_MSG ("Error: Bandwidth distribution file not found.");
    }

  // ... and read out the number of buckets following
  getline (file, line);
  lineBuffer.str (line);
  lineBuffer >> numberOfBuckets;

  // Then, we read the size of each of the following buckets
  for (uint32_t i = 0; i < numberOfBuckets; ++i)
    {
      line.clear ();
      lineBuffer.clear ();
      getline (file, line);
      lineBuffer.str (line);
      lineBuffer >> bucketSize;
      bucketSizes.push_back (bucketSize);
      numberOfSamples += bucketSize;
    }

  /*
   * Next, we read the buckets.
   * While doing this, we also initialize the random variable we will later use to choose one of the
   * buckets for the bandwidths of one specific node. The buckets are rated according to their size
   * compared to the cumulative size of all buckets. The last bucket gets a probability of 1.
   */
  bucketChooser.CDF (0, 0.0);
  double probability = 0.0;
  for (uint32_t i = 0; i < numberOfBuckets; ++i)
    {
      for (uint32_t j = 0; j < bucketSizes[i]; ++j)
        {
          line.clear ();
          lineBuffer.clear ();
          getline (file, line);
          lineBuffer.str (line);
          lineBuffer >> rx >> tx;
          buckets[i].push_back (std::pair<std::string, std::string> (tx + "bps", rx + "bps"));
        }

      if (i < numberOfBuckets - 1)
        {
          probability += static_cast<double> (bucketSizes[i]) / numberOfSamples;
          bucketChooser.CDF (i, probability);

          NS_LOG_INFO ("BriteToplogyHelper: Bucket " << i << ", cumulative probability: " << probability << ".");
        }
    }
  bucketChooser.CDF (numberOfBuckets - 1, 1.0);

  NS_LOG_INFO ("BriteTopologyHelper: Bucket " << numberOfBuckets - 1 << ", own probability: " << static_cast<double> (bucketSizes[numberOfBuckets - 1]) / numberOfSamples << ".");

  // Now, we generate our bandwidth distribution list
  UniformVariable uv;
  for (uint32_t node = 0; node < numberOfNodes; ++node)
    {
      uint32_t bucket = bucketChooser.GetInteger ();
      uint32_t bucketIndex = uv.GetInteger (0, buckets[bucket].size () - 1);
      result.push_back (buckets[bucket][bucketIndex]);
    }

  file.close ();

  return result;
}

std::list<std::pair<std::string, std::string> > BriteTopologyHelper::GetSimpleDSLDistribution (uint32_t numberOfNodes)
{
  std::list<std::pair<std::string, std::string> > result;

  IntEmpiricalVariable randomDownstream;
  randomDownstream.CDF (1023, 0.0);
  randomDownstream.CDF (1024, 0.1);
  randomDownstream.CDF (2047, 0.1);
  randomDownstream.CDF (2048, 0.5);
  randomDownstream.CDF (6015, 0.5);
  randomDownstream.CDF (6016, 0.85);
  randomDownstream.CDF (15999, 0.85);
  randomDownstream.CDF (16000, 1);

  for (uint32_t node = 0; node < numberOfNodes; ++node)
    {
      uint32_t downstreamBandwidth = randomDownstream.GetInteger ();
      uint32_t upstreamBandwidth;

      switch (downstreamBandwidth)
        {
        case 1024:
          upstreamBandwidth = 128;               // 128
          break;
        case 2048:
          upstreamBandwidth = 192;               // 192
          break;
        case 6016:
          upstreamBandwidth = 576;               // 576
          break;
        case 16000:
          upstreamBandwidth = 1024;
          break;
        default:
          upstreamBandwidth = 0;
          break;
        }

      std::stringstream down, up;

      down << downstreamBandwidth << "kbps";
      up << upstreamBandwidth << "kbps";

      result.push_back (std::make_pair (up.str (), down.str ()));
    }

  return result;
}

void BriteTopologyHelper::SetBandwidthSamplesFile (std::string bandwidthSamplesFile)
{
  m_bandwidthSamplesFile = bandwidthSamplesFile;
}

std::string BriteTopologyHelper::GetBandwidthSamplesFile () const
{
  return m_bandwidthSamplesFile;
}

void BriteTopologyHelper::SetClientLinkDelays (uint32_t minDelay, uint32_t maxDelay)
{
  if (minDelay >= 0 && maxDelay >= minDelay)
    {
      m_minDelay = minDelay;
      m_maxDelay = maxDelay;
    }
}

uint32_t BriteTopologyHelper::GetClientLinkDelaysMin () const
{
  return m_minDelay;
}

uint32_t BriteTopologyHelper::GetClientLinkDelaysMax () const
{
  return m_maxDelay;
}

void BriteTopologyHelper::EnablePcaps (std::string pcapFilePrefix)
{
  m_pcapFilePrefix = pcapFilePrefix;
}

std::string BriteTopologyHelper::GetPcapFilePrefix () const
{
  return m_pcapFilePrefix;
}

void BriteTopologyHelper::DistributeRouterNodesToMPISystems()
{
#ifdef NS3_MPI
  if(MpiInterface::GetSize() == 1)
  {
    NS_LOG_INFO ("BriteTopologyHelper: MPI is not enabled. Performing all computations for routers on 1 system.");
    return;
  }
    else
  {
    NS_LOG_INFO ("BriteTopologyHelper: Performing computations on " << MpiInterface::GetSize() << " systems.");
  }

  for(std::map<std::string, Ptr<Node> >::iterator it = m_routers.begin(); it != m_routers.end(); ++it)
  {
    ((*it).second)->SetAttribute ("SystemId", UintegerValue(m_autonomousSystems[lexical_cast<std::string>(((*it).second)->GetId())] % MpiInterface::GetSize()));
    NS_LOG_INFO("BriteTopologyHelper: AS " << m_autonomousSystems[lexical_cast<std::string>(((*it).second)->GetId())] << " routers execute on CPU " << m_autonomousSystems[lexical_cast<std::string>(((*it).second)->GetId())] % MpiInterface::GetSize() << ".");
  }
  for(std::map<std::string, Ptr<Node> >::iterator it = m_borderRouters.begin(); it != m_borderRouters.end(); ++it)
  {
      ((*it).second)->SetAttribute ("SystemId", UintegerValue(m_autonomousSystems[lexical_cast<std::string>(((*it).second)->GetId())] % MpiInterface::GetSize()));
      NS_LOG_INFO("BriteTopologyHelper: AS " << m_autonomousSystems[lexical_cast<std::string>(((*it).second)->GetId())] << " border routers execute on CPU " << m_autonomousSystems[lexical_cast<std::string>(((*it).second)->GetId())] % MpiInterface::GetSize() << ".");
  }
#endif
}

void BriteTopologyHelper::DistributeClientNodesToMPISystems()
{
#ifdef NS3_MPI
  if(MpiInterface::GetSize() == 1)
  {
    NS_LOG_INFO ("BriteTopologyHelper: MPI is not enabled. Performing all computations for normal nodes on 1 system.");
    return;
  }

  for(std::map<std::string, Ptr<Node> >::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
  {
    ((*it).second)->SetAttribute ("SystemId", UintegerValue(m_clientsToRouters[lexical_cast<std::string>(((*it).second)->GetId())]->GetSystemId()));
    NS_LOG_INFO("BriteTopologyHelper: Node " << ((*it).second)->GetId() << " executes on CPU " << m_clientsToRouters[lexical_cast<std::string>(((*it).second)->GetId())]->GetSystemId() << ".");
  }
#endif
}

bool BriteTopologyHelper::WriteLastTopologyToGraphVizFile (std::string fileName) const
{
   // Open the output stream
   ofstream file;
   file.open ((fileName + ".dot").c_str (), ofstream::out | ofstream::trunc);
   if (!file.is_open ())
     {
       NS_ABORT_MSG ("BriteTopologyHelper: Error: Could not open file \"output/" << fileName << ".dot\" for output. Aborting.");
       return false;
     }

   // Now, write the beginning of the dot file to the stream
   file << "graph Network {\n" << "\tbgcolor=\"transparent\";\n";
   file.flush ();

   // Now, add the routers to the network
   std::map<std::string, Ptr<Node> >::const_iterator nodeIt;
   file << "\tnode [shape=house]\n";
   int i = 0;
   for (nodeIt = m_borderRouters.begin (); nodeIt != m_borderRouters.end (); nodeIt++)
     {
       file << "\t" << (*nodeIt).first << "\n";
       if (i == 100)
         {
           file.flush ();
           i = 0;
         }
       else
         {
           ++i;
         }
     }
   file.flush ();
   file << "\tnode [shape=doublecircle]\n";
   i = 0;
  for (nodeIt = m_backboneRouters.begin (); nodeIt != m_backboneRouters.end (); nodeIt++)
    {
      file << "\t" << (*nodeIt).first << "\n";
      if (i == 100)
        {
          file.flush ();
          i = 0;
        }
      else
        {
          ++i;
        }
    }
  file.flush ();
   file << "\tnode [shape=circle]\n";
   i = 0;
   for (nodeIt = m_routers.begin (); nodeIt != m_routers.end (); nodeIt++)
     {
       file << "\t" << (*nodeIt).first << "\n";
       if (i == 100)
         {
           file.flush ();
           i = 0;
         }
       else
         {
           ++i;
         }
     }
   file.flush ();
   file << "\tnode [shape=square]\n";
   i = 0;
   for (nodeIt = m_clients.begin (); nodeIt != m_clients.end (); nodeIt++)
     {
       file << "\t" << (*nodeIt).first << " [label =\"" << (*nodeIt).second->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal () << "\"]\n";

       if (i == 100)
         {
           file.flush ();
           i = 0;
         }
       else
         {
           ++i;
         }
     }
   file.flush ();

   // And now, add the generated links
   ConstLinksIterator linksIt;
   i = 0;
   for (linksIt = LinksBegin (); linksIt != LinksEnd (); linksIt++)
     {
       file << "\t" << (*linksIt).GetFromNodeName () + " -- " + (*linksIt).GetToNodeName () + "\n";
       if (i == 100)
         {
           file.flush ();
           i = 0;
         }
       else
         {
           ++i;
         }
     }
   file.flush ();

   // Write ending sequence and close the file
   file << "}";
   file.close ();

   return true;
}

} // ns bittorrent
} // ns ns3
