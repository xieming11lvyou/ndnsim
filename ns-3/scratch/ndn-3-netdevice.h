#ifndef A_NDN_NETDVICEFACE_H_H
#define A_NDN_NETDVICEFACE_H_H
#include "math.h"

#include "ns3/ndn-net-device-face.h"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

#include "a-ndn-tag.h"

using namespace std;


namespace ns3{
namespace ndn{


class ThreeNetDeviceFace : public NetDeviceFace{
public:
  static TypeId
  GetTypeId ();
  double  range;
  static int totalnterestNum;
  static int duplicte;
  double m_dec;
  bool  m_isbetween;
//  static int totalInterestNum;

  ThreeNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice);
  ThreeNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice,double i);
  ThreeNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice,uint32_t array[],uint32_t numP,double p,double decs);
  virtual ~ThreeNetDeviceFace();
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
  bool GetProbability (double distance);
  double interestNum;
  double m_number;
  uint32_t m_currentIndex;
  uint32_t temp;
  double m_probility;




  double s_number;
  bool receiveInterestFlag;
  bool receiveDataFlag;

  bool reSendInterestFlag;
  bool reSendDataFlag;

  bool nearFlag;

  EventId sendInterestEvent;
  EventId sendDataEvent;

  EventId reSendInterestEvent;
  EventId reSendDataEvent;
  double waitTime;
  uint32_t reSendTimes_I;
  uint32_t reSendTimes_D;
  uint32_t tempTimes;


  bool print;

  uint32_t consumerId;
  uint32_t producerId;
  uint32_t numProducer;
 // uint32_t producerArray[8];//1~numProducer
  bool isProducer;
private:
  Ptr<NetDevice> m_netDevice;
};

NS_OBJECT_ENSURE_REGISTERED (ThreeNetDeviceFace);

int ThreeNetDeviceFace::totalnterestNum = 0;
int ThreeNetDeviceFace::duplicte = 0;
TypeId
ThreeNetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::ThreeNetDeviceFace")
    .SetParent<NetDeviceFace> ();
  return tid;
}

ThreeNetDeviceFace::ThreeNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice,double i)
  : NetDeviceFace (node, netDevice),interestNum(0),m_number(0),s_number(0)
  ,receiveInterestFlag(false),receiveDataFlag(false),reSendInterestFlag(true),reSendDataFlag(true),waitTime(0.002)
  ,reSendTimes_I(0),tempTimes(0),print(0),m_netDevice(netDevice),reSendTimes_D(0)
  {
    consumerId = i-2;
    producerId = i-1;
    range = 300;
    m_currentIndex = -1;
    nearFlag = false;
  }
ThreeNetDeviceFace::ThreeNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice,uint32_t array[],uint32_t numP,double p,double decs)
  : NetDeviceFace (node, netDevice),interestNum(0),m_number(0),s_number(0)
  ,receiveInterestFlag(false),receiveDataFlag(false),reSendInterestFlag(true),reSendDataFlag(true),waitTime(0.002)
  ,reSendTimes_I(0),tempTimes(0),print(0),m_netDevice(netDevice),reSendTimes_D(0)
  {
    consumerId = array[0];
    m_probility = p;
    numProducer = numP;
    temp = 0;
    m_dec = decs;
    m_isbetween = false;
 //   cout<<m_node->GetId()<<endl;
    for(int i=1;i<numProducer+1;i++)
      {
//      producerArray[i] = array[i];
//      cout<<"xiao"<<producerArray[i]<<endl;
      if(m_node->GetId()==array[i])
        {
        isProducer = true;
//      cout<<"true"<<m_node->GetId()<<endl;
        }
      else
        {
        isProducer = false;
 //       cout<<"false"<<endl;
        }
      }
    range = 300;
    m_currentIndex = -1;
    nearFlag = false;
  }

ThreeNetDeviceFace::ThreeNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : NetDeviceFace (node, netDevice),interestNum(0),m_number(0),s_number(0)
  ,receiveInterestFlag(false),receiveDataFlag(false),reSendInterestFlag(true),reSendDataFlag(true),waitTime(0.002)
  ,reSendTimes_I(0),tempTimes(0),print(0),m_netDevice(netDevice),reSendTimes_D(0)
  {
   consumerId = 8;
   producerId = 9;
   range = 300;
  }

void
ThreeNetDeviceFace::SetRange(double rangex)
{
range = rangex;
}
ThreeNetDeviceFace::~ThreeNetDeviceFace ()
{
  //NS_LOG_FUNCTION_NOARGS ();
}

bool
ThreeNetDeviceFace::Send (Ptr<Packet> packet)
{
  //Ptr<Node> node = this->GetNode();

  if (!Face::Send (packet))
    {
      return false;
    }
    // FwHopCountTag hopCountTag;
    //   packet->PeekPacketTag (hopCountTag);
    //   hopCountTag.Print(cout);
    //   cout<<endl;

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
  //  Simulator::Schedule(Seconds(0.0), &ThreeNetDeviceFace::DoSendInterest, this, packet);
  else
    {
      if (isProducer)
      {
        HopTag hopTag;
        hopTag.SetHop(0);
        hopTag.SetOverHead(1);
        //cout<<"TEMP"<<tempTimes;
        packet->AddPacketTag (hopTag);
        DoSendData(packet);
 //       Simulator::Schedule(Seconds(0.0), &ThreeNetDeviceFace::DoSendData, this, packet);
      }
      else
        {
        HopTag hopTag;
        packet->RemovePacketTag(hopTag);
        hopTag.UpdateOverHead();
        packet->AddPacketTag(hopTag);
        DoSendData(packet);
        }

   //     Simulator::Schedule(Seconds(0.0), &ThreeNetDeviceFace::DoSendData, this, packet);
    }

  return true;

}

void
ThreeNetDeviceFace::DoSendInterest (Ptr<Packet> packet)
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
      packet->AddPacketTag (hopTag);
//      Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel> ();
//      cout<<Simulator::Now().GetSeconds()<<"\t"<<" RESEND_I "<<m_node->GetId()<<"\t"<<mobility->GetPosition().x<<endl;
    }
    totalnterestNum++;
//    cout<<Simulator::Now().GetSeconds()<<"\t"<<m_node->GetId()<<"\t"<<++totalnterestNum<<endl;

    Ptr<Packet> p = packet->Copy();
//   if(reSendTimes_I<6)
//     {
    Simulator::Cancel(reSendDataEvent);
    Simulator::Cancel(sendDataEvent);
    m_netDevice->Send (p, m_netDevice->GetBroadcast (),
                               L3Protocol::ETHERNET_FRAME_TYPE);
}

void
ThreeNetDeviceFace::DoSendData (Ptr<Packet> packet)
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
    reSendDataEvent = Simulator::Schedule(Seconds(0.05), &ThreeNetDeviceFace::DoSendData, this, p1);
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
ThreeNetDeviceFace::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler)
{
   //NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandlers (interestHandler, dataHandler);

  m_node->RegisterProtocolHandler (MakeCallback (&ThreeNetDeviceFace::ReceiveFromNetDevice, this),
                                   L3Protocol::ETHERNET_FRAME_TYPE, GetNetDevice (), true/*promiscuous mode*/);
  //cout<<"REGISTER";
}


void
ThreeNetDeviceFace::ReceiveFromNetDevice (Ptr<NetDevice> device,
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



  IndexTag indexTag;
  if(packet->PeekPacketTag(indexTag))
    {
  if (m_currentIndex!=indexTag.GetIndex())
    {
    receiveInterestFlag = false;
    m_currentIndex = indexTag.GetIndex();
    Simulator::Cancel(sendInterestEvent);
    Simulator::Cancel(reSendInterestEvent);
    Simulator::Cancel(sendDataEvent);
    Simulator::Cancel(reSendDataEvent);
    }
    }


  // HopTag hopTag;
  // packet->PeekPacketTag(hopTag);
  //cout<<"RECEIVED"<<endl;

  //cout<<"At: "<<Simulator::Now()<<" from node:"<<node->GetId()<<" location:"<<mobilityTag.GetMobility()<<endl;

  // cout<<"At: "<<Simulator::Now()<<" node:"<<node->GetId()
  //     <<" location:"<<mobilityTag.GetMobility()
  //     <<" velocity:"<<velocityTag.GetVelocity()<<endl;
  SourceTag sourceTag;
  if(packet->PeekPacketTag(sourceTag))
      {
  if(abs(m_node->GetObject<MobilityModel>()->GetPosition().x-sourceTag.GetSource()) <=
      abs(mobilityTag.GetMobility()-sourceTag.GetSource()) &&
      abs(m_node->GetObject<MobilityModel>()->GetPosition().x-mobilityTag.GetMobility())<=
      abs(mobilityTag.GetMobility()-sourceTag.GetSource()) )
    nearFlag = true;
  else
    nearFlag = false;

  if(2010>=m_node->GetObject<MobilityModel>()->GetPosition().x &&
      m_node->GetObject<MobilityModel>()->GetPosition().x>=sourceTag.GetSource())
    {
    m_isbetween = true;
    }
  else
    m_isbetween = false;
      }

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
   if(print)
   {
    cout<<distance<<endl;
    }

    UniformVariable randomWaitTime;
    double randomDelayTime = randomWaitTime.GetValue(0.000,0.002);
    delayTime += randomWaitTime.GetValue(0,waitTime);
    bool receive = GetProbability(distance);
//    if (!receive)
//      cout<<Simulator::Now().GetSeconds()<<"\t"<<m_node->GetId()<<"CANCEL "<<distance<<"\t"<<
//      m_node->GetObject<MobilityModel>()->GetPosition().x<<endl;

    if (type==HeaderHelper::INTEREST_NDNSIM && isProducer && receiveInterestFlag == false)
    {

      HopTag hopTag;
      packet->RemovePacketTag(hopTag);
      tempTimes = hopTag.GetHop();
      duplicte = totalnterestNum;
      //packet->AddPacketTag (hopTag);
 //     cout<<"Producer"<<"\t"<<Simulator::Now().GetSeconds()<<"\t"<<"ID"<<m_node->GetId()<<"\t"<<endl;
  //    cout<<Simulator::Now().GetSeconds()<<"\t"<<"Producer"<<"\t"<<"ID"<<m_node->GetId()<<"\t"<<totalnterestNum <<endl;
    //  Simulator::Destroy();

//      cout<<"Producer Receive"<<"\t"<<Simulator::Now().GetSeconds()<<"\t";
//      FwHopCountTag hopCountTag;
//      packet->PeekPacketTag (hopCountTag);
//      hopCountTag.Print(cout);
//      cout<<endl;

      Receive(packet);
      receiveInterestFlag = true;
    }
    else if (type==HeaderHelper::INTEREST_NDNSIM && receive && receiveInterestFlag == false)// && m_isbetween )//&& receiveInterestFlag==false )//&& velocity >0)
    {
//      cout<<Simulator::Now().GetSeconds()<<"\t"<<m_node->GetId()<<"\t"<<"GET "<<distance<<"\t"<<
//          m_node->GetObject<MobilityModel>()->GetPosition().x<<"Scheme"<<(Simulator::Now().GetSeconds()+randomDelayTime)<<endl;
 //  cout<<m_node->GetId()<<" "<<m_node->GetObject<MobilityModel>()->GetPosition().x<<endl;

      receiveInterestFlag = true;
      Simulator::Cancel(sendDataEvent);
      Simulator::Cancel(reSendDataEvent);
//      cout<<randomDelayTime<<endl;
      sendInterestEvent = Simulator::Schedule(Seconds(randomDelayTime), &ThreeNetDeviceFace::Receive, this, packet);
//     Receive (packet);
    }
    else if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM && i== consumerId )
    {
      Receive(packet);
    }

    else if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM && nearFlag)//distance<=0 )//&& receiveDataFlag==false )//&& velocity <0)

    {

//      if(m_node->GetId() == 24 ||m_node->GetId() == 23)
//        {
//        cout<<endl;
//        cout<<"99999999999999999999999999999999999999999999999999999999999"<<endl;
//        }
        reSendTimes_D = 0;
      //cout<<"1"<<endl;
  //    cout<<Simulator::Now()<<"\t"<<m_node->GetId()<<"\tget cancel"<<endl;
      if(sendDataEvent.IsRunning())
       Simulator::Cancel(sendDataEvent);
      sendDataEvent = Simulator::Schedule(Seconds(delayTime), &ThreeNetDeviceFace::Receive, this, packet);
      Simulator::Cancel(sendInterestEvent);
      Simulator::Cancel(reSendInterestEvent);
      //c/out<<"this!!!!!!!!!";
      receiveDataFlag = true;
    }

    else if (type==HeaderHelper::CONTENT_OBJECT_NDNSIM && !nearFlag)//distance>=0 )//&& receiveDataFlag==true )//&& velocity <0)
    {
      //cout<<"2"<<endl;
 //     cout<<Simulator::Now()<<"\t"<<m_node->GetId()<<"\tget cancel"<<endl;
      receiveDataFlag = false;
      //cout<<"that!!!!!!!";
      reSendDataFlag = false;
      Simulator::Cancel(sendDataEvent);
      Simulator::Cancel(reSendDataEvent);
      Simulator::Cancel(sendInterestEvent);
      Simulator::Cancel(reSendInterestEvent);
    }
    else
    {
      //cout<<"3"<<endl;
//      Simulator::Cancel(sendDataEvent);
//      Simulator::Cancel(reSendDataEvent);
//      Simulator::Cancel(sendInterestEvent);
//      Simulator::Cancel(reSendInterestEvent);
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
ThreeNetDeviceFace::Receive (Ptr<const Packet> p)
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
ThreeNetDeviceFace::GetDistance(double sourcePosition)
{
  Ptr<Node> node= this->GetNode();
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  double destPosition = mobility->GetPosition().x;
  return destPosition-sourcePosition;
}
bool
ThreeNetDeviceFace::GetProbability (double distance)
{
  UniformVariable probability;
  double i = probability.GetValue (0.0,1.0);
//  double p = abs(distance)/range*0.8+100/((producerId+1)*10)*0.5;
  double p = abs(distance)/range*m_probility*m_dec+abs(m_node->GetObject<MobilityModel> ()->GetVelocity().x)/60*(1-m_probility)*m_dec;
//  double p = abs(distance)/range;
  //cout<<i<<"\t"<<p<<endl;
  if(abs(distance)>range*0.0)
    return true;
  if (i<=p)
    return true;
  else
    return false;
}





}
}
#endif

