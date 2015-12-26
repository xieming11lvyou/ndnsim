/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2012 ComSys, RWTH Aachen University
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

#ifndef BRITE_TOPOLOGY_READER_H__
#define BRITE_TOPOLOGY_READER_H__

#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/string.h"
#include "ns3/topology-reader.h"

#include <string>

namespace ns3 {
/**
 * \ingroup topology
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
class BriteTopologyReader : public TopologyReader
{
public:
  static TypeId GetTypeId ();

  BriteTopologyReader ();
  virtual ~BriteTopologyReader ();

  /**
   * \brief Main topology reading function.
   *
   * This method opens an input stream and reads the BRITE-format file.
   * It reads until the end of the file and creates links
   * between the respective nodes as denoted in the file.
   *
   * The Links created by this method contain (if applicable) references to the following values:\n
   * * The data rate of the connection between the two nodes (attributes "Device-DataRate0" and "Device-DataRate1" (symmetric channels)).\n
   * * The delay of the channel between the two nodes ("Channel-Delay").
   *
   * \return the container of the nodes created (or empty container if there was an error).
   * Note that you can retrieve specially annotated nodes by special member functions.
   */
  virtual NodeContainer Read ();

  /**
   * \brief Get the nodes representing border routers / AS's in the last call to BriteTopologyReader::Read().
   *
   * Each BriteTopologyReader instance remembers the nodes tagged as border routers
   * or single AS's which were found in the last call to the instance's Read function.
   * This function returns a std::map<std::string, Ptr<Node> > to access these nodes.
   *
   * Note: This information is only kept until a subsequent call to the Read() function.
   *
   * \return a map of the nodes designated as border routers / AS's in the last run of Read, with strings as indices
   */
  std::map<std::string, Ptr<Node> >::const_iterator GetLastBorderRouters (void) const;

  /**
   * \brief Get the number of nodes representing border routers / AS's in the last call to BriteTopologyReader::Read().
   *
   * \return a number indicating the size of the map for which an iterator is returned by BriteTopologyReader::GetLastBorderRouters().
   */
  uint32_t GetLastBorderRouterCount () const;

  /**
   * \brief Get the nodes representing backbone routers / AS's in the last call to BriteTopologyReader::Read().
   *
   * Each BriteTopologyReader instance remembers the nodes tagged as backbone routers
   * or single AS's which were found in the last call to the instance's Read function.
   * This function returns a std::map<std::string, Ptr<Node> > to access these nodes.
   *
   * Note: This information is only kept until a subsequent call to the Read() function.
   *
   * \return a map of the nodes designated as backbone routers / AS's in the last run of Read, with strings as indices
   */
  std::map<std::string, Ptr<Node> >::const_iterator GetLastBackboneRouters (void) const;

  /**
   * \brief Get the number of nodes representing border routers / AS's in the last call to BriteTopologyReader::Read().
   *
   * \return a number indicating the size of the map for which an iterator is returned by BriteTopologyReader::GetLastBorderRouters().
   */
  uint32_t GetLastBackboneRouterCount () const;

  /**
   * \brief Get the nodes representing routers in the last call to BriteTopologyReader::Read().
   *
   * Each BriteTopologyReader instance remembers the nodes designated as routers
   * which were found in the last call to the instance's Read function.
   * This function returns a std::map<std::string, Ptr<Node> > to access these noes.
   *
   * Note: This list contains all nodes tagged as leaf or stub routers and such nodes not tagged at all.
   *
   * Note 2: This information is only kept until a subsequent call to the Read() function.
   *
   * \return a map of the nodes routers in the last run of Read, with strings as indices
   */
  std::map<std::string, Ptr<Node> >::const_iterator GetLastRouters () const;

  /**
   * \brief Get the number of nodes representing routers in the last call to BriteTopologyReader::Read().
   *
   * \return a number indicating the size of the map for which an iterator is returned by BriteTopologyReader::GetLastRouters().
   */
  uint32_t GetLastRouterCount () const;

  /**
   * \brief Get the autonomous systems read bsy the last call to BriteTopologyReader::Read().
   *
   * For every node in the topology, the BRITE format specifies an AS (autonomous system) membership.
   * This function returns a std::map<std::string, uint32_t> which for each node (represented by the key)
   * yields the AS specified by BRITE (the value).
   *
   * \return a map of the autonomous systems, indexed by the name of the node as specified by the BRITE topology file.
   */
  std::map<std::string, uint32_t > GetAutonomousSystems () const;

  /**
   * \brief get the highest ID of an autonomous system read in.
   *
   * \returns the ID of the highest ID of an autonomous system specified by the last BRITE file read in via BriteTopologyReader::Read().
   * If assigned, the IDs of ASs in BRITE form a continuous sequence starting at 0, so the number of ASs specified by brite is GetHighestASId() - 1.
   * The return value is -1 if no IDs were assigned (e.g., if BriteTopologyReader::Read() has not been called previously).
   */
  int32_t GetHighestASId () const;


private:
  BriteTopologyReader (const BriteTopologyReader&);
  BriteTopologyReader& operator= (const BriteTopologyReader&);

protected:
  NodeContainer m_nodes;
  NodeContainer m_tapNodes;
  std::map<std::string, Ptr<Node> > m_backboneRouters;
  std::map<std::string, Ptr<Node> > m_borderRouters;
  std::map<std::string, Ptr<Node> > m_routers;
  std::map<std::string, uint32_t> m_autonomousSystems;
  int32_t m_maxASId; // leave as signed
};

} // ns ns3

#endif // BRITE_TOPOLOGY_READER_H__
