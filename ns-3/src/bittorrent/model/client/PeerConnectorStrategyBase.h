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
 * Authors: Rene Glebke, Martin Lang (principal authors), Alexander Hocks
 */

#ifndef PEERCONNECTORSTRATEGY_H_
#define PEERCONNECTORSTRATEGY_H_

#include "AbstractStrategy.h"

#include "BitTorrentHttpClient.h"

#include "ns3/address.h"
#include "ns3/event-id.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/socket.h"

#include <map>
#include <set>
#include <utility>

namespace ns3 {
namespace bittorrent {

class BitTorrentClient;
class Peer;

/**
 * \ingroup BitTorrent
 *
 * \brief Implements a HTTP-tracker-based peer discovery mechanism for BitTorrent swarms.
 *
 * This class implements the "traditional" HTTP-tracker based method to discover and connect to peers in a BitTorrent swarm.
 * It uses a simplified HTTP client implementation provided by the BitTorrentHttpClient class
 * to communicate with both internal (i.e., ns-3 based) and external BitTorrent trackers
 * using the standardized HTTP-based BitTorrent Tracker protocol.
 *
 * This class implements several methods commonly used in BitTorrent peer discovery mechanisms and applies them to the tracker-based approach.
 * These methods can, however, be overridden in derived classes to implement other peer discovery mechanisms.
 */
class PeerConnectorStrategyBase : public AbstractStrategy
{
// Types used
public:
  enum TrackerContactReason
  {
    STARTED = 0,
    STOPPED = 1,
    COMPLETED = 2,
    REGULAR_UPDATE = 3
  };

// Fields
protected:
  // Connections of the associated client
  std::set<uint32_t> m_connectedTo;                    // IP addresses (in integer representation) of clients we are already connected to
  std::set<uint32_t> m_pendingConnections;             // IP addresses of clients we are currently trying to establish a connection with
  std::set<uint32_t> m_knownSeeders;                   // IP addresses that we have gotten to know of that they are seeders
  std::set<std::pair<uint32_t, uint16_t> > m_potentialClients;             // All known clients, regularly cleaned from too old announcements

  // Management of incoming connection requests
  Ptr<Socket>        m_serverSocket;                   // The socket used to allow other clients to establish a connection with us

  // Reannouncements
  Time               m_periodicInterval;               // The interval at which the strategy tries to connect to new peers
  EventId            m_nextPeriodicEvent;              // The next associated periodic event

  // Swarm participant retrieval via standard HTTP tracker
  BitTorrentHttpClient m_httpCC;                       // The HTTP client used to contact the tracker
  std::stringstream  m_trackerBuffer;                  // Used to store (intermediate) answers from the tracker

  Time               m_timeout;                        // Time after which contacting the tracker is considered to have failed
  EventId            m_timeoutEvent;                   // The associated timeout event

  Time               m_reannouncementInterval;         // The interval at which we announce ourselves to the tracker (e.g., regular updates); may be overridden by the tracker
  EventId            m_nextPeriodicReannouncement;                         // The associated timeout event

  std::string        m_trackerId;                      // The id we were assigned by the tracker; used for subsequent tracker contacts

  uint8_t            m_clientUpdateCycles;             // The number of tracker contact cycles of which we want to keep returned clients in our list (i.e., a ring buffer)
  uint8_t            m_currentClientUpdateCycle;       // The current tracker contact cycle

// Constructors etc.
public:
  PeerConnectorStrategyBase (Ptr<BitTorrentClient> myClient);
  virtual ~PeerConnectorStrategyBase ();
  /**
   * \brief Initialze the strategy.
   *
   * Register the needed event listeners with the associated client.
   */
  virtual void DoInitialize ();

// Getters, setters
public:
  /**
   * \brief Get the list of clients that the client has so far discovered.
   *
   * @returns a list of <IP, port> pairs representing the clients so far discovered.
   */
  const std::set<std::pair<uint32_t, uint16_t> > & GetPotentialClients () const;

  /**
   * @returns the interval at which the client should try connecting to new peers found via the discovery mechanism.
   */
  Time GetPeriodicInterval () const;

  /**
   * \brief Set the interval at which the client should try to establish connections to peers found via the discovery mechanism.
   *
   * Per round, the strategy tries to establish as many connections to peers as appropriate respective to its other settings.
   *
   * Note: This is a client-internal setting and should not to be confused with the reannouncement period,
   * which is dictated by the discovery mechanism. Also, calls to the ConnectToPeer, ConnectToPeers, DisconnectPeers and related member
   * functions are not affected by this setting and are directly executed.
   *
   * The setting is applied at the end of the current scheduling round, i.e., it takes at most GetPeriodicInterval () until the new value is adopted.
   *
   * @param periodicInterval the desired time that shall elapse between the client's tries to connect to other peers. Ignored, if not strictly positive.
   */
  void SetPeriodicInterval (Time periodicInterval);

  /**
   * \brief Get the interval at which the client should periodically re-announce its present (update its data) with the discovery mechanism.
   *
   * @returns the period that was last set by either the a simulation control event or the discovery mechanism itself.
   */
  Time GetReannouncementInterval () const;

  /**
   * \brief Set the interval at which the client should periodically re-announce its present (update its data) with the discovery mechanism.
   *
   * Note: This setting may be overridden by the discovery mechanism. This is consistent with the a href="http://wiki.theory.org/BitTorrentSpecification" target="_blank">theory.org specification</a>.
   *
   * @param reannouncementInterval the desired re-announcement period. Ignored, if not strictly positive.
   */
  void SetReannouncementInterval (Time reannouncementInterval);

  /**
   * \brief Get the period of time in which an answer from the peer discovery mechanism has to arrive at the client.
   *
   * After this period of time has elapsed, the request is considered to have timed out and the request is re-issued if no newer request was issued in the mean time.
   *
   * @returns the desired period of time that may elapse between sending out a request to the peer discovery mechanism and an answer. Ignored, if not strictly positive.
   */
  Time GetTimeoutPeriod () const;

  /**
   * \brief Set the period of time in which an answer from the peer discovery mechanism has to arrive at the client.
   *
   * After this period of time has elapsed, the request is considered to have timed out and the request is re-issued if no newer request was issued in the mean time.
   * If a new request was issued in the mean time, the timed-out request is considered outdated and discarded.
   *
   * @param timeoutPeriod the desired period of time that may elapse between sending out a request to the peer discovery mechanism and an answer. Ignored, if not strictly positive.
   */
  void SetTimeoutPeriod (Time timeoutPeriod);

  /**
   * \brief Get the number of peers that the client is currently holding connections with.
   *
   * @returns the number of BitTorrent clients to which the client holds active connections plus the number of peers which the client is currently establishing a connection with.
   */
  virtual uint16_t GetPeerCount () const;

// Interaction methods
public:
  /**
   * \brief Connect the BitTorrent client with the cloud.
   *
   * Calling this function establishes a connection with the peer discovery mechanism and announces the presence of this client to that mechanism.
   *
   * The client will accept incoming connections from other clients when the respective listening port was set up using the StartListening method.
   *
   * Note: Depending on the used discovery mechanism, a call to this method may result in a list of clients to connnect to to be returned in the future.
   */
  virtual void ConnectToCloud ();

  /**
   * \brief Send a message to the cloud that this client is now a seeder.
   *
   * Note: Depending on the used discovery mechanism, it may be necessary for clients starting off as seeders to first call the ConnectToCloud method before.
   */
  virtual void AnnounceAsSeeder ();

  /**
   * \brief Disconnect the client from the cloud.
   *
   * This method announces to the discovery mechanism that the client will be leaving the cloud soon AND closes all active connections to other peers.
   * The client will also reject further incoming connection requests from remote clients in this state.
   */
  virtual void DisconnectFromCloud ();

  /**
   * \brief Open the specified port to listen for incoming connection attempts from remote clients.
   *
   * The client will listen on the specified port for incoming connections and assume that these connections are initiated by BitTorrent clients.
   *
   * The default port for standard BitTorrent is 6881-6889. Note that using a port not in this range may result in certain clients being unable to establish a
   * connection with the simulated client.
   *
   * @param port the port to use for listening for incoming connection attempts.
   */
  virtual void StartListening (uint16_t port);

protected:
  /**
   * \brief Close the connection to a specific peer.
   *
   * This method closes the connection to the specified remote peer.
   * The Peer class generates events that inform the rest of the client of the closed connection.
   *
   * Note: Due to concurrency issues, you should wait until the simulation has advanced to a further time stamp if you want to connect with the same remote peer again.
   *
   * @param the Peer instance representing the peer which should be disconnected.
   */
  virtual void DisconnectPeer (Ptr<Peer> peer);

  /**
   * \brief Silently close the connection to a specific peer.
   *
   * This method closes the connection to the specified remote peer as DisconnectPeer does,
   * but the Peer class will NOT issue internal events notifying the rest of the client of this situation.
   *
   * Note: This method may cause inconsistencies in the data structures of the PeerConnectorStrategyBase class, since these are only updated in response to connection
   * close events, which are not created by this method. Use with caution.
   *
   * @param the Peer instance representing the peer which should be disconnected.
   */
  virtual void DisconnectPeerSilent (Ptr<Peer> peer);

// Internal methods
protected:
  // Initiate a connection with a given client
  virtual void ConnectToPeer (Ipv4Address address, uint16_t port);
  // Initiate connections to a given number of clients arbitrarily-chosen from the available swarm members
  virtual uint16_t ConnectToPeers (uint16_t count);
  // Disconnect from a given number of arbitrarily-chosen peers
  virtual void DisconnectPeers (int32_t count);

  // Handles and resolves situations in which two clients independently try to establish connections with each other.
  virtual uint8_t AcceptConnection (Ptr<Socket> socket, Address addr);

  // Add a connection to a specific IP address to the list of established connections.
  // TODO: Change this to also consider the port, so connections to multiple clients at the same IP are supported.
  virtual void AddConnection (uint32_t address);

  // Delete a connection to a specific IP address from the list of established connections
  virtual void DeleteConnection (uint32_t address);

  /**
   * \brief This method checks whether the given number of maximum connections is reached and if not, tries to establish further connections.
   *
   * The periodicity of this method is determined by the SetPeriodicInterval member function.
   *
   * It is safe to call this function "out of schedule".
   */
  virtual void ProcessPeriodicSchedule ();

  /**
   * \brief This method takes care of the periodic reannouncements to the peer discovery mechanism.
   *
   * Periodic reannouncements are sent at lowest priority and may hence be "overridden" by any more specific request.
   *
   * The periodicity of this method is determined by the SetReannouncementInterval member function.
   *
   * It is safe to call this function "out of schedule".
   */
  virtual void ProcessPeriodicReannouncements ();

  /**
   * \brief Parse the bencoded response received from the peer discovery mechanism.
   *
   * This method inserts peers found within the response into the internal list of available peers.
   *
   * @param response an istream object containing the response of the peer discovery mechanism.
   */
  virtual void ParseResponse (std::istream &response);

// Internal callback methods
protected:
  /**
   * \brief Event handler for the TCP socket of the connection server set up with the StartListening method.
   *
   * Decides whether or not to accept an incoming connection from a remote peer.
   *
   * @returns true, if the connection is accepted.
   */
  virtual bool AcceptCallback (Ptr<Socket> sock, const  Address &addr);

  /**
   * \brief This is the event handler for accepted connections.
   *
   * This method creates an instance of the Peer class for the newly-connected peer and schedules the appropriate methods of that object
   * to initiate a Peer Wire Protocol session.
   *
   * Additionally, a timeout period is set up in which the Peer Wire Protocol connection handshake has to be exchanged.
   * Only after this period, a connection may be regarded as established (see the CheckAndDisconnectIfRejected method).
   */
  virtual void NewConnectionCreatedCallback (Ptr<Socket> sock, const  Address &addr);

  /**
   * \brief Check and add a connection based on rejection status.
   *
   * This method checks if the connection with a peer was successfully established, e.g., that BitTorrent's Peer Wire handshakes
   * were successfully exchanged. If this is the case, the new connection is announced to the client. Else, the connection is
   * silently closed.
   */
  virtual void CheckAndDisconnectIfRejected (Ptr<Peer> peer);

// Tracker contact functionality
protected:
  /**
   * \brief Listen for responses from the HTTP-based tracker and starts processesing them once they have fully arrived.
   */
  void TrackerResponseEvent (Ptr<Socket> socket);

  /**
   * \brief Handle timeouts for the peer discovery mechanism.
   *
   * @param reason the reason why the discovrery mechanism is contacted.
   * @param numwant the number of cloud members the response from the discovery mechanism should include.
   * @param closeCurrentConnection if set to true, a currently-ongoing connection with the discovery mechanism is closed and a new one initiated.
   *  Can be used to prioritize events.
   */
  void TrackerTimeout (TrackerContactReason reason, uint16_t numwant, bool closeCurrentConnection);

  /**
   * \brief Generate a GET request ("announce") for a HTTP-based tracker, establish a connection to it and send out the request.
   *
   * @param the reason why the discovrery mechanism, in this case the HTTP-based tracker, is contacted.
   * @param numwant the number of cloud members the response from the discovery mechanism should include.
   * @param additionalParameters a dictionary of parameters that should be passed to the tracker in addition to the ones defined in the
   *  <a href="http://wiki.theory.org/BitTorrentSpecification#Tracker_Request_Parameters" target="_blank">BitTorrent Protocol Specification v1.0</a>. The passed values are automatically hex-escaped.
   * @param closeCurrentConnection if set to true, a currently-ongoing connection with the discovery mechanism is closed and a new one initiated.
*  Can be used to prioritize events.
   */
  bool ContactTracker (TrackerContactReason event, uint16_t numwant, std::map<std::string, std::string> additionalParameters, bool closeCurrentConnection);
  // A wrapper for ContactTracker

  /**
   * \brief A wrapper for the ContactTracker method without the support for additional parameters.
   *
   * Can be used in Simulator::Schedule.
   */
  bool ContactTrackerWrapper (TrackerContactReason reason, uint16_t aNumwant, bool closeCurrentConnection);

  /**
   * \brief Abort an ongoing connection with the HTTP-based tracker.
   */
  void CancelCurrentTrackerRequest ();

// Event listeners
public:
  /**
   * \brief Adds information about a client to the internal data structure keeping track of peers connected to.
   */
  virtual void ProcessPeerConnectionEstablishedEvent (Ptr<Peer> peer);

  /**
   * \brief In case the establishment of a connection with a client failed, removes the respective cloud member from the list of clients we are currently trying to connect to.
   */
  virtual void ProcessPeerConnectionFailEvent (Ptr<Peer> peer);

  /**
   * \brief Remove information about a client when an opened connection is closed.
   */
  virtual void ProcessConnectionCloseEvent (Ptr<Peer> peer);

  /**
   * \brief Close an attempted connection by a seeder if this client is also a seeder.
   */
  virtual void ProcessPeerBitfieldReceivedEvent (Ptr<Peer> peer);

  /**
   * \brief Process a completed download.
   *
   * When download has finished, close connections to other seeders and
   * send out a download completed message to the peer discovery mechanism to announce that this client is a seeder now.
   */
  virtual void ProcessDownloadCompleteEvent ();
};

} // ns bittorrent
} // ns ns3

#endif /* PEERCONNECTORSTRATEGY_H_ */
