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
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "request-tag.h"

namespace ns3 {

//NS_LOG_COMPONENT_DEFINE ("UdpEchoClient1024Application");
NS_OBJECT_ENSURE_REGISTERED (UdpEchoClient1024);

TypeId
UdpEchoClient1024::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpEchoClient1024")
    .SetParent<Application> ()
    .AddConstructor<UdpEchoClient1024> ()
    .AddAttribute ("MaxPackets", 
                   "The maximum number of packets the application will send",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpEchoClient1024::m_count),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Number",
                   "Packet need receive when a packet send",
                    UintegerValue (100),
                    MakeUintegerAccessor (&UdpEchoClient1024::m_num),
                    MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Interval", 
                   "The time to wait between packets",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&UdpEchoClient1024::m_interval),
                   MakeTimeChecker ())
    .AddAttribute ("RemoteAddress", 
                   "The destination Address of the outbound packets",
                   AddressValue (),
                   MakeAddressAccessor (&UdpEchoClient1024::m_peerAddress),
                   MakeAddressChecker ())
    .AddAttribute ("RemotePort", 
                   "The destination port of the outbound packets",
                   UintegerValue (0),
                   MakeUintegerAccessor (&UdpEchoClient1024::m_peerPort),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("PacketSize", "Size of echo data in outbound packets",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpEchoClient1024::SetDataSize,
                                         &UdpEchoClient1024::GetDataSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddTraceSource ("Tx", "A new packet is created and is sent",
                     MakeTraceSourceAccessor (&UdpEchoClient1024::m_txTrace))
  ;
  return tid;
}

UdpEchoClient1024::UdpEchoClient1024 ()
{
//  NS_LOG_FUNCTION (this);
  m_sent = 0;
  m_receive = 0;
  m_socket = 0;
  m_sendEvent = EventId ();
  m_sendEvent1 = EventId ();
  m_data = 0;
  m_dataSize = 0;
  m_firstTypeRepuest = true;
  flagChoose = false;
  percentage = 0;
  m_totalReceive =0;
}

UdpEchoClient1024::~UdpEchoClient1024()
{
  //NS_LOG_FUNCTION (this);
  m_socket = 0;

  delete [] m_data;
  m_data = 0;
  m_dataSize = 0;
}

void 
UdpEchoClient1024::SetRemote (Address ip, uint16_t port)
{
  //NS_LOG_FUNCTION (this << ip << port);
  m_peerAddress = ip;
  m_peerPort = port;
}

void 
UdpEchoClient1024::SetRemote (Ipv4Address ip, uint16_t port)
{
  //NS_LOG_FUNCTION (this << ip << port);
  m_peerAddress = Address (ip);
  m_peerPort = port;
}

void 
UdpEchoClient1024::SetRemote (Ipv6Address ip, uint16_t port)
{
//  NS_LOG_FUNCTION (this << ip << port);
  m_peerAddress = Address (ip);
  m_peerPort = port;
}

void
UdpEchoClient1024::DoDispose (void)
{
  //NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
UdpEchoClient1024::StartApplication (void)
{
 // NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind();
          m_socket->SetAllowBroadcast (true);
          m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
      else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind6();
          m_socket->Connect (Inet6SocketAddress (Ipv6Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
    }
  m_socket->SetRecvCallback (MakeCallback (&UdpEchoClient1024::HandleRead, this));

  ScheduleTransmit (Seconds (0.));
}

void 
UdpEchoClient1024::StopApplication ()
{
  //NS_LOG_FUNCTION (this);

  if (m_socket != 0) 
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
      m_socket = 0;
    }

  Simulator::Cancel (m_sendEvent);
}

void 
UdpEchoClient1024::SetDataSize (uint32_t dataSize)
{
  //NS_LOG_FUNCTION (this << dataSize);

  //
  // If the client is setting the echo packet data size this way, we infer
  // that she doesn't care about the contents of the packet at all, so 
  // neither will we.
  //
  delete [] m_data;
  m_data = 0;
  m_dataSize = 0;
  m_size = dataSize;
}

uint32_t 
UdpEchoClient1024::GetDataSize (void) const
{
 // NS_LOG_FUNCTION (this);
  return m_size;
}

void 
UdpEchoClient1024::SetFill (std::string fill)
{
  //NS_LOG_FUNCTION (this << fill);

  uint32_t dataSize = fill.size () + 1;

  if (dataSize != m_dataSize)
    {
      delete [] m_data;
      m_data = new uint8_t [dataSize];
      m_dataSize = dataSize;
    }

  memcpy (m_data, fill.c_str (), dataSize);

  //
  // Overwrite packet size attribute.
  //
  m_size = dataSize;
}

void 
UdpEchoClient1024::SetFill (uint8_t fill, uint32_t dataSize)
{
  //NS_LOG_FUNCTION (this << fill << dataSize);
  if (dataSize != m_dataSize)
    {
      delete [] m_data;
      m_data = new uint8_t [dataSize];
      m_dataSize = dataSize;
    }

  memset (m_data, fill, dataSize);

  //
  // Overwrite packet size attribute.
  //
  m_size = dataSize;
}

void 
UdpEchoClient1024::SetFill (uint8_t *fill, uint32_t fillSize, uint32_t dataSize)
{
  //NS_LOG_FUNCTION (this << fill << fillSize << dataSize);
  if (dataSize != m_dataSize)
    {
      delete [] m_data;
      m_data = new uint8_t [dataSize];
      m_dataSize = dataSize;
    }

  if (fillSize >= dataSize)
    {
      memcpy (m_data, fill, dataSize);
      m_size = dataSize;
      return;
    }

  //
  // Do all but the final fill.
  //
  uint32_t filled = 0;
  while (filled + fillSize < dataSize)
    {
      memcpy (&m_data[filled], fill, fillSize);
      filled += fillSize;
    }

  //
  // Last fill may be partial
  //
  memcpy (&m_data[filled], fill, dataSize - filled);

  //
  // Overwrite packet size attribute.
  //
  m_size = dataSize;
}

void 
UdpEchoClient1024::ScheduleTransmit (Time dt)
{
 // NS_LOG_FUNCTION (this << dt);
  m_sendEvent = Simulator::Schedule (dt, &UdpEchoClient1024::Send, this,false);
}

void 
UdpEchoClient1024::Send (bool retranFlag)
{
 // int type;
  //Retransmit init
  if (retranFlag)
    {
    flagChoose = false;
    m_firstTypeRepuest = true;
    m_receive = 0;
//    std::cout<<"retransmit"<<std::endl;
    m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), m_peerPort));
    }

///////////////////  add header
  RequestHeader requestTpye;
  if (m_firstTypeRepuest)
    {
//    type = 0;
    requestTpye.SetSeq(uint32_t(0));
    }
  else
    {
//    type = 1;
    requestTpye.SetSeq(uint32_t(1));
    }
  //////////////////////


////////////////////////Create Packet
  Ptr<Packet> p;
  if (m_dataSize)
    {
      p = Create<Packet> (m_data, m_dataSize);
    }
  else
    {
      p = Create<Packet> (m_size);
    }

  //////////////////////////////////send num output
//    Ptr<Node> node = this->GetNode();
//    uint32_t i = node->GetId();

    m_txTrace (p);
    p->AddHeader(requestTpye);
    m_socket->Send (p);
    /////////////////////////////////////////output/////////////////////////////////////////////////////////////////
//    std::cout<< Simulator::Now ().GetSeconds () <<"\t"<<i<<"\t"<<"client_send " << m_size << "\t" <<
//                     Ipv4Address::ConvertFrom (m_peerAddress) << "\t" << m_peerPort<<"\t"<<"\t"<<" Type "<<type<<std::endl;


  ++m_sent;


//  std::cout<<i<<"\t"<<"client_send"<<"\t"<<Simulator::Now().GetSeconds()<<"\t"<<m_sent<<std::endl;


///////////////////////////////////retransmit///////////////////////////////////
  if (m_sendEvent1.IsRunning())
    {
    Simulator::Cancel (m_sendEvent1);
//    std::cout<<"Cancel Schedule2"<<std::endl;
    }
  if (m_receive < m_num)
    {
//    std::cout<<"Built2"<<std::endl;
    m_sendEvent1  = Simulator::Schedule (Seconds(0.1), &UdpEchoClient1024::Send,this,true);
    }
//////////////////////////////////////////////////////////////////////////////////
  if (m_sent < m_count) 
    {
      ScheduleTransmit (m_interval);
    }
}

void
UdpEchoClient1024::HandleRead (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {


//    Ptr<Node> node = this->GetNode();
//    uint32_t i = node->GetId();
     RequestHeader requestTpye;
     packet->RemoveHeader(requestTpye);
     uint32_t type  = requestTpye.GetSeq();
     if (type == 1 )
       {
       ipNewComing =  InetSocketAddress::ConvertFrom (from).GetIpv4 ();
//       std::cout<<"NEW  "<<ipNewComing<<std::endl;
       if ( ipNewComing == ipServering)
         {
         ++ m_receive;
         ++m_totalReceive;

         ///////////////////////////////output/////////////////////////////////////////////////////////////
//              std::cout<< Simulator::Now ().GetSeconds () <<"\t"<<i<<"\t"<<"client_received " << packet->GetSize () << "\t" <<
//                              InetSocketAddress::ConvertFrom (from).GetIpv4 () << "\t" <<
//                              InetSocketAddress::ConvertFrom (from).GetPort ()<<"\t"<<m_receive<<"\t"<<"Type "<<type<<std::endl;
         /////////////////////////////////////////////gnuplot//////////////////////////////////////
         percentage = double(m_receive)/double(m_num);
         if (percentage>1.0)
           {
           percentage = 1.0;
           }
         std::cout<<Simulator::Now ().GetSeconds ()<<"\t"<<percentage*100<<"\t"<<m_sent<<"\t"<<m_totalReceive<<std::endl;
         if (m_sendEvent1.IsRunning())
           {
           Simulator::Cancel (m_sendEvent1);
    //       std::cout<<"Cancel Schedule1"<<std::endl;
           }
         if (m_receive < m_num)
           {

           m_sendEvent1 = Simulator::Schedule (Seconds(0.1), &UdpEchoClient1024::Send,this,true);
           }
         }
       }


     if(type == 0&&!flagChoose)
       {
       ipServering =  InetSocketAddress::ConvertFrom (from).GetIpv4 ();
//       std::cout<<"Server  "<<ipServering<<std::endl;

       ///////////////////////////////output/////////////////////////////////////////////////////////////
//            std::cout<< Simulator::Now ().GetSeconds () <<"\t"<<i<<"\t"<<"client_received " << packet->GetSize () << "\t" <<
//                            InetSocketAddress::ConvertFrom (from).GetIpv4 () << "\t" <<
//                            InetSocketAddress::ConvertFrom (from).GetPort ()<<"\t"<<m_receive<<"\t"<<"Type "<<type<<std::endl;
       /////////////////////////////////////////////gnuplot//////////////////////////////////////
       flagChoose = true;
       m_firstTypeRepuest = false;
       m_socket->Connect (InetSocketAddress (InetSocketAddress::ConvertFrom (from).GetIpv4 (), m_peerPort));
       Send(false);
       if (m_sendEvent1.IsRunning())
         {
         Simulator::Cancel (m_sendEvent1);
  //       std::cout<<"Cancel Schedule1"<<std::endl;
         }
       if (m_receive < m_num)
         {
  //       std::cout<<"Built1"<<std::endl;
         m_sendEvent1 = Simulator::Schedule (Seconds(0.1), &UdpEchoClient1024::Send,this,true);
         }
       }




///////////////////////////retransmit////////////////////////////////////////////////////////




///////////////////////////receive num output///////////////////////////////////
//     std::cout<<i<<"\t"<<"client_receive"<<"\t"<<Simulator::Now().GetSeconds()<<"\t"<<m_receive<<"\t"<<"Type"<<"\t"<<type<<std::endl;
 ///////////////////////////////////////////////////////////////////////////////////////
}
}

} // Namespace ns3
