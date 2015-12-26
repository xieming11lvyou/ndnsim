/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 ComSys, RWTH Aachen University
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
 * Authors: Martin Lang (principal author), Rene Glebke
 */

#ifndef BTPACKET_H_
#define BTPACKET_H_

#include "ns3/header.h"

namespace ns3 {
namespace bittorrent {

/************************************************************************************************/
/************************************** BitTorrentHandshakeMessage *************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief The BitTorrent handshake message.
 *
 * This class implements the BitTorrent handshake message as a derivative of the Header class.
 */
class BitTorrentHandshakeMessage : public Header
{
// Fields
private:
  std::string m_protocol;    // The string referencing the protocol
  uint8_t m_infoHash[20];    // The info hash of the torrent
  uint8_t m_peerId[20];      // Some ID that the client wishes to be identified with at the remote side

// Constructors etc.
public:
  BitTorrentHandshakeMessage ();
  ~BitTorrentHandshakeMessage ();
  static TypeId GetTypeId (void);

// Getters, Setters

  /**
   * @returns the protocol identifier stored in the packet.
   */
  std::string GetProtocolString () const
  {
    return m_protocol;
  }

  /**
   * \brief Set the protocol identifier of the packet.
   *
   * The standard setting is "BitTorrent protocol".
   *
   * @param protocol the protocol identifier.
   */
  void SetProtocolString (const std::string &protocol)
  {
    m_protocol = protocol;
  }

  /**
   * \brief Get the info hash of the shared file.
   *
   * The info hash identifies which swarm the client wants to establish a connection for with the peer.
   *
   * @returns a pointer to the beginning of an array containing the info hash of the shared file.
   */
  const uint8_t* GetInfoHash () const
  {
    return m_infoHash;
  }

  /**
   * \brief Set the info hash of the shared file.
   *
   * The info hash identifies which swarm the client wants to establish a connection for with the peer.
   *
   * @param hash a pointer to the beginning of an array containing the info hash of the shared file.
   */
  void SetInfoHash (const uint8_t *hash)
  {
    memcpy (m_infoHash,hash,20);
  }

  /**
   * @ returns a pointer to the beginning of an array containing the ID the sending client wants to be identified with.
   */
  const uint8_t* GetPeerId () const
  {
    return m_peerId;
  }

  /**
   * \brief Set the ID the client wants to be identified with at the peer.
   *
   * This setting has no algorithmic meaning in standard BitTorrent but may be used in BitTorrent-based protocols for further
   * distinguishing between clients.
   *
   * @param peerId a pointer to the beginning of an array containing the ID the sending client wants to be identified with.
   */
  void SetPeerId (const uint8_t *peerId)
  {
    memcpy (m_peerId,peerId,20);
  }

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 49 + m_protocol.size ();
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/************************************ BitTorrentLengthHeader ************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief The sub-header indicating the length of a Peer Wire message.
 *
 * This class implements the length field of a Peer Wire message as a derivative of the Header class.
 */
class BitTorrentLengthHeader : public Header
{
// Fields
private:
  uint32_t m_len;            // The length of the message following this length header

// Constructors etc.
public:
  BitTorrentLengthHeader ();
  BitTorrentLengthHeader (uint32_t len);
  ~BitTorrentLengthHeader ();
  static TypeId GetTypeId (void);

// Getters, setters
  /**
   * \brief Get the length of the Peer Wire message to follow.
   *
   * @returns the size, in bytes, of the Peer Wire message following this header message. Includes 1 byte for the message type identifier.
   */
  uint32_t GetPacketLength () const
  {
    return m_len;
  }

  /**
   * \brief Set the length of the Peer Wire message to follow.
   *
   * @param len the length. in bytes, of the Peer Wire message to follow. Must include 1 byte for the message type identifier.
   */
  void SetPacketLength (uint32_t len)
  {
    m_len = len;
  }

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 4;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/************************************** BitTorrentTypeHeader ************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief The type identifier of a Peer Wire message.
 *
 * This class implements the message type field of a Peer Wire message as a derivative of the Header class.
 */
class BitTorrentTypeHeader : public Header
{
// Fields etc.
public:
  enum BitTorrentMessageType
  {
    KEEP_ALIVE = -1,
    CHOKE = 0,
    UNCHOKE = 1,
    INTERESTED = 2,
    NOT_INTERESTED = 3,
    HAVE = 4,
    BITFIELD = 5,
    REQUEST = 6,
    PIECE = 7,
    CANCEL = 8,
    PORT = 9,
    EXTENDED = 20,  // BitTorrent Extension Protocol according to BEP-10,
	HANDSHAKE = 128
  };

private:
  BitTorrentMessageType m_type;        // The type of the message that is following in the stream

// Constructors etc.
public:
  BitTorrentTypeHeader ();

  /**
   * \brief Construct a BitTorrentMessageType header.
   *
   * @param type the desired type.
   */
  BitTorrentTypeHeader (BitTorrentMessageType type);
  ~BitTorrentTypeHeader ();
  static TypeId GetTypeId (void);

// Getters, setters
public:
  /**
   * @returns the type of Peer Wire protocol message immediately following this header.
   */
  BitTorrentMessageType GetMessageType () const
  {
    return m_type;
  }

  /**
   * \brief Set the type of Peer Wire protocol message immediately following this header.
   *
   * @param type the desired Peer Wire message type.
   */
  void SetMessageType (BitTorrentMessageType type)
  {
    m_type = type;
  }

// (De)-Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 1;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/****************************************** BitTorrentHaveMessage *****************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent HAVE message.
 *
 * This class implements the Peer Wire HAVE message as a derivative of the Header class.
 */
class BitTorrentHaveMessage : public Header
{
// Fields
private:
  uint32_t m_pieceIndex;     // The index of the piece that the client wishes to announce

// Constructors etc.
public:
  BitTorrentHaveMessage ();

  /**
   * \brief Construct a HAVE message.
   *
   * @param pieceIndex the index in the bitfield of the piece the client wants to announce.
   */
  BitTorrentHaveMessage (uint32_t pieceIndex);
  ~BitTorrentHaveMessage ();
  static TypeId GetTypeId (void);

// Getters, setters
public:

  /**
   * @returns the index in the bitfield of the piece the client has announced.
   */
  uint32_t GetPieceIndex () const
  {
    return m_pieceIndex;
  }

  /**
   * \brief Set the index in the bitfield of the piece the client wants to announce.
   *
   * @param pieceIndex the desired index.
   */
  void SetPieceIndex (uint32_t pieceIndex)
  {
    m_pieceIndex = pieceIndex;
  }

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 4;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/**************************************** BitTorrentBitfieldMessage ***************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent BITFIELD message.
 *
 * This class implements the Peer Wire BITFIELD message as a derivative of the Header class.
 */
class BitTorrentBitfieldMessage : public Header
{
// Fields
private:
  uint8_t *m_bitField;       // The bitfield
  uint32_t m_bitFieldSize;   // The size of m_bitField

// Constructors etc.
public:
  BitTorrentBitfieldMessage ();

  /**
   * @param bitfieldSize the size, in bytes, of the bitfield to be contained within the message.
   */
  BitTorrentBitfieldMessage (uint32_t bitFieldSize);
  ~BitTorrentBitfieldMessage ();
  static TypeId GetTypeId (void);

// Getters, setters
public:
  /**
   * @returns the size, in bytes, of the bitfield contained within the message.
   */
  uint32_t GetBitFieldSize () const
  {
    return m_bitFieldSize;
  }

  /**
   * \brief Set the size, in bytes, that the bitfield contained within thie message requires.
   *
   * The internal bitfield for the packet is reallocated to match the given size. Previous contents is discarded.
   *
   * @param bitfieldSize the desired size.
   */
  void SetBitFieldSize (uint32_t bitFieldSize);

  /**
   * \brief Copy the contents of the given bitfield into the packet.
   *
   * Note: The number of bytes the bitfield needs to be stored needs to be at most the number of bytes allocated for the bitfield using the
   * constructor or the GetBitFieldSize member function.
   *
   * @param sourceField pointer to the beginning of the source bitfield.
   */
  void CopyBitFieldFrom (const std::vector<uint8_t>* sourceField);

  /**
   * \brief Copy the bitfield contained within the packet to another memory location.
   *
   * @param targetField pointer to the beginning of the target bitfield.
   */
  void CopyBitFieldTo (std::vector<uint8_t>* targetField) const;

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return m_bitFieldSize;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/**************************************** BitTorrentRequestMessage ****************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent REQUEST message.
 *
 * This class implements the Peer Wire REQUEST message as a derivative of the Header class.
 */
class BitTorrentRequestMessage : public Header
{
// Fields
private:
  uint32_t m_pieceIndex;     // The index of the requested piece
  uint32_t m_blockOffset;    // The start of the requested block
  uint32_t m_blockLength;    // The length of the requested block

// Constructors etc.
public:
  BitTorrentRequestMessage ();

  /**
   * \brief Construct a request for a piece.
   *
   * @param pieceIndex the index of the piece requested.
   * @param blockOffset the offset, in bytes, within the piece of the block requested.
   * @param blockLength the length, in bytes, of the block requested.
   */
  BitTorrentRequestMessage (uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);
  ~BitTorrentRequestMessage ();
  static TypeId GetTypeId (void);

// Getters, setters
public:
  /**
   * @returns the index of the piece requested.
   */
  uint32_t GetPieceIndex () const
  {
    return m_pieceIndex;
  }

  /**
   * @returns the offset, in bytes, within the piece requested.
   */
  uint32_t GetBlockOffset () const
  {
    return m_blockOffset;
  }

  /**
   * @returns the length, in bytes, of the block requested.
   */
  uint32_t GetBlockLength () const
  {
    return m_blockLength;
  }

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 12;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);
};

/************************************************************************************************/
/***************************************** BitTorrentPieceMessage *****************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent PIECE message.
 *
 * This class implements the Peer Wire BITFIELD message as a derivative of the Header class.
 */
class BitTorrentPieceMessage : public Header
{
// Fields
private:
  uint32_t m_index;          // The index of the piece this message contains
  uint32_t m_begin;          // The start of the block of the piece in m_index that this message contains
  // Note: There is no field for the message itself in here, it has to be read out of the stream directly!

// Constructors etc.
public:
  BitTorrentPieceMessage ();

  /**
   * \brief Construct a Peer Wire PIECE message.
   *
   * Note: The PIECE message does NOT contain the actual payload data; this data must be read out directly from the
   * stream after this message.
   *
   * @param pieceIndex the index of the piece the message contains.
   * @param blockOffset the offset, in bytes, of the block that the message contains.
   */
  BitTorrentPieceMessage (uint32_t index, uint32_t begin);
  ~BitTorrentPieceMessage ();
  static TypeId GetTypeId (void);

// Getters, setters
public:
  /**
   * @returns the index of the piece the message contains.
   */
  uint32_t GetIndex () const
  {
    return m_index;
  }

  /**
   * @returns the offset, in bytes, of the block that the message contains.
   */
  uint32_t GetBegin () const
  {
    return m_begin;
  }

// (De-)Serialization OF THE PIECE MESSAGE HEAD (payload must be read out directly from the stream!)
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 8;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/***************************************** BitTorrentCancelMessage ****************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent CANCEL message.
 *
 * This class implements the Peer Wire CANCEL message as a derivative of the Header class.
 *
 * This message is the "reverse" of the REQUEST message handled by the BitTorrentBitTorrentRequestMessage class.
 */
class BitTorrentCancelMessage : public Header // This message is the "reverse" of a REQUEST message
{
// Fields
private:
  uint32_t m_pieceIndex;     // The index of the cancelled piece
  uint32_t m_blockOffset;    // The start of the cancelled block
  uint32_t m_blockLength;    // The length of the cancelled block

// Constructors etc.
public:
  BitTorrentCancelMessage ();

  /**
   * \brief Construct a cancellation of a piece request.
   *
   * @param pieceIndex the index of the piece cancelled.
   * @param blockOffset the offset, in bytes, within the piece of the block cancelled.
   * @param blockLength the length, in bytes, of the block cancelled.
   */
  BitTorrentCancelMessage (uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength);
  ~BitTorrentCancelMessage ();
  static TypeId GetTypeId (void);

// Getters, setters
public:
  /**
   * @returns the index of the piece cancelled.
   */
  uint32_t GetPieceIndex () const
  {
    return m_pieceIndex;
  }

  /**
   * @returns the offset, in bytes, within the piece cancelled.
   */
  uint32_t GetBlockOffset () const
  {
    return m_blockOffset;
  }

  /**
   * @returns the length, in bytes, of the block cancelled.
   */
  uint32_t GetBlockLength () const
  {
    return m_blockLength;
  }

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 12;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/****************************************** BitTorrentPortMessage *****************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent PORT message.
 *
 * This class implements the Peer Wire PORT message as a derivative of the Header class.
 *
 * The PORT message is used by clients implementing a DHT tracker to announce the port that the
 * DHT tracker is listening on.
 */
class BitTorrentPortMessage : public Header
{
// Fields
private:
  uint16_t m_listenPort;     // The port the client's DHT implementation is listening on

// Constructors etc.
public:
  BitTorrentPortMessage ();

  /**
   * \brief Construct a PORT message.
   *
   * @param listenPort the port of the DHT tracker to announce.
   */
  BitTorrentPortMessage (uint16_t listenPort);
  ~BitTorrentPortMessage ();
  static TypeId GetTypeId (void);

// Getters, setters
public:
  /**
   * @returns the announced listening port of the DHT tracker.
   */
  uint16_t GetListenPort () const
  {
    return m_listenPort;
  }

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return 2;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

/************************************************************************************************/
/**************************************** BitTorrentExtensionMessage **************************************/
/************************************************************************************************/

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent Extension Protocol message.
 *
 * This class implements the <a href="http://www.bittorrent.org/beps/bep_0010.html" target="_blank">BitTorrent Extension Protocol (BEP 10)</a>BitTorrent Extension Protocol</a> message.
 *
 * This message class may be used to exchange further information via an existing Peer Wire connection,
 * e.g. to implement additional functionality.
 */
class BitTorrentExtensionMessage : public Header
{
// Fields
private:
  uint32_t m_packetLength;   // The length of the message
  uint8_t m_messageId;       // The "extended message ID" of the extended message (i.e., the type of contents in the message)
  uint8_t* m_content;        // The actual content of the extended message

// Constructors etc.
public:
  BitTorrentExtensionMessage ();

  /**
   * \brief Create an empty Extension Protocol message.
   *
   * You can use the SetMessageId and SetContent methods to change the information contained within the message.
   *
   * @param contentLength the length, in bytes, of the content contained within this message.
   */
  BitTorrentExtensionMessage (uint32_t contentLength);

  /**
   * \brief Create an Extension Protocol message with the specified content.
   *
   * @param messageId the ID of the message constructed. The ID must be known by the peer in order to understand the message.
   *  Use messageId = 0 to indicate a Extension Protocol handshake message.
   * @param content the content of the message.
   */
  BitTorrentExtensionMessage (uint8_t messageId, const std::string& content);
  ~BitTorrentExtensionMessage ();
  static TypeId GetTypeId (void);

// Getters, setters
public:
  /**
   * @returns the length, in bytes, of the message. Includes 1 byte for the ID of the contained message.
   */
  uint32_t GetPacketLength () const
  {
    return m_packetLength;
  }

  /**
   * @returns the ID of the containted message. A message ID of 0 indicates a handshake message.
   */
  uint8_t GetMessageId () const
  {
    return m_messageId;
  }

  void SetMessageId (uint8_t messageId)
  {
    m_messageId = messageId;
  }

  /**
   * @returns the content of the message.
   */
  std::string GetContent () const;

  void SetContent (const std::string& content);

// (De-)Serialization
public:
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t GetSerializedSize (void) const
  {
    return m_packetLength;
  }

  virtual void Print (std::ostream &os) const
  {
    // Stub
  }

private:
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
};

} // ns bittorrent
} // ns ns3

#endif /* BTPACKET_H_ */
