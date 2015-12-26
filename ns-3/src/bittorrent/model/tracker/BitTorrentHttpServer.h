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

#ifndef BITTORRENTHTTPSERVER_H_
#define BITTORRENTHTTPSERVER_H_

#include "ns3/address.h"
#include "ns3/ptr.h"
#include "ns3/socket.h"
#include "ns3/object.h"

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Implements a simplistic HTTP Webserver.
 *
 * This class implements a very simplistic HTTP server for use with HTTP-based applications.
 * Currently, the server only supports HTTP GET requests with standard settings.
 */
class BitTorrentHttpServer : public Object
{
// Fields
public:
  Ptr<Socket> m_socket;

// Constructors etc.
public:
  BitTorrentHttpServer ();
  virtual ~BitTorrentHttpServer ();
private:
  BitTorrentHttpServer (const BitTorrentHttpServer&);
  BitTorrentHttpServer& operator = (const BitTorrentHttpServer);

// Main interaction methods
public:
  /**
   * \brief Create and open a listening socket on port 80 on the node the server is installed on.
   *
   * @param node a reference to the node the port and socket should be associated with.
   * @param typeidvar the type of the socket to use.
   * @param handleConnectionCreation the callback handler to be triggered when an incoming connection attempt is detected.
   */
  void StartListening (Ptr<Node> node, TypeId typeidvar, Callback<void, Ptr<Socket>, const Address&> handleConnectionCreation);

  /**
   * \brief Close the socket opened via StartListening, effectively causing the server to stop accepting incoming connections.
   */
  void StopListening (void);

  /**
   * \brief A callback stub handler for accepted connections. Sets the appropriate internal handlers for reception and connection closure.
   *
   * @param socket the socket that the callback function shall be used for.
   * @param addr the address of the initiator of the connection.
   * @param handleReceiveRequest the callback handler to be triggered when data is received via the connection.
   */
  void ConnectionCreated (Ptr<Socket> socket, const Address &addr, Callback<void, Ptr<Socket> > handleReceiveRequest);

  /**
   * \brief A callback default handler for data reception on accepted connections.
   *
   * Extracts the request data from the HTTP request and calls the
   * supplied data generation method for the generation of a response. Used by the standard implementation of the ConnectionCreated method.
   *
   * @param socket the socket that the callback function shall be used for.
   * @param handleDataCreater the callback handler to be triggered to generate an answer to the request received.
   * @param handleErrorOccurance the callback handler to be triggered to generate an anwer to the request in case of an error in request parsing.
   */
  void ReceiveRequest (Ptr<Socket> socket, Callback<void, std::string, Ptr<Socket>, const Address& > handleDataCreater, Callback<void, Ptr<Socket>, std::string, const Address& > handleErrorOccurance);

  /**
   * \brief Generate an HTTP-formated answer string and send it out via a socket.
   *
   * @param socket the socket the function shall be used for.
   * @param dataToSend pointer to a block of data that contains the answer.
   * @param ContentLength the length of the data block (in characters) to send.
   * @param code the HTTP return code to use. Currently supported are "200" for success, "400" for a bad request by the client and "404" for not found resource.
   */
  void SendData (Ptr<Socket> socket, uint8_t* dataToSend, uint32_t ContentLength, std::string code);

private:
  // tries sending out data on the socket a number of times
  void TrySending(Ptr<Socket> socket, std::string toSend, uint32_t retries);

  // determines whether a connection is accepted. Returns true for all connections
  bool ConnectionRequest (Ptr<Socket> socket, const Address &addr);

  // simply send the [blocks] of [dataToSend]
  void SimpleSend (Ptr<Socket> socket, uint8_t* dataToSend, uint32_t blocks, uint32_t ContentLength);

  // Invoked when peer closes the connection gracefully. Sets m_close to true.
  void NormalClose (Ptr<Socket> socket);

  // Invoked when connection is closed abnormally. Sets m_close to true.
  void ErrorClose (Ptr<Socket> socket);
};

} // ns bittorrent
} // ns ns3

#endif // BITTORRENTHTTPSERVER_H_

