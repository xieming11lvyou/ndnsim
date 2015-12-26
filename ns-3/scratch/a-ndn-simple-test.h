#ifndef A_NDN_SIMPLE_TEST_H
#define A_NDN_SIMPLE_TEST_H

#include "math.h"

#include "ns3/ndn-net-device-face.h"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

#include "a-ndn-tag.h"

using namespace std;


namespace ns3{
namespace ndn{


class MyNetDeviceFace : public NetDeviceFace{
public:
  static TypeId
  GetTypeId ();
  double  range;
  static int totalInterestNum;
  bool m_isbetween;

  MyNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice);
  MyNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice,double i);
  virtual ~MyNetDeviceFace();
  virtual void RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler);
  bool Receive (Ptr<const Packet> p);
  void DoSend (Ptr<Packet> p);
  void DoSendData (Ptr<Packet> packet);
  void DoSendInterest (Ptr<Packet> packet); 
  void SetRange(double rangex);

protected:
  virtual bool Send (Ptr<Packet> p);
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
  uint32_t reSendTimes_I;
  uint32_t reSendTimes_D;
  uint32_t tempTimes;
  uint32_t m_overHead;


  bool print;

  uint32_t consumerId;
  uint32_t producerId;
private:
  Ptr<NetDevice> m_netDevice; 
};

NS_OBJECT_ENSURE_REGISTERED (MyNetDeviceFace);

int MyNetDeviceFace::totalInterestNum = 0;
TypeId
MyNetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::MyNetDeviceFace")
    .SetParent<NetDeviceFace> ();
  return tid;
}

MyNetDeviceFace::MyNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice,double i)
  : NetDeviceFace (node, netDevice),interestNum(0),m_number(0),s_number(0)
  ,receiveInterestFlag(false),receiveDataFlag(false),reSendInterestFlag(true),reSendDataFlag(true),waitTime(0.002)
  ,reSendTimes_I(0),tempTimes(0),print(0),m_netDevice(netDevice),reSendTimes_D(0)
  {
    consumerId = i-2;
    producerId = i-1;
    range = 300;
    m_overHead = 0;
    m_isbetween = false;
  }
MyNetDeviceFace::MyNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : NetDeviceFace (node, netDevice),interestNum(0),m_number(0),s_number(0)
  ,receiveInterestFlag(false),receiveDataFlag(false),reSendInterestFlag(true),reSendDataFlag(true),waitTime(0.002)
  ,reSendTimes_I(0),tempTimes(0),print(0),m_netDevice(netDevice),reSendTimes_D(0)
  {
   consumerId = 8;
   producerId = 9;
   range = 300;
   m_overHead = 0;
  }

void
MyNetDeviceFace::SetRange(double rangex)
{
range = rangex;
}
MyNetDeviceFace::~MyNetDeviceFace ()
{
  //NS_LOG_FUNCTION_NOARGS ();
}

bool
MyNetDeviceFace::Send (Ptr<Packet> packet)
{ 
  //Ptr<Node> node = this->GetNode();



//INCREASE   FwHopTag
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
          cout<<"Node "<<i<<"Send";
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
          cout<<"Node "<<i<<"Send"<<endl;
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
    DoSendInterest(packet);
  //  Simulator::Schedule(Seconds(0.0), &MyNetDeviceFace::DoSendInterest, this, packet);
  else
    {
      if (m_node->GetId() == producerId)
      {
        HopTag hopTag;
        hopTag.SetHop(tempTimes);
        hopTag.SetOverHead(m_overHead);
        //cout<<"TEMP"<<tempTimes;
        packet->AddPacketTag (hopTag);
        DoSendData(packet);
 //       Simulator::Schedule(Seconds(0.0), &MyNetDeviceFace::DoSendData, this, packet);
      }
      else
        DoSendData(packet);
   //     Simulator::Schedule(Seconds(0.0), &MyNetDeviceFace::DoSendData, this, packet);
    }

  return true;

}

void
MyNetDeviceFace::DoSendInterest (Ptr<Packet> packet)
{
    // cout<<Simulator::Now().GetSeconds()<<" ";
    // cout<<"SENDING";
   reSendTimes_I++;
    //c//out<<m_node->GetId()<<" SENDING!";
 //   if (reSendTimes_I > 1)
    {
      HopTag hopTag;
      packet->RemovePacketTag(hopTag);
      hopTag.UpdateHop();
      hopTag.UpdateOverHead();
      packet->AddPacketTag (hopTag);
//      Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
//      cout<<Simulator::Now().GetSeconds()<<"\t"<<" RESEND_I "<<m_node->GetId()<<"\t"<<mobility->GetPosition().x<<endl;
    }

    totalInterestNum++;
    Ptr<Packet> p = packet->Copy();
//   if(reSendTimes_I<6)
//     {
    Simulator::Cancel(reSendDataEvent);
    Simulator::Cancel(sendDataEvent);
    m_netDevice->Send (p, m_netDevice->GetBroadcast (),
                               L3Protocol::ETHERNET_FRAME_TYPE);


  //if (reSendInterestFlag && m_node->GetId()!=1)

  //{ 
    Ptr<Packet> p1 = packet->Copy();
    MobilityTag mobilityTag;
    VelocityTag velocityTag;
    p1->RemovePacketTag(velocityTag);
    p1->RemovePacketTag(mobilityTag);
    Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();   
    mobilityTag.SetMobility((mobility->GetPosition()).x);
    p1->AddPacketTag (mobilityTag);
    velocityTag.SetVelocity((mobility->GetVelocity()).x);
    p1->AddPacketTag (velocityTag);



    //cout<<"NODE "<<m_node->GetId()<<" ReSending!"<<endl;
    if(reSendInterestEvent.IsRunning())
     Simulator::Cancel(reSendInterestEvent);
    reSendInterestEvent = Simulator::Schedule(Seconds(0.05), &MyNetDeviceFace::DoSendInterest, this, p1);

//}
//    else
//      {
//      reSendTimes_I = 0;
//      Simulator::Cancel(sendInterestEvent);
//      Simulator::Cancel(reSendInterestEvent);
//      }
}

void
MyNetDeviceFace::DoSendData (Ptr<Packet> packet)
{

  reSendTimes_D++;
 // cout<<m_node->GetId()<<" "<<reSendDataFlag;
    //   cout<<Simulator::Now().GetSeconds()<<" ";
    // cout<<"SENDING";
    //cout<<m_node->GetId()<<" SENDING!";
  //cout<<m_node->GetId()<<" SENDING!";
  //  if (reSendTimes_D > 1)
    {
      HopTag hopTag;
      packet->RemovePacketTag(hopTag);
      hopTag.UpdateHop();
      packet->AddPacketTag (hopTag);


//      Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
//     cout<<Simulator::Now().GetSeconds()<<"\t"<<" RESEND_D "<<m_node->GetId()<<"\t"<<mobility->GetPosition().x<<endl;
    }

    Ptr<Packet> p = packet->Copy();
//    if(reSendTimes_D<6)
//      {
    Simulator::Cancel(reSendInterestEvent);
    Simulator::Cancel(sendInterestEvent);
    m_netDevice->Send (p, m_netDevice->GetBroadcast (),
                               L3Protocol::ETHERNET_FRAME_TYPE);

  //if (reSendDataFlag && m_node->GetId()!=0)
  //{ 
    Ptr<Packet> p1 = packet->Copy();
    MobilityTag mobilityTag;
    VelocityTag velocityTag;
    
    //sp1->RemovePacketTag(hopTag);
    p1->RemovePacketTag(velocityTag);
    p1->RemovePacketTag(mobilityTag);
    Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();   
    mobilityTag.SetMobility((mobility->GetPosition()).x);
    p1->AddPacketTag (mobilityTag);
    velocityTag.SetVelocity((mobility->GetVelocity()).x);
    p1->AddPacketTag (velocityTag);
    

    //cout<<"NODE "<<m_node->GetId()<<" ReSending!"<<endl;
    if(reSendDataEvent.IsRunning())
     Simulator::Cancel(reSendDataEvent);
    reSendDataEvent = Simulator::Schedule(Seconds(0.05), &MyNetDeviceFace::DoSendData, this, p1);
  //}
//}
//    else
//      {
//      reSendTimes_D = 0;
//      Simulator::Cancel(sendDataEvent);
//      Simulator::Cancel(reSendDataEvent);
//      }
}


void
MyNetDeviceFace::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler)
{
   //NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandlers (interestHandler, dataHandler);

  m_node->RegisterProtocolHandler (MakeCallback (&MyNetDeviceFace::ReceiveFromNetDevice, this),
                                   L3Protocol::ETHERNET_FRAME_TYPE, GetNetDevice (), true/*promiscuous mode*/);
  //cout<<"REGISTER";
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

  SourceTag sourceTag;
  if(packet->PeekPacketTag(sourceTag))
      {
  if(2010>=m_node->GetObject<MobilityModel>()->GetPosition().x &&
      m_node->GetObject<MobilityModel>()->GetPosition().x>=sourceTag.GetSource())
    {
    m_isbetween = true;
    }
  else
    m_isbetween = false;
      }

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


  //cout<<endl;
  //cout<<"---------------------------------"<<endl;

  //cout<<"---------------------------------"<<endl;

//---------------

//  bool verbose = 1;
  // if(i!=0&&type==HeaderHelper::CONTENT_OBJECT_NDNSIM)
  // {  
  //   //cout<<"delete";
  //   //delete p;
  // }

  // else
  //  Receive(p);
  // else

   double delayTime = (range-min(abs(distance),range))/range*0.05;

//   UniformVariable randomWaitTime;
//    delayTime += randomWaitTime.GetValue(0,waitTime);


   if(print)
   {
    cout<<distance<<endl;
    }
  // {
    if (type==HeaderHelper::INTEREST_NDNSIM  && i== producerId )//&& receiveInterestFlag==false )
    {

      HopTag hopTag;
      packet->RemovePacketTag(hopTag);
      tempTimes = hopTag.GetHop();
      m_overHead = hopTag.GetOverHead();
      //packet->AddPacketTag (hopTag);

//      cout<<"Producer Receive"<<"\t"<<Simulator::Now().GetSeconds()<<"\t";
//      FwHopCountTag hopCountTag;
//      packet->PeekPacketTag (hopCountTag);
//      hopCountTag.Print(cout);
//      cout<<endl;

      Receive(packet);
      receiveInterestFlag = true;
    }
    else if (type==HeaderHelper::INTEREST_NDNSIM && distance>=0 && m_isbetween)//&& receiveInterestFlag==false )//&& velocity >0)
    { 
//     Ptr<Node> node = this->GetNode();
//     int i= node->GetId();
      m_isbetween =false;
      receiveInterestFlag = true;
      Simulator::Cancel(sendDataEvent);
      Simulator::Cancel(reSendDataEvent);
      Simulator::Cancel(sendInterestEvent);
      sendInterestEvent = Simulator::Schedule(Seconds(delayTime), &MyNetDeviceFace::Receive, this, packet);
//      if(0==i)
//       {
//       Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
//       cout<<"case big   "<<mobility->GetPosition().x<<endl;
//       if(sendInterestEvent.IsRunning())
//         cout<<Simulator::Now().GetSeconds()+delayTime<<endl;
//       }
      
    }
    else if (type==HeaderHelper::INTEREST_NDNSIM && distance<= 0 )//&& receiveInterestFlag==true )//&& velocity >0)
    { 
//      Ptr<Node> node = this->GetNode();
//      int i= node->GetId();
//      if(0==i)
//        {
//        Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
//        cout<<"case small  "<<mobility->GetPosition().x<<endl;
//        }
      //cout<<"5"<<endl;
      receiveInterestFlag = false;
      reSendInterestFlag = false;
      Simulator::Cancel(sendInterestEvent);
      Simulator::Cancel(reSendInterestEvent);
    }
    else if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM && i== consumerId )
    { 
      Receive(packet);
    }

    else if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM && distance<=0 && m_isbetween )//&& receiveDataFlag==false )//&& velocity <0)

    {
      reSendTimes_D = 0;
      m_isbetween = false;
      //cout<<"1"<<endl;
      if(sendDataEvent.IsRunning())
       Simulator::Cancel(sendDataEvent);
      sendDataEvent = Simulator::Schedule(Seconds(delayTime), &MyNetDeviceFace::Receive, this, packet);
      //c/out<<"this!!!!!!!!!";
      reSendInterestFlag = false;
      receiveDataFlag = true;
      Simulator::Cancel(sendInterestEvent);
      Simulator::Cancel(reSendInterestEvent);
    }

    else if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM && distance>=0 )//&& receiveDataFlag==true )//&& velocity <0)
    { 
      //cout<<"2"<<endl;
      receiveDataFlag = false;
      //cout<<"that!!!!!!!";
      reSendInterestFlag = false;
      reSendDataFlag = false;
      Simulator::Cancel(sendInterestEvent);
      Simulator::Cancel(reSendInterestEvent);
      Simulator::Cancel(sendDataEvent);
      Simulator::Cancel(reSendDataEvent);
    }
    else
    {
      //cout<<"3"<<endl;
      Simulator::Cancel(reSendInterestEvent);
      Simulator::Cancel(reSendDataEvent);
    }

  //   if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM&&distance<=50)
  //   {
  //     /* code */
  //     Receive (p);
  //   }
  // }
    //Receive (packet);

}

bool
MyNetDeviceFace::Receive (Ptr<const Packet> p)
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
MyNetDeviceFace::GetDistance(double sourcePosition)
{
  Ptr<Node> node= this->GetNode();
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  double destPosition = mobility->GetPosition().x;
  return destPosition-sourcePosition;
}





}
}
#endif

