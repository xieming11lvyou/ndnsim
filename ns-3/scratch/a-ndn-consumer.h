/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef A_NDN_CONSUMER_H
#define A_NDN_CONSUMER_H

#include "ns3/ndnSIM/apps/ndn-consumer.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/traced-value.h"
#include "ns3/ndnSIM/utils/batches.h"
#include "a-ndn-tag.h"
#include "ndn-netdeviceface.h"
#include "a-ndn-simple-test.h"

using namespace std;

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * \brief Ndn application for sending out Interest packets in batches
 */
class MyConsumer: public Consumer
{
public: 
  static TypeId GetTypeId ();
        
  /**
   * \brief Default constructor 
   */
  MyConsumer ();

private:
  virtual void
  StartApplication ();    ///< @brief Called at time specified by Start
  
  void
  AddBatch (uint32_t amount);
  void RertransmitPacket();

  // void
  // JudgeFull ();
protected:
  /**
   * \brief Constructs the Interest packet and sends it using a callback to the underlying NDN protocol
   */
  virtual void
  ScheduleNextPacket ();

  virtual void
  SendPacket ();

  virtual void
  OnData (Ptr<const Data> contentObject);

  void SetRetxTimer (Time retxTimer);
  Time GetRetxTimer () const;
  void CheckRetxTimeout ();
  void OnTimeout (uint32_t sequenceNumber);
  void Destroy ();



private:


  uint32_t seq;
  uint32_t m_numData;
  double t1,t2;
  uint32_t retrans;

  uint32_t m_seqMax;
  EventId m_reSendEvent;

  // static const int m_size = 20;
  // //Ptr<Interest> m_interest;
  // //Ptr<Name> nameWithSequence[m_size];
  // uint32_t m_array[m_size];

};
 
NS_OBJECT_ENSURE_REGISTERED (MyConsumer);
    
TypeId
MyConsumer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::MyConsumer")
    .SetGroupName ("Ndn")
    .SetParent<Consumer> ()
    .AddConstructor<MyConsumer> ()
    .AddAttribute ("MaxSeq",
                   "Maximum sequence number to request",
                   IntegerValue (std::numeric_limits<uint32_t>::max ()),
                   MakeIntegerAccessor (&MyConsumer::m_seqMax),
                   MakeIntegerChecker<uint32_t> ())

    ;

  return tid;
}

MyConsumer::MyConsumer ()
  : seq(0)
{
  m_numData = 0;
  t1=t2=0;
  retrans = 0;
}

void
MyConsumer::StartApplication ()
{
  Consumer::StartApplication ();
}

void
MyConsumer::ScheduleNextPacket ()
{
  if (!m_sendEvent.IsRunning ())
  {

//  //     // Time delay = Seconds (0);
//  //     // if (!m_initial) delay = m_rtt->RetransmitTimeout ();
//
//  //     //m_initial = false;
//  //     //cout<<"At: "<<Simulator::Now()<<" send!"<<endl;
//  //       m_sendEvent = Simulator::Schedule (Seconds(2.0), &MyConsumerBatches::SendPacket, this);
//
  SendPacket();
//
  }
}
void
MyConsumer::RertransmitPacket()
{
  seq++;
  retrans++;
  Ptr<Name> nameWithSequence = Create<Name> (m_interestName);
  nameWithSequence->appendSeqNum (seq);
  //

  Ptr<Interest> interest = Create<Interest> ();
  interest->SetNonce               (m_rand.GetValue ());
  interest->SetName                (nameWithSequence);
  interest->SetInterestLifetime    (m_interestLifeTime);

  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  //cout<<"send a "<<interest<<endl;

  WillSendOutInterest (seq);

  // cout<<"real send ";
  //nameWithSequence->Print(cout);
  // cout<<"fubnk!!!!!!!";
  //scout<<*nameWithSequence<<endl;
  FwHopCountTag hopCountTag;
  interest->GetPayload ()->AddPacketTag (hopCountTag);

  HopTag hopTag;
  hopTag.SetHop(0);
  hopTag.SetOverHead(0);
  interest->GetPayload ()->AddPacketTag (hopTag);

  IndexTag indexTag;
  indexTag.SetIndex(seq);
//  cout<<"Index"<<indexTag.GetIndex()<<endl;
  interest->GetPayload ()->AddPacketTag (indexTag);

  SourceTag sourceTag;
  sourceTag.SetSource(m_node->GetObject<MobilityModel>()->GetPosition().x);
  interest->GetPayload ()->AddPacketTag (sourceTag);


  m_transmittedInterests (interest, this, m_face);
  m_face->ReceiveInterest (interest);
  m_reSendEvent = Simulator::Schedule (Seconds(1), &MyConsumer::RertransmitPacket, this);
}

void
MyConsumer::SendPacket ()
{
  seq++;
  //
  Ptr<Name> nameWithSequence = Create<Name> (m_interestName);
  nameWithSequence->appendSeqNum (seq);
  //

  Ptr<Interest> interest = Create<Interest> ();
  interest->SetNonce               (m_rand.GetValue ());
  interest->SetName                (nameWithSequence);
  interest->SetInterestLifetime    (m_interestLifeTime);

  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  //cout<<"send a "<<interest<<endl;

  WillSendOutInterest (seq);  

  // cout<<"real send ";
  //nameWithSequence->Print(cout);
  // cout<<"fubnk!!!!!!!";
  //scout<<*nameWithSequence<<endl;
  FwHopCountTag hopCountTag;
  interest->GetPayload ()->AddPacketTag (hopCountTag);

  HopTag hopTag;
  hopTag.SetHop(0);
  hopTag.SetOverHead(0);
  interest->GetPayload ()->AddPacketTag (hopTag);

  IndexTag indexTag;
  indexTag.SetIndex(seq);
//  cout<<"Index"<<indexTag.GetIndex()<<endl;
  interest->GetPayload ()->AddPacketTag (indexTag);

  SourceTag sourceTag;
  sourceTag.SetSource(m_node->GetObject<MobilityModel>()->GetPosition().x);
  interest->GetPayload ()->AddPacketTag (sourceTag);
  retrans = 0;
  m_node->GetObject<ns3::ndn::NewNetDeviceFace>()->totalnterestNum = 0;
  m_node->GetObject<ns3::ndn::NewNetDeviceFace>()->duplicte = 0;
  m_transmittedInterests (interest, this, m_face);
  t1 = Simulator::Now().GetSeconds();
  cout<<"Send"<<"\t"<<t1<<"\t";
  m_face->ReceiveInterest (interest);
  m_reSendEvent = Simulator::Schedule (Seconds(1), &MyConsumer::RertransmitPacket, this);

}



void
MyConsumer::OnData (Ptr<const Data> data)
{


  if (!m_active) return;

  Simulator::Cancel (m_reSendEvent);

//-----------------------
  t2 = Simulator::Now().GetSeconds();
  cout<<"Receive"<<"\t"<<t2<<"\t"<<"delay"<<"\t"<<(t2-t1)<<"\t";
  m_numData++;


  cout<<"Index"<<"\t"<<m_numData<<"\t";
  FwHopCountTag hopCountTag;
  data->GetPayload ()->PeekPacketTag (hopCountTag);
  HopTag hopTag;
  data->GetPayload ()->PeekPacketTag (hopTag);
//new  cout<<(t2-t1)<<"\t"<<m_node->GetObject<NewNetDeviceFace>()->duplicte<<endl;//new
  cout<<"OverHead"<<"\t"<<m_node->GetObject<NewNetDeviceFace>()->totalnterestNum<<"\t";
  cout<<"Hop"<<"\t"<<hopCountTag.Get()<<"\t"<<"retrans"<<"\t"<<retrans<<"\t";
//  if(hopTag.GetHop()>hopTag.GetOverHead())
//    {
//    cout<<"Retransmit"<<hopTag.GetHop()<<" "<<hopTag.GetOverHead()<<" "<<(hopTag.GetHop()-hopTag.GetOverHead());
//    }
  cout<<endl;





      //"total"<<"\t"<<hopTag.GetHop()<<"\t";


//  if(hopCountTag.Get()<hopTag.GetHop())
//    cout<<"Retransmit"<<"\t"<<(hopTag.GetHop()-hopCountTag.Get());
//  cout<<endl;








if(m_numData<1000)
  {
  ScheduleNextPacket ();
//  Simulator::Schedule (Seconds(1), &MyConsumer::ScheduleNextPacket, this);
  }
else
  Simulator::Destroy();




//------------------------
  Consumer::OnData (data);
  //

  

  //seq++;
//  ScheduleNextPacket ();


}
void
MyConsumer::SetRetxTimer (Time retxTimer)
{

}

Time
MyConsumer::GetRetxTimer () const
{
  Time t = Seconds (0);
  return t;
}

void
MyConsumer::CheckRetxTimeout ()
{
  
}
void
MyConsumer::OnTimeout (uint32_t sequenceNumber)
{
}
void 
MyConsumer::Destroy ()
{
  Simulator::Destroy();
}


} // namespace ndn
} // namespace ns3

#endif
