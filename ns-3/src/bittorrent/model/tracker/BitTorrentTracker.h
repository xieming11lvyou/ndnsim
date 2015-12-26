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

#ifndef BITTORRENT_TRACKER_H
#define BITTORRENT_TRACKER_H

#include "BitTorrentHttpServer.h"

#include "ns3/BitTorrentUtilities.h"
#include "ns3/Torrent.h"

#include "ns3/address.h"
#include "ns3/application.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/socket.h"

#include <map>
#include <set>
#include <string>

namespace ns3 {
namespace bittorrent {

// Classes and types used
typedef std::map<std::string, std::string>      BTDict;
typedef std::map<std::string, BTDict>           BTDoubleDict;

/**
 * \ingroup BitTorrent
 *
 * \brief Implements a simple, HTTP-based BitTorrent tracker.
 *
 * This class implements a HTTP-based tracker for BitTorrent swarms.
 * It uses a simplified HTTP server implementation provided by the BitTorrentHttpServer class
 * to communicate with both internal (i.e., ns-3 based) and external BitTorrent clients
 * using the standardized HTTP-based BitTorrent Tracker protocol.
 *
 * The tracker can handle multiple swarms (i.e., .torrent files) at the same time. Each torrent that
 * the tracker should accept must be previously registered using the AddTorrent member function. The
 * tracker returns a HTTP 404 (not found) error when a requested torrent swarm was not registered.
 *
 * Although the tracker keeps track of the state of the respective swarm members for the implementation of
 * specialized tracker behavior but does not provide access to them outside the class itself.
 * The GlobalMetricsGatherer class is intended for collecting such information.
 */
class BitTorrentTracker : public Application
{
// Internal classes and types used
protected:
  /**
   * Saves information about all clouds this tracker manages.
   *
   * @cond HIDDEN
   */
  struct BitTorrentTrackerCloudInfo
  {
    BTDoubleDict             m_clients;          // Peer id as index; also stores seeders!
    BTDoubleDict             m_seeders;          // To store seeders only (to more easily retrieve them)
    BTDoubleDict             m_leftSeeders;      // Those seeders who have left the cloud (for logging purposes)
    BTDict                   m_info;             // Various information on the cloud
    int                      m_completed;        // number of "completed" events received
  };
  /// @endcond HIDDEN

// Fields
private:
  // Main attributes
  BitTorrentHttpServer       HttpCS;             // The HTTP server this tracker uses

  std::string                m_announcePath;     // The announce URL path that the tracker accepts
  std::string                m_scrapePath;       // The scrape URL path that the tracker accepts
  std::string                m_updateInterval;   // Reannounce interval; a string since we only send it

protected:
  /// @cond HIDDEN
  std::map<std::string, BitTorrentTrackerCloudInfo> m_cloudInfo;     // The clouds that the tracker serves; info hash as index
  /// @endcond HIDDEN

protected:
  // Event listeners and callbacks
  /// @cond HIDDEN
  Callback<void, Ptr<Node>, TypeId, Callback<void, Ptr<Socket>, const Address &> > m_handleStartListening;
  Callback<void> m_handleStopListening;

  Callback<void, Ptr<Socket>, const Address &> m_handleConnectionCreation;
  Callback<void, Ptr<Socket>, const Address &, Callback<void, Ptr<Socket> > > m_handleConnectionCreated;

  Callback<void, Ptr<Socket>, Callback<void, std::string, Ptr<Socket>, const Address& >, Callback<void, Ptr<Socket>, std::string, const Address& > > m_handleReceiveRequest;
  Callback<void, Ptr<Socket> > m_handleReceiveReq;

  Callback<void, Ptr<Socket>, std::string, const Address& > m_handleErrorOccurance;
  Callback<void, std::string, Ptr<Socket>, const Address& > m_handleDataCreater;

  Callback<void, Ptr<Socket>, uint8_t*, uint32_t, std::string > m_handleSendData;
  /// @endcond HIDDEN

// Constructors etc.
public:
  BitTorrentTracker ();
  virtual ~BitTorrentTracker ();
  static TypeId GetTypeId (void);
public:
  virtual void StartApplication (void);
  virtual void StopApplication (void);
protected:
  virtual void DoDispose (void);
private:
  BitTorrentTracker (const BitTorrentTracker&);
  BitTorrentTracker& operator = (const BitTorrentTracker);

// Getters, setters
public:
  std::string GetAnnouncePath () const;

  /**
   * \brief Set the path that the tracker should use as its announce path.
   *
   * The default is "/announce".
   *
   * @param announcePath The announce path to use.
   */
  void SetAnnouncePath (std::string announcePath);

  std::string GetScrapePath () const;

  /**
   * \brief Set the path that the tracker should use as its scrape path.
   *
   * The default is "/scrape".
   *
   * @param scrapePath The scrape path to use.
   */
  void SetScrapePath (std::string scrapePath);

  // Retrieves the full announce path, including IP address and port number
  /**
   * @returns the fully-qualified announce URL to use in a request to the tracker, including the tracker's IP and listening port.
   */
  std::string GetAnnounceURL () const;

  /**
   * @returns the fully-qualified scrape URL to use in a request to the tracker, including the tracker's IP and listening port.
   */
  std::string GetScrapeURL () const;

  Time GetUpdateInterval () const;

  /**
   * \brief Set the interval at which clients shall send their periodic announcements to the tracker.
   *
   * This interval is included within each answer of the tracker to an announce of a client.
   *
   * The default implementation of the tracker does not include any time-out for clients, i.e., it does not erase clients that fail to comply
   * to this setting; it is solely used to more accurately mimic BitTorrent traffic on networks.
   *
   * Note that the settings does not get effective immediately since each client needs at least one announcement after changing this setting
   * to retrieve the new value.
   *
   * @param updateInveral The interval at which clients shall dend their periodic announcements to the tracker. Must be strictly positive.
   */
  void SetUpdateInterval (Time updateInterval);

// Main interaction methods
public:
  /**
   * \brief Load a torrent into the simulation and register it with the tracker. Only loaded torrents are accepted by the tracker.
   *
   * The returned Torrent object can be directly used as an input to the BitTorrentClient class.
   *
   * @param path the path (relative to the current execution directory) to the ".torrent" file to be loaded.
   *
   * @returns a pointer to a simulation-global Torrent class instance that contains all data needed by the BitTorrentClient class to work within a specific
   * torrent swarm. Any tracker information within the ".torrent" file such as tracker URLs are edited to reflect the situation within the simulation.
   */
  Ptr<Torrent> AddTorrent (std::string path, std::string file);

  /**
   * \brief Issue a call to the internal data structure for a certain torrent that it shall await a flash-crowd in the near future.
   *
   * Can be used to speed up the simulation in certain scenarios.
   *
   * @param torrent the Torrent object representing the swarm that expects the flash crowd.
   * @param expectedClients the expected number of clients to arrive.
   */
  void PrepareForManyClients (Ptr<Torrent> torrent, uint32_t expectedClients);

  /**
   * \brief Make the tracker ignore requests for a swarm associated with a specific Torrent object.
   *
   * Information on members of the swarm will be removed from the tracker and requests for a removed swarm will be answered with a HTTP 404 (not found) response.
   *
   * @param torrent the Torrent for which information should be removed from the tracker.
   */
  void IgnoreTorrent (Ptr<Torrent> torrent);

  /**
   * \brief Accept requests for a swarm associated with a specific Torrent object again.
   *
   * After calling this method, the tracker will accept requests for the torrent swarm again.
   *
   * Note that, by the BitTorrent protocol, clients have to re-announce themselves with event "started" in order to be added to the swarm by the tracker.
   *
   * @param torrent the Torrent for for which the tracker should accept requests again.
   */
  void AcceptTorrent (Ptr<Torrent> torrent);

// Event listeners and callbacks
private:
  // Simply calls the ConnectionCreated from HttpCS
  void ConnectionCreation (Ptr<Socket> socket, const Address &addr);

  // Calls the ReceiveRequest from HttpCS to receive the incoming request
  void ReceiveReq (Ptr<Socket> socket);

  // Takes care of sending correct error codes
  void ErrorOccurance (Ptr<Socket> socket, std::string ErrorCode, const Address& fromAddress);

  // The callback that is triggered when the server wants to answer; triggers the generation of an answer
  void DataCreater (std::string path, Ptr<Socket> socket, const Address& fromAddress);

// Overridable members for tracker-specific behavior
public:
  /**
   * \brief Generate a bencoded response to a peer's request (announce / scrape).
   *
   * This method creates a tracker response to a BitTorrent client.
   * The message is bencoded according to <a href="http://www.bittorrent.org/beps/bep_0003.html" target="_blank">Bram Cohen's official BitTorrent specification</a>.
   * The default implementation returns up to <a href="http://wiki.theory.org/BitTorrentSpecification#Tracker_Request_Parameters" target="_blank">numwant</a> randomly-selected swarm members
   * as the client list.
   *
   * @param clientInfo the clientInfo structure of the client for which the response should be generated.
   * @returns a string holding the bencoded response for a peer.
   */
  std::string GenerateResponseForPeer (const BTDict& clientInfo) const;

  /**
   * \brief Convert information supplied in a HTTP GET path from a client into the map data structure used to internally store client information.
   *
   * The HTTP GET request in the supplied string is converted into STL a dictionary of type std::map<std::string, std::string> (BTDict),
   * with the info_hash being converted into a string represenation and url-encoded strings also being appropiately decoded. A standard
   * request of type http://tracker/announce?a=b&c=d is translated into a dictionary with two keys, "a" and "c", with corresponding entries "b" and "d".
   *
   * @param path the HTTP GET path to parse and convert.
   * @returns the request in a STL dictionary of type std::map<std::string, std::string> (BTDict).
   */
  static BTDict ExtractInfoFromClientMessage (std::string path);

// Internal methods
private:
  // Adds an info_hash to the trackers internal data structures so that announces for this torrent will be handled
  void AddInfoHash (std::string info_hash);
  // The inverse of AddInfoHash. A torrent without a registered info_hash will be ignored
  void RemoveInfoHash (std::string info_hash);

  // Adds a client to a cloud's information structure
  void AddClient (BTDict& clientInfo);
  // Updates the information stored about a client in a cloud's information structure
  void UpdateClient (BTDict& clientInfo);
  // Moves a client in a cloud's information structure from the set of peers to the set of seeders
  void SetClientToSeeder (BTDict& clientInfo);
  // The inverse of AddClient()
  void RemoveClient (const BTDict& clientInfo);
};

} // ns bittorrent
} // ns ns3

#endif // BITTORRENT_TRACKER_H
