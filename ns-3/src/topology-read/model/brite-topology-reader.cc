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

#include "brite-topology-reader.h"

#include "ns3/abort.h"
#include "ns3/log.h"

#include <fstream>
#include <string>

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BriteTopologyReader");
NS_OBJECT_ENSURE_REGISTERED (BriteTopologyReader);

TypeId BriteTopologyReader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BriteTopologyReader").SetParent<Object> ();
  return tid;
}

BriteTopologyReader::BriteTopologyReader ()
{
  NS_LOG_FUNCTION (this);
}

BriteTopologyReader::~BriteTopologyReader ()
{
  NS_LOG_FUNCTION (this);
}

// Reads in the supplied topology file
NodeContainer BriteTopologyReader::Read ()
{
  // Initialize possible return variables
  NodeContainer nodes;
  m_borderRouters.clear ();
  m_routers.clear ();
  m_autonomousSystems.clear ();
  m_maxASId = -1;

  // Open the input stream and declare the associated buffer variables
  ifstream file;
  istringstream lineBuffer;
  string line;
  string dummy;

  file.open (GetFileName ().c_str ());
  if (!file.is_open ())
    {
      NS_ABORT_MSG ("BriteTopologyReader: Topology file " << GetFileName ().c_str () << "not found. Aborting.");
    }

  // Get number of nodes and edges in the layout
  uint32_t numberOfNodes;
  uint32_t numberOfEdges;
  if (!file.eof ())
    {
      getline (file, line);
      lineBuffer.str (line);

      lineBuffer >> dummy >> dummy;
      lineBuffer >> numberOfNodes;
      lineBuffer >> dummy;
      lineBuffer >> numberOfEdges;
    }

  // Skip to nodes section
  bool sectionEnd = false;
  while (!file.eof () && !sectionEnd)
    {
      line.clear ();
      lineBuffer.clear ();
      getline (file, line);
      lineBuffer.str (line);

      lineBuffer >> dummy;

      if (dummy == "Nodes:")
        {
          sectionEnd = true;
        }
    }

  // Now, create the respective nodes; distinguish between border routers and normal routers
  string nodeId;
  string nodeType;
  int32_t ASId;
  map<string, Ptr<Node> > nodeMap;
  sectionEnd = false;
  while (!file.eof () && !sectionEnd)
    {
      line.clear ();
      lineBuffer.clear ();
      getline (file, line);
      lineBuffer.str (line);

      uint32_t onNode = 0;

      if (!line.empty ())
        {
          lineBuffer >> nodeId;
          lineBuffer >> dummy >> dummy >> dummy >> dummy;
          lineBuffer >> ASId;
          lineBuffer >> nodeType;

          Ptr<Node> node = CreateObject<Node> (onNode);

          if (nodeType == "RT_BORDER" || nodeType == "AS_BORDER")
            {
              m_borderRouters[nodeId] = node;
            }
          else if (nodeType == "RT_BACKBONE" || nodeType == "AS_BACKBONE")
            {
              m_backboneRouters[nodeId] = node;
            }
          else
            {
              m_routers[nodeId] = node;
            }

          m_autonomousSystems[nodeId] = ASId;
          if (static_cast<int64_t> (ASId) > static_cast<int64_t> (m_maxASId))
            {
              m_maxASId = ASId;
            }

          nodeMap[nodeId] = node;
          nodes.Add (node);
        }
      else
        {
          sectionEnd = true;
          line.clear ();
          // getline(file, line); // May resolve compatibility issues w.r.t. C++ / Java versions of BRITE
        }
    }

  // Skip to edges section
  sectionEnd = false;
  while (!file.eof () && !sectionEnd)
    {
      line.clear ();
      lineBuffer.clear ();
      getline (file, line);
      lineBuffer.str (line);

      lineBuffer >> dummy;

      if (dummy == "Edges:")
        {
          sectionEnd = true;
        }
    }

  // Now, create the links
  string nodeA, nodeB;
  string delay, bandwidth;
  sectionEnd = false;
  std::string lbr = "1";
  while (!file.eof () && !sectionEnd)
    {
      line.clear ();
      lineBuffer.clear ();
      getline (file, line);
      lineBuffer.str (line);

      if (!line.empty ())
        {
          lineBuffer >> dummy;
          lineBuffer >> nodeA >> nodeB;
          lineBuffer >> dummy;
          lineBuffer >> delay >> bandwidth;
          if (delay == "-1")
            {
              delay = "0";
            }
          delay += "ms";
          bandwidth += "Mbps";

          Link link (nodeMap[nodeA], nodeA, nodeMap[nodeB], nodeB);
          link.SetAttribute ("Device-Type", "topology");
          link.SetAttribute ("Device-DataRate0", bandwidth);
          link.SetAttribute ("Device-DataRate1", bandwidth);
          link.SetAttribute ("Channel-Delay", delay);

          AddLink (link);

          NS_LOG_DEBUG ("BriteTopologyReader: Added link from node " << (nodeA) << " to node " << (nodeB) << " with a bandwidth of " << bandwidth  << " and delay of " << delay << ".");
        }
    }

  m_nodes = nodes;

  return m_nodes;
}

std::map<std::string, Ptr<Node> >::const_iterator BriteTopologyReader::GetLastBorderRouters (void) const
{
  return m_borderRouters.begin ();
}

uint32_t BriteTopologyReader::GetLastBorderRouterCount () const
{
  return m_borderRouters.size ();
}

uint32_t BriteTopologyReader::GetLastBackboneRouterCount () const
{
  return m_backboneRouters.size ();
}

std::map<std::string, Ptr<Node> >::const_iterator BriteTopologyReader::GetLastBackboneRouters (void) const
{
  return m_backboneRouters.begin ();
}

std::map<std::string, Ptr<Node> >::const_iterator BriteTopologyReader::GetLastRouters (void) const
{
  return m_routers.begin ();
}

uint32_t BriteTopologyReader::GetLastRouterCount () const
{
  return m_routers.size ();
}

std::map<std::string, uint32_t > BriteTopologyReader::GetAutonomousSystems () const
{
  return m_autonomousSystems;
}

int32_t BriteTopologyReader::GetHighestASId () const
{
  return m_maxASId;
}

} // ns ns3
