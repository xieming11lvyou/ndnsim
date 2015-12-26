/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
 * Copyright (c) 2009-2011 ComSys, RWTH Aachen University
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
 * Authors: Rene Glebke, Suraj Prabhakaran, Hendrik vom Lehn
 */

#include "BitTorrentHttpClient.h"

#include "ns3/callback.h"
#include "ns3/inet-socket-address.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/socket.h"

#include <string>
#include <sstream>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("BitTorrentHttpClient");
NS_OBJECT_ENSURE_REGISTERED (BitTorrentHttpClient);

BitTorrentHttpClient::BitTorrentHttpClient ()
{
  m_requestActive = false;
}

BitTorrentHttpClient::~BitTorrentHttpClient ()
{
}

bool BitTorrentHttpClient::GetRequestActive () const
{
  return m_requestActive;
}

bool BitTorrentHttpClient::HasBadHeader () const
{
  return m_badHeader;
}

bool BitTorrentHttpClient::HttpGetRequest (Ptr<Node> node, TypeId typeidvar, Ipv4Address addr, uint16_t port, std::string path, Callback<void, Ptr<Socket> > ReplyHandler)
{
  if (m_requestActive)
    {
      NS_LOG_INFO ("BitTorrentHttpClient: Pending request. Aborting new request.");
      return false;
    }

  // Step 1: Initialize variables needed to handle a reply
  m_remoteIp = addr;
  CloseAndReInit ();
  m_requestActive = true;

  // Step 2: Create the socket and bind it
  m_socket = Socket::CreateSocket (node, typeidvar);
  m_socket->Bind ();
  m_socket->SetRecvCallback (ReplyHandler);
  m_socket->Connect (InetSocketAddress (addr, port));

  // Step 3: Send out the request
  std::ostringstream request;
  request << "GET " << path << " HTTP/1.1\r\nHost: " << addr << "\r\nConnection: close\r\n\r\n";
  std::string requeststr = request.str ();
  if (m_socket->GetTxAvailable () >= requeststr.length ())
    {
      m_socket->Send ((uint8_t*)requeststr.c_str (), requeststr.length (), 0);
      return true;
    }
  else
    {
      NS_LOG_INFO ("Cannot send out HTTP reply; socket transmission buffer too small.");
      return false;
    }
}

void BitTorrentHttpClient::CloseAndReInit ()
{
  if (m_requestActive)
    {
      m_socket->Close ();
    }

  m_requestActive = false;
  m_dataStarted = false;
  m_badHeader = false;

  m_buffer.clear ();
  m_bytesToRead = -1;
  m_dataStart = 0;
}

std::string BitTorrentHttpClient::HttpReceiveReply (Ptr<Socket> socket, bool waitUntilFinished, bool dataOnly, size_t& dataStart, bool& finished)
{
  // Step 1: Indicate that we have not yet finished processing the whole answer
  finished = false;

  // Step 2: Read from the incoming stream as long as possible
  uint8_t* buf = new uint8_t[1024];
  size_t bufLen;
  while (socket->GetRxAvailable () > 0)
    {
      bufLen = socket->Recv (buf, 1024, 0);
      m_buffer.append (std::string (reinterpret_cast<const char*> (buf), bufLen));
    }
  delete[] buf;

  // Step 3: Search for the end of the HTTP header, indicated by a double line break/carriage return
  if (!m_dataStarted)
    {
      dataStart = m_buffer.find ("\r\n\r\n");

      if (dataStart != std::string::npos)
        {
          dataStart += 4;
          m_dataStart = dataStart;
          m_dataStarted = true;
        }
    }

  // Step 4: If the data part of the answer has started to arrive, write it into the respective buffer
  if (m_dataStarted)
    {
      // Step 4a: Try analyzing the header for necessary data
      if (m_bytesToRead == -1)
        {
          size_t checkPos = m_buffer.find ("Content-Length: ");
          if (checkPos == std::string::npos)
            {
              NS_LOG_DEBUG ("BitTorrentHttpClient: Bad Header: No Content-Length field found.");

              m_badHeader = true;
              return "Bad header";
            }
          else
            {
              size_t checkPos2 = m_buffer.find ("\r\n", checkPos);
              if (checkPos2 == std::string::npos || checkPos2 >= m_dataStart)
                {
                  NS_LOG_DEBUG ("BitTorrentHttpClient: Bad Header: No return after Content-Length field found.");

                  m_badHeader = true;
                  return "Bad header";
                }
              else
                {
                  std::istringstream converter;
                  converter.str (m_buffer.substr (checkPos + 16, checkPos2 - checkPos - 16));
                  converter.clear ();
                  converter >> m_bytesToRead;

                  if (!converter)
                    {
                      NS_LOG_DEBUG ("BitTorrentHttpClient: Content-Length could not be converted.");

                      m_badHeader = true;
                      return "Bad content length";
                    }
                  else
                    {
                      if (m_bytesToRead == 0)
                        {
                          dataStart = std::string::npos;
                          finished = true;
                          return "";
                        }
                    }
                }
            }
        }

      // Step 4b: After handling the header, copy the data part of the answer into the respective buffer
      NS_LOG_DEBUG ("BitTorrentHttpClientConnector: Should read " << m_bytesToRead << " starting at " << m_dataStart << "; have: " << m_buffer.size () << ".");

      finished = m_buffer.size () >= (m_bytesToRead + m_dataStart);
      if (finished|| !waitUntilFinished)
        {
          if (finished)
            {
              NS_LOG_DEBUG ("BitTorrentHttpClientConnector: Transfer finished. Bytes to read: " << m_bytesToRead << "; buffer size: " << m_buffer.size () << ".");
            }

          if (dataOnly)
            {
              std::string result = m_buffer.substr (m_dataStart, m_buffer.size () - m_dataStart);
              m_buffer.clear ();
              return result;
            }
          else
            {
              std::string result = m_buffer;
              m_buffer.clear ();
              return result;
            }
        }
      else
        {
          return "";
        }
    }
  else // if(m_dataStarted)
    {
      return "";
    }
}

} // ns bittorrent
} // ns ns3
