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

#ifndef A_NDN_CONSUMERBATCHES_H
#define A_NDN_CONSUMERBATCHES_H

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

using namespace std;

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * \brief Ndn application for sending out Interest packets in batches
 */
class MyConsumerBatches: public Consumer
{
public: 
  static TypeId GetTypeId ();
        
  /**
   * \brief Default constructor 
   */
  MyConsumerBatches ();

private:
  virtual void
  StartApplication ();    ///< @brief Called at time specified by Start
  
  void
  AddBatch (uint32_t amount);

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



private:
  bool m_initial;

  Batches m_batches;
  EventId m_resendEvent;

  // static const int m_size = 20;
  // //Ptr<Interest> m_interest;
  // //Ptr<Name> nameWithSequence[m_size];
  // uint32_t m_array[m_size];

};
 
NS_OBJECT_ENSURE_REGISTERED (MyConsumerBatches);
    
TypeId
MyConsumerBatches::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::MyConsumerBatches")
    .SetGroupName ("Ndn")
    .SetParent<Consumer> ()
    .AddConstructor<MyConsumerBatches> ()

    .AddAttribute ("MyBatches", "MyBatches to schedule. Should be vector, containing pairs of time and amount",
                   // TypeId::ATTR_SET, 
                   StringValue (""),
                   MakeBatchesAccessor (&MyConsumerBatches::m_batches),
                   MakeBatchesChecker ())
    ;

  return tid;
}

MyConsumerBatches::MyConsumerBatches ()
  : m_initial (true)
{
}

void
MyConsumerBatches::StartApplication ()
{
  Consumer::StartApplication ();
  
  //std::cout << "Batches: " << batches << "\n";
  for (Batches::const_iterator i = m_batches.begin (); i != m_batches.end (); i++)
    {
      Simulator::ScheduleWithContext (GetNode ()->GetId (), i->get<0> (), &MyConsumerBatches::AddBatch, this, i->get<1> ());
    }
  //SendPacket();

}

void
MyConsumerBatches::AddBatch (uint32_t amount)
{
  // std::cout << Simulator::Now () << " adding batch of " << amount << "\n";
  m_seqMax += amount;
  m_rtt->ClearSent (); // this is important, otherwise RTT estimation for the new batch will be affected by previous batch history
  m_initial = true;
  //cout<<"add a batch"<<endl;
  ScheduleNextPacket ();
}

void
MyConsumerBatches::ScheduleNextPacket ()
{
  if (!m_sendEvent.IsRunning ())
    {
      // Time delay = Seconds (0);
      // if (!m_initial) delay = m_rtt->RetransmitTimeout ();
      
      //m_initial = false;
      //cout<<"At: "<<Simulator::Now()<<" send!"<<endl;
        m_sendEvent = Simulator::Schedule (Seconds(2.0), &MyConsumerBatches::SendPacket, this);
    }
}

void
MyConsumerBatches::SendPacket ()
{
  if (!m_active) return;

  //NS_LOG_FUNCTION_NOARGS ();

  // uint32_t seq=std::numeric_limits<uint32_t>::max (); //invalid

  // while (m_retxSeqs.size ())
  //   {
  //     seq = *m_retxSeqs.begin ();
  //     m_retxSeqs.erase (m_retxSeqs.begin ());
  //     break;
  //   }

  // if (seq == std::numeric_limits<uint32_t>::max ())
  //   {
  //     if (m_seqMax != std::numeric_limits<uint32_t>::max ())
  //       {
  //         if (m_seq >= m_seqMax)
  //           {
  //             return; // we are totally done
  //           }
  //       }

  //     seq = m_seq++;
  //   }

  //
  Ptr<Name> nameWithSequence = Create<Name> (m_interestName);
  nameWithSequence->appendSeqNum (0);
  //

  Ptr<Interest> interest = Create<Interest> ();
  interest->SetNonce               (m_rand.GetValue ());
  interest->SetName                (nameWithSequence);
  interest->SetInterestLifetime    (m_interestLifeTime);

  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  //cout<<"send a "<<interest<<endl;

  WillSendOutInterest (0);  

  // cout<<"real send ";
  //nameWithSequence->Print(cout);
  //scout<<*nameWithSequence<<endl;
  FwHopCountTag hopCountTag;
  interest->GetPayload ()->AddPacketTag (hopCountTag);


  m_transmittedInterests (interest, this, m_face);
  m_face->ReceiveInterest (interest);


  ScheduleNextPacket ();
}



void
MyConsumerBatches::OnData (Ptr<const Data> data)
{
  if (!m_active) return;
  //cout<<" On A Data!"<<endl;

  Simulator::Cancel (m_sendEvent);
  Simulator::Cancel (m_resendEvent);
  
  Consumer::OnData (data);

  //cout<<"fuck"<<endl;
  

  m_resendEvent = Simulator::Schedule (Seconds(2.0), &Consumer::SendPacket, this);

}
void
MyConsumerBatches::SetRetxTimer (Time retxTimer)
{

}

Time
MyConsumerBatches::GetRetxTimer () const
{
  Time t = Seconds (0);
  return t;
}

void
MyConsumerBatches::CheckRetxTimeout ()
{
  
}
void
MyConsumerBatches::OnTimeout (uint32_t sequenceNumber)
{
}


} // namespace ndn
} // namespace ns3

#endif
