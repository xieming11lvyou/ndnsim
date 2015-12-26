/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
 * Copyright (c) 2009, 2012 RWTH Aachen University
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
 * Authors: Hendrik vom Lehn, Rene Glebke
 */

#include "BitTorrentHttpServer.h"

#include "ns3/address.h"
#include "ns3/callback.h"
#include "ns3/inet-socket-address.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include <sstream>
#include <stdint.h>

NS_LOG_COMPONENT_DEFINE ("BitTorrentHttpServer");

namespace ns3 {
namespace bittorrent {

BitTorrentHttpServer::BitTorrentHttpServer ()
{
}

BitTorrentHttpServer::~BitTorrentHttpServer ()
{
}

void BitTorrentHttpServer::StartListening (Ptr<Node> node, TypeId typeidvar, Callback<void, Ptr<Socket>, const Address&> handleConnectionCreation)
{
  // setup socket (bind, create callbacks, listen)
  m_socket = Socket::CreateSocket (node, typeidvar);
  m_socket->Bind (InetSocketAddress (Ipv4Address::GetAny (), 80));

  m_socket->SetAcceptCallback (
    MakeCallback (&BitTorrentHttpServer::ConnectionRequest, this),
    handleConnectionCreation
    );

  m_socket->Listen ();
}

void BitTorrentHttpServer::StopListening ()
{
  // delete callback and close socket
  m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  m_socket->Close ();
}

bool BitTorrentHttpServer::ConnectionRequest (Ptr<Socket> socket, const Address &addr)
{
  return true; // accept all connections
}

void BitTorrentHttpServer::ConnectionCreated (Ptr<Socket> socket, const Address &addr, Callback<void, Ptr<Socket> > handleReceiveRequest)
{
  socket->SetRecvCallback (handleReceiveRequest);
  socket->SetCloseCallbacks (MakeCallback (&BitTorrentHttpServer::NormalClose, this),
                             MakeCallback (&BitTorrentHttpServer::ErrorClose, this)
                             );
}

void BitTorrentHttpServer::ReceiveRequest (Ptr<Socket> socket, Callback<void, std::string, Ptr<Socket>, const Address& > handleDataCreater, Callback<void, Ptr<Socket>, std::string, const Address&> handleErrorOccurance)
{
  // get packet as string
  std::string data;
  Ptr<Packet> packet;
  uint32_t packet_size;

  std::string ErrorCode;
  Address fromAddress;
  while ((packet = socket->RecvFrom (fromAddress)) && (packet->GetSize () > 0))
    {
      packet_size = packet->GetSize ();
      uint8_t *buf = new uint8_t[packet->GetSize ()];
      packet->CopyData (buf, packet_size);
      data.append (std::string (reinterpret_cast<const char *> (buf), packet->GetSize ()));
      delete[] buf;
    }
  // parse url
  if (data.find ("GET /",0) == 0)
    {
      std::string::size_type pathend = data.find (" HTTP/1.", 0);
      if (pathend != std::string::npos)
        {
          // try to extract path out of url
          std::string path;
          std::istringstream pathin;
          pathin.str (data.substr (4, pathend - 4));
          pathin >> path;
          handleDataCreater (path, socket, fromAddress);
          return;
        }
    }

  ErrorCode = "400";
  handleErrorOccurance (socket, ErrorCode, fromAddress);
  return;
}


void BitTorrentHttpServer::SendData (Ptr<Socket> socket, uint8_t* dataToSend, uint32_t ContentLength, std::string code)
{
  std::ostringstream reply;
  std::string reply_str;
  if (code == "400")
    {
      std::ostringstream reply;
      reply << "HTTP/1.1 400 Bad Request\r\nContent-type: text/plain\r\nContent-Length: 11\r\nConnection: Close\r\n\r\nBad request";
      std::string replystr = reply.str ();
      replystr.append (reinterpret_cast<const char*> (dataToSend), ContentLength);
      TrySending (socket, replystr, 3);
    }

  if (code == "404")
    {
      std::ostringstream reply;
      reply << "HTTP/1.1 404 Not Found\r\nContent-type: text/plain\r\nContent-Length: 9\r\nConnection: Close\r\n\r\nNot found";
      std::string replystr = reply.str ();
      replystr.append (reinterpret_cast<const char*> (dataToSend), ContentLength);
      TrySending (socket, replystr, 3);
    }

  if (code == "200")
    {
      std::ostringstream reply;
      reply << "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\nContent-Length: " << ContentLength << "\r\nConnection: close\r\n\r\n";
      std::string replystr = reply.str ();
      replystr.append (reinterpret_cast<const char*> (dataToSend), ContentLength);
      TrySending (socket, replystr, 3);
    }
}

void BitTorrentHttpServer::TrySending (Ptr<Socket> socket, std::string toSend, uint32_t retries)
{
  if (socket->GetTxAvailable () > toSend.length ())
    {
      socket->Send ((uint8_t*)toSend.c_str (), toSend.length (), 0);
    }
  else if (retries > 0)
    {
      --retries;
      Simulator::Schedule (Seconds (1), &BitTorrentHttpServer::TrySending, this, socket, toSend, retries);
    }
}

void BitTorrentHttpServer::SimpleSend (Ptr<Socket> socket, uint8_t* dataToSend, uint32_t blocks, uint32_t ContentLength)
{
  if (blocks > 0)
    {
      socket->Send (dataToSend, ContentLength, 0);
      Simulator::Schedule (MilliSeconds (200), &BitTorrentHttpServer::SimpleSend, this, socket, dataToSend, blocks - 1, ContentLength);
    }
  else
    {
      socket->Close ();
    }
}

void BitTorrentHttpServer::NormalClose (Ptr<Socket> socket)
{
}

void BitTorrentHttpServer::ErrorClose (Ptr<Socket> socket)
{
}

} // ns bittorrent
} // ns ns3

