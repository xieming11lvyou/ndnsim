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
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "request-tag.h"

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"

namespace ns3 {

  NS_LOG_COMPONENT_DEFINE ("UdpEchoServer1024Application");
  NS_OBJECT_ENSURE_REGISTERED (UdpEchoServer1024);
TypeId
UdpEchoServer1024::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpEchoServer1024")
    .SetParent<Application> ()
    .AddConstructor<UdpEchoServer1024> ()
    .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                   UintegerValue (9),
                   MakeUintegerAccessor (&UdpEchoServer1024::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("Number", "Num of Packet need send when receive a packet from client.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&UdpEchoServer1024::m_num),
                   MakeUintegerChecker<uint16_t> ())
  ;
  return tid;
}

UdpEchoServer1024::UdpEchoServer1024 ()
{
  NS_LOG_FUNCTION (this);
  index=0;
  m_receive = 0;
  flagChoose = false;
}

UdpEchoServer1024::~UdpEchoServer1024()
{
  NS_LOG_FUNCTION (this);
  m_socket = 0;
  m_socket6 = 0;
}

void
UdpEchoServer1024::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
UdpEchoServer1024::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      m_socket->Bind (local);
      if (addressUtils::IsMulticast (m_local))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, m_local);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local6 = Inet6SocketAddress (Ipv6Address::GetAny (), m_port);
      m_socket6->Bind (local6);
      if (addressUtils::IsMulticast (local6))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket6);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, local6);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  m_socket->SetRecvCallback (MakeCallback (&UdpEchoServer1024::HandleRead, this));
  m_socket6->SetRecvCallback (MakeCallback (&UdpEchoServer1024::HandleRead, this));
}

void
UdpEchoServer1024::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_socket != 0)
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
  if (m_socket6 != 0)
    {
      m_socket6->Close ();
      m_socket6->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void
UdpEchoServer1024::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  m_receive++;
//  Ptr<Node> node = this->GetNode();
//  uint32_t i = node->GetId();

//  std::cout<<i<<"\t"<<"server_receive"<<"\t"<<Simulator::Now().GetSeconds()<<"\t"<<m_receive<<std::endl;
  index = 0;

  while ((packet = socket->RecvFrom (from)))
    {
//    RequestHeader requestTpye;
//    packet->PeekHeader(requestTpye);
//    uint32_t type = requestTpye.GetSeq();
    if (m_sendEvent.IsRunning())
      {
      Simulator::Cancel (m_sendEvent);
      }
    if (InetSocketAddress::IsMatchingType (from))
            {
////////////////////////////////////////////////////////////////output/////////////////////////////////////////////////////////
//              std::cout<< Simulator::Now ().GetSeconds () <<"\t"<<i<<"\t"<<"server_received " << packet->GetSize () << "\t" <<
//                           InetSocketAddress::ConvertFrom (from).GetIpv4 () << "\t" <<
//                           InetSocketAddress::ConvertFrom (from).GetPort ()<<"\t"<<"\t"<<" Type "<<type<<std::endl;
            }
          else if (Inet6SocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server received " << packet->GetSize () << " bytes from " <<
                           Inet6SocketAddress::ConvertFrom (from).GetIpv6 () << " port " <<
                           Inet6SocketAddress::ConvertFrom (from).GetPort ());
            }


    packet->RemoveAllPacketTags ();
    packet->RemoveAllByteTags ();
    Send(packet,0,from,socket);
    }
}

void
UdpEchoServer1024::Send (Ptr<Packet> p, uint32_t flags,  const Address &toAddress,Ptr<Socket> socket)
{

//        Time nextTime (Seconds (p->GetSize () *8/
//            static_cast<double>( 3000000))); // Time till next packet

        //after receive ,then send num
//        Ptr<Node> node = this->GetNode();
//        uint32_t j = node->GetId();
        RequestHeader requestTpye;
        p->PeekHeader(requestTpye);
        uint32_t type = requestTpye.GetSeq();

 //       NS_LOG_INFO(index);
        NS_LOG_LOGIC ("Echoing packet");

         socket->SendTo (p, flags, toAddress);

        if (InetSocketAddress::IsMatchingType (toAddress))

          {
          /////////////////////////////////////////////output////////////////////////////////////////////////////////
//           std::cout<< Simulator::Now ().GetSeconds () <<"\t"<<j<<"\t"<< "server_sent "<<p->GetSize ()<<"\t" <<
//                         InetSocketAddress::ConvertFrom (toAddress).GetIpv4 () << "\t"<<
//                         InetSocketAddress::ConvertFrom (toAddress).GetPort ()<<"\t";
          }
        else if (Inet6SocketAddress::IsMatchingType (toAddress))
          {
            NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server sent " << p->GetSize () << " bytes to " <<
                         Inet6SocketAddress::ConvertFrom (toAddress).GetIpv6 () << " port " <<
                         Inet6SocketAddress::ConvertFrom (toAddress).GetPort ());
          }

        if (type == 1)
          {
          index++;

              if (index < m_num)
                {
                if (m_sendEvent.IsRunning())
                  {
                  Simulator::Cancel (m_sendEvent);
                  }
                m_sendEvent = Simulator::Schedule (Seconds(0.01), &UdpEchoServer1024::Send,this,p,0,toAddress,socket);
                 }
          }
//        std::cout<<index<<"\t"<<" Type "<<type<<std::endl;

}

} // Namespace ns3
