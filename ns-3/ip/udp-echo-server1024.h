/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
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
 */

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"

namespace ns3 {
  using namespace ns3;
//class Socket;
//class Packet;

/**
 * \ingroup applications 
 * \defgroup udpecho UdpEcho
 */

/**
 * \ingroup udpecho
 * \brief A Udp Echo server
 *
 * Every packet received is sent back.
 */
class UdpEchoServer1024 : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  UdpEchoServer1024 ();
  virtual ~UdpEchoServer1024 ();

protected:
  virtual void DoDispose (void);

private:
  /**
   * \brief Send a packet
   */
  void Send (Ptr<Packet> p, uint32_t flags,  const Address &toAddress,Ptr<Socket> socket);
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  /**
   * \brief Handle a packet reception.
   *
   * This function is called by lower layers.
   *
   * \param socket the socket the packet was received to.
   */
  void HandleRead (Ptr<Socket> socket);


  Ptr<Socket> m_socket; //!< IPv4 Socket
  Ptr<Socket> m_socket6; //!< IPv6 Socket
  Address m_local; //!< local multicast address
  uint16_t m_port; //!< Port on which we listen for incoming packets.
  uint16_t m_num;
  uint16_t index;
  uint16_t m_receive;
  bool flagChoose;
  EventId m_sendEvent; //!< Event to send the next packet

};


} // namespace ns3


