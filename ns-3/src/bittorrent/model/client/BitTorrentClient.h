/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2012, 2014 ComSys, RWTH Aachen University.
 * Partially copyright (c) 2014 National and Kapodistrian University of Athens.
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
 * Authors: Rene Glebke, Martin Lang (principal authors), Elias Weingaertner (initial author), Alexander Hocks
 * Contributors: Aristotelis Glentis (U Athens)
 */

#ifndef BITTORRENTCLIENT_H_
#define BITTORRENTCLIENT_H_

#include "AbstractStrategy.h"

#include "ns3/BitTorrentUtilities.h"

#include "BitTorrentPeer.h"
#include "ns3/Torrent.h"

#include "ns3/application.h"
#include "ns3/data-rate.h"

#include <list>
#include <map>
#include <vector>

// An precompiler method for announcing value changes
#define CHANGED_OPTION(optionName, oldValue, newValue) { StrategyOptionsChangedEvent (optionName, lexical_cast<std::string> (oldValue), lexical_cast<std::string> (newValue)); }

namespace ns3 {
namespace bittorrent {

class AbstractStrategy;

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent client application for ns-3.
 *
 * This class provides a BitTorrent client application model for ns-3. The client model ships with an implementation of the BitTorrent protocol that adheres to Bram Cohen's official <a href="http://www.bittorrent.org/beps/bep_0003.html" target="_blank">BitTorrent Protocol Specification</a>
 * and provides client logic according to the descriptions found on the <a href="http://wiki.theory.org/BitTorrentSpecification" target="_blank">BitTorrent Protocol Specification on wiki.theory.org</a>.
 * This class is a centralized point of information storage and -exchange for one instance of the BitTorrent client model.
 *
 * The central idea of the client model is the ability for researchers to exchange or enhance the client logic in order to test protocols based on the BitTorrent paradigm with low effort.
 * For this reason, the client logic is intended to be subdivided into logical units referred to as "strategies". Classes implementing strategies can be notified of BitTorrent's Peer Wire protocol messages and exchange information with other classes of the same client instance by triggering and listening to events handled by this BitTorrentClient class.
 * Communication with other members of a BitTorrent swarm is provided by methods of the Peer class.
 *
 * For a general overview of the architecture of the system, see the WNS3 2012 workshop paper "Building a modular BitTorrent model for ns-3".\n
 * To see a very basic example on the encouraged way of implementing strategies, open the implementation of the RequestSchedulingStrategyBase class in the related file.
 * \n
 * \n
 * \n
 * The methods provided by this class can be subdivided into the following logical subsets. The ordering in the list of methods adheres to the order of these subdivisions:
 *
 * <b>1. General methods and settings</b>\n
 * <i>Constructors and related methods</i>: Used for the initialization of the client class, choosing the shared file to download, the employed downloading method (SetTorrent and SetProtocol methods, respectively), the initial download status of the client (bitfield manipulation methods) and starting up the client (DoInitialize method).\n
 * <i>Report generation methods</i>: Methods used to generate periodic output by the client for user-defined metrics.\n
 * <i>Local client information</i>: Grants access to information about the client such as ID in the cloud or uplink/downlink bandwidth information.
 *
 * <b>2. Client behavior settings</b>\n
 * <i>Cloud member behavior methods</i>: Allow to define when the client shall connect to the cloud during its lifetime and the length of its lifetime as a seeder.\n
 * <i>BitTorrent parameters</i>: Control the central parameters of a client within a BitTorrent network, such as number of peers and request sizes.
 *
 * <b>3. Internal data</b>\n
 * <i>Bitfield-related methods</i>: These methods give access to the client's internal bitfield that saves download progress.\n
 * <i>Strategy methods</i>: Access to the strategies operating for the respective client instance and their respective settings.\n
 * <i>Peer list</i>: Access to the peers currently associated with the client and methods for initiation and closing of connections.
 *
 * <b>4. Internal events</b> (Sorted by occurence in standard usage scenarios)\n
 * <i>Application initialization event</i>: Triggered when the application and all associated strategies have been initialized.\n
 * <i>Peer discovery-related events</i>:  Events triggered by the respective peer discovery mechanism.\n
 * <i>Connection events</i>: Events triggered by incoming, successfully created or closed connections from/to remote clients.\n
 * <i>Peer Wire Message-related events</i>: Events triggered by the reception of a Peer Wire Protocol message from a remote client.\n
 * <i>Download/upload-related events</i>: Events triggered by the (partial) download/upload of a certain block of the shared file from/to a remote client.\n
 * <i>Settings change events</i>: Triggered when client or strategy settings are changed (e.g., using the methods in 2. and 3.).\n
 * <i>Report generation events and methods</i>: Triggered to gather information within the client for output (cf. the methods in 1.).
 *
 * <b>5. Event listener registration and deregistration methods</b> (adhering to the ordering of 4.).
 *
 * <b>6. Peer connection strategy callbacks</b>\n
 * <i>Cloud connection methods</i>: Methods that may be called from outside the client (e.g., a controlling script) to establish or tear down connections to the cloud.\n
 * <i>Callback establishmeht methods</i>: Methods that may be used by strategies to announce their ability to serve a certain cloud or peer connection functionality.
 */
class BitTorrentClient : public Application
{
// Fields
private:
  // The main attributes of the BitTorrentClient
  Ptr<Torrent>                         m_torrent;                    // Reference to the global Torrent instance
  std::string                          m_protocol;                   // String indicating the strategy the client should apply (for StrategyFactory)
  const uint8_t*                       m_torrentDataPtr;             // Reference to the global copy of the shared file from the StorageManager
  std::string                          m_bitfieldFillType;           // A string indicating the way the bitfield of this client instance is filled at start (i.e., download status). Using this after initialization in StartApplication() ins meaningless.
  std::map<uint32_t, uint8_t>          m_bitfieldManipulations;      // Can be used to edit the bitfield after pre-filling it. Using this after initialization in StartApplication() ins meaningless.
  Time                                 m_gatherMetricsEventPeriodicity;        // The intervals at which the client should collect status information for output

  std::string                          m_peerId;                     // The client's self-generated peer id

  // Attributes derived from the node the client is installed on
  uint16_t                             m_interfaceId;                // The id of the interface the client operates on. Influences m_ip.
	Ptr<NetDevice>                       m_interface;                  // The interface               
  Ipv4Address                          m_ip;                         // The IP address of this client. To circumvent too many "pointer hops"
  uint16_t                             m_port;                       // The port the client operates on 

  // BitTorrent-related settings
  bool                                 m_autoConnect;                // Whether the client should directly connect to the cloud upon initialization
  Time                                 m_seedingDuration;            // The time the client should stay in the cloud after having completed download. Negative => stay forever

  uint16_t                             m_desiredPeers;               // The number of peers the client should connect to
  uint16_t                             m_maxPeers;                   // The maximum number of peers the client should hold open connections to
  uint16_t                             m_maxUnchokedPeers;           // The maximum number of peers the client may unchoke in one "unchoke run"

  uint16_t                             m_maxRequestsPerPeer;         // The maximum number of concurrent requests sent to a peer
  uint16_t                             m_maxRequestsPerPiece;        // The maximum number of concurrent requests for a piece (i.e., how many peers should be asked)
  uint16_t                             m_maxRequestsPerBlock;        // Similar to above, but for block level requests
  uint16_t                             m_maxRequestsPerPeerPerPiece; // Similar to above, but for block level requests

  uint32_t                             m_requestBlockSize;           // The number of bytes each REQUEST message should ask for
  uint32_t                             m_sendBlockSize;              // The number of bytes each PIECE message should contain. May be lower than the request size.

  Time                                 m_pieceTimeout;               // Time after which a piece should be conisdered to have timed out
  // Time                                 m_postPieceTimeoutPatience;   // A currently unused attribute for a work-in-progress heuristic in the base part selection strategy

  bool                                 m_checkDownloadedData;        // Whether to perform SHA-1 checks on downloaded pieces

  // Internal derived variables (stored for faster access to them)
  uint32_t                             m_piecesCompleted;            // Number of pieces downloaded so far
  uint64_t                             m_bytesCompleted;             // The number of bytes downloaded so far
  bool                                 m_downloadCompleted;          // Whether the download of the file was completed

  // Internal variables
  std::list<Ptr<AbstractStrategy> >    m_strategyList;               // Stores the components of the applied protocol
  std::map<std::string, std::string>   m_strategyOptions;            // Stores the parameters passed to the protocol strategies
  std::map<std::string, std::pair<std::string, std::string> >        m_lastChangedStrategyOptions; // Provides the settings delta for the strategies. Settings in GetOptions() are the same but also include previously-set options.
  std::string                          m_lastChangedStrategyOptionName;        // If only one option was changed, this field stores its name during the StrategyOptionsChangedEvent

  std::vector<Ptr<Peer> >              m_peerList;                   // Contains the list of all currently associated Peer objects (i.e., communication partners)

  bool                                 m_connectedToCloud;           // Whether the client is currently connected to the cloud
  bool                                 m_connectionToCloudSuspended; // Whether returns by the tracker should be processed (i.e., connections established) or not

protected:
  // The main attributes of the BitTorrentClient
  std::vector<uint8_t>                 m_bitfield;                   // Client-local bitfield
  // TODO: Change all to private once the new bitfield type has been established

private:
  // Listeners for connection-related events
  std::list<Callback<void, Ptr<Peer> > >         m_establishedEventListeners;  // After a connection has been established. Bitfield not yet ready, communication instable.
  std::list<Callback<void, Ptr<Peer> > >         m_failEventListeners;         // After a connection encountered a failure
  std::list<Callback<void, Ptr<Peer> > >         m_closeEventListeners;        // After a connection was closed (i.e., no communication possible anymore)
  std::list<Callback<void, Ptr<BitTorrentClient> > >       m_cloudConnectionEstablishedEventListeners;
  std::list<Callback<void, Ptr<BitTorrentClient> > >       m_cloudConnectionSuspendedEventListeners;

  // Listeners for tracker-related events
  std::list<Callback<void> >                                         m_trackerResponseReceivedListerners;

  // Listeners for BitTorrent Peer Wire Protocol (PWP) messages, sorted by ID
  std::list<Callback<void, Ptr<Peer> > >         m_chokeEventListeners;        // Also: unchoke
  std::list<Callback<void, Ptr<Peer> > >         m_interedEventListeners;      // Also: uninterested
  std::list<Callback<void, Ptr<Peer>,uint32_t> > m_haveEventListeners;
  std::list<Callback<void, Ptr<Peer> > >         m_bitfieldEventListeners;     // The central event triggering strategies. Indicates readiness for communication.
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >   m_requestEventListeners;
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >   m_cancelEventListeners;
  std::list<Callback<void, Ptr<Peer>,uint16_t> >                     m_portMessageEventListeners;
  std::map<uint8_t, std::list<Callback<void, Ptr<Peer>, const std::string& > > >         m_extensionMessageListeners;  // Can be used for arbitrary additional messages according to BEP 10.

  // Listeners for download-related events (PIECE messages, REQUEST messages)
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >   m_blockCompleteEventListeners;
  std::list<Callback<void, Ptr<Peer>, uint32_t > >                   m_pieceCompleteEventListeners;
  std::list<Callback<void> >                                         m_downloadCompleteEventListeners;
  std::list<Callback<void, Ptr<Peer>, uint32_t > >                   m_pieceTimeoutEventListeners;

  // Listeners for upload-related events
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >   m_blockUploadCompleteEventListeners;

  // Listners for application initialization
  std::list<Callback<void, Ptr<BitTorrentClient> > >                 m_applicationInitializedEventListeners;

  // Listeners for changes in the strategy options
  std::list<Callback<void> >                                         m_strategyOptionsChangedEventListeners;                                                       // Each strategy with attributes that may be changed at run-time should register to this

  // Listeners for the periodic status generation calls by the GlobalMetricsGatherer, if existent
  std::list<Callback<std::map<std::string, std::string> > >          m_gatherMetricsEventListeners;

private:
  // Callback plugs for the connector strategy (e.g., tracker-based, DHT, ...)
  Callback<void>                                 m_connectToCloud;             // To connect to the cloud
  Callback<void>                                 m_disconnectFromCloud;        // To disconnect from the cloud
  Callback<void>                                 m_announceAsSeeder;           // To announce oneself as a seeder (e.g., after completion of download)
  Callback<void, Ipv4Address, uint16_t>          m_connectToPeer;              // To explicitly connect to a given peer (address, port)
  Callback<uint16_t, uint16_t>                   m_connectToPeers;             // To connect to a given number of peers (which ones is at the implementing strategy's discretion)
  Callback<void, int32_t>                        m_disconnectPeers;            // To terminate the connection to a number of peers
  Callback<void, Ptr<Peer> >                     m_disconnectPeer;             // To terminate the connection to a certain peer (e.g., if it is stale)
  Callback<uint16_t>                             m_peerCount;                  // To determine the sum of open and pending connections (more reliable than the size of the peer list)

private:
  // TODO: Fields to rework
  std::list<Callback<void, Ptr<Peer>, uint32_t > > m_pieceRequestedEventListeners;
  std::list<Callback<void, Ptr<Peer>, uint32_t > > m_pieceCancelledEventListeners;
  // After a REQUEST message was queued for transmission?
  // Maybe change to block level? Also provide this version (and a piece-reduced version) for UploadComplete...

// Constructors etc.
public:
  BitTorrentClient ();
  virtual ~BitTorrentClient ();
  static TypeId GetTypeId ();

protected:
  virtual void StopApplication (void);
  virtual void DoDispose (void);

public:
  /**
   * \brief Initialize the application.
   *
   * A call to this method initializes the BitTorrent client application by attaching the appropriate strategies to the client,
   * filling the bitfield according to the predefined method, opening the server for incoming connections and, if the auto connect
   * functionality (see SetAutoConnect()) is turned on, joining the cloud by sending a joining announce to the tracker.
   *
   * The application issues an ApplicationInitializedEvent to which all strategies may listen to after full initialization has taken place.
   *
   * Note: You must set the shared file the client shall download and the employed downloading strategies with the SetTorrent and SetProtocol
   * methods, respectively, before calling this method. Otherwise, initialization of the client will fail and the simulation may abort.
   */
  virtual void StartApplication (void);

// Getters, setters
public:
  // The main attributes of the BitTorrentClient

  /**
   * @returns a pointer to the Torrent object representing the shared file the client instance is trying to download.
   */
  const Ptr<Torrent> GetTorrent () const
  {
    return m_torrent;
  }

  /**
   * \brief Attach a Torrent class instance to the client.
   *
   * This method must be called before the DoInitialize method is called.
   *
   * This method assigns a (usually, shared) Torrent class instance to the client. The Torrent object contains
   * central information on the swarm and the shared file used by almost every strategy (= client logic) implementation.
   *
   * @param torrent pointer to the desired torrent. Ignored, if already set.
   *
   */
  void SetTorrent (Ptr<Torrent> torrent);

  /**
   * @returns a string identifying the protocol this instance of the BitTorrent client implements.
   */
  const std::string GetProtocol () const
  {
    return m_protocol;
  }

  /**
   * \brief Set the protocol the instance of the BitTorrent client shall use.
   *
   * The string identifies the protocol to be attached to the BitTorrentClient class by the ProtocolFactory.
   *
   * Set this to "default" to choose the default BitTorrent implementation with sequential part selection, or to "rarest-first"
   * to implement standard BitTorrent behavior with a rarest-first part selection strategy. Identifiers for further
   * strategies must be implemented into the ProtocolFactory::CreateStrategyBundle method.
   *
   * @param protocol the protocol to be attached. Ignored, if already set.
   */
  void SetProtocol (std::string protocol);

  /**
   * \brief Get the initial filling of the client's bitfield.
   *
   * @returns a string identifying the method used to initialize the client's bitfield at startup.
   */
  std::string GetInitialBitfield () const;

  /**
   * \brief Set the initial filling of the client's bitfield.
   *
   * With this method, you can set how the bitfield of the client is initialized at startup to simulate different types of clients
   * (e.g., seeders, leechers, peers in-between).
   *
   * The supported settings are:\n
   * "empty" for an empty bitfield,\n
   * "full" for a completely full bitfield (the client will announce itself as a seeder at startup),\n
   * "random" for a bitfield with randomly-set entries to simulate a client in the middle of a download process, or\n
   * (any other setting) for a bitfield filled up to a certain point from the left, with a tail of randomly-set entries to simulate a Video-on-Demand client during playback.
   *
   * Calls to this method after a call to DoInitialize are meaningless.
   *
   * @param bitfieldFillType the string identifying the method to be used to fill the bitfield at startup.
   */
  void SetInitialBitfield (std::string bitfieldFillType);

  /**
   * \brief Manipulate the initial filling of the client's bitfield entry-wise.
   *
   * You can manipluate the bitfield's initial filling in an entry-wise fashion. Generally, the filling of the bitfield is controlled by the
   * SetInitialBitfield method, but you may use this method to specifically set some pieces to locally available/unavailable. Note that the
   * bitfield is stored in the same fashion as BITFIELD messages are organized, i.e., the high bit of the first byte in the bitfield
   * corresponds to the first piece in the file.
   *
   * Calls to this method after a call to DoInitialize are meaningless.
   *
   * @param bitfieldIndex the index of the entry in the bitfield you want to manipulate.
   * @param value the new value of the given entry.
   */
  void ManipulateInitialBitfield (uint32_t bitfieldIndex, uint8_t value);

  /**
   * @returns the interval between triggerings of the metrics gathering event.
   */
  Time GetGatherMetricsEventPeriodicity () const;

  /**
   * \brief Set the interval between triggerings of the client-local metrics gathering event.
   *
   * The client can periodically locally collect metric data for output. Use this setting to control the periodicity of the associated event.
   *
   * @param gatherMetricsEventPeriodicity the periodicity of the event. Ignored, if not strictly positive.
   */
  void SetGatherMetricsEventPeriodicity (Time gatherMetricsEventPeriodicity);

  /**
   * \brief Get a pointer to the memory location in which the <b>whole</b> shared file is stored by the StorageManager class.
   *
   * This pointer can be used to access data of the file for transmission. See the documentation of the StorageManager class for details.
   *
   * Do <b>not</b> cast away the constness of this pointer! Changing the contents of the shared file may lead to corrupted data being
   * transferred within the simulation, and SHA-1 hashes to break.
   *
   * @returns a pointer to the start of an array holding the shared file.
   */
  const uint8_t* GetTorrentDataBuffer () const
  {
    return m_torrentDataPtr;
  }

  /**
   * @returns a pointer to the memory location holding the info hash of the shared file as byte values.
   */
  const uint8_t* GetCurrentInfoHash () const;

  /**
   * @returns the client's chosen identifier within the swarm. It is composed of the word "VODSim", a dash, the ID of the Node the Application is installed on, another dash and a random sequence.
   */
  std::string GetPeerId () const;
  
  /**
   * \brief Get the id of the interface the client uses for communication.
   *
   * @returns the id of the interface the client uses for communication.
   */
  uint16_t GetInterfaceId () const;
  
  /**
   * \brief Get the  interface the client uses for communication.
   *
   * @returns the interface the client uses for communication.
   */
  Ptr<NetDevice> GetInterface() const;
  
  /**
   * \brief Set the interface the client should use for communication.
   * 
   * Note: Do not use this method while the client is operating; it may break operation.
   *
   * @param interfaceId the ID of the interface the client should use. It automatically derives and updates the IP address used. Default is the first interface of the node.
   */
  void SetInterfaceId (uint16_t interfaceId) ;

  /**
   * \brief Get the local IP address of the interface used for communication.
   *
   * Note: By default, this returns the IP address of the first Interface of the Node the BitTorrentClient Application is installed on.
   * You may need to override this method to support other scenarios.
   *
   * @returns the IP address assigned to the first interface of the Node that the BitTorrentClient class is installed on.
   */
  virtual Ipv4Address GetIp () const;
  
  /**
   * \brief Set the port the BitTorrent client should listen on for incoming connections.
   *
   * Use this method to make the BitTorrent client open a specific port for incoming connections if you have installed multiple clients on one specific or interface.
   *
   * @param the port the local server for incoming connection should listen on. Default is 6881, BitTorrent's default range is 6881-6889.
   */
  void SetPort (uint16_t port);

  /**
   * \brief Get the port the BitTorrent client is listening on for incoming connections.
   *
   * @returns the port the local server for incoming connection attempts of BitTorrent clients is listening on.
   */
  uint16_t GetPort () const;

  /**
   * \brief Get the downlink speed of the interface used for communication.
   *
   * Note: By default, this method returns the downlink speed of the first Interface of the Node the BitTorrentClient Application is installed on.
   * You may need to override this method to support other scenarios.
   *
   * @returns the downlink speed of the interface used for communication.
   */
  virtual DataRate GetBpsDownlink () const;

  /**
   * \brief Get the uplink speed of the interface used for communication.
   *
   * Note: By default, this method returns the uplink speed of the first Interface of the Node the BitTorrentClient Application is installed on.
   * You may need to override this method to support other scenarios.
   *
   * @returns the uplink speed of the interface used for communication.
   */
  virtual DataRate GetBpsUplink () const;

  // BitTorrent-related settings

  /**
   * @returns the auto-connect setting of the client.
   */
  bool GetAutoConnect () const;

  /**
   * \brief Set the auto-connect setting of the client.
   *
   * The client can automatically connect to the cloud at startup.
   *
   * @param autoConnect set to true if the client should connect automatically to the cloud at startup.
   */
  void SetAutoConnect (bool autoConnect);

  /**
   * @returns the time the client remains in the cloud after finishing its download.
   */
  Time GetSeedingDuration () const;

  /**
   * \brief Set the time the client remains in the cloud after finishing its download.
   *
   * The client can remain connected to the cloud after finishing its download to simulate different altruism levels of cloud members.
   *
   * @param seedingDuration the time the client shall spend as a seeder. If negative, the client remains a seeder until the end of the simulation.
   */
  void SetSeedingDuration (Time seedingDuration);

  /**
   * @returns the number of desired peers.
   */
  uint16_t GetDesiredPeers () const;

  /**
   * \brief Set the number of peers that the clients wants to be connected to.
   *
   * The peer selection strategy will try to actively connect to as many clients as allowed by this setting.
   *
   * @param desiredPeers the number of desired peers. Default is 30.
   */
  void SetDesiredPeers (uint16_t desiredPeers);

  /**
   * @returns the maximum number of peers.
   */
  uint16_t GetMaxPeers () const
  {
    return m_maxPeers;
  }

  /**
   * \brief Set the maximum number of connections the client shall have.
   *
   * The client will allow incoming connection attempts from other clients until the number of connections exceeds this setting.
   *
   * Note: Peers that are actively connected to by the client (see SetDesiredPeers) are also counted.
   *
   * @param maxPeers the maximum number of peers. Default is 55.
   */
  void SetMaxPeers (uint16_t maxPeers);

  /**
   * @returns the maximum number of concurrently unchoked peers.
   */
  uint16_t GetMaxUnchokedPeers () const;

  /**
   * \brief Set the maximum number of concurrently unchoked peers.
   *
   * This setting controls how many peers are concurrently unchoked by the choking/unchoking strategy. The strategy may unchoke as
   * many peers as allowed by this setting. Note that the optimistically unchoked peers may, depending on the strategy implementation,
   * not be affected by this setting and may be unchoked in addition.
   *
   * @param maxUnchokedPeers the maximum number of unchoked peers. Default is 4.
   */
  void SetMaxUnchokedPeers (uint16_t maxUnchokedPeers);

  /**
   * @returns the maximum number of concurrent requests sent to a peer.
   */
  uint16_t GetMaxRequestsPerPeer () const
  {
    return m_maxRequestsPerPeer;
  }

  /**
   * \brief Set the maximum number of requests concurrently sent to a peer.
   *
   * At most this many requests will be sent out to any peer by part selection strategy. The setting is on a per-peer basis.
   * Note that setting this too high may cause a large number of time-outs and hence reduce performance.
   *
   * To control the number of requests sent out concurrently to a peer for a given piece, see the SetMaxRequestsPerPeerPerPiece method.
   *
   * @param maxRequestsPerPeer the maximum number of requests per peer. Default is 16.
   */
  void SetMaxRequestsPerPeer (uint16_t maxRequestsPerPeer);

  /**
   * @returns the maximum number of concurrent block requests sent out for a piece.
   */
  uint16_t GetMaxRequestsPerPiece () const
  {
    return m_maxRequestsPerPiece;
  }

  /**
   * \brief Set the maximum number of block requests concurrently sent out for a piece.
   *
   * At most this many block requests will be sent out for a certain piece concurrently. This setting controls how many requests
   * are sent out in total for a piece, i.e., it does not control how many peers are asked for blocks of the piece.
   *
   * Example: Consider a piece divided into 3 blocks A, B and C. Setting this setting to 4 may (depending on the part selection
   * strategy) allow either 4 requests for block A, or 3 requests for block B and 1 request for block C, etc. to be sent out
   * concurrently. When one of this requests failed, another request is allowed. If one request succeeded, the part selection
   * strategy should cancel the other requests to avoid transmission of already-available data.
   *
   * @param maxRequestsPerPiece the maximum number of requests per piece. Default is 8.
   */
  void SetMaxRequestsPerPiece (uint16_t maxRequestsPerPiece);

  /**
   * @returns the maxmimum number of concurrent requests per block.
   */
  uint16_t GetMaxRequestsPerBlock () const
  {
    return m_maxRequestsPerBlock;
  }

  /**
   * \brief Set the maximum number of concurrent requests per block.
   *
   * At most this many concurrent requests will be sent out for a single block.
   *
   * @param maxRequestsPerblock the maximum number of requests per block. Default is 1, which means that each block is requested
   * from exactly one peer at any given time.
   */
  void SetMaxRequestsPerBlock (uint16_t maxRequestsPerBlock);

  /**
   * @returns the maxmimum number of concurrent requests related to one piece per peer.
   */
  uint16_t GetMaxRequestsPerPeerPerPiece () const
  {
    return m_maxRequestsPerPeerPerPiece;
  }

  /**
   * \brief Set the maxmimum number of requests for the blocks of a piece sent to a peer.
   *
   * This setting controls, how many concurrent requests are sent out to one peer for one piece.
   *
   * Note that setting this too high may cause a large number of time-outs and hence reduce performance.
   *
   * Example: Consider a piece divided into 6 blocks. Setting this setting to 3 would yield the part selection strategy to
   * send out at most 3 requests for different blocks of the respective piece to the same peer concurrently.
   * When one of these requests is answered, another request may be issued.
   *
   * To control the total number of requests sent out concurrently to a peer, see the SetMaxRequestsPerPeer method.
   */
  void SetMaxRequestsPerPeerPerPiece (uint16_t maxRequestsPerPeerPerPiece);

  /**
   * @returns the size of the blocks requested from a peer, in bytes.
   */
  uint32_t GetRequestBlockSize () const
  {
    return m_requestBlockSize;
  }

  /**
   * \brief Set the size of block requests.
   *
   * This setting controls, how large the blocks, in bytes, requested from peers using REQUEST messages are.
   *
   * @param requestBlockSize the size of requested blocks in bytes. Ignored, if equal to zero.
   *
   * Note: Parameter checking is not performed on the upper size of requests, which is the piece length as defined
   * in the ".torrent" file (see the Torrent class for details). Strategies may show undefined behavior when this
   * implicit border is exceeded. Note that for the last piece of a Torrent, which may have shorter length than the
   * other pieces (see Torrent::GetTrailingPieceLength()), request sizes should be adjusted automatically by the
   * piece selection strategies.
   *
   * @param requestBlockSize the size of blocks requested from a peer, in bytes.
   */
  void SetRequestBlockSize (uint32_t requestBlockSize);

  /**
   * @returns the size of blocks sent to a peer, in bytes.
   */
  uint32_t GetSendBlockSize () const
  {
    return m_sendBlockSize;
  }

  /**
   * \brief Set the size of block request answers.
   *
   * It is possible to alter the size of PIECE messages sent to peers as an analogy to the request size set by the SentRequestBlockSize method.
   * The strategy responsible for upload scheduling may opt to locally change the requests of peers and break them up into smaller or larger
   * chunks.
   *
   * Caution: Having this setting not aligned with the might break certain strategy implementations, such as the default one provided by the PartSelectionStrategyBase class.
   *
   * @param sendBlockSize the size of blocks sent to a peer, in bytes.
   *
   */
  void SetSendBlockSize (uint32_t sendBlockSize);

  /**
   * @returns the time that is allowed to elapse before a requested piece (not a block) times out.
   */
  Time GetPieceTimeout () const
  {
    return m_pieceTimeout;
  }

  /**
   * \brief Set the time until a piece is considered to have timed out.
   *
   * After the part selection strategy has chosen a piece for download, its aim should be to download that piece completely to ensure it can
   * be further distributed within the swarm. This setting controls the time that is allowed to elapse before a piece chosen for download is
   * considered to have timed out.
   *
   * Note: This is a per-piece and not a per-block setting. Time-outs for blocks may be calculated based on this setting and the length of pieces.
   *
   * @param pieceTimeout the time allowed to elapse. Ignored, if not strictly positive.
   */
  void SetPieceTimeout (Time pieceTimeout);

  /**
   * @returns true, if a check of downloaded data shall be performed.
   */
  bool GetCheckDownloadedData () const
  {
    return m_checkDownloadedData;
  }

  /**
   * \brief Control whether downloaded data should be checked by the client.
   *
   * You may set this setting to false in scenarios where you do not assume corruptions in the payload of PIECE messages.
   * Since no checks are performed on the received data, setting this to false may result in a simulation speedup.
   *
   * Note that checks are already performed using a direct memory content comparison for speedup reasons (see the StorageManager class for details).
   *
   * @param checkDownloadedData whether to check data downloaded from a peer
   */
  void SetCheckDownloadedData (bool checkDownloadedData);

  // Internal derived variables

  /**
   * @returns the number of pieces already successfully downloaded.
   */
  uint32_t GetPiecesCompleted () const
  {
    return m_piecesCompleted;
  }

  /**
   * \brief Update the bitfield after the completion of a piece download.
   *
   * This method sets the client-local bit corresponding to the given piece to 1, indicating a download.
   * Additionally, the number downloaded bytes (for announcements in the peer discovery mechanism) is updated.
   *
   * Note: You need not call this method for pieces already present at the initialization of the client; this is
   * done implicitly within the DoInitialize method.
   *
   * @param pieceIndex the piece for which to announce download completion.
   */
  // RENE: Should this be changed to block level?
  void SetPieceComplete (uint32_t pieceIndex);

  /**
   * @returns the number of bytes successfully downloaded from other peers. Only the length of pieces themselves are counted,
   * communication overhead (including Peer Wire messages) is not taken into consideration.
   */
  uint64_t GetBytesCompleted () const
  {
    return m_bytesCompleted;
  }

  /**
   * @returns true, if the download of the shared file has finished, i.e., when the client is in seeding mode.
   */
  bool GetDownloadCompleted () const
  {
    return m_downloadCompleted;
  }

  /**
   * \brief Get the client's bitfield.
   *
   * This method allows access to the client's local bitfield indicating the download status of the pieces of the shared file.
   *
   * Note that the bitfield is stored in the same fashion as BITFIELD messages are organized, i.e., the high bit of the first byte in the bitfield
   * corresponds to the first piece in the file.
   *
   * @returns a pointer to the first entry of the bitfield.
   */
  const std::vector<uint8_t>* GetBitfield () const
  {
    return &m_bitfield;
  }

  // Internal variables

  /**
   * @returns a pointer to a list containing the object instances implementing the client's strategy.
   */
  const std::list<Ptr<AbstractStrategy> > & GetStrategies () const;

  /**
   * @returns an iterator to the beginning of the strategy list.
   */
  std::list<Ptr<AbstractStrategy> >::const_iterator GetStrategyListIterator () const;

  /**
   * @returns an iterator to the end of the strategy list.
   */
  std::list<Ptr<AbstractStrategy> >::const_iterator GetStrategyListEnd () const;

  /**
   * @returns a pointer to a map containing the currently-set strategy options.
   */
  const std::map<std::string, std::string >& GetStrategyOptions () const;

  /**
   * \brief Get the old and current value of a recently-changed strategy option.
   *
   * @param name the name of the option.
   *
   * @returns a pair of strings, where the first value corresponds to the old, the second value to the new setting.
   * A pair of empty strings is returned in case the option has not been set in the last call to the SetStrategyOptions method.
   */
  const std::pair<std::string, std::string> GetStrategyOptionChangePair (std::string name) const;

  /**
   * @returns the name of the most recently changed strategy option.
   */
  std::string GetLastChangedStrategyOptionName () const;

  /**
   * \brief Set additional options for strategies.
   *
   * With this method, you can set strategy-specific options which are not covered by the default options offered by the
   * BitTorrentClient class. Each call to this method issues a subsequent StrategyOptionsChangedEvent to which strategies
   * may subscribe in order to obtain the new settings.
   *
   * If an option for which a previous value was set is supplied to this method, both old and new value will be available via
   * the GetStrategyOptionChangePair method. If only one option is set using this method, the name of the respective changed
   * option is available via the GetLastChangedStrategyOptionName method.
   *
   * Note: For easiness of implementation, options once set are retained during the lifetime of the BitTorrentClient class.
   * You may unset an option by setting its corresponding value to "" (empty string).
   *
   * @param options a map mapping the strategy options (keys) to their respective values.
   */
  void SetStrategyOptions (std::map<std::string, std::string> options);

  /**
   * @returns a vector containing pointers to the Peer class instances representing the active connections of the client.
   */
  const std::vector<Ptr<Peer> > & GetActivePeers () const
  {
    return m_peerList;
  }

  /**
   * @returns an iterator to the beginning of the list of peers.
   */
  std::vector<Ptr<Peer> >::const_iterator GetPeerListIterator () const;

  /**
   * @returns an iterator to the end of the list of peers.
   */
  std::vector<Ptr<Peer> >::const_iterator GetPeerListEnd () const;

  /**
   * @returns the number of currently connected peers
   */
  uint16_t GetConnectedPeerCount () const
  {
    return m_peerList.size ();
  }

  /**
   * \brief Add a peer to the client's list of peers.
   *
   * @param peer pointer to the Peer object to add.
   */
  void RegisterPeer (Ptr<Peer> peer);

  /**
   * \brief Remove a peer from the client's list of peers.
   *
   * @param peer pointer to the Peer object to remove from the list.
   */
  void UnregisterPeer (Ptr<Peer> peer);

  /**
   * @returns true, if the client is currently connected to the cloud AND a suitable list of peers has been received.
   */
  bool GetConnectedToCloud () const;

  /**
   * \brief Set the client's current cloud connection status.
   *
   * This method sets the cloud connection status returned by the GetConnectedToCloud method. This method is intended to be called
   * by the peer discovery mechanism to indicate the current connection status to other strategies.
   *
   * @param connectedToCloud should be set to true if the connection to the cloud at least once resulted in a suitable list of peers to connect to.
   *
   */
  // TODO: This should be changed to a callback into the peer discovery mechanism.
  void SetConnectedToCloud (bool connectedToCloud);

  /**
   * @returns false, if the client is currently considering itself to be connected to the cloud.
   */
  bool GetConnectionToCloudSuspended () const;

  /**
   * \brief Set the client's current cloud connection status.
   *
   * This method sets the cloud connection status returned by the GetConnectionToCloudSuspended method. This method is intended to be called
   * by the peer discovery mechanism to indicate the current connection status to other strategies.
   *
   * @param connectionToCloudSuspended should be set to true if the client currently does <b>not</b> consider itself to be part of the cloud,
   * e.g., when the server for incoming connections is torn down.
   */
  void SetConnectionToCloudSuspended (bool connectionToCloudSuspended);

// Events
public:
  // Application initialization

  /**
   * \brief An event informing all interested strategies that the initialization of the application (including the initialization steps of all
   * strategies) have finished (i.e., the DoInitialize() method has reached its end).
   */
  void ApplicationInitializedEvent ();

  // Tracker-related events

  /**
   * \brief Triggered by the peer discovery strategy, this event informs other strategies that a list of possible peers was returned
   * by the peer discovery mechanism.
   */
  void TrackerResponseReceivedEvent ();

  // Connection-related events

  /**
   * \brief This event is one of the two events that usually start the operation of a strategy.
   *
   * It is triggered after the connection with a peer
   * has been successfully established (i.e., after the handshake messages have been exchanged). The connection to the remote peer
   * is ready for message sending and reception.
   */
  void PeerConnectionEstablishedEvent (Ptr<Peer> peer);

  /**
   * \brief This event is triggered when a connection attempts with a peer failed.
   *
   * Note: The Peer object pointed to by the argument of this method is <b>not</b> guaranteed to exist after this event is triggered.
   * A strategy should not save a reference to this object. Indeed, it should try removing all references to this object.
   */
  void PeerConnectionFailEvent (Ptr<Peer> peer);

  /**
   * \brief This event is triggered when a connection to a peer was closed, either by the local or the remote client.
   *
   * Note: The Peer object pointed to by the argument of this method is <b>not</b> guaranteed to exist after this event is triggered.
   * A strategy should not save a reference to this object. Indeed, it should try removing all references to this object.
   */
  void PeerConnectionCloseEvent (Ptr<Peer> peer);

  /**
   * \brief This event is triggered by the peer discovery mechanism to indicate that the peer discovery mechanism at least once returned a usable
   * list of peers to connect to.
   *
   * The current cloud connection status can be obtained using the GetConnectedToCloud and GetConnectionToCloudSuspended methods.
   */
  void CloudConnectionEstablishedEvent ();

  /**
   * \brief When this event is triggered, the client does not consider itself to be connected to the cloud anymore.
   *
   * This nay happen, e.g., after the server for incoming connections was torn down. Note that this does not indicate that a connection to a peer was closed.
   *
   * The current cloud connection status can be obtained using the GetConnectedToCloud and GetConnectionToCloudSuspended methods.
   */
  void CloudConnectionSuspendedEvent ();

  // BitTorrent Peer Wire Protocol (PWP) messages, sorted by ID

  /**
   * \brief This event informs about a change in this client's choking status at a peer.
   *
   * The current choking status (choked / unchoked) can be accessed via the IsChoking method of the Peer class.
   */
  void PeerChokeChangingEvent (Ptr<Peer> peer);

  /**
   * \brief This event is triggered when the remote peer changed its interest status.
   *
   * The current interest status (interested / uninterested) can be accessed via the IsInterested method of the Peer class.
   */
  void PeerInterestedChangingEvent (Ptr<Peer> peer);

  /**
   * \brief This event is triggerd upon the reception of a HAVE message by a peer, informing this client about the availability of a new
   * piece at the peer.
   *
   * @param pieceIndex the index of the newly-available piece.
   */
  void PeerHaveEvent (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief This event is one of the two events that usually start the operation of a strategy.
   *
   * It is triggered when a BITFIELD message is sent by a peer after completion of the handshake.
   *
   * The bitfield of the peer can be accessed piece-wise via the HasPiece method of the Peer class.
   *
   * See BitTorrentClient::PeerConnectionEstablishedEvent() for the related event.
   */
  void PeerBitfieldReceivedEvent (Ptr<Peer> peer);

  /**
   * \brief This event informs about the reception of a request by a peer to send a (part of a) piece.
   *
   * @param pieceIndex the index of the requested piece.
   * @param blockOffset the offset (in bytes) of the block requested within the piece.
   * @param blockLength the length of the block requested.
   */
  void PeerRequestEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  /**
   * \brief This event is the "inverse" of the PeerRequestEvent, informing about the reception of a cancellation request.
   */
  void PeerCancelEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  /**
   * \brief This event informs about the reception of a PORT message from the peer.
   *
   * @param port the announced port the peer's DHT tracker is listening on.
   */
  void PeerPortMessageEvent (Ptr<Peer> peer, uint16_t port);

  /**
   * \brief This event is triggered upon the reception of a message of the <a href="http://www.bittorrent.org/beps/bep_0010.html" target="_blank">BitTorrent Extension Protocol (BEP 10)</a>.
   *
   * @param messageType the type of the message received. 0 Indicates a Extension Message Protocol handshake, > 0 some message negotiated during the handshake.
   * @param message the content of the sent message.
   */
  void PeerExtensionMessageEvent (Ptr<Peer> peer, uint8_t messageType, std::string message);

  // Download-related events (PIECE messages, REQUEST messages, ...)

  /**
   * \brief This event is triggered by the piece selection strategy to indicate that it has begun requesting blocks of a certain piece.
   *
   * @param peer the peer the first request for a block is sent to. Note that further block requests may also be sent to different peers,
   * without additional notifications.
   * @param pieceIndex the index of the newly-requested piece.
   */
  void PieceRequestedEvent (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief This is the inverse event of the PieceRequestedEvent.
   *
   * It is triggered when there are not remaining requests left over for a non-completed piece.
   *
   * @param peer the peer the last remaining request was sent to.
   * @param pieceIndex the index of the the piece which has no remaining requests.
   */
  void PieceCancelledEvent (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief This event is triggered when the download of a requested block from a certain peer has finished.
   *
   * It notifies of the complete reception of a PIECE message.
   *
   * @param peer the peer that sent the piece.
   * @param piecIndex the index of the received piece.
   * @param blockOffset the offset (in bytes) of the received block within the piece.
   * @param blockLength the length of the received block.
   */
  void PeerBlockCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  /**
   * \brief This event is triggered by the part selection strategy when the download of a piece has reached a timeout.
   *
   * This depends on the implementation of the part selection strategy but usually indicates that the download of a piece could not be
   * completed within a certain time span since the time a first request for a block of the piece was issued.
   *
   * Note: Timeouts of blocks are intended to be handled internally by the part selection strategy only, which is why there is
   * only this event for the whole piece.  However, for the sake of simplicity, only the last peer involved in the download process
   * is announced via this event.
   *
   * @param peer the peer from which the last remaining part of the piece timed out. Note that, depending on the part selection strategy,
   * more than one peer might be involved in the download of a piece.
   * @param pieceIndex the index of the piece that timed out.
   */
  void PieceTimeoutEvent (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief This event is triggered by the part selection strategy when the download of a piece has been completed.
   *
   * @param peer the peer that has sent the last remaining part of the piece which completed the download. Note that, depending on the
   * part selection strategy, more than one peer might be involved in the download of a piece. However, for the sake of simplicity,
   * only the last peer involved in the download process is announced via this event.
   * @param pieceIndex the index of the downloaded piece.
   */
  void PieceCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief This event signalizes the finalization of the download of a shared file, if recognized by the part selection strategy.
   *
   * This method also triggers the announcement of the peer as a seeder with the peer discovery mechanism (see the TriggerCallbackAnnounceAsSeeder() method)
   * and initiates the timeout until the newly-seeding client leaves the cloud (see the SetSeedingDuration() method).
   *
   * Note that this event is also triggered at the beginning of the operation of the part selection strategy for seeders
   * to inform the other strategies that the client starts of in seeding mode.
   */
  void DownloadCompleteEvent ();

  // Upload-related events

  /**
   * \brief This event informs strategies about the finished upload of a block (PIECE message) to a peer.
   *
   * @param peer the peer that the piece was sent to.
   * @param piecIndex the index of the sent piece.
   * @param blockOffset the offset (in bytes) of the sent block within the piece.
   * @param blockLength the length of the sent block.
   */
  void PeerBlockUploadCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  // Changes in the strategy options

  /// @cond HIDDEN
  // Used internally
  void StrategyOptionsChangedEvent (std::string optionName, std::string oldValue, std::string newValue);
  /// @endcond HIDDEN

  /**
   * \brief This event is triggered whenever a setting for a strategy option has changed.
   *
   * In order to obtain old and current settings of strategy options, see the GetStrategyOptions, GetStrategyOptionChangePair and
   * GetLastChangedStrategyOptionName methods.
   */
  void StrategyOptionsChangedEvent ();

  // Events and methods for the periodic status generation calls by the GlobalMetricsGatherer, if existent

  /**
   * \brief This event is triggered in regular intervals in order to obtain status information about the client and its strategies.
   *
   * All associated listeners to the GatherMetricsEvent within strategies are called, and the returned maps containing the names
   * of the metrics as keys and the values as the respective entries of the map are combined and announced via the AnnounceMetrics
   * method.
   */
  void GatherMetricsEvent ();

  /**
   * \brief Announce client-local metrics to some output mechanism.
   *
   * This method is the central output-generation instance for client-local metrics gathered during a simulation.
   * The default output channel for the metrics is the standard output of the ns3 simulation, but you can override this
   * method in order to use other output channels or filter output data.
   *
   * Note that the argument is a <b>multi</b>map, which explicitly allows multiple entries with the same keys, which may
   * be useful when combining different metric calculation methods.
   *
   * @param metrics the metrics to announce.
   */
  virtual void AnnounceMetrics (std::multimap<std::string, std::string> metrics);

// Event listener registration
public:
  // Listeners for application initialization

  void RegisterCallbackApplicationInitializedEvent (Callback<void, Ptr<BitTorrentClient > > eventCallback);
  void UnregisterCallbackApplicationInitializedEvent (Callback <void, Ptr<BitTorrentClient > > eventCallback);

  // Listeners for tracker-related events

  void RegisterCallbackTrackerResponseReceivedEvent (Callback<void> eventCallback);
  void UnregisterCallbackTrackerResponseReceivedEvent (Callback<void> eventCallback);

  // Listeners for connection-related events

  void RegisterCallbackConnectionEstablishedEvent (Callback<void, Ptr<Peer> > eventCallback);
  void UnregisterCallbackConnectionEstablishedEvent (Callback<void, Ptr<Peer> > eventCallback);
  void RegisterCallbackConnectionFailEvent (Callback<void, Ptr<Peer> > eventCallback);
  void UnregisterCallbackConnectionFailEvent (Callback<void, Ptr<Peer> > eventCallback);
  void RegisterCallbackConnectionCloseEvent (Callback<void, Ptr<Peer> > eventCallback);
  void UnregisterCallbackConnectionCloseEvent (Callback<void, Ptr<Peer> > eventCallback);
  void RegisterCallbackCloudConnectionEstablishedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback);
  void UnregisterCallbackCloudConnectionEstablishedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback);
  void RegisterCallbackCloudConnectionSuspendedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback);
  void UnregisterCallbackCloudConnectionSuspendedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback);

  // Listeners for BitTorrent Peer Wire Protocol (PWP) messages, sorted by ID

  void RegisterCallbackChokeChangingEvent (Callback<void, Ptr<Peer> > eventCallback);
  void UnregisterCallbackChokeChangingEvent (Callback<void, Ptr<Peer> > eventCallback);
  void RegisterCallbackInterestedChangingEvent (Callback<void, Ptr<Peer> > eventCallback);
  void UnregisterCallbackInterestedChangingEvent (Callback<void, Ptr<Peer> > eventCallback);
  void RegisterCallbackPeerHaveEvent (Callback<void, Ptr<Peer>,uint32_t> eventCallback);
  void UnregisterCallbackPeerHaveEvent (Callback<void, Ptr<Peer>,uint32_t> eventCallback);
  void RegisterCallbackBitfieldReceivedEvent (Callback<void, Ptr<Peer> > eventCallback);
  void UnregisterCallbackBitfieldReceivedEvent (Callback<void, Ptr<Peer> > eventCallback);
  void RegisterCallbackRequestEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);
  void UnregisterCallbackRequestEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);
  void RegisterCallbackCancelEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);
  void UnregisterCallbackCancelEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);
  void RegisterCallbackPortMessageEvent (Callback<void, Ptr<Peer>, uint16_t> eventCallback);
  void UnregisterCallbackPortMessageEvent (Callback<void, Ptr<Peer>, uint16_t> eventCallback);
  void RegisterCallbackExtensionMessageEvent (uint8_t messageId, Callback<void, Ptr<Peer>, const std::string& > eventCallback);
  void UnregisterCallbackExtensionMessageEvent (uint8_t messageId, Callback<void, Ptr<Peer>, const std::string& > eventCallback);

  // Listeners for download-related events (PIECE messages, REQUEST messages)

  void RegisterCallbackPieceRequestedEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);
  void UnregisterCallbackPieceRequestedEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);
  void RegisterCallbackBlockCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);
  void UnregisterCallbackBlockCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);
  void RegisterCallbackPieceCancelledEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);
  void UnregisterCallbackPieceCancelledEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);
  void RegisterCallbackPieceCompleteEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);
  void UnregisterCallbackPieceCompleteEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);
  void RegisterCallbackDownloadCompleteEvent (Callback<void> eventCallback);
  void UnregisterCallbackDownloadCompleteEvent (Callback<void> eventCallback);
  void RegisterCallbackPieceTimeoutEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);
  void UnregisterCallbackPieceTimeoutEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback);

  // Listeners for upload-related events

  void RegisterCallbackBlockUploadCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);
  void UnregisterCallbackBlockUploadCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback);

  // Listeners for changes in the strategy options

  void RegisterCallbackStrategyOptionsChangedEvent (Callback<void> eventCallback);
  void UnregisterCallbackStrategyOptionsChangedEvent (Callback<void> eventCallback);

  // Listeners for the periodic status generation calls by the GlobalMetricsGatherer, if existent

  void RegisterCallbackGatherMetricsEvent (Callback<std::map<std::string, std::string> > eventCallback);
  void UnregisterCallbackGatherMetricsEvent (Callback<std::map<std::string, std::string> > eventCallback);

// Callback triggers
public:
  // Cloud connection

  /**
   * \brief Connect with the cloud.
   *
   * A call to this method causes the peer discovery strategy to open a connection to the peer discovery mechanism,
   * obtain a list of peers to connect to and connect to other members of the cloud using that information, effectively
   * inserting the BitTorrent client into the cloud.
   */
  void JoinCloud ();

  /**
   * \brief Disconnect from the cloud.
   *
   * This method effectively closes all connections to other clients and sends a leave message to the peer discovery mechanism,
   * effectively removing the client from the cloud.
   */
  void DisconnectFromCloud ();

  /**
   * \brief A pass-through to the method responsible for the announcement of the client as a seeder within the peer discovery mechanism.
   */
  void TriggerCallbackAnnounceAsSeeder ();

  // Peer connection

  /**
   * \brief A pass-through to the method responsible for connecting to other members of the cloud.
   *
   * @param count the number of cloud members to connect to. Note that no further connections will be established when the client
   * is already connected to (more than) the number of peers returned by the GetDesiredPeers method.
   */
  void TriggerCallbackConnectToPeers (uint16_t count);

  /**
   * \brief Force the establishment of a connection to a certain known peer.
   *
   * This method forces the peer discovery mechanism to establish a connection with the given peer.
   *
   * Note: This method circumvents the bound of peers set by the SetDesiredPeers and SetMaxPeers methods.
   *
   * @param address the IP address of the remote client to connect to.
   * @param the port on which the remote client listens for incoming connections. Set to 0 to use the default value.
   */
  void TriggerCallbackConnectToPeer (Ipv4Address address, uint16_t port);

  /**
   * \brief Disconnect from a number of peers.
   *
   * This method allows you to forcefully disconnect from a number of peers. The peers to disconnect to are chosen by the respective
   * peer connection strategy.
   *
   * @param count the number of peers to disconnect. Set to -1 to disconnect from all peers.
   */
  void TriggerCallbackDisconnectPeers (int32_t count);

  /**
   * \brief Force the disconnection from a certain known peer.
   *
   * The connection to the supplied peer is closed immediately.
   *
   * @peer pointer to the Peer object representing the peer to close the connection with.
   */
  void TriggerCallbackDisconnectPeer (Ptr<Peer> peer);

  /**
   * \brief Get the current number of peers the client is connected to.
   *
   * This method is a pass-through to the peer discovery strategy to ask for the number of currently-held connections, including
   * pending connection attempts. As a conequence, the result of this method may not be equal to that of the GetConnectedPeerCount method,
   * which only counts peers whith which handshake information has already been exchanged (i.e., it does not count pending connections).
   *
   * @returns the number of currently-held connections to peers accpording to the peer discovery strategy.
   */
  uint16_t TriggerCallbackGetPeerCount ();

// Callback setters
public:
  // Cloud connection

  void SetCallbackConnectToCloud (Callback<void > eventCallback);
  void SetCallbackDisconnectFromCloud (Callback<void > eventCallback);
  void SetCallbackAnnounceAsSeeder (Callback<void > eventCallback);

  // Peer connection

  void SetCallbackConnectToPeers (Callback<uint16_t, uint16_t> eventCallback);
  void SetCallbackConnectToPeer (Callback<void, Ipv4Address, uint16_t> eventCallback);
  void SetCallbackDisconnectPeers (Callback<void, int32_t> eventCallback);
  void SetCallbackDisconnectPeer (Callback<void, Ptr<Peer> > eventCallback);
  void SetCallbackGetPeerCount (Callback<uint16_t> eventCallback);
};

} // ns bittorrent
} //namespace ns3;

#endif /* BITTORRENTCLIENT_H_ */
