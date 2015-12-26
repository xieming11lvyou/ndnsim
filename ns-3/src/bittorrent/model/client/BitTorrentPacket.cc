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

#include "BitTorrentPacket.h"

#include "ns3/log.h"
#include "ns3/packet.h"

#include <cstring>

namespace ns3 {
namespace bittorrent {

/************************************************************************************************/
/************************************** BitTorrentHandshakeMessage *************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentHandshakeMessage);

BitTorrentHandshakeMessage::BitTorrentHandshakeMessage ()
{
}

BitTorrentHandshakeMessage::~BitTorrentHandshakeMessage ()
{
}

void BitTorrentHandshakeMessage::Serialize (Buffer::Iterator start) const
{
  NS_ASSERT (m_protocol.size () <= 256);
  uint8_t buffer[8];
  std::memset (buffer,0,8);

  start.WriteU8 ((uint8_t)m_protocol.size ());
  start.Write (reinterpret_cast<const uint8_t*> (m_protocol.c_str ()),m_protocol.size ());
  // The next few bits are reserved space
  start.Write (buffer, 5);
  // Announce that we support the extension protocol
  uint8_t extensionBit = 0x10;
  start.WriteU8 (extensionBit);
  start.Write (buffer, 2);
  // Write the rest of the message
  start.Write (m_infoHash,20);
  start.Write (m_peerId,20);
}

uint32_t BitTorrentHandshakeMessage::Deserialize (Buffer::Iterator start)
{
  // Two-step reading: First the length of the protocol string, then the protocol string itself
  uint8_t buffer[257];

  unsigned char pstrLen = start.ReadU8 ();
  start.Read (buffer,pstrLen);      // protocol string
  m_protocol = reinterpret_cast<char*> (buffer);
  start.Read (buffer,8);      // Reserved space; TODO: Read out announcements for "extension protocol" messages (see Serialize())
  start.Read (m_infoHash,20);
  start.Read (m_peerId,20);
  return pstrLen + 49;
}

TypeId BitTorrentHandshakeMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentHandshakeMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentHandshakeMessage> ();

  return tid;
}

/************************************************************************************************/
/************************************** BitTorrentLengthHeader **********************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentLengthHeader);

BitTorrentLengthHeader::BitTorrentLengthHeader ()
{
  m_len = 0;
}

BitTorrentLengthHeader::BitTorrentLengthHeader (uint32_t len)
{
  m_len = len;
}

BitTorrentLengthHeader::~BitTorrentLengthHeader ()
{

}

uint32_t BitTorrentLengthHeader::Deserialize (Buffer::Iterator start)
{
  m_len = start.ReadNtohU32 ();

  return 4;
}

void BitTorrentLengthHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteHtonU32 (m_len);
}

TypeId BitTorrentLengthHeader::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentLengthHeader").SetParent<Header> ()
    .AddConstructor<BitTorrentLengthHeader> ();

  return tid;
}

/************************************************************************************************/
/************************************** BitTorrentTypeHeader ************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentTypeHeader);

BitTorrentTypeHeader::BitTorrentTypeHeader ()
{
}

BitTorrentTypeHeader::BitTorrentTypeHeader (BitTorrentMessageType type)
{
  m_type = type;
}

BitTorrentTypeHeader::~BitTorrentTypeHeader ()
{
}

uint32_t BitTorrentTypeHeader::Deserialize (Buffer::Iterator start)
{
  m_type = static_cast<BitTorrentMessageType> (start.ReadU8 ());

  return 1;

}

void BitTorrentTypeHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (static_cast<uint8_t> (m_type));
}

TypeId BitTorrentTypeHeader::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentTypeHeader").SetParent<Header> ()
    .AddConstructor<BitTorrentTypeHeader> ();

  return tid;
}

/************************************************************************************************/
/***************************************** BitTorrentHaveMessage ******************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentHaveMessage);

BitTorrentHaveMessage::BitTorrentHaveMessage ()
{
}

BitTorrentHaveMessage::BitTorrentHaveMessage (uint32_t pieceIndex)
{
  m_pieceIndex = pieceIndex;
}

BitTorrentHaveMessage::~BitTorrentHaveMessage ()
{
}

void BitTorrentHaveMessage::Serialize (Buffer::Iterator start) const
{
  start.WriteHtonU32 (m_pieceIndex);
}

uint32_t BitTorrentHaveMessage::Deserialize (Buffer::Iterator start)
{
  m_pieceIndex = start.ReadNtohU32 ();
  return 4;
}

TypeId BitTorrentHaveMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentHaveMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentHaveMessage> ();

  return tid;
}

/************************************************************************************************/
/************************************** BitTorrentBitfieldMessage *****************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentBitfieldMessage);

BitTorrentBitfieldMessage::BitTorrentBitfieldMessage ()
{
  m_bitField = 0;
  m_bitFieldSize = 0;
}

BitTorrentBitfieldMessage::BitTorrentBitfieldMessage (uint32_t bitFieldSize)
{
  m_bitField = new uint8_t[bitFieldSize];
  m_bitFieldSize = bitFieldSize;
}

BitTorrentBitfieldMessage::~BitTorrentBitfieldMessage ()
{
  delete [] m_bitField;
  m_bitField = 0;
}

void BitTorrentBitfieldMessage::SetBitFieldSize (uint32_t bitFieldSize)
{
  delete [] m_bitField;
  m_bitField = new uint8_t[bitFieldSize];
  m_bitFieldSize = bitFieldSize;
}

void BitTorrentBitfieldMessage::CopyBitFieldFrom (const std::vector<uint8_t>* sourceField)
{
  for (uint8_t i = 0; i < m_bitFieldSize; ++i)
    {
      m_bitField[i] = (*sourceField)[i];
    }
}

void BitTorrentBitfieldMessage::CopyBitFieldTo (std::vector<uint8_t>* targetField) const
{
  for (uint8_t i = 0; i < m_bitFieldSize; ++i)
    {
      (*targetField)[i] = m_bitField[i];
    }
}

void BitTorrentBitfieldMessage::Serialize (Buffer::Iterator start) const
{
  start.Write (m_bitField, m_bitFieldSize);
}

uint32_t BitTorrentBitfieldMessage::Deserialize (Buffer::Iterator start)
{
  // NOTE: For this method to function properly, the size of the bitfield must be already known (e.g., from the previous BitTorrentLengthHeader)
  start.Read (m_bitField, m_bitFieldSize);
  return m_bitFieldSize;
}



TypeId BitTorrentBitfieldMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentBitfieldMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentBitfieldMessage> ();

  return tid;
}

/************************************************************************************************/
/************************************** BitTorrentRequestMessage  *****************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentRequestMessage);

BitTorrentRequestMessage::BitTorrentRequestMessage ()
{
}

BitTorrentRequestMessage::BitTorrentRequestMessage (uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  m_pieceIndex = pieceIndex;
  m_blockOffset = blockOffset;
  m_blockLength = blockLength;
}

BitTorrentRequestMessage::~BitTorrentRequestMessage ()
{
}

void BitTorrentRequestMessage::Serialize (Buffer::Iterator start) const
{
  start.WriteHtonU32 (m_pieceIndex);
  start.WriteHtonU32 (m_blockOffset);
  start.WriteHtonU32 (m_blockLength);
}

uint32_t BitTorrentRequestMessage::Deserialize (Buffer::Iterator start)
{
  m_pieceIndex = start.ReadNtohU32 ();
  m_blockOffset = start.ReadNtohU32 ();
  m_blockLength = start.ReadNtohU32 ();

  return 12;
}

TypeId BitTorrentRequestMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentRequestMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentRequestMessage> ();

  return tid;
}

/************************************************************************************************/
/*************************************** BitTorrentCancelMessage  *****************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentCancelMessage);

BitTorrentCancelMessage::BitTorrentCancelMessage ()
{
}

BitTorrentCancelMessage::BitTorrentCancelMessage (uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  m_pieceIndex = pieceIndex;
  m_blockOffset = blockOffset;
  m_blockLength = blockLength;
}

BitTorrentCancelMessage::~BitTorrentCancelMessage ()
{
}

void BitTorrentCancelMessage::Serialize (Buffer::Iterator start) const
{
  start.WriteHtonU32 (m_pieceIndex);
  start.WriteHtonU32 (m_blockOffset);
  start.WriteHtonU32 (m_blockLength);
}

uint32_t BitTorrentCancelMessage::Deserialize (Buffer::Iterator start)
{
  m_pieceIndex = start.ReadNtohU32 ();
  m_blockOffset = start.ReadNtohU32 ();
  m_blockLength = start.ReadNtohU32 ();

  return 12;
}

TypeId BitTorrentCancelMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentCancelMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentCancelMessage> ();

  return tid;
}

/************************************************************************************************/
/**************************************** BitTorrentPieceMessage  *****************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentPieceMessage);

BitTorrentPieceMessage::BitTorrentPieceMessage ()
{
}

BitTorrentPieceMessage::BitTorrentPieceMessage (uint32_t index, uint32_t begin)
{
  m_index = index;
  m_begin = begin;
}

BitTorrentPieceMessage::~BitTorrentPieceMessage ()
{
}

void BitTorrentPieceMessage::Serialize (Buffer::Iterator start) const
{
  start.WriteHtonU32 (m_index);
  start.WriteHtonU32 (m_begin);
}

uint32_t BitTorrentPieceMessage::Deserialize (Buffer::Iterator start)
{
  m_index = start.ReadNtohU32 ();
  m_begin = start.ReadNtohU32 ();
  return 8;       // Note: This of course does not include the size of the payload, which is to be read separately from the stream after reading this message HEADER
}

TypeId BitTorrentPieceMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentPieceMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentPieceMessage> ();

  return tid;
}

/************************************************************************************************/
/***************************************** BitTorrentPortMessage  *****************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentPortMessage);

BitTorrentPortMessage::BitTorrentPortMessage ()
{
}

BitTorrentPortMessage::BitTorrentPortMessage (uint16_t listenPort)
{
  m_listenPort = listenPort;
}

BitTorrentPortMessage::~BitTorrentPortMessage ()
{
}

void BitTorrentPortMessage::Serialize (Buffer::Iterator start) const
{
  start.WriteHtonU32 (m_listenPort);
}

uint32_t BitTorrentPortMessage::Deserialize (Buffer::Iterator start)
{
  m_listenPort = start.ReadNtohU16 ();
  return 2;
}

TypeId BitTorrentPortMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentPortMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentPortMessage> ();

  return tid;
}


/************************************************************************************************/
/***************************************** BitTorrentExtensionMessage  ************************************/
/************************************************************************************************/

NS_OBJECT_ENSURE_REGISTERED (BitTorrentExtensionMessage);

BitTorrentExtensionMessage::BitTorrentExtensionMessage ()
{
  m_content = 0;
  m_packetLength = 1;
}

BitTorrentExtensionMessage::BitTorrentExtensionMessage (uint32_t packetLength)
{
  NS_ASSERT (packetLength > 1);
  m_packetLength = packetLength;
  m_content = new uint8_t[packetLength - 1];
}

BitTorrentExtensionMessage::BitTorrentExtensionMessage (uint8_t messageId, const std::string& content)
{
  m_messageId = messageId;
  m_packetLength = content.size () + 1;  // + 1 for the messageId
  m_content = new uint8_t[content.size ()];
  content.copy (reinterpret_cast<char* > (m_content), content.size (), 0);
}

BitTorrentExtensionMessage::~BitTorrentExtensionMessage ()
{
  delete[] m_content;
}

void BitTorrentExtensionMessage::SetContent (const std::string& content)
{
  m_packetLength = content.size () + 1;  // + 1 for the messageId
  delete[] m_content;
  m_content = new uint8_t[content.size ()];
  content.copy (reinterpret_cast<char* > (m_content), content.size (), 0);
}

void BitTorrentExtensionMessage::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (static_cast<uint8_t> (m_messageId));
  start.Write (&m_content[0], m_packetLength - 1);
}

uint32_t BitTorrentExtensionMessage::Deserialize (Buffer::Iterator start)
{
  m_messageId = start.ReadU8 ();
  start.Read (m_content, m_packetLength - 1);

  return m_packetLength;
}

TypeId BitTorrentExtensionMessage::GetTypeId ()
{

  static TypeId tid = TypeId ("ns3::bittorrent::BitTorrentExtensionMessage").SetParent<Header> ()
    .AddConstructor<BitTorrentExtensionMessage> ();

  return tid;
}

std::string BitTorrentExtensionMessage::GetContent () const
{
  std::string result;
  result.assign (reinterpret_cast<const char*> (m_content), m_packetLength - 1);

  return result;
}

} // ns bittorrent
} // ns ns3
