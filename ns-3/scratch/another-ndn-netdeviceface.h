#ifndef ANOTHER_NDN_NETDEVICEFACE_H
#define ANOTHER_NDN_NETDEVICEFACE_H

#include "math.h"

#include "ns3/ndn-net-device-face.h"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

#include "a-ndn-tag.h"

using namespace std;


namespace ns3{
namespace ndn{


class NdnNetDeviceFace : public NetDeviceFace{
public:
  static TypeId
  GetTypeId ();


  NdnNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice);
  virtual ~NdnNetDeviceFace();
  virtual void RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler);
  bool Receive (Ptr<const Packet> p);
  void DoSend (Ptr<Packet> p);
  void DoSendData (Ptr<Packet> packet);
  void DoSendInterest (Ptr<Packet> packet); 

protected:
  virtual bool Send (Ptr<Packet> p);
private:
  void ReceiveFromNetDevice (Ptr<NetDevice> device,
                           Ptr<const Packet> p,
                           uint16_t protocol,
                           const Address &from,
                           const Address &to,
                           NetDevice::PacketType packetType);

  double GetDistance (double sourcePosition);
  bool GetProbability (double distance);
  double interestNum;
  double m_number;

 

  
  double s_number;
  bool receiveInterestFlag;
  bool receiveDataFlag;

  bool reSendInterestFlag;
  bool reSendDataFlag;

  EventId sendInterestEvent;
  EventId sendDataEvent;

  EventId reSendInterestEvent;
  EventId reSendDataEvent;
  double waitTime;
  uint32_t reSendTimes;
  uint32_t tempTimes;

  bool print;

  uint32_t consumerId;
  uint32_t producerId;
private:
  Ptr<NetDevice> m_netDevice; 
};

NS_OBJECT_ENSURE_REGISTERED (NdnNetDeviceFace);
TypeId
NdnNetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::NdnNetDeviceFace")
    .SetParent<NetDeviceFace> ();
  return tid;
}

NdnNetDeviceFace::NdnNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : NetDeviceFace (node, netDevice),interestNum(0),m_number(0),s_number(0)
  ,receiveInterestFlag(false),receiveDataFlag(false),reSendInterestFlag(true),reSendDataFlag(true),waitTime(0.005)
  ,reSendTimes(0),tempTimes(0),print(0),m_netDevice(netDevice)
  {
    consumerId = 48;
    producerId = 49;
  }


NdnNetDeviceFace::~NdnNetDeviceFace ()
{
  //NS_LOG_FUNCTION_NOARGS ();
}

bool
NdnNetDeviceFace::Send (Ptr<Packet> packet)
{ 
  //Ptr<Node> node = this->GetNode();

  if (!Face::Send (packet))
    {
      return false;
    }

  uint32_t i = m_node->GetId();
  //cout<<"NODE "<<i<<" TRY SEND";
  HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
  // try
  // {
    
  //   //cout<<"SEND ";
    switch (type)
      {
      case HeaderHelper::INTEREST_NDNSIM:
        if (print)
        { cout<<"INTEREST_NDNSIM "<<endl;
          //interestNum++; 
          cout<<"Node "<<i<<" Send "<<endl;
        }
        break;
      case HeaderHelper::INTEREST_CCNB:
        //cout<<"INTEREST_CCNB"<<endl;
        break;
        //return ReceiveInterest (Wire::ToInterest (packet, Wire::WIRE_FORMAT_CCNB));
      case HeaderHelper::CONTENT_OBJECT_NDNSIM:
        if (print)
        {
          cout<<"DATA_NDNSIM "<<endl;
          //s_number++;
          cout<<"Node "<<i<<" Send "<<endl;
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
        return false;
        //cout<<"fuck";
      }

  //     // exception will be thrown if packet is not recognized
  //   }
  // catch (UnknownHeaderException)
  //   {
  //     //NS_FATAL_ERROR ("Unknown NDN header. Should not happen");
  //     return false;
  //   }

  Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
  
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
  
  if (type == HeaderHelper::INTEREST_NDNSIM)
    Simulator::Schedule(Seconds(0.0), &NdnNetDeviceFace::DoSendInterest, this, packet);
  else
    {
      if (m_node->GetId() == producerId)
      {
        HopTag hopTag;
        hopTag.SetHop(tempTimes);
        //cout<<"TEMP"<<tempTimes;
        packet->AddPacketTag (hopTag);
        Simulator::Schedule(Seconds(0.0), &NdnNetDeviceFace::DoSendData, this, packet);
      }
      else
        Simulator::Schedule(Seconds(0.0), &NdnNetDeviceFace::DoSendData, this, packet);
    }

  return true;

}

void
NdnNetDeviceFace::DoSendInterest (Ptr<Packet> packet)
{
    // cout<<Simulator::Now().GetSeconds()<<" ";
    // cout<<"SENDING";

    //c//out<<m_node->GetId()<<" SENDING!";
    if (reSendTimes > 1)
    {
      HopTag hopTag;
      packet->RemovePacketTag(hopTag);
      hopTag.UpdateHop();
      cout<<hopTag.GetHop()<<endl;
      packet->AddPacketTag (hopTag);
 //     cout<<" RESEND_I "<<m_node->GetId();
    }
    Ptr<Packet> p = packet->Copy();
   
    m_netDevice->Send (p, m_netDevice->GetBroadcast (),
                               L3Protocol::ETHERNET_FRAME_TYPE);
  
  //   reSendTimes++;
  // //if (reSendInterestFlag && m_node->GetId()!=1)

  // //{ 
  //   Ptr<Packet> p1 = packet->Copy();
  //   MobilityTag mobilityTag;
  //   VelocityTag velocityTag;
  //   p1->RemovePacketTag(velocityTag);
  //   p1->RemovePacketTag(mobilityTag);
  //   Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();   
  //   mobilityTag.SetMobility((mobility->GetPosition()).x);
  //   p1->AddPacketTag (mobilityTag);
  //   velocityTag.SetVelocity((mobility->GetVelocity()).x);
  //   p1->AddPacketTag (velocityTag);



  //   //cout<<"NODE "<<m_node->GetId()<<" ReSending!"<<endl;
  //   reSendInterestEvent = Simulator::Schedule(Seconds(0.05), &NdnNetDeviceFace::DoSendInterest, this, p1);
   //}
     
}

void
NdnNetDeviceFace::DoSendData (Ptr<Packet> packet)
{
 // cout<<m_node->GetId()<<" "<<reSendDataFlag;
    //   cout<<Simulator::Now().GetSeconds()<<" ";
    // cout<<"SENDING";
    //cout<<m_node->GetId()<<" SENDING!";
  //cout<<m_node->GetId()<<" SENDING!";
    if (reSendTimes > 1)
    {
      HopTag hopTag;
      packet->RemovePacketTag(hopTag);
      hopTag.UpdateHop();
      packet->AddPacketTag (hopTag);
 //     cout<<" RESEND_D "<<m_node->GetId();
    }

    Ptr<Packet> p = packet->Copy();
    
    m_netDevice->Send (p, m_netDevice->GetBroadcast (),
                               L3Protocol::ETHERNET_FRAME_TYPE);
  //   reSendTimes++;
  // //if (reSendDataFlag && m_node->GetId()!=0)
  // //{ 
  //   Ptr<Packet> p1 = packet->Copy();
  //   MobilityTag mobilityTag;
  //   VelocityTag velocityTag;
    
  //   //sp1->RemovePacketTag(hopTag);
  //   p1->RemovePacketTag(velocityTag);
  //   p1->RemovePacketTag(mobilityTag);
  //   Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();   
  //   mobilityTag.SetMobility((mobility->GetPosition()).x);
  //   p1->AddPacketTag (mobilityTag);
  //   velocityTag.SetVelocity((mobility->GetVelocity()).x);
  //   p1->AddPacketTag (velocityTag);
    

  //   //cout<<"NODE "<<m_node->GetId()<<" ReSending!"<<endl;
  //   reSendDataEvent = Simulator::Schedule(Seconds(0.05), &NdnNetDeviceFace::DoSendData, this, p1);
  // //}
   
}


void
NdnNetDeviceFace::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler)
{
   //NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandlers (interestHandler, dataHandler);

  m_node->RegisterProtocolHandler (MakeCallback (&NdnNetDeviceFace::ReceiveFromNetDevice, this),
                                   L3Protocol::ETHERNET_FRAME_TYPE, GetNetDevice (), true/*promiscuous mode*/);
  //cout<<"REGISTER";
}


void
NdnNetDeviceFace::ReceiveFromNetDevice (Ptr<NetDevice> device,
                           Ptr<const Packet> p,
                           uint16_t protocol,
                           const Address &from,
                           const Address &to,
                           NetDevice::PacketType packetType)
{ 

  double distance;
  //p ->PeekPacketTag
  //cout<<endl;
  //cout<<"fuck"<<endl;
  Ptr<Packet> packet = p->Copy();
  Ptr<Node> node = this->GetNode();
  uint32_t i = node->GetId();
  //cout<<"node!!!!!!!!"<<endl;

    HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
  // cout<<"RECEIVED ";
    switch (type)
      {
      case HeaderHelper::INTEREST_NDNSIM:
        //cout<<endl;
        if (print)
        { 

          cout<<Simulator::Now().GetSeconds();
          cout<<"Node: "<<i;
          cout<<"INTEREST_NDNSIM"<<endl;
        }
        
        break;
      case HeaderHelper::INTEREST_CCNB:
        //cout<<"INTEREST_CCNB"<<endl;
        break;
        //return ReceiveInterest (Wire::ToInterest (packet, Wire::WIRE_FORMAT_CCNB));
      case HeaderHelper::CONTENT_OBJECT_NDNSIM:
       // cout<<endl;
        if (print)
        {
          cout<<Simulator::Now().GetSeconds();
          cout<<"Node: "<<i;
          cout<<"DATA_NDNSIM"<<endl;
        }
        //m_number++;
        //}
        break;
        //return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_NDNSIM));
      case HeaderHelper::CONTENT_OBJECT_CCNB:
        //return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_CCNB));
        //cout<<"DATA_CCNB"<<endl;
        break;
        //return ReceiveData (Wire::ToData (pack
      default:
        //NS_FATAL_ERROR ("Not supported NDN header");
        //<<"false";
        cout<<"fuck";
      }
     // cout<<"false";
  

      //exception will be thrown if packet is not recognized
  //   }
  // catch (UnknownHeaderException)
  //   {
  //     //NS_FATAL_ERROR ("Unknown NDN header. Should not happen");
  //     cout<<"false";
  //   }

  //cout<<"runing!!!!";
  VelocityTag velocityTag;
  packet->PeekPacketTag(velocityTag);

  MobilityTag mobilityTag;
  packet->PeekPacketTag(mobilityTag);

  // HopTag hopTag;
  // packet->PeekPacketTag(hopTag);
  //cout<<"RECEIVED"<<endl;
  
  //cout<<"At: "<<Simulator::Now()<<" from node:"<<node->GetId()<<" location:"<<mobilityTag.GetMobility()<<endl;
  
  // cout<<"At: "<<Simulator::Now()<<" node:"<<node->GetId()
  //     <<" location:"<<mobilityTag.GetMobility()
  //     <<" velocity:"<<velocityTag.GetVelocity()<<endl;

  distance = GetDistance(mobilityTag.GetMobility());

  
  //double velocity = m_node->GetObject<MobilityModel> ()->GetVelocity().x;

  // p->PrintPacketTags(cout);
  //cout<<velocity<<endl;
  
  packet->RemovePacketTag(velocityTag);

  packet->RemovePacketTag(mobilityTag);

   double delayTime = (500-abs(distance))*0.0001;

   // UniformVariable randomWaitTime;
   // delayTime += randomWaitTime.GetValue(0,waitTime);
   bool receive = GetProbability(distance);

  if (!receive)
  {
    cout<<m_node->GetId()<<"CANCEL "<<distance<<endl;
  }
  else
  {
    cout<<m_node->GetId()<<"GET "<<distance<<endl;
  }

  //cout<<m_node->GetId()<<" "<<receive<<endl;
 //cout<<m_node->GetId()<<" "<<receiveInterestFlag<<endl;
 if (type==HeaderHelper::INTEREST_NDNSIM && i==producerId && receiveInterestFlag == false)
   {
   HopTag hopTag;
   packet->RemovePacketTag(hopTag);
   tempTimes = hopTag.GetHop();
   cout<<"999999999999999999999999999999999"<<endl;
   Simulator::Destroy();
 /* code */
 cout<<m_node->GetId()<<" "<<m_node->GetObject<MobilityModel>()->GetPosition().x<<endl;
 receiveInterestFlag = true;
 Receive (packet);

   }
 else if (type==HeaderHelper::INTEREST_NDNSIM && receive && receiveInterestFlag == false)
  { 
    /* code */
    cout<<m_node->GetId()<<" "<<m_node->GetObject<MobilityModel>()->GetPosition().x<<endl;
    receiveInterestFlag = true;
    Receive (packet);
  }
  else if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM && receive && receiveDataFlag == false)
  { 
        if (m_node->GetId() == consumerId )
      {
    Simulator::Destroy();
    }

    receiveDataFlag = true;
    Receive (packet);
  }


}

bool
NdnNetDeviceFace::Receive (Ptr<const Packet> p)
{
  //NS_LOG_FUNCTION (this << p << p->GetSize ());

  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  Ptr<Packet> packet = p->Copy (); // give upper layers a rw copy of the packet
  try
    {
      HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
      switch (type)
        {
        case HeaderHelper::INTEREST_NDNSIM:
          return ReceiveInterest (Wire::ToInterest (packet, Wire::WIRE_FORMAT_NDNSIM));
        case HeaderHelper::INTEREST_CCNB:
          return ReceiveInterest (Wire::ToInterest (packet, Wire::WIRE_FORMAT_CCNB));
        case HeaderHelper::CONTENT_OBJECT_NDNSIM:
          return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_NDNSIM));
        case HeaderHelper::CONTENT_OBJECT_CCNB:
          return ReceiveData (Wire::ToData (packet, Wire::WIRE_FORMAT_CCNB));
        default:
          //NS_FATAL_ERROR ("Not supported NDN header");
          return false;
        }

      // exception will be thrown if packet is not recognized
    }
  catch (UnknownHeaderException)
    {
      //NS_FATAL_ERROR ("Unknown NDN header. Should not happen");
      return false;
    }

  return false;
}



double
NdnNetDeviceFace::GetDistance (double sourcePosition)
{
  Ptr<Node> node= this->GetNode();
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  double destPosition = mobility->GetPosition().x;
  return destPosition-sourcePosition;
}

bool
NdnNetDeviceFace::GetProbability (double distance)
{ 
  // uint32_t upper 
  // UniformVariable probability;
  // for (uint32_t i = 0; i<=500; i++)
  // {
  //   if 
  // }
  // delayTime += probability.GetValue(0,waitTime);
  UniformVariable probability;

  double i = probability.GetValue (0.0,500.0);
  if (i<=abs(distance)*1.5)
    return true;
  else
    return false;

}



}
}
#endif
