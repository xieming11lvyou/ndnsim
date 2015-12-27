#include "ns3/ndn-forwarding-strategy.h"
#include "ns3/ndn-name.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndnSIM/model/pit/ndn-pit-impl.h"
#include <iomanip>
#include <boost/foreach.hpp>
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

using namespace std;

namespace ns3 {
namespace ndn {
namespace fw {


class ndnFw : public ForwardingStrategy
{
public:
        static TypeId GetTypeId ();
        ndnFw();
 void OnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest);
 void DoOnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest);
        void OnData (Ptr<Face> inFace,Ptr<Data> data);
        // void DoOnData (Ptr<Face> inFace,Ptr<Data> data);

  bool DoPropagateInterest (Ptr<Face> inFace,
                                  Ptr<const Interest> interest,
                            Ptr<pit::Entry> pitEntry);

  bool TrySendOutInterest (Ptr<Face> inFace,
                          Ptr<Face> outFace,
                          Ptr<const Interest> interest,
                          Ptr<pit::Entry> pitEntry);

  void DidReceiveUnsolicitedData (Ptr<Face> inFace,
                                               Ptr<const Data> data,
                                               bool didCreateCacheEntry);

private:

        static const int m_size = 10000;
        //Ptr<Interest> m_interest;

        uint32_t dataNum;
        uint32_t interestNum;

        //Name perReceived[m_size];

};

NS_OBJECT_ENSURE_REGISTERED (ndnFw);
//-------------------------------------------------
TypeId
ndnFw::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::ndnFw")
    .SetParent<ForwardingStrategy> ()
    .SetGroupName ("Ndn")
    .AddConstructor<ndnFw> ();
  return tid;
}

ndnFw::ndnFw():dataNum(0), interestNum(0)
{

}

void
ndnFw::DoOnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest)
{

        Ptr<Node> node = inFace->GetNode();
        uint32_t i = node->GetId();

  Ptr<pit::Entry> pitEntry = m_pit->Lookup (*interest);
  bool similarInterest = true;
  if (pitEntry == 0)
    {
      similarInterest = false;

        cout<<"NODE "<<i<<" Create!"<<endl;
      pitEntry = m_pit->Create (interest);
      if (pitEntry != 0)
        {
          DidCreatePitEntry (inFace, interest, pitEntry);
          cout<<"DID";
        }
      else
        {

          FailedToCreatePitEntry (inFace, interest);
          cout<<"FAILED";
          return;
        }
    }

  bool isDuplicated = true;
  if (!pitEntry->IsNonceSeen (interest->GetNonce ()))
    {
      pitEntry->AddSeenNonce (interest->GetNonce ());
      isDuplicated = false;
    }

  if (isDuplicated)
    {
      DidReceiveDuplicateInterest (inFace, interest, pitEntry);
      return;
    }

  Ptr<Data> contentObject;
  cout<<"!!!"<<endl;
  contentObject = m_contentStore->Lookup (interest);
  if (contentObject != 0)
    {
      cout<<"NODE "<<i<<" NO CONTENT"<<endl;
      FwHopCountTag hopCountTag;
      if (interest->GetPayload ()->PeekPacketTag (hopCountTag))
        {
          contentObject->GetPayload ()->AddPacketTag (hopCountTag);
        }

      pitEntry->AddIncoming (inFace/*, Seconds (1.0)*/);

      // Do data plane performance measurements
      WillSatisfyPendingInterest (0, pitEntry);

      // Actually satisfy pending interest
      SatisfyPendingInterest (0, contentObject, pitEntry);
      return;
    }
     else
     cout<<"NODE "<<i<<" CONTENT"<<endl;

  if (similarInterest && ShouldSuppressIncomingInterest (inFace, interest, pitEntry))
    {
      pitEntry->AddIncoming (inFace/*, interest->GetInterestLifetime ()*/);
      // update PIT entry lifetime
      pitEntry->UpdateLifetime (interest->GetInterestLifetime ());

      // Suppress this interest if we're still expecting data from some other face
      //NS_LOG_DEBUG ("Suppress interests");
      m_dropInterests (interest, inFace);

      DidSuppressSimilarInterest (inFace, interest, pitEntry);
      return;
    }

  if (similarInterest)
    {
      DidForwardSimilarInterest (inFace, interest, pitEntry);
    }

  PropagateInterest (inFace, interest, pitEntry);
}







void
ndnFw::OnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest)
{
//  Ptr<Node> node = inFace->GetNode();
//  uint32_t i = node->GetId();
  //cout<<"NODE "<<i<<" SEND"<<endl;
  DoOnInterest(inFace, interest);
}
void
ndnFw::DidReceiveUnsolicitedData (Ptr<Face> inFace,
                                               Ptr<const Data> data,
                                               bool didCreateCacheEntry)
{
  // inFace->SendData (data);
}


void
ndnFw::OnData (Ptr<Face> inFace, Ptr<Data> data)
{
        Ptr<Node> node = inFace->GetNode();
        uint32_t i = node->GetId();
        //cout<<"On Data "<<"Node "<<i<<endl;
        //cout<<m_pit<<endl;
        if (i==0)
        {
                dataNum++;
        cout<<Simulator::Now().GetSeconds()<<" ";
        cout<<(double)(m_contentStore->GetSize())*100/m_size<<" ";
        cout<<interestNum<<" ";
        cout<<dataNum<<endl;
  //   cout<<node->GetObject<ns3::ndn::MyNetDeviceFace>()->interestNum<<" ";
  //   cout<<node->GetObject<ns3::ndn::MyNetDeviceFace>()->m_number<<" "<<endl;
                // //cout<<m_pit->GetSize()<<" ";
                if (m_contentStore->GetSize() >m_size-1)
                {
                        cout<<" Finished! ";

                        // Ptr<ns3::ndn::MyNetDeviceFace> face = node->GetObject<ns3::ndn::MyNetDeviceFace>();
                        // double ai = face->interestNum;
   //    cout<<ai;
   //    //cout<<" interestNum "<<face->interestNum<<endl;
      Simulator::Destroy ();
                }

                //cout<<setprecision(4)<<setiosflags(ios::fixed)

    //<<" Percentage "<<"% "<<endl;
        }
        ForwardingStrategy::OnData (inFace, data);
        //DoOnData(inFace, data);
}

// void
// MyFw::DoOnData (Ptr<Face> inFace, Ptr<Data> data)
// {

// }
bool
ndnFw::TrySendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry)
{
  // if (!CanSendOutInterest (inFace, outFace, interest, pitEntry))
  //   { 
  //     cout<<"false";
  //     return false;
  //   }
//  Ptr<Node> node = inFace->GetNode();
//  uint32_t i = node->GetId();
  //cout<<"NODE "<<i<<" TRY SEND"<<endl;

  pitEntry->AddOutgoing (outFace);

  //transmission
  bool successSend = outFace->SendInterest (interest);
  interestNum++;
  if (!successSend)
    {
      m_dropInterests (interest, outFace);
    }

  DidSendOutInterest (inFace, outFace, interest, pitEntry);

  return true;
}


bool
ndnFw::DoPropagateInterest (Ptr<Face> inFace,
                       Ptr<const Interest> interest,
                       Ptr<pit::Entry> pitEntry)
{
  //NS_LOG_FUNCTION (this);
 //cout<<endl;
//cout<<"do!"<<endl;
 int propagatedCount = 0;

  BOOST_FOREACH (const fib::FaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<fib::i_metric> ())
    {

        // interest->Print(cout);
     //  cout<<endl;
      if (metricFace.GetStatus () == fib::FaceMetric::NDN_FIB_RED)
      // ||
        //  metricFace.GetStatus () == fib::FaceMetric::NDN_FIB_YELLOW)
        break; //propagate only to green faces
      // interest->Print(cout);
      // cout<<endl;

      if (!TrySendOutInterest (inFace, metricFace.GetFace (), interest, pitEntry))
        {
          continue;
        }

      propagatedCount++;
      break; // propagate only one interest
    }

  return propagatedCount > 0;
}



}
}
}
