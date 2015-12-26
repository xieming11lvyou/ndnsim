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

#ifndef BITTORRENTHTTPCLIENT_H_
#define BITTORRENTHTTPCLIENT_H_

#include "ns3/address.h"
#include "ns3/ptr.h"
#include "ns3/socket.h"

#include <string>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Implements a simplistic HTTP client.
 *
 * This class implements a very simplistic HTTP client to implement HTTP-based applications.
 * Currently, the client only supports HTTP GET requests with standard settings.
 */
class BitTorrentHttpClient : public Object
{
// Fields
private:
  Ptr<Socket> m_socket;      // The TCP socket used for communication
  Ipv4Address m_remoteIp;    // The IP address of the server

  bool m_requestActive;      // Needed to determine if another request can be started
  bool m_dataStarted;        // Set to true after header has been received
  bool m_badHeader;          // Set to true if header cannot be parsed

  std::string m_buffer;      // Holds the reply
  int64_t m_bytesToRead;     // Number of bytes to read until data ends (needed in case socket is not closed)
  size_t m_dataStart;        // Denotes where the received data starts

// Constructors etc.
public:
  BitTorrentHttpClient ();
  virtual ~BitTorrentHttpClient ();

// Getters, setters
public:
  /**
   * \brief Get the state of the HTTP client.
   *
   * @returns true, if a previously-issued request is still pending.
   */
  bool GetRequestActive () const;

  /**
   * \brief Get the state of the received header.
   *
   * @returns true, if the received header was considered erroneous and could not be used to decode the data section of the reply.
   */
  bool HasBadHeader () const;

// Interaction methods
public:
  /**
   * \brief Close the contained socket and re-initialize the object for another request.
   *
   * After a call to this method, it is safe to again call the HttpGetRequest method to send a further request.
   */
  void CloseAndReInit ();

  /**
   * \brief Send out a HTTP GET request to a remote node.
   *
   * This method creates a HTTP request and tries to send it to a remote node using the built-in socket of the HTTP client.
   *
   * @param node the node that the socket to be used for this requestion shall be associated with
   * @param typeidvar the type of the socket to create.
   * @param addr the address of the remote node to send the request to.
   * @param port the port of the remote node to send the request to.
   * @param callback the callback function to be calld once a reply is received.
   */
  bool HttpGetRequest (Ptr<Node> node, TypeId typeidvar, Ipv4Address addr, uint16_t port, std::string path, Callback<void, Ptr<Socket> > callback);

// Callback handlers
public:
  /**
   * \brief React to incoming HTTP responses.
   *
   * This method is used to react to incoming HTTP responses. It analyzes the received input stream for any valid HTTP response contained within
   * and returns the extracted payload of the reponse.
   *
   * Use the HasBadHeader member function to determine whether some error in the decoding of the header occurred. In this case, this method may
   * additionally return "Bad error" or "Bad content length".
   *
   * @param socket the socket used to receive the HTTP response.
   * @param waitUntilFinished if set to true, the method only returns a non-empty string when the HTTP response was received completely.
   * @param payloadOnly if set to true, the header part of the response is stripped from the returned string.
   * @param dataStart address of a variable to store the starting position of the payload within the returned string. Set to 0 if payloadOnly is set to true.
   * @param finished address of a variable to store whether the reception of the response has finished.
   */
  std::string HttpReceiveReply (Ptr<Socket> socket, bool waitUntilFinished, bool dataOnly, size_t& dataStart, bool& finished);
};

} // ns bittorrent
} // ns ns3

#endif // BITTORRENTHTTPCLIENT_H_
