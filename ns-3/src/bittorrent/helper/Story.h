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

#ifndef STORY_H_
#define STORY_H_

#include "brite-topology-helper.h"

#include "ns3/Torrent.h"

#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"

#include <string>
#include <map>
#include <set>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief BitTorrent simulation setup utility.
 *
 * This class can be used to read in scripts for simulations employing the BitTorrent model provided by the BitTorrentClient class and related classes.
 *
 * It reads an input "story" file in a specific format and schedules calls to specific methods of the BitTorrent simulation model classes to set up
 * simulations with user-scripted behavior. All events described in the story file have a time stamp and are inserted into the global scheduling queue
 * administered by the Simulator class for the respective time within the simulation.
 *
 * A sample story file or a BitTorrent simulation, which should give some intuition on the layout of story files, looks like the following;
 * a complete reference for the story file language is provided in the file "src/bittorrent/helper/Story-Reference.txt": \n
 * \code
 * // Set the seed for random numbers within the simulation. Use the keyword "time" instead of a number for indeterministic simulations
 * 0h0m0s: simulation set random seed 1234
 *
 * // Set the needed torrent files and folders
 * 0h0m0s: simulation set folder input/bittorrent/torrent-data
 * 0h0m0s: simulation set file input/bittorrent/torrent-data/some_torrent.torrent
 *
 * // Configure the simulation to check data transmissions
 * 0h0m0s: simulation set checkdata 1
 *
 * // Define and configure the topology to work on and add a tracker to it
 * 0h0m0s: topology set file input/bittorrent/topology/some_topology.brite
 * 0h0m0s: topology set delays min 1 max 5 // in milliseconds
 * 0h0m0s: topology add tracker
 * 0h0m0s: tracker set update interval 0h1m0s // or 0h0m60s
 *
 * // Add clients to the topology as seeders and leechers, respectively (using set arithmetics)
 * 0h0m0s: topology add clients count 2 uplink 448kbps downlink 2048kbps delay 0ms
 * 0h0m0s: all clients join group seeders
 * 0h0m0s: topology add clients count 4 uplink 448kbps downlink 2048kbps delay 0ms
 * 0h0m0s: all clients join group leechers
 * 0h0m0s: group seeders leave group leechers
 *
 * // Configure the clients
 * 0h0m0s: group seeders set initial bitfield full
 * 0h0m0s: group leechers set initial bitfield empty
 *
 * // Configure the clients a little more
 * 0h0m0s: all clients set block size request 32768 // Each request shall have length 32kB
 * 0h0m0s: all clients set autoconnect 1 // Clients shall connect automatically to the cloud when initialized
 * 0h0m0s: all clients set protocol rarest-first // Set the used protocol accordingly (rarest-first is the BitTorrent default)
 *
 * // Schedule the initiations of the clients
 * 0h0m0s: group seeders init
 * from 0h0m5s until 1h0m5s: group leechers init
 * \endcode
 *
 */
class Story : public Object
{
// Fields
private:
  // Main settings for the story file
  std::string                m_fileName;                   // The name of the story file to use
  uint32_t                   m_simulationDuration;         // The maximum length of the simulation (for this point in time, an abort is scheduled)

  // Read from the story file
  std::string                m_simulationId;               // The ID the user assigned to the simulation
  bool                       m_loggingToFile;              // Whether to direct logging output to a file or the screen
  bool                       m_useFakeData;                // Whether to use fake data or not
  bool                       m_checkData;                  // Whether to check the data upon reception
  uint32_t                   m_btNodeCount;                // The number of nodes hosting BitTorrentClients
  uint32_t                   m_otherNodeCount;             // The number of other nodes in the simulation
  std::string                m_torrentFolder;              // The folder in which the torrent data resides
  std::string                m_torrentFile;                // The used torrent file for the swarm
  std::string                m_bandwidthSamplesFile;       // The bandwidth samples file used

  std::map<std::string, std::string> m_variables;          // User-defined variables, may be read out somewhere in the code

  // Internal data fields
  BriteTopologyHelper*       m_topologyHelper;

  NodeContainer*             m_routerNodes;                // Router nodes returned by the BRITE topology helper
  NodeContainer*             m_btNodes;                    // Nodes with BitTorrentClients
  NodeContainer*             m_otherNodes;                 // Other nodes returned by the BRITE topology helper
  NodeContainer*             m_btTracker;                  // Should contain exactly one node for a tracker
  NodeContainer*             m_tapNodes;                   // Tap nodes for emulation

  std::map<std::string, NodeContainer > m_groups;          // Groups used for easy programming of crowd behavior

  ApplicationContainer*      m_clientAppContainer;         // All BitTorrentClients
  ApplicationContainer*      m_trackerAppContainer;        // Should contain exactly one BitTorrentTracker

  bool                       m_trackerAdded;               // Whether the tracker was already added (not possible again)
  bool                       m_randomSeedSet;              // Whether the random seed was already set (not possible again)

// Constructors etc. (singleton pattern)
private:
  Story ();
  virtual ~Story ();
public:
  /**
   * @returns the single instance of the Story file reader per ns3 simulation run.
   */
  static Story* GetInstance ()
  {
    static Story m_instance;
    return &m_instance;
  }

  // Setters for needed containers

  /**
   * \brief Set the node container into which the created router nodes of the topology are stored.
   *
   * @param routerNodes pointer to the node container.
   */
  void SetRouterNodeContainer (NodeContainer* routerNodes)
  {
    m_routerNodes = routerNodes;
  }

  /**
   * \brief Set the node container into which the created nodes with BitTorrent clients are stored.
   *
   * @param btNodes pointer to the node container.
   */
  void SetBTNodeContainer (NodeContainer* btNodes)
  {
    m_btNodes = btNodes;
  }

  /**
   * \brief Set the node container into which the created node with the BitTorrentTracker is stored.
   *
   * @param btTracker pointer to the node container.
   */
  void SetBTTrackerContainer (NodeContainer* btTracker)
  {
    m_btTracker = btTracker;
  }

  /**
   * \brief Set the node container into which further created nodes are stored.
   *
   * @param otherNodes pointer to the node container.
   */
  void SetOtherNodeContainer (NodeContainer* otherNodes)
  {
    m_otherNodes = otherNodes;
  }

  /**
   * \brief Set the node container into which the nodes with tap bridges are stored.
   */
  void SetTapNodeContainer (NodeContainer* tapNodes)
  {
    m_tapNodes = tapNodes;
  }

  /**
   * \brief Set the application container into which the created BitTorrentClient instances are stored.
   *
   * @param clientAppContainer the application container.
   */
  void SetBTNodeApplicationContainer (ApplicationContainer* clientAppContainer)
  {
    m_clientAppContainer = clientAppContainer;
  }

  /**
   * \brief Set the application container into which the created BitTorrenttracker instance is stored.
   *
   * @param trackerAppContainer the application container.
   */
  void SetBTTrackerApplicationContainer (ApplicationContainer* trackerAppContainer)
  {
    m_trackerAppContainer = trackerAppContainer;
  }

  void SetBriteTopologyHelper (BriteTopologyHelper* topologyHelper)
  {
    m_topologyHelper = topologyHelper;
  }

// Main interaction method
public:
  // The main method of the class: Reads and executes the story scripting functionality
  /**
   * \brief Read the contents of a story file and schedule the respective included events.
   *
   * @param filePath path to the story file to read, relative to the ns3 folder.
   * @param simulationDuration maximum duration the simulation shall take, in seconds. For this point in time, an Application::Stop event
   *  is scheduled for all simulated BitTorrent clients.
   */
  void ReadAndScheduleStory (std::string filePath, uint32_t simulationDuration);

// Getters for read simulation parameters
public:
  /**
   * @returns the ID a user set for the simulation for identification.
   */
  std::string GetSimulationId () const;

  /**
   * @returns true, if logging to file was enabled.
   */
  bool GetLoggingToFile () const;

  /**
   * @returns true, if the StorageManager should be configured to use fake payload data. See StorageManager::SetUseFakeData for details.
   */
  bool GetUseFakeData () const;

  /**
   * @returns true, if the BitTorrentPeer class shall be configured to check received data for checksum errors.
   * Can result in undefined behavior if fake data is turned on - for obvious reasons.
   */
  bool GetCheckData () const;

  /**
   * @returns the number of nodes containing instances of the BitTorrentClient class.
   */
  uint32_t GetBTNodeCount () const;

  /**
   * @returns the number of other nodes contained within the generated network.
   */
  uint32_t GetOtherNodeCount () const;

  /**
   * @returns a string containing the path to the folder containing the data of the shared files, relative to the ns3 folder.
   */
  std::string GetTorrentFolder () const;

  /**
   * @returns a string containing the path to the ".torrent" file to use within the simulation, relative to the ns3 folder.
   */
  std::string GetTorrentFile () const;

  /**
   * @returns a string containing the path to the file to use as a sample file for bandwidths in the topology to generate, relative to the ns3 folder.
   */
  std::string GetBandwidthSamplesFile () const;

  /**
   * @returns the "current" value of a variable set using the "set variable" command.
   */
  std::string GetReplacement (std::string variableName);

  /**
   * \brief Set a replacement string for variables while reading story files.
   *
   * This method sets replacement strings for variables that are applied while parsing story files.
   *
   * Variables have the form "$<i>variable_name</i>" and each occurrence of such a substring is replaced with the given string while the story
   * file is read. You can set and override such variables in story files themselves using the command "simulation set variable <i>variable_name</i> <value>".
   *
   * @param variableName the name of the variable, without any leading dollar signs.
   * @param value the value of the variable.
   */
  void SetReplacement (std::string variableName, std::string value);

  /**
   * \brief Parse a replacement string for variables while reading story files.
   *
   * This method is similar to the SetVariable member function but instead of one (variable name, value) pair it parses a string of various
   * concatenated variables of form "variable_1:value_1/variable_2:value_2" and registers each pair for replacement during story file parsing.
   *
   * @param toParse the string containing the (variable name, value) pairs to parse.
   */
  void ParseReplacements (std::string toParse);

  /**
   * @returns true, if a tracker has been added to the simulation by the story file.
   */
  bool GetTrackerAdded () const;

  /**
   * @returns true, if the random seed for the simulation has already been set.
   */
  bool GetRandomSeedSet () const;

// Static methods offered by this class
private:
  /*
   * \brief Parse a time input in a form like "0h13m37s" or "0h13m37s0u" into MilliSeconds (int result).
   *
   * @param lineBuffer the buffer from which the time shall be read. The time must start at the next available character in the buffer.
   * @param result pointer to the field storing the result
   * @param isRelative pointer to the field storing whether the parsed time shall be interpreted as a relative (true) or absolute (false) time
   * @param currentLine (optional) the line in which the time to parse is located within the story file.
   *  Used to generate messages in case of errorneous inputs.
   */
  static void ParseVideoTime (std::istringstream& lineBuffer, int32_t& result, bool& isRelative, uint32_t currentLine);
};

} // ns bittorrent
} // ns ns3

#endif /* STORY_H_ */
