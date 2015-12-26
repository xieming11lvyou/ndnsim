#ifndef A_NDN_SIMPLE_TEST_H
#define A_NDN_SIMPLE_TEST_H

#include "math.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"

#include "ns3/ndnSIM-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"

#include "ns3/ndn-net-device-face.h"
#include "ns3/ndn-app-face.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"
#include "iostream"
#include "ns3/ndn-header-helper.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/random-variable.h"
#include "ns3/traced-callback.h"

#include "ns3/net-device.h"
#include "ns3/ndn-net-device-face.h"
#include "ns3/ndn-header-helper.h"

#include "a-ndn-tag.h"

using namespace std;
using namespace ns3;
using namespace ndn;


class MyNetDeviceFace : public NetDeviceFace{
public:
  static TypeId
  GetTypeId ();


  MyNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice);
  virtual ~MyNetDeviceFace();
  virtual void RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler);
protected:
  virtual bool
  Send (Ptr<Packet> p);
private:
  void ReceiveFromNetDevice (Ptr<NetDevice> device,
                           Ptr<const Packet> p,
                           uint16_t protocol,
                           const Address &from,
                           const Address &to,
                           NetDevice::PacketType packetType);

// private:
//   Ptr<NetDevice> m_netDevice; ///< \brief Smart pointer to NetDevice
  double GetDistance (double sourcePosition);
  double m_number;
  double s_number;
};


TypeId
MyNetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::MyNetDeviceFace")
    .SetParent<NetDeviceFace> ();
  return tid;
}

MyNetDeviceFace::MyNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : NetDeviceFace (node, netDevice),m_number(0),s_number(0){};


MyNetDeviceFace::~MyNetDeviceFace ()
{
  //NS_LOG_FUNCTION_NOARGS ();
}

bool
MyNetDeviceFace::Send (Ptr<Packet> packet)
{
  if (!Face::Send (packet))
    {
      return false;
    }
  
  //NS_LOG_FUNCTION (this << packet);

  //NS_ASSERT_MSG (packet->GetSize () <= m_netDevice->GetMtu (),
                 // "Packet size " << packet->GetSize () << " exceeds device MTU "
                 // << m_netDevice->GetMtu ()
                 // << " for Ndn; fragmentation not supported");
  // Ptr<Packet> p = packet->Copy ();
  // HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (p);
  try
  {
    HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
    //cout<<"SEND ";
    switch (type)
      {
      case HeaderHelper::INTEREST_NDNSIM:
        cout<<"INTEREST_NDNSIM"<<endl;
        break;
      case HeaderHelper::INTEREST_CCNB:
        cout<<"INTEREST_CCNB"<<endl;
        break;
        //return ReceiveInterest (Wire::ToInterest (packet, Wire::WIRE_FORMAT_CCNB));
      case HeaderHelper::CONTENT_OBJECT_NDNSIM:
        // cout<<"DATA_NDNSIM"<<endl;
        // s_number++;
        // cout<<"Send " <<s_number<<endl;
        break;
        //return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_NDNSIM));
      case HeaderHelper::CONTENT_OBJECT_CCNB:
        //return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_CCNB));
        cout<<"DATA_CCNB"<<endl;
        break;
        //return ReceiveData (Wire::ToData (pack
      default:
        //NS_FATAL_ERROR ("Not supported NDN header");
        return false;
        //cout<<"fuck";
      }

      // exception will be thrown if packet is not recognized
    }
  catch (UnknownHeaderException)
    {
      //NS_FATAL_ERROR ("Unknown NDN header. Should not happen");
      return false;
    }


  Ptr<Node> node= this->GetNode();
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  
  // cout<<"At: "<<Simulator::Now()<<" node:"<<node->GetId()
  //     <<" location:"<<(mobility->GetPosition()).x
  //     <<" velocity:"<<(mobility->GetVelocity()).x<<endl;
  
  MobilityTag mobilityTag;
  mobilityTag.SetMobility((mobility->GetPosition()).x);
  packet->AddPacketTag (mobilityTag);

  VelocityTag velocityTag;
  velocityTag.SetVelocity((mobility->GetVelocity()).x);
  packet->AddPacketTag (velocityTag);


  // cout<<endl;
  // cout<<"---------------------------------"<<endl;
  // packet->PrintPacketTags(cout);
  // cout<<endl;
  // //
  return NetDeviceFace::Send (packet);

}
void
MyNetDeviceFace::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler)
{
   //NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandlers (interestHandler, dataHandler);

  m_node->RegisterProtocolHandler (MakeCallback (&MyNetDeviceFace::ReceiveFromNetDevice, this),
                                   L3Protocol::ETHERNET_FRAME_TYPE, GetNetDevice (), true/*promiscuous mode*/);
}


void
MyNetDeviceFace::ReceiveFromNetDevice (Ptr<NetDevice> device,
                           Ptr<const Packet> p,
                           uint16_t protocol,
                           const Address &from,
                           const Address &to,
                           NetDevice::PacketType packetType)
{ 

  double distance;
  //p ->PeekPacketTag
  //cout<<"fuck"<<endl;
  Ptr<Packet> packet = p->Copy();
  Ptr<Node> node = this->GetNode();
  uint32_t i = node->GetId();


    HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
    //cout<<"RECEIVED ";
    switch (type)
      {
      case HeaderHelper::INTEREST_NDNSIM:
        //cout<<"INTEREST_NDNSIM"<<endl;
        
        break;
      case HeaderHelper::INTEREST_CCNB:
        //cout<<"INTEREST_CCNB"<<endl;
        break;
        //return ReceiveInterest (Wire::ToInterest (packet, Wire::WIRE_FORMAT_CCNB));
      case HeaderHelper::CONTENT_OBJECT_NDNSIM:
        //cout<<"DATA_NDNSIM"<<endl;
        m_number++;
        if (i==0)
        {
           cout<<"Received " <<m_number<<endl;
        }
        break;
        //return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_NDNSIM));
      case HeaderHelper::CONTENT_OBJECT_CCNB:
        //return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_CCNB));
        //cout<<"DATA_CCNB"<<endl;
        break;
        //return ReceiveData (Wire::ToData (pack
      default:
        //NS_FATAL_ERROR ("Not supported NDN header");
        cout<<"false";
        //cout<<"fuck";
      }

      // exception will be thrown if packet is not recognized
    //}
  // catch (UnknownHeaderException)
  //   {
  //     //NS_FATAL_ERROR ("Unknown NDN header. Should not happen");
  //     cout<<"false";
  //   }

  VelocityTag velocityTag;
  p->PeekPacketTag(velocityTag);

  MobilityTag mobilityTag;
  p->PeekPacketTag(mobilityTag);
  //cout<<"RECEIVED"<<endl;
  
  //cout<<"At: "<<Simulator::Now()<<" from node:"<<node->GetId()<<" location:"<<mobilityTag.GetMobility()<<endl;
  
  // cout<<"At: "<<Simulator::Now()<<" node:"<<node->GetId()
  //     <<" location:"<<mobilityTag.GetMobility()
  //     <<" velocity:"<<velocityTag.GetVelocity()<<endl;

  distance = GetDistance(mobilityTag.GetMobility());

  //cout<<endl;
  //cout<<"---------------------------------"<<endl;

  //cout<<"---------------------------------"<<endl;

//---------------

  bool verbose = 1;
  if(verbose)
  {  
    Receive(p);
  }
  else
  {
    if (type==HeaderHelper::INTEREST_NDNSIM&&distance<=50)
    {
      /* code */
      Receive (p);
    }
    if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM&&distance<=50)
    {
      /* code */
      Receive (p);
    }
  }

}

double
MyNetDeviceFace::GetDistance(double sourcePosition)
{
  Ptr<Node> node= this->GetNode();
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  double DestPosition = mobility->GetPosition().x;
  return abs(DestPosition-sourcePosition);
}


#endif
