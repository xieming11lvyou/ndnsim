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
 * Authors: Rene Glebke
 */

#ifndef PEER_H_
#define PEER_H_

#include "ns3/BitTorrentDefines.h"

#include "BitTorrentPacket.h"

#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/socket.h"

#include <list>
#include <stdexcept>
#include <vector>

namespace ns3 {
namespace bittorrent {

class BitTorrentClient;

/**
 * \ingroup BitTorrent
 *
 * \brief Implements the BitTorrent Peer Wire Protocol connection endpoint.
 *
 * This class represents the TCP-based communication endpoint for a connection with another peer held by a BitTorrent client.
 * There are two independendent instances of this class for a running connection, one for each side. These instances are <b>not</b> linked,
 * since the information available at the other end is usually unavailable in a distributed scenario.
 *
 * The Peer class exposes a number of methods to send BitTorrent Peer Wire Protocol messages over an established communication channel.
 * Additionally, it keeps track of several characteristics and statistics of the connection, which can be used to implement client logic.
 *
 * All default BitTorrent Peer Wire Protocol messages as defined in the <a href="http://wiki.theory.org/BitTorrentSpecification#Messages" target="_blank">Bittorrent Protocol Specification v1.0</a> are supported.
 * Additionally, you can send messages adhering to the <a href="http://www.bittorrent.org/beps/bep_0010.html" target="_blank">BitTorrent Extension Protocol (BEP 10)</a>.
 *
 * Note: The sending and analysis of a BitTorrent Extension Protocol handshake message are currently not implemented. However, you may still use the SendExtendedMessage member function to send messages other
 * than those defined in the standard Peer Wire Protocol specification.
 */
class Peer : public Object
{
// Types used
public:
  // Identifiers the state of the connection
  enum PeerState
  {
    CONN_STATE_NOT_CONNECTED,
    CONN_STATE_AWAIT_CONNECTION,
    CONN_STATE_AWAIT_HANDSHAKE,        // The connection was established, but we await the initial handshake message
    CONN_STATE_CONNECTED,              // The connection is fully operational
    CONN_STATE_FAILED,
    CONN_STATE_CLOSED,
    CONN_STATE_CLOSED_WITH_ERROR,
    /*
     * Special state: The Peer class is still existent due to concurrency issues, but deinitialized.
     * The connection is closed and accessing some methods may cause errors.
     * All information obtained from a Peer object in this state should be considered potentially erroneous.
     */
    CONN_STATE_DEINITIALIZED
  };
private:
  // Stores information about a block request.
  struct RequestInformation
  {
    uint32_t pieceIndex;
    uint32_t blockOffSet;
    uint32_t blockLength;

    bool operator== (const RequestInformation &lhs)
    {
      return pieceIndex == lhs.pieceIndex && blockOffSet == lhs.blockOffSet && blockLength == lhs.blockLength;
    }
  };

// Fields
private:
  // Main attributes
  Ptr<BitTorrentClient>           m_myClient;              // The client that this peer is associated with
  Ptr<Socket>                     m_peerSocket;            // The socket that we are using in this Peer instance

  // Remote attributes
  Ipv4Address                     m_remoteIp;              // The IPv4 address of the remote peer
  uint16_t                        m_remotePort;            // The port of the remote peer
  std::string                     m_remotePeerId;          // The peer id sent by the remote peer to identify itself

  // Current status of the connection
  PeerState                       m_connectionState;      // The current state of the connection represented by this class

  bool                            m_peerChoking;           // Whether we are choked
  bool                            m_peerInterested;        // Whether the peer expressed interest in one of our PIECEs
  bool                            m_amChoking;             // Whether we are choking the remote peer
  bool                            m_amInterested;          // Whether we have expressed interest in one of the remote peer's PIECEs

  std::vector<uint8_t>            m_bitfield;              // The bitfield of the remote peer, updated upon reception of HAVE messages
  uint8_t*                        m_pieceCorruptionMap;    // An array indicating which of the received pieces were corrupted


  // Packet reception members and corresponding state machine attributes
  Ptr<Packet>                     m_packetBuffer;          // All incoming data is collected in a buffer represented by a Packet instance
  BitTorrentLengthHeader          m_lengthHeader;          // Stores the announced size of the next BitTorrent PeerWire Protocol message
  BitTorrentTypeHeader            m_typeHeader;            // Stores the announced type of the next BitTorrent PeerWire Protocol message
  bool                            m_lengthHeaderRemoved;   // Whether the length header was already removed from the m_packetBuffer field

  uint8_t*                        m_blockBuffer;           // The buffer that we use to catch PIECE payloads in, so we don't have to allocate it each time again
  uint32_t                        m_blockBufferSize;       // The size of m_blockBuffer, for dynamic adjustment

  // Packet transmission members and corresponding state machine attributes
  std::list<RequestInformation>   m_requestQueue;          // The REQUESTs we have to fulfill for the other peer (NOTE: Requests are NOT automatically added here!)

  std::list<Ptr<Packet> >         m_sendQueue;             // The queue that holds the packets that we want to send out
  std::list<bool>                 m_sendQueuePieceMessageIndicators; // Whether the next message in m_sendQueue is a PIECE message or not

  uint8_t*                        m_blockSendBuffer;       // The buffer that we use to send PIECE messages from
  const uint8_t*                  m_blockSendPtr;          // The last position from which we read to send out the data
  uint32_t                        m_blockSendDataLeft;     // The number of bytes which have still to be sent until the transmission of a PIECE (block) is completed
  bool                            m_blockSendingActive;    // Whether we are currently sending out a PIECE or whether we are "idle" in the sense of being able to transmit some other message

  // Statistics
  Time                            m_connectionEstablishmentTime;     // The time (in the simulation) that the connection was established

  uint64_t                        m_totalBytesDownloaded;  // The total amount of data (PIECE message payload) downloaded from the remote client
  uint64_t                        m_totalBytesUploaded;    // The total amount of data (PIECE message payload) uploaded to the remote client

  // Used for the calculation of the rolling average of the download and upload speeds
  uint64_t                        m_downloadHistory[BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS];
  uint64_t                        m_downloadHistoryReSetTime[BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS];
  uint64_t                        m_uploadHistory[BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS];
  uint64_t                        m_uploadHistoryReSetTime[BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS];

// Constructors etc.
public:
  Peer (Ptr<BitTorrentClient> myClient);
  virtual ~Peer ();

// Interaction methods
public:
  // General connection methods

  /**
   * \brief Establish a connection to a remote peer.
   *
   * The client, upon a call to this method, tries to connect to the BitTorrent client at the given address. It automatically creates the
   * needed TCP socket and arranges for all necessary callback connections for the TCP socket class to be present.
   *
   * When the connection was established, all needed Peer Wire Protocol handshake messages are automatically sent.
   *
   * Note: The connection is ready after the bitfield of the remote was received; the class will issue a PeerBitfieldReceivedEvent event if this message
   * is received. Prior to that, you should not use any of the communication methods provided by the class. Note that this behavior is not 100% compliant
   * with the specification, as sending the bitfield message is optional if a client has no pieces. This may lead to some clients not being accepted
   * as peers.
   *
   * @param address the address of the remote client.
   * @param port the address the remote client listens on for connections.
   */
  // TODO: Try to find a way to also accept clients not sending the bitfield after connection establishment.
  void ConnectToPeer (Ipv4Address address, uint16_t port);

  /**
   * \brief Handle an connection request from a remote client.
   *
   * This method is the counterpart to the ConnectToPeer method on the respective other side of the connection.
   * It should be called by the local server for incoming connections to establish the local connection endpoint.
   *
   * @param socket the socket to use as transport layer communication endpoint.
   */
  void ServeNewPeer (Ptr<Socket> socket, Ipv4Address address, uint16_t port);

  /**
   * \brief Close the connection to the peer.
   *
   * Closes the socket to the peer.
   *
   * @param silent whether to inform other parts of the BitTorrent client implementation about the closed connection.
   */
  void CloseConnection (bool silent);

  /**
   * \brief Set an INTERESTED or UNINTERESTED message to the peer.
   *
   * Note: The message is only sent when it is needed, i.e., an (UN)INTERESTED message is not repeated if the current client state is already (un)interested.
   *
   * @param amInterested whether the local client is interested in a piece of the remote peer.
   *
   */
  void SetAmInterested (bool amInterested);

  /**
   * \brief Request a piece (block) from the peer.
   *
   * This method creates a REQUEST message for the remote peer and adds it to the local send queue.
   *
   * @param pieceIndex the index of the piece to request.
   * @param blockOffset the offset (in bytes) of the block to request within the piece.
   * @param blockLength the length of the block to request.
   */
  void RequestPiece (uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  /**
   * \brief Cancel a requested piece.
   *
   * This method creates a CANCEL message for the remote peer and adds it at the top of the local send queue. The message is sent with
   * higher priority than other messages in the hope that the remote peer receives it before initiating the transfer of the piece to cancel.
   *
   * @param pieceIndex the index of the piece to cancel.
   * @param blockOffset the offset (in bytes) of the block to cancel within the piece.
   * @param blockLength the length of the block to cancel.
   */
  void CancelRequest (uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);

  /*
   * \brief Send the client's current bitfield to the remote peer.
   *
   * Note: Sending this message at any other time other than directly after the handshake message has been sent is a breach of protocol.
   * However, this message is optional.
   *
   */
  void SendBitfield ();

  /**
   * \brief Send a have message the peer.
   *
   * The created message is sent added at the top of the local send queue to give it a higher priority than other messages to help with
   * spreading rare pieces in the swarm.
   *
   * @param pieceIndex the index of the piece to announce.
   */
  void SendHaveMessage (uint32_t pieceIndex);

  /**
   * \brief Send a CHOKE or UNCHOKE message to the peer.
   *
   * Note: The message is only sent when it is needed, i.e., an (UN)CHOKE message is not repeated if the current client state is already (un)choked.
   *
   * @param amChoking whethter the local client wants to choke the remote peer.
   */
  void SetAmChoking (bool amChoking);

  /**
   * \brief Send a PIECE message to the peer.
   *
   * Inserts a PIECE message into the message queue to transfer actual file data to the remote peer.
   *
   * @param pieceIndex the index of the piece to send.
   * @param blockOffset the offset (in bytes) of the block to send within the piece.
   * @param blockLength the length of the block to send.
   */
  void SendBlock (uint32_t pieceIndex, uint32_t blockOffSet, uint32_t blockLength);

  /**
   * \brief Send an BitTorrent Extension Protocol message to the peer.
   *
   * This method creates and inserts an <a href="http://www.bittorrent.org/beps/bep_0010.html" target="_blank">BitTorrent Extension Protocol (BEP 10)</a>
   * message into the send queue. The created message has a Peer Wire message ID of 20 ("Extension Protocol") and the payload can be arbitrary.
   *
   * Note: The BitTorrent Extension Protocol defines a handshake message that shall be sent before initiating communication using this protocol
   * so different sent messages can be distinguished without a global message registry. At the moment, there is no function to generate this message,
   * but if existant, it can also be sent using this method.
   *
   * @param messageId the ID of the extended message. Must be 0 for a handshake message and >0 for non-handshake messages.
   * @param message the message to send.
   */
  void SendExtendedMessage (uint8_t messageId, const std::string& message);

// Getters, Setters
public:
  // Attributes of the remote peer

  /**
   * @returns the IP address of the remote peer.
   */
  Ipv4Address GetRemoteIp () const;

  /**
   * @returns the port of the remote endpoint of the connection represented by this option.
   */
  uint16_t GetRemotePort () const;

  /**
   * @returns the peer id of the remote peer.
   */
  std::string GetRemotePeerId () const;

  // Current status of the connection

  /**
   * @returns the current state of the connection with the remote peer.
   *
   * Possible states are:\n
   * CONN_STATE_NOT_CONNECTED if the connection has not yet been established,\n
   * CONN_STATE_AWAIT_CONNECTION if the connection is currently in establishment,\n
   * CONN_STATE_AWAIT_HANDSHAKE if the connection was established but the handshake was not yet sent,\n
   * CONN_STATE_CONNECTED while the connection is fully operational,\n
   * CONN_STATE_FAILED if the connection attempt failed,\n
   * CONN_STATE_CLOSED if the connection was closed (either locally or from the remote peer),\n
   * CONN_STATE_CLOSED_WITH_ERROR if the connection was closed due to an error, or\n
   * CONN_STATE_DEINITIALIZED if the instance was declared waste and is not safe to use any more.
   */
  PeerState GetConnectionState () const
  {
    return m_connectionState;
  }

  /**
   * @returns true, if the local client is currently coking the remote client.
   */
  bool GetAmChoking () const
  {
    return m_amChoking;
  }

  /**
   * @returns true, if the local client is currently interested in a piece of the remote client.
   */
  bool GetAmInterested () const
  {
    return m_amInterested;
  }

  /**
   * \brief Check whether the remote client has announced the possession of a piece.
   *
   * @param pieceId the piece to check for.
   *
   * @returns true, if the remote client has announced the possession of the piece.
   */
  bool HasPiece (uint32_t pieceId) const
  {
    try
      {
        uint32_t result = m_bitfield.at (pieceId / 8) & (1 << (7 - pieceId % 8));
        return result;
      }
    catch (std::out_of_range oor)
      {
        return false;
      }
  }

  /**
   * @returns true, if the remote peer is currently choking the local client.
   */
  bool IsChoking () const;

  /**
   * @returns true, if the remote peer has expressed interest in one of the pieces available at the local client.
   */
  bool IsInterested () const;

  /**
   * \brief Get the piece corruption map of the remote client.
   *
   * The piece corruption map indicates for each piece of the shared file whether it was successfully transferred by the remote peer or not.
   *
   * The possible states as defined in the file BitTorrentDefines.h for each piece are:\n
   * BT_PEER_PIECE_RECEPTION_NOT_RECEIVED if the piece has not been transferred by the remote client,\n
   * BT_PEER_PIECE_RECEPTION_CHECKSUM_OK if the checksum of the transferred piece did not match to that in the .torrent meta information file, and\n
   * BT_PEER_PIECE_RECEPTION_CHECKSUM_NOT_OK if the piece was successfully transferred by the remote client.
   *
   * @returns a pointer to the beginning of an array with as many entries as there are pieces in the shared file.
   */
  const uint8_t* GetPieceCorruptionMap () const;

  // Statistics

  /**
   * @returns the time within the simulation that the first Peer Wire protocol message (i.e., the handshake message) was received
   */
  Time GetConnectionEstablishmentTime () const;

  /**
   * \brief Get the amount of file data downloaded from the peer.
   *
   * This method retrurns the number of bytes of the shared file downloaded from the peer. Note that this measure may differ
   * from the amount of usable data obtained, since this method does not take checksums and, hence, wrong data transmissions into account.
   *
   * @returns the number of payload data (in bytes) received from the peer.
   */
  uint64_t GetTotalBytesDownloaded () const;

  /**
   * \brief Get the amount of file data uploaded to the peer.
   *
   * This method is the local counterpart of the GetTotalBytesDownloaded method.
   *
   * @returns the amount of payload data (in bytes) uploaded to the peer.
   */
  uint64_t GetTotalBytesUploaded () const;

  /**
   * \brief Get an estimation of the current download speed from the peer.
   *
   * The estimation estimated using a rolling-average scheme over the downloaded payload data.
   *
   * @returns an estimation of the download speed from that peer, in bps.
   */
  double GetBpsDownload ();

  /**
   * \brief Get an estimation of the current upload speed to the peer.
   *
   * This method is the local counterpart to the GetBpsDownload method. Only actual file (payload) data is taken into account.
   *
   * @returns an estimation of the upload speed to the peer, in bps.
   */
  double GetBpsUpload ();

// Internal methods
private:
  // Internal message generation methods
  void NotifyPeerOfChokeChange ();
  void NotifyPeerOfInterestedChange ();
  void HandleCancel (Ptr<Packet> packet);

  // Handling of PIECE messages
  bool HandlePiece (Ptr<Packet> packet, uint32_t packetLength);

  // The main method for reading from the TCP socket's stream
  void HandleRead (Ptr<Socket> socket);

  // The main method for sending
  void HandleSend (Ptr<Socket> socket, uint32_t bytesFree);

  // Handler for the data sent callback of the socket. Calculates the transmitted data.
  void HandleDataSent (Ptr<Socket> socket, uint32_t dataSent);

  // Connection-related event handlers
  void HandleConnected (Ptr<Socket> socket);
  void HandleConnectionFail (Ptr<Socket> socket);
  void HandleConnectionClosed (Ptr<Socket> socket);
  void HandleConnectionClosedWithError (Ptr<Socket> socket);

// Debugging
private:
  // We experienced the problem that the Peer class may be deinitialized too early so events in the global event queue fail.
  // As a temporary workaround, each Peer class instance holds a pointer to itself, so it won't be de-initialized at all.
  // FIXME: This is very, very ugly. Although it does the job, we MUST find out where this comes from.
  Ptr<Peer> m_dontDeInitializeMe;
  void PseudoDeInitializeMe ();
  void PseudoDeInitializeMePhase2 ();
};

} // ns bittorrent
} // ns ns-3

#endif /* PEER_H_ */
