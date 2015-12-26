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

#include "BitTorrentPeer.h"

#include "ns3/BitTorrentDefines.h"

#include "BitTorrentClient.h"
#include "BitTorrentPacket.h"
#include "StorageManager.h"

#include "ns3/inet-socket-address.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/uinteger.h"

#include <cstring> // for memcpy
#include <limits>
#include <list>
#include <vector>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::Peer");
NS_OBJECT_ENSURE_REGISTERED (Peer);

Peer::Peer (Ptr<BitTorrentClient> myClient)
{
  // Main attributes
  m_myClient = myClient;

  // Current status of the connection -->
  m_connectionState = CONN_STATE_NOT_CONNECTED;
  m_peerChoking = true;
  m_peerInterested = false;
  m_amChoking = true;
  m_amInterested = false;

  uint32_t bitfieldSize = m_myClient->GetTorrent ()->GetNumberOfPieces () / 8;
  if ((m_myClient->GetTorrent ()->GetNumberOfPieces () % 8) > 0)
    {
      bitfieldSize++;
    }
  m_bitfield.resize (bitfieldSize);
  for (uint8_t i = 0; i < bitfieldSize; ++i)
    {
      m_bitfield[i] = 0;
    }

  m_pieceCorruptionMap = new uint8_t [m_myClient->GetTorrent ()->GetNumberOfPieces ()];
  for (uint32_t i = 0; i < m_myClient->GetTorrent ()->GetNumberOfPieces (); i++)
    {
      m_pieceCorruptionMap[i] = BT_PEER_PIECE_RECEPTION_NOT_RECEIVED;
    }
  // <-- Current status of the connection

  // Packet reception members and corresponding state machine attributes
  m_packetBuffer = Create<Packet> ();
  m_lengthHeader.SetPacketLength (BT_PROTOCOL_MESSAGES_LENGTHHEADER_LENGTH);
  m_lengthHeaderRemoved = false;
  m_blockBuffer = 0;
  m_blockBufferSize = 0;

  // Packet transmission members and corresponding state machine attributes
  m_blockSendBuffer = 0;
  m_blockSendDataLeft = 0;
  m_blockSendingActive = false;

  // Statistics
  m_connectionEstablishmentTime = MilliSeconds (0) - MilliSeconds (1);     // Simulator::Now()

  m_totalBytesDownloaded = 0;
  m_totalBytesUploaded = 0;

  // DEBUG
  m_dontDeInitializeMe = this;
}

Peer::~Peer ()
{
  delete[] m_blockBuffer;
  delete[] m_blockSendBuffer;
  delete[] m_pieceCorruptionMap;
}


void Peer::ConnectToPeer (Ipv4Address address, uint16_t port)
{
  if (m_connectionState != CONN_STATE_NOT_CONNECTED)      // Only allow this directly if we haven't initialized a connection before
    {
      return;
    }

  // Step 1: Initialize the member variables on the peer
  m_remoteIp = address;
  m_remotePort = port;

  // Step 2: Create a TCP socket that we can use to initiate the connection
  TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
  m_peerSocket = Socket::CreateSocket (m_myClient->GetNode (), tid);
  m_peerSocket->SetAttribute ("SndBufSize", UintegerValue (BT_PEER_SOCKET_TRANSMIT_BUFFER_SIZE));
  m_peerSocket->SetAttribute ("RcvBufSize", UintegerValue (BT_PEER_SOCKET_RECEIVE_BUFFER_SIZE));
  m_peerSocket->SetAttribute ("SegmentSize", UintegerValue (BT_PEER_SOCKET_TCP_SEGMENT_SIZE_MAX));

  // Step 3: Register all needed callbacks for this peer socket
  m_peerSocket->SetConnectCallback (MakeCallback (&Peer::HandleConnected, this), MakeCallback (&Peer::HandleConnectionFail, this));
  m_peerSocket->SetCloseCallbacks (MakeCallback (&Peer::HandleConnectionClosed, this), MakeCallback (&Peer::HandleConnectionClosedWithError, this));
  m_peerSocket->SetRecvCallback (MakeCallback (&Peer::HandleRead, this));
  m_peerSocket->SetSendCallback (MakeCallback (&Peer::HandleSend, this));
  m_peerSocket->SetDataSentCallback (MakeCallback (&Peer::HandleDataSent, this));

  // Step 4: Prepare the connection establishment
  m_connectionState = CONN_STATE_AWAIT_CONNECTION;

  // Step 5: Bind the socket to a local endpoint and connect
  m_peerSocket->Bind (m_myClient->GetIp ());
  m_peerSocket->Connect (InetSocketAddress (address, port));
}

void Peer::ServeNewPeer (Ptr<Socket> socket, Ipv4Address address, uint16_t port)
{
  if (m_connectionState != CONN_STATE_NOT_CONNECTED)      // Only allow this directly if we haven't initialized a connection before
    {
      return;
    }

  // Step 1: Register the created socket as the socket to use for this peer
  m_peerSocket = socket;
  m_peerSocket->SetAttribute ("SndBufSize", UintegerValue (BT_PEER_SOCKET_TRANSMIT_BUFFER_SIZE));
  m_peerSocket->SetAttribute ("RcvBufSize", UintegerValue (BT_PEER_SOCKET_RECEIVE_BUFFER_SIZE));
  m_remoteIp = address;
  m_remotePort = port;

  // Step 2: Register all needed callbacks for this peer socket
  m_peerSocket->SetConnectCallback (MakeCallback (&Peer::HandleConnected, this), MakeCallback (&Peer::HandleConnectionFail, this));
  m_peerSocket->SetCloseCallbacks (MakeCallback (&Peer::HandleConnectionClosed, this), MakeCallback (&Peer::HandleConnectionClosedWithError, this));
  m_peerSocket->SetRecvCallback (MakeCallback (&Peer::HandleRead, this));
  m_peerSocket->SetSendCallback (MakeCallback (&Peer::HandleSend, this));
  m_peerSocket->SetDataSentCallback (MakeCallback (&Peer::HandleDataSent, this));

  // Step 3: Since the other client is already initialized (remote establishment of connection), we await the BitTorrent handshake packet and prepare to send ours -->
  m_connectionState = CONN_STATE_AWAIT_HANDSHAKE;

  BitTorrentHandshakeMessage handshake;
  handshake.SetProtocolString (BT_PROTOCOL_MESSAGES_HANDSHAKE_PROTOCOL_STRING);
  uint8_t peerId[BT_PROTOCOL_MESSAGES_HANDSHAKE_PEERID_LENGTH_MAX];
  std::memcpy (peerId,m_myClient->GetPeerId ().c_str (), std::min (static_cast<size_t> (BT_PROTOCOL_MESSAGES_HANDSHAKE_PEERID_LENGTH_MAX), m_myClient->GetPeerId ().size ()));
  handshake.SetPeerId (peerId);
  handshake.SetInfoHash (m_myClient->GetCurrentInfoHash ());
    
  Ptr<Packet> announcementPacket = Create<Packet> ();
  announcementPacket->AddHeader (handshake);

  // <-- Step 3: Since the other client is already initialized (remote establishment of connection), we await the BitTorrent handshake packet and prepare to send ours

  // Step 4: Add the announcement message to the send queue
  m_sendQueue.push_back (announcementPacket);
  m_sendQueuePieceMessageIndicators.push_back (false);

  // Step 5: Send out the announcement by processing the send queue
  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::CloseConnection (bool silent)
{
  NS_LOG_INFO ("Peer: Closing connection with " << GetRemoteIp () << "; silent: " << silent);

  m_connectionState = CONN_STATE_CLOSED;

  m_amChoking = true;
  m_peerChoking = true;

  m_peerSocket->Close ();

  if (!silent)
    {
      Simulator::ScheduleNow (&BitTorrentClient::UnregisterPeer, m_myClient, this);
      Simulator::ScheduleNow (&BitTorrentClient::PeerConnectionCloseEvent, m_myClient, this);
    }

  // DEBUG
  Simulator::ScheduleNow (&Peer::PseudoDeInitializeMe, this);
}

void Peer::SetAmInterested (bool amInterested)
{
  if (m_amInterested != amInterested)
    {
      m_amInterested = amInterested;
      NotifyPeerOfInterestedChange ();
    }
}

// NOTE: Since all message generation methods are organized more or less equally, this method is annotated exemplarily
void Peer::RequestPiece (uint32_t pieceIndex, uint32_t blockOffSet, uint32_t blockLength)
{
  // Step 0: Only send out something, if the connection is not yet closed (due to concurrency within the strategies and the simulation)
  if (m_connectionState != CONN_STATE_CONNECTED)
    {
      return;
    }

  // Step 1: Create a packet
  Ptr<Packet> packet = Create<Packet> ();

  // Step 2: Create a BitTorrentLengthHeader so we can read out the length of the message properly again at the remote Peer
  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_REQUEST_LENGTH);

  // Step 3: Create a BitTorrentTypeHeader so we can read out the type of the message properly again at the remote Peer
  BitTorrentTypeHeader typeHead (BitTorrentTypeHeader::REQUEST);

  // Step 4: Create the message specific to this call (in this case, a REQUEST message)
  BitTorrentRequestMessage reqMsg (pieceIndex,blockOffSet,blockLength);

  // Step 5: Combine the headers from steps 2..4 by adding them to the created packet
  packet->AddHeader (reqMsg);
  packet->AddHeader (typeHead);
  packet->AddHeader (lenHead);

  // Step 6: Enqueue the packet
  // Step 6a: Insert it into the send queue
  m_sendQueue.push_back (packet);
  // Step 6b: Indicate that this message is NOT a PIECE message (which is handled differently)
  m_sendQueuePieceMessageIndicators.push_back (false);
  
  NS_LOG_INFO ("Peer: Enqueueing request to " << GetRemoteIp () << "; for " << reqMsg.GetPieceIndex () << "@" << reqMsg.GetBlockOffset () << "->" << reqMsg.GetBlockOffset () + reqMsg.GetBlockLength () << ".");

  // Step 7: Send out messages in the send queue
  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::CancelRequest (uint32_t pieceIndex, uint32_t blockOffSet, uint32_t blockLength)
{
  if (m_connectionState != CONN_STATE_CONNECTED)
    {
      return;
    }

  Ptr<Packet> packet = Create<Packet> ();

  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_CANCEL_LENGTH);
  BitTorrentTypeHeader typeHead(BitTorrentTypeHeader::CANCEL);
  BitTorrentCancelMessage canMsg (pieceIndex, blockOffSet, blockLength);

  packet->AddHeader (canMsg);
  packet->AddHeader (typeHead);
  packet->AddHeader (lenHead);

  // Prioritized sending
  if (m_blockSendingActive)      // Insert the cancel request right at the beginning but after the currently sending block
    {
      std::list<Ptr<Packet> >::iterator sendQueueIt = m_sendQueue.begin ();
      std::list<bool>::iterator sendQueuePieceMessageIndicatorsIt = m_sendQueuePieceMessageIndicators.begin ();

      ++sendQueueIt;
      ++sendQueuePieceMessageIndicatorsIt;

      if (sendQueueIt == m_sendQueue.end ())
        {
          m_sendQueue.push_back (packet);
          m_sendQueuePieceMessageIndicators.push_back (false);
        }
      else
        {
          m_sendQueue.insert (sendQueueIt, packet);
          m_sendQueuePieceMessageIndicators.insert (sendQueuePieceMessageIndicatorsIt, false);
        }
    }
  else
    {
      m_sendQueue.push_front (packet);
      m_sendQueuePieceMessageIndicators.push_front (false);
    }

  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::SendBitfield ()
{
  if (m_connectionState != CONN_STATE_CONNECTED)
    {
      return;
    }

  Ptr<Packet> packet = Create<Packet> ();

  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_BITFIELD_LENGTH_MIN + m_myClient->GetTorrent ()->GetBitfieldSize ());
  BitTorrentTypeHeader typeHead (BitTorrentTypeHeader::BITFIELD);
  BitTorrentBitfieldMessage bitFMsg (m_myClient->GetTorrent ()->GetBitfieldSize ());
  bitFMsg.CopyBitFieldFrom (m_myClient->GetBitfield ());

  packet->AddHeader (bitFMsg);
  packet->AddHeader (typeHead);
  packet->AddHeader (lenHead);

  m_sendQueue.push_back (packet);
  m_sendQueuePieceMessageIndicators.push_back (false);

  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::SendHaveMessage (uint32_t pieceIndex)
{
  if (m_connectionState != CONN_STATE_CONNECTED)
    {
      return;
    }

  Ptr<Packet> packet = Create<Packet> ();

  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_HAVE_LENGTH);
  BitTorrentTypeHeader typeHead (BitTorrentTypeHeader::HAVE);
  BitTorrentHaveMessage haveMsg (pieceIndex);

  packet->AddHeader (haveMsg);
  packet->AddHeader (typeHead);
  packet->AddHeader (lenHead);

  // Prioritized sending
  if (m_blockSendingActive)      // Insert the have message right at the beginning but after the currently sending block
    {
      std::list<Ptr<Packet> >::iterator sendQueueIt = m_sendQueue.begin ();
      std::list<bool>::iterator sendQueuePieceMessageIndicatorsIt = m_sendQueuePieceMessageIndicators.begin ();

      ++sendQueueIt;
      ++sendQueuePieceMessageIndicatorsIt;

      if (sendQueueIt == m_sendQueue.end ())
        {
          m_sendQueue.push_back (packet);
          m_sendQueuePieceMessageIndicators.push_back (false);
        }
      else
        {
          m_sendQueue.insert (sendQueueIt, packet);
          m_sendQueuePieceMessageIndicators.insert (sendQueuePieceMessageIndicatorsIt, false);
        }
    }
  else
    {
      m_sendQueue.push_front (packet);
      m_sendQueuePieceMessageIndicators.push_front (false);
    }

  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::SetAmChoking (bool amChoking)
{
  if (m_amChoking != amChoking)
    {
      m_amChoking = amChoking;
      NotifyPeerOfChokeChange ();
    }
}

void Peer::SendBlock (uint32_t pieceIndex, uint32_t blockOffSet, uint32_t blockLength)
{
  RequestInformation reqInfo;
  reqInfo.pieceIndex = pieceIndex;
  reqInfo.blockOffSet = blockOffSet;
  reqInfo.blockLength = std::min (m_myClient->GetTorrent ()->GetPieceLength () - blockOffSet, blockLength);

  Ptr<Packet> dummyPacket = Create<Packet> ();

  m_sendQueue.push_back (dummyPacket);
  m_sendQueuePieceMessageIndicators.push_back (true);
  m_requestQueue.push_back (reqInfo);

  // Blocks (PIECE messages) are handled separately by HandleSend, just call it
  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::SendExtendedMessage (uint8_t messageId, const std::string& message)
{
  if (m_connectionState != CONN_STATE_CONNECTED)
    {
      return;
    }

  Ptr<Packet> packet = Create<Packet> ();

  BitTorrentTypeHeader typeHead (BitTorrentTypeHeader::EXTENDED);
  BitTorrentExtensionMessage extMsg (messageId, message);
  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_EXTENSIONPROTOCOL_LENGTH_MIN + extMsg.GetPacketLength ());  

  packet->AddHeader (extMsg);
  packet->AddHeader (typeHead);
  packet->AddHeader (lenHead);

  m_sendQueue.push_back (packet);
  m_sendQueuePieceMessageIndicators.push_back (false);

  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

Ipv4Address Peer::GetRemoteIp () const
{
  return m_remoteIp;
}

uint16_t Peer::GetRemotePort () const
{
  return m_remotePort;
}

std::string Peer::GetRemotePeerId () const
{
  return m_remotePeerId;
}

bool Peer::IsChoking () const
{
  return m_peerChoking;
}

bool Peer::IsInterested () const
{
  return m_peerInterested;
}

const uint8_t* Peer::GetPieceCorruptionMap () const
{
  return m_pieceCorruptionMap;
}

Time Peer::GetConnectionEstablishmentTime () const
{
  return m_connectionEstablishmentTime;
}

uint64_t Peer::GetTotalBytesDownloaded () const
{
  return m_totalBytesDownloaded;
}

uint64_t Peer::GetTotalBytesUploaded () const
{
  return m_totalBytesUploaded;
}

double Peer::GetBpsDownload ()
{
  uint64_t currentSecond = static_cast<uint64_t> (Simulator::Now ().GetSeconds ());

  double bpsDownload = 0;
  for (uint32_t i = 0; i < BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS; ++i)
    {
      if (currentSecond - m_downloadHistoryReSetTime[i] > BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS)
        {
          m_downloadHistory[i] = 0;
          m_downloadHistoryReSetTime[i] = currentSecond;
        }

      bpsDownload += m_downloadHistory[i];
    }

  return (bpsDownload * 8) / BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS;
}

double Peer::GetBpsUpload ()
{
  uint64_t currentSecond = static_cast<uint64_t> (Simulator::Now ().GetSeconds ());

  double bpsUpload = 0;
  for (uint32_t i = 0; i < BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS; ++i)
    {
      if (currentSecond - m_uploadHistoryReSetTime[i] > BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS)
        {
          m_uploadHistory[i] = 0;
          m_uploadHistoryReSetTime[i] = currentSecond;
        }

      bpsUpload += m_uploadHistory[i];
    }

  return (bpsUpload * 8) / BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS;
}

void Peer::NotifyPeerOfChokeChange ()
{
  if (m_connectionState != CONN_STATE_CONNECTED)
    {
      return;
    }

  Ptr<Packet> packet = Create<Packet> ();

  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_CHOKE_LENGTH);
  BitTorrentTypeHeader typeHead (BitTorrentTypeHeader::CHOKE);
  if (!m_amChoking)
    {
      typeHead.SetMessageType (BitTorrentTypeHeader::UNCHOKE);
    }

  packet->AddHeader (typeHead);
  packet->AddHeader (lenHead);

  m_sendQueue.push_back (packet);
  m_sendQueuePieceMessageIndicators.push_back (false);

  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::NotifyPeerOfInterestedChange ()
{
  if (m_connectionState != CONN_STATE_CONNECTED)
    {
      return;
    }

  Ptr<Packet> packet = Create<Packet> ();

  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_INTERESTED_LENGTH);
  BitTorrentTypeHeader typeHead (BitTorrentTypeHeader::INTERESTED);
  if (!m_amInterested)
    {
      typeHead.SetMessageType (BitTorrentTypeHeader::NOT_INTERESTED);
    }

  packet->AddHeader (typeHead);
  packet->AddHeader (lenHead);

  m_sendQueue.push_back (packet);
  m_sendQueuePieceMessageIndicators.push_back (false);

  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::HandleCancel (Ptr<Packet> packet)
{
  BitTorrentCancelMessage cancelMsg;
  packet->RemoveHeader (cancelMsg);

  RequestInformation reqInfo;
  reqInfo.pieceIndex = cancelMsg.GetPieceIndex ();
  reqInfo.blockOffSet = cancelMsg.GetBlockOffset ();
  reqInfo.blockLength = cancelMsg.GetBlockLength ();

  std::list<bool>::iterator it = m_sendQueuePieceMessageIndicators.begin ();
  std::list<Ptr<Packet> >::iterator it2 = m_sendQueue.begin ();
  std::list<RequestInformation>::iterator it3 = m_requestQueue.begin ();

  bool requestFound = false;

  if (m_blockSendingActive && (*it3) == reqInfo)
    {
      // We don't cancel the first request while it is active
      return;
    }

  while (!requestFound && it != m_sendQueuePieceMessageIndicators.end () && it3 != m_requestQueue.end ())
    {
      if (*it)          // If we have found a scheduled PIECE message
        {
          if ((*it3) == reqInfo)              // If the information on that PIECE message matches CANCEL message...
            {
              requestFound = true;
              break;
            }
          else               // ... else, we know that this PIECE message is not what we have to cancel. We go on to the next one.
            {
              ++it3;
            }
        }

      NS_ASSERT (!requestFound);

      ++it;           // Next packet type indicator
      ++it2;           // Next packet
    }

  if (requestFound)
    {
      m_sendQueuePieceMessageIndicators.erase (it);
      m_sendQueue.erase (it2);
      m_requestQueue.erase (it3);

      m_myClient->PeerCancelEvent (this, reqInfo.pieceIndex, reqInfo.blockOffSet, reqInfo.blockLength);
    }
  else
    {
      NS_LOG_INFO ("Peer: Received CANCEL for non-existing prior REQUEST: " << cancelMsg.GetPieceIndex () << "@" << cancelMsg.GetBlockOffset () << "->" << cancelMsg.GetBlockOffset () + cancelMsg.GetBlockLength () << ".");
    }
}

bool Peer::HandlePiece (Ptr<Packet> packet, uint32_t packetLength)
{
  BitTorrentPieceMessage pieceMsg;
  packet->RemoveHeader (pieceMsg);

  uint32_t blockLength = packetLength - BT_PROTOCOL_MESSAGES_PIECE_LENGTH_MIN;
  uint32_t blockPieceIndex = pieceMsg.GetIndex ();
  uint32_t blockBlockOffSet = pieceMsg.GetBegin ();

  if (m_blockBufferSize < blockLength)
    {
      delete [] m_blockBuffer;
      m_blockBuffer = 0;
      m_blockBuffer = new uint8_t[blockLength];
      m_blockBufferSize = blockLength;
    }

  uint32_t dataToRead = std::min (packet->GetSize (),blockLength);
  m_totalBytesDownloaded += dataToRead;

  packet->CopyData (m_blockBuffer, dataToRead);
  packet->RemoveAtStart (dataToRead);

  if (blockLength - dataToRead == 0)
    {
      if (m_myClient->GetCheckDownloadedData ())
        {
          if (std::memcmp (
                m_blockBuffer,
                m_myClient->GetTorrentDataBuffer () + blockPieceIndex * m_myClient->GetTorrent ()->GetPieceLength () + blockBlockOffSet,
                blockLength)
              == 0)
            {
              m_pieceCorruptionMap[blockPieceIndex] = BT_PEER_PIECE_RECEPTION_CHECKSUM_OK;
            }
          else
            {
              m_pieceCorruptionMap[blockPieceIndex] = BT_PEER_PIECE_RECEPTION_CHECKSUM_NOT_OK;
            }
        }

      // Call the BlockCompletedEvent for this block (even if it was not downloaded correctly, that case is also handled in the called method)
      m_myClient->PeerBlockCompleteEvent (this,blockPieceIndex,blockBlockOffSet,blockLength);

      return false;           // i.e., there is no more data left to receive
    }

  return true;       // i.e., there is more data left to receive until the block can be finished
}

void Peer::HandleRead (Ptr<Socket> socket)
{
  if (!(m_connectionState == CONN_STATE_CONNECTED || m_connectionState == CONN_STATE_AWAIT_HANDSHAKE))
    {
      return;
    }

  uint32_t available = socket->GetRxAvailable ();
  while (available > 0)
    {
      m_packetBuffer->AddAtEnd (socket->Recv ());

      uint64_t currentSecond = static_cast<uint64_t> (Simulator::Now ().GetSeconds ());
      uint64_t currentSecondModulo = currentSecond % BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS;
      if (m_downloadHistoryReSetTime[currentSecondModulo] != currentSecond)
        {
          m_downloadHistoryReSetTime[currentSecondModulo] = currentSecond;
          m_downloadHistory[currentSecondModulo] = available;
        }
      else
        {
          m_downloadHistory[currentSecondModulo] += available;
        }

      available = socket->GetRxAvailable ();
    }

  while (m_packetBuffer->GetSize () >= m_lengthHeader.GetPacketLength ())
    {
      if (m_connectionState == CONN_STATE_AWAIT_HANDSHAKE)
        {
          /*
           * RENE: Add way to Get protocol length from the packet first
           * down here -------------------+ (Just as with BitTorrentLengthHeader, but 1 byte instead of 4)
           *                              |
           *                              v
           */
          if (m_packetBuffer->GetSize () >= BT_PROTOCOL_MESSAGES_HANDSHAKE_LENGTH_MIN + BT_PROTOCOL_MESSAGES_HANDSHAKE_PROTOCOL_STRING_LENGTH)
            {
              BitTorrentHandshakeMessage handshake;
              m_packetBuffer->RemoveHeader (handshake);

              // Store data from packet
              m_remotePeerId.append (reinterpret_cast<const char*> (handshake.GetPeerId ()),BT_PROTOCOL_MESSAGES_HANDSHAKE_PEERID_LENGTH_MAX);

              m_connectionEstablishmentTime = Simulator::Now ();
              m_connectionState = CONN_STATE_CONNECTED;

              m_lengthHeader.SetPacketLength (BT_PROTOCOL_MESSAGES_LENGTHHEADER_LENGTH);
            }
          else
            {
              break;                   // We still wait for the completion of the handshake message
            }
        }
      else if (m_connectionState == CONN_STATE_CONNECTED)
        {
          if (!m_lengthHeaderRemoved)              // We have not yet inspected the beginning of the next message
            {
              NS_ASSERT (m_packetBuffer->GetSize () >= m_lengthHeader.GetPacketLength ());

              m_packetBuffer->RemoveHeader (m_lengthHeader);

              if (m_lengthHeader.GetPacketLength () == 0)                 // We received a keep-alive message
                {
                  m_lengthHeader.SetPacketLength (BT_PROTOCOL_MESSAGES_LENGTHHEADER_LENGTH);                      // So we will end up in this same else if(!m_lengthHeaderRemoved) section again
                }
              else
                {
                  // If we receive messages > BT_PROTOCOL_MESSAGES_LENGTHHEADER_THRESHOLD_PIECE_OR_EXTENSIONPROTOCOL_MESSAGE bytes, we assume they are piece messages and start estimating the upload bandwidth of the peer
                  // Note: Since we do this all the time, it does not matter that a bitfield message might be sent; the time is just replaced
                  m_lengthHeaderRemoved = true;
                }
            }
          else
            {
              NS_ASSERT (m_packetBuffer->GetSize () >= m_lengthHeader.GetPacketLength ());

              m_packetBuffer->RemoveHeader (m_typeHeader);

              switch (m_typeHeader.GetMessageType ())
                {
                case BitTorrentTypeHeader::CHOKE:
                  {
                    if (!m_peerChoking)
                      {
                        m_peerChoking = true;

                        uint64_t currentSecond = static_cast<uint64_t> (Simulator::Now ().GetSeconds ());
                        uint64_t currentSecondModulo = currentSecond % BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS;
                        if (m_downloadHistoryReSetTime[currentSecondModulo] != currentSecond)
                          {
                            m_downloadHistoryReSetTime[currentSecondModulo] = currentSecond;
                            m_downloadHistory[currentSecondModulo] = 0;
                          }

                        m_myClient->PeerChokeChangingEvent (this);
                      }
                    break;
                  }
                case BitTorrentTypeHeader::UNCHOKE:
                  {
                    if (m_peerChoking)
                      {
                        m_peerChoking = false;
                        m_myClient->PeerChokeChangingEvent (this);
                      }
                    break;
                  }
                case BitTorrentTypeHeader::INTERESTED:
                  {
                    if (!m_peerInterested)
                      {
                        m_peerInterested = true;
                        m_myClient->PeerInterestedChangingEvent (this);
                      }
                    break;
                  }
                case BitTorrentTypeHeader::NOT_INTERESTED:
                  {
                    if (m_peerInterested)
                      {
                        m_peerInterested = false;
                        m_myClient->PeerInterestedChangingEvent (this);
                      }
                    break;
                  }
                case BitTorrentTypeHeader::HAVE:
                  {
                    BitTorrentHaveMessage haveMsg;
                    m_packetBuffer->RemoveHeader (haveMsg);

                    uint32_t pieceIndex = haveMsg.GetPieceIndex ();

                    m_bitfield[pieceIndex / 8] |= (1 << (7 - pieceIndex % 8));

                    m_myClient->PeerHaveEvent (this, pieceIndex);
                    break;
                  }
                case BitTorrentTypeHeader::BITFIELD:
                  {
                    BitTorrentBitfieldMessage bitfieldMsg (m_lengthHeader.GetPacketLength () - 1);
                    m_packetBuffer->PeekHeader (bitfieldMsg);
                    if (m_lengthHeader.GetPacketLength () - 1 != m_myClient->GetTorrent ()->GetBitfieldSize ())
                      {
                        NS_LOG_INFO ("Peer: Received a bitfield of wrong length from " << GetRemoteIp () << ".");
                        return;
                      }
					else
					  {
					    m_packetBuffer->RemoveHeader (bitfieldMsg);
				      }
                    bitfieldMsg.CopyBitFieldTo (&m_bitfield);

                    m_myClient->PeerBitfieldReceivedEvent (this);
                    break;
                  }
                case BitTorrentTypeHeader::REQUEST:
                  {
                    BitTorrentRequestMessage reqMsg;
                    m_packetBuffer->RemoveHeader (reqMsg);

                    m_myClient->PeerRequestEvent (this, reqMsg.GetPieceIndex (), reqMsg.GetBlockOffset (), reqMsg.GetBlockLength ());
                    break;
                  }
                case BitTorrentTypeHeader::PIECE:
                  {
                    HandlePiece (m_packetBuffer, m_lengthHeader.GetPacketLength ());
                    break;
                  }
                case BitTorrentTypeHeader::CANCEL:
                  {
                    HandleCancel (m_packetBuffer);
                    break;
                  }
                case BitTorrentTypeHeader::PORT:
                  {
                    BitTorrentPortMessage portMsg;
                    m_packetBuffer->RemoveHeader (portMsg);

                    m_myClient->PeerPortMessageEvent (this, portMsg.GetListenPort ());
                    break;
                  }
                case BitTorrentTypeHeader::EXTENDED:
                  {
                    BitTorrentExtensionMessage extMsg (m_lengthHeader.GetPacketLength () - 1);
                    m_packetBuffer->RemoveHeader (extMsg);

                    m_myClient->PeerExtensionMessageEvent (this, extMsg.GetMessageId (), extMsg.GetContent ());
                    break;
                  }
                default:
                  {
                    m_packetBuffer->RemoveAtStart (m_lengthHeader.GetPacketLength ());
                    break;
                  }
                }                 // end of switch(m_typeHeader.GetMessageType())

              m_lengthHeaderRemoved = false;                   // So we end up in the other branch of the if statement again
              m_lengthHeader.SetPacketLength (BT_PROTOCOL_MESSAGES_LENGTHHEADER_LENGTH);                   // So we will stay in / enter this while loop if we have enough bytes in the buffer left
            }
        }
    }
}

void Peer::HandleSend (Ptr<Socket> socket, uint32_t bytesFree)
{
  // We use an indicator whether or not to start another recursive call to add further data
  bool nextIteration = true;

  while (nextIteration)
    {

      if (!(m_connectionState == CONN_STATE_CONNECTED || m_connectionState == CONN_STATE_AWAIT_HANDSHAKE))
        {
          return;
        }

      if (m_sendQueue.empty ())
        {
          return;
        }

      if (m_peerSocket->GetTxAvailable () == 0)
        {
          return;
        }

      if (m_blockSendingActive)
        {
          // Step 1: Calculate the actual amount of bytes that we still have to send
          uint32_t bytesToSend = std::min (m_peerSocket->GetTxAvailable (), m_blockSendDataLeft);

          // Step 2: Create a packet of the appropriate size containing the real data we have to send
          Ptr<Packet> nextPart = Create<Packet> (m_blockSendPtr, bytesToSend);

          // Step 3: Correct the data pointers so we read the correct data in a (possible) next iteration
          m_blockSendPtr += bytesToSend;
          m_blockSendDataLeft -= bytesToSend;

          // Step 4: Send out the data
          m_peerSocket->Send (nextPart);
          m_totalBytesUploaded += bytesToSend;

          /*
           * Step 5: If we are about to finish serving a request, we inform the application of it, clean up
           * and prevent other packets to be sent to prevent probable side effects (just in case)
           */
          if (m_blockSendDataLeft <= 0)
            {
              NS_LOG_INFO ("Peer: Finished upload of request " << m_requestQueue.front ().pieceIndex << "@" << m_requestQueue.front ().blockOffSet << "->" <<  m_requestQueue.front ().blockOffSet +  m_requestQueue.front ().blockLength << " to " << GetRemoteIp () << "; free bytes left in tx buffer: " << m_peerSocket->GetTxAvailable () << "; requests left: " << m_requestQueue.size () - 1 << ".");

              m_blockSendingActive = false;

              m_myClient->PeerBlockUploadCompleteEvent (this, m_requestQueue.front ().pieceIndex,m_requestQueue.front ().blockOffSet,  m_requestQueue.front ().blockLength);

              m_sendQueuePieceMessageIndicators.pop_front ();
              m_sendQueue.pop_front ();
              m_requestQueue.pop_front ();
            }

          nextIteration = true;
        }
      else
        {
          if (m_sendQueuePieceMessageIndicators.front ())
            {
              NS_ASSERT (!m_requestQueue.empty ());

              if (m_peerSocket->GetTxAvailable () >= BT_PROTOCOL_MESSAGES_LENGTHHEADER_LENGTH + BT_PROTOCOL_MESSAGES_PIECE_LENGTH_MIN)
                {
                  // Step 1: Prepare the buffer that we will send our packet data from
                  m_blockSendPtr =
                    m_myClient->GetTorrentDataBuffer () +
                    m_requestQueue.front ().pieceIndex * m_myClient->GetTorrent ()->GetPieceLength () +
                    m_requestQueue.front ().blockOffSet;
                  m_blockSendDataLeft = m_requestQueue.front ().blockLength;

                  // Step 2: Create the prelude of the piece message
                  Ptr<Packet> packet = Create<Packet> ();

                  BitTorrentLengthHeader lenHead (BT_PROTOCOL_MESSAGES_PIECE_LENGTH_MIN + m_blockSendDataLeft);
                  BitTorrentTypeHeader typeHead (BitTorrentTypeHeader::PIECE);
                  BitTorrentPieceMessage pieceMsg (m_requestQueue.front ().pieceIndex,m_requestQueue.front ().blockOffSet);

                  packet->AddHeader (pieceMsg);
                  packet->AddHeader (typeHead);
                  packet->AddHeader (lenHead);

                  // Step 3: Initiate the sending of this packet
                  m_blockSendingActive = true;
                  m_totalBytesUploaded += m_blockSendDataLeft;

                  // Step 4: Send out the packet
                  m_peerSocket->Send (packet);

                  // Step 5: Start another iteration so the piece data can be appended directly
                  nextIteration = true;

                  NS_LOG_INFO ("Peer: Starting to upload to " << GetRemoteIp () << "; piece " << pieceMsg.GetIndex () << "@" << m_requestQueue.front ().blockOffSet << "->" << m_requestQueue.front ().blockOffSet + m_requestQueue.front ().blockLength << ".");
                }
              else
                {
                  nextIteration = false;
                }
            }
          else
            {
              if (m_peerSocket->GetTxAvailable () >= m_sendQueue.front ()->GetSize ())
                {
                  // Step 1: Get the packet to send
                  Ptr<Packet> packet = m_sendQueue.front() ;
				  
				  // Step 2: Send the packet
				  m_peerSocket->Send (packet);

                  // Step 3: Remove the packet data from the internal queue
                  m_sendQueue.pop_front ();
                  m_sendQueuePieceMessageIndicators.pop_front ();

                  // Step 4: Start another iteration so further packets can be sent out directly
                  nextIteration = true;
                }
              else
                {
                  nextIteration = false;
                }
            }
        }
    }
}

void Peer::HandleDataSent (Ptr<Socket> socket, uint32_t dataSent)
{
  uint64_t currentSecond = static_cast<uint64_t> (Simulator::Now ().GetSeconds ());
  uint64_t currentSecondModulo = currentSecond % BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS;
  if (m_uploadHistoryReSetTime[currentSecondModulo] != currentSecond)
    {
      m_uploadHistoryReSetTime[currentSecondModulo] = currentSecond;
      m_uploadHistory[currentSecondModulo] = dataSent;
    }
  else
    {
      m_uploadHistory[currentSecondModulo] += dataSent;
    }

//  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::HandleConnected (Ptr<Socket> socket)
{
  if (m_connectionState != CONN_STATE_AWAIT_CONNECTION)
    {
      return;
    }

  // When we are connected, we send out the handshake packet and start waiting for the packet of our peer
  m_connectionState = CONN_STATE_AWAIT_HANDSHAKE;

  BitTorrentHandshakeMessage handshake;
  handshake.SetProtocolString (BT_PROTOCOL_MESSAGES_HANDSHAKE_PROTOCOL_STRING);
  uint8_t peerId[BT_PROTOCOL_MESSAGES_HANDSHAKE_PEERID_LENGTH_MAX];
  std::memcpy (peerId,m_myClient->GetPeerId ().c_str (), std::min (static_cast<size_t> (BT_PROTOCOL_MESSAGES_HANDSHAKE_PEERID_LENGTH_MAX), m_myClient->GetPeerId ().size ()));
  handshake.SetPeerId (peerId);
  handshake.SetInfoHash (m_myClient->GetCurrentInfoHash ());
    
  Ptr<Packet> announcementPacket = Create<Packet> ();
  announcementPacket->AddHeader (handshake);

  m_sendQueue.push_back (announcementPacket);
  m_sendQueuePieceMessageIndicators.push_back (false);

  HandleSend (m_peerSocket, m_peerSocket->GetTxAvailable ());
}

void Peer::HandleConnectionFail (Ptr<Socket> socket)
{
  NS_LOG_INFO ("Peer: Connection with " << GetRemoteIp () << " failed.");

  m_connectionState = CONN_STATE_FAILED;

  Simulator::ScheduleNow (&BitTorrentClient::UnregisterPeer, m_myClient, this);
  Simulator::ScheduleNow (&BitTorrentClient::PeerConnectionFailEvent, m_myClient, this);

  // DEBUG
  Simulator::ScheduleNow (&Peer::PseudoDeInitializeMe, this);
}

void Peer::HandleConnectionClosed (Ptr<Socket> socket)
{
  NS_LOG_INFO ("Peer: Connection with " << GetRemoteIp () << " closed.");

  if (m_connectionState == CONN_STATE_CLOSED || m_connectionState == CONN_STATE_CLOSED_WITH_ERROR)
    {
      return;
    }

  m_peerSocket->Close ();
  m_connectionState = CONN_STATE_CLOSED;

  m_amChoking = true;
  m_peerChoking = true;

  Simulator::ScheduleNow (&BitTorrentClient::UnregisterPeer, m_myClient, this);
  Simulator::ScheduleNow (&BitTorrentClient::PeerConnectionCloseEvent, m_myClient, this);

  // DEBUG
  Simulator::ScheduleNow (&Peer::PseudoDeInitializeMe, this);
}

void Peer::HandleConnectionClosedWithError (Ptr<Socket> socket)
{
  NS_LOG_INFO ("Peer: Connection with " << GetRemoteIp () << " closed with error.");

  if (m_connectionState == CONN_STATE_CLOSED || m_connectionState == CONN_STATE_CLOSED_WITH_ERROR)
    {
      return;
    }

  m_connectionState = CONN_STATE_CLOSED_WITH_ERROR;

  m_peerSocket->Close ();

  Simulator::ScheduleNow (&BitTorrentClient::UnregisterPeer, m_myClient, this);
  Simulator::ScheduleNow (&BitTorrentClient::PeerConnectionFailEvent, m_myClient, this);

  // DEBUG
  Simulator::ScheduleNow (&Peer::PseudoDeInitializeMe, this);
}

// DEBUG
void Peer::PseudoDeInitializeMe ()
{
  if (m_connectionState == CONN_STATE_DEINITIALIZED)
    {
      return;
    }

  NS_LOG_INFO ( m_myClient->GetNode ()->GetId () << ": Pseudo-deinitializing the peer class for " << GetRemoteIp () );

  m_remotePeerId.clear ();

  m_bitfield.clear ();

  m_packetBuffer->RemoveAtStart (m_packetBuffer->GetSize ());
  m_packetBuffer = Create<Packet> ();

  m_sendQueuePieceMessageIndicators.clear ();
  
  std::list<Ptr<Packet> >* pDummy = new std::list<Ptr<Packet> > ();
  std::list<RequestInformation>* pDummy2 = new std::list<RequestInformation> ();
  m_sendQueue.clear ();
	m_sendQueue.swap(*pDummy);
  m_requestQueue.clear ();
	m_requestQueue.swap(*pDummy2);
  delete pDummy;
  delete pDummy2;
  delete[] m_blockBuffer;
  m_blockBuffer = 0;
  delete[] m_blockSendBuffer;
  m_blockSendBuffer = 0;
  delete[] m_pieceCorruptionMap;
  m_pieceCorruptionMap = 0;

  m_connectionState = CONN_STATE_DEINITIALIZED;
}

// DEBUG; NOTE: CALLING THIS FUNCTION AT THE WRONG TIME MIGHT CAUSE SEGFAULTS!!!
void Peer::PseudoDeInitializeMePhase2 ()
{
  m_dontDeInitializeMe = NULL;
}

} // ns bittorrent
} // ns ns3
