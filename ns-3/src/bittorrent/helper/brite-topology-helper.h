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

#ifndef BRITE_TOPOLOGY_HELPER_H__
#define BRITE_TOPOLOGY_HELPER_H__

#include "ns3/brite-topology-reader.h"
#include "ns3/net-device.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/string.h"

#include <list>
#include <map>
#include <string>

namespace ns3 {
namespace bittorrent {
/**
 * \ingroup BitTorrent
 *
 * \brief Topology file reader (BRITE-format type) for the BitTorrent simulation framework.
 *
 * This class takes an input file in BRITE format, extracts the nodes and links
 * contained therein and builds up a client-level topology based on the BRITE
 * topology. Up to a specified limit, a random number of client nodes are attached
 * to each (non-border)-router in the topology so that the final topology resembles
 * routers or AS's with routers where the routers each have connections to
 * several client nodes.
 */
class BriteTopologyHelper : public BriteTopologyReader
{
// Fields
private:
  BriteTopologyHelper (const BriteTopologyHelper&);
  BriteTopologyHelper& operator= (const BriteTopologyHelper&);

protected:
  std::map<std::string, Ptr<Node> > m_clients;
  std::map<std::string, Ptr<Node> > m_clientsToRouters;
  std::string m_bandwidthSamplesFile;
  uint32_t m_minDelay;
  uint32_t m_maxDelay;
  bool m_hasTapNode;
  std::string m_pcapFilePrefix;
  std::list<std::string> m_tapDeviceNames;
  std::map<Ptr<NetDevice>, std::string> m_specialIPs;

// Constructors etc.
public:
  static TypeId GetTypeId (void);

  BriteTopologyHelper ();
  virtual ~BriteTopologyHelper ();

// Main interaction methods
public:
  /**
   * \brief Attach additional "client" nodes to the topology.
   *
   * Adds "client" nodes to the list of nodes to be created. Since BRITE is not capable of
   * generating sub-router level topologies (i.e., client networks and nodes), you should
   * call this function to get a fully-operable topology. The links are annotated using
   * typical DSL up- and downlink bandwidths and typical random delays.
   *
   * Note: The additional nodes created by this function are only kept until a subsequent call to the Read() function.
   *
   * \return a container of the client nodes
   *
   */
  NodeContainer AddClientNodesToLastTopology (uint32_t numberOfClientNodes, std::string deviceType);

  /**
   * \brief Attach one node to the topology
   *
   * Adds a child node to the given parent node and creates a (probably asymmetric) link
   * between the two nodes annotated using the supplied bandwidths and the given delay.
   * Be sure to use appropriate unit suffixes in the arguments (see further ns3 documentation)
   * since there will be no modifications made to the supplied arguments; they will be used unaltered
   * to establish and configure the "real" links using ns3 functionality.
   *
   * Note: The additional nodes created by this function are only kept until a subsequent call to the Read() function.
   *
   * @param parent the parent (router) node the new node shall be attached to.
   * @param deviceType the type of the device (channel) to connect the two nodes. Supported values are "point-to-point" and "csma".
   * @param upstreamBandwidth the bandwidth at which the new node shall be able to upload data towards the parent node, in a format compatible to the ns3 string representation of data rates.
   * @param downstreamBandwidth the bandwidth at which the new node shall be able to receive data. If the connection type has symmetric bandwidths (e.g., "csma"), this setting is ignored.
   * @param delay the delay of the connection, expressed in a format compatible to the ns3 string representation of delays.
   * @param isRouterNode whether the node to add shall be considered a router node in the topology. Typically, client applications only run on non-router nodes.
   * @param isBorderRouterNode whether the node to shall be considered a border router. Only of concern when isRouterNode is set to true.
   * @param AS the (internal) autonomous system the new node shall be associated with. One may separate large simulations based on AS interconnectivity.
   * @param isTrackerNode whether the node is supposed to have a BitTorrentTracker application installed or not. Tracker nodes are always assigned IP addresses starting with 10.1.0.1 so the tracker can be easily found even in more complicated scenarios.
   * @param specialIP defines the IP address of the node. If set to the empty string (default), the node is automatically assigned an IP.
   *
   * \return a container containing the added node
   *
   */
  NodeContainer AddNodeToLastTopology (Ptr<Node> parent, std::string deviceType, std::string upstreamBandwidth, std::string downstreamBandwidth, std::string delay, bool isRouterNode, bool isBorderRouterNode, int32_t AS, bool isTrackerNode, std::string specialIP);

  /**
   * \brief Attach one node to the topology for communication with external entities
   *
   * This method works similarly to the AddNodeToLastTopology method, but instead of creating a normal ns3 node,
   * it creates a node connected via a CSMA channel with an additional tap device attached to it. This method allows the
   * connection between nodes simulated by ns3 and entities outside the ns3 simulation via a tap device.
   *
   * Please refer to the ns3 documentation on tap devices and the documentation of AddNodeToLastTopology for further information, especially
   * on
   *
   * @param tapDeviceName the name of the tap device the node shall be attached to.
   * @param parent the parent (router) node the new node shall be attached to.
   * @param upstreamBandwidth the bandwidth at which the new node shall be able to upload data towards the parent node, in a format compatible to the ns3 string representation of data rates.
   * @param downstreamBandwidth the bandwidth at which the new node shall be able to receive data. If the connection type has symmetric bandwidths (e.g., "csma"), this setting is ignored.
   * @param delay the delay of the connection, expressed in a format compatible to the ns3 string representation of delays.
   * @param isRouterNode whether the node to add shall be considered a router node in the topology. Typically, client applications only run on non-router nodes.
   * @param isBorderRouterNode whether the node to shall be considered a border router. Only of concern when isRouterNode is set to true.
   * @param AS the (internal) autonomous system the new node shall be associated with. One may separate large simulations based on AS interconnectivity.
   */
  NodeContainer AddTapNodeToLastTopology (std::string tapDeviceName, Ptr<Node> parent, std::string upstreamBandwidth, std::string downstreamBandwidth, std::string delay, bool isRouterNode, bool isBorderRouterNode = false, int32_t AS = 0);

  /**
   * \brief Checks for the existence of a tap node in the topology.
   *
   * \returns true, if a simulation on the topology must be run in emulation mode, i.e., when a tap bridge was installed on a node of the topology.
   *   If this returns false, the test run can be performed in a pure simulation-only fashion.
   */
  bool HasTapNode () const;

  /**
   * \brief Creates a topology based on the input file and added client nodes
   *
   * Adds links between the nodes created by BriteTopologyHelper::Read() and
   * BriteTopologyHelper::AddClientNodesToLastTopology(). The links are created using the
   * annotations (bandwidth, delay) set by these two functions. Additionally, all generated
   * interfaces are assigned IP addresses starting at 10.1.0.1 (point-to-point), 10.100.0.1
   * (CSMA) and 10.200.0.1 (tap devices).
   *
   * \return true if the operation was successful
   */
  bool EstablishNetworkOnLastTopology ();

  /**
   * \brief Creates a bandwidth distribution for bidirectional channels based on bandwidth distribution data
   * read from a data file specified by a prior call to the SetBandwidthSamplesFile method.
   *
   * The file format is as follows:
   * [line 1] <number of buckets>
   * [lines 2..<number of buckets - 1>] <size of bucket (<line no> - 2)
   * [further lines] <rx in bps> <tab> <tx in bps> [sorted by buckets]
   *
   * \return a list of pairs of strings representing connection bandwidths
   */
  std::list<std::pair<std::string, std::string> > GetBandwidthDistributionFromFile (uint32_t numberOfNodes);

  /**
   * \brief Creates a bandwidth distribution for bidirectional channels to model DSL speeds
   *
   * The returned list contains a set of strings representing the bandwidths of an asymmetric
   * DSL connection in Germany from the client's (customer's) view , i.e (upstream, downstream).
   * The uniform random distribution is as follows (Format: (Upstream in kbps, Downstream in kbps, probability)):
   * (128, 1024, 0.1)
   * (192, 2048, 0.4)
   * (576, 6016, 0.35)
   * (1024, 16000, 0.15)
   *
   * \return a list of pairs of strings representing "typical" German DSL bandwidths
   */
  static std::list<std::pair<std::string, std::string> > GetSimpleDSLDistribution (uint32_t numberOfNodes);

// Getters and setters for parameters for unparameterized methods
public:
  /**
   * \brief Set a file for the topology helper to use for assigning connection bandwidths to nodes.
   *
   * The file is implicitly referred to in the GetBandwidthDistributionFromFile method.
   *
   * The allowed file format is as follows:
   * [line 1] <number of buckets>
   * [lines 2..<number of buckets - 1>] <size of bucket (<line no> - 2)
   * [further lines] <rx in bps> <tab> <tx in bps> [sorted by buckets]
   *
   * Note: The file is not previously checked for adherence to this format.
   *
   * @param bandwidthSamplesFile the path to the file containing the bandwidth samples distribution.
   */
  void SetBandwidthSamplesFile (std::string bandwidthSamplesFile);

  /**
   * @returns the path to the file containing the bandwidth samples distribution. Empty, if not set.
   */
  std::string GetBandwidthSamplesFile () const;

  /**
   * \brief Set default delays to use for connections.
   *
   * This method allows you to set a minimum and maximum delay time (interpreted as milliseconds) which are
   * used by unparameterized methods for the addition of nodes to the topology. The delays are chosen randomly
   * within the given bounds. Default values are between 10 and 32 ms.
   *
   * @param minDelay the minimum delay.
   * @param maxDelay the maximum delay.
   *
   */
  void SetClientLinkDelays (uint32_t minDelay, uint32_t maxDelay);

  /**
   * @returns the minimum delay (in milliseconds) used for the addition of nodes to the topology by unparameterized methods.
   */
  uint32_t GetClientLinkDelaysMin () const;

  /**
   * @returns the maximum delay (in milliseconds) used for the addition of nodes to the topology by unparameterized methods.
   */
  uint32_t GetClientLinkDelaysMax () const;

  /**
   * \brief Enable PCAP logging on devices handled by this helper.
   *
   * @param pcapFilePrefix the prefix for the pcap files. If set to the empty string, pcap logging is disabled.
   */
  void EnablePcaps (std::string pcapFilePrefix);

  /**
   * @returns the prefix for pcap files representing logs of the devices handled by this helper.
   */
  std::string GetPcapFilePrefix () const;

  /**
   * Distribute the execution of the routers to multiple CPUs based on their AS membership for MPI-based simulations.
   *
   * This method automatically distributes the execution of the routers based on the AS membership as defined by either the
   * read-in topology file or the attributes specified upon node creation. The CPU ID assigned to a node is AS mod (number of available CPUs).
   *
   */
  void DistributeRouterNodesToMPISystems();

  /**
   * Distribute the execution of the client nodes to multiple CPUs based on their AS membership for MPI-based simulations.
   *
   * This method automatically distributes the execution of the client nodes based on the AS membership of the router they are attached to.
   * The CPU ID assigned to a node is (AS of router node) mod (number of available CPUs).
   *
   */
  void DistributeClientNodesToMPISystems();

  bool WriteLastTopologyToGraphVizFile (std::string fileName) const;
};

} // ns bittorrent
} // ns ns3

#endif // BRITE_TOPOLOGY_HELPER_H__
