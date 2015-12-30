#include "ns3/ndn-forwarding-strategy.h"
#include "ns3/ndn-name.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndnSIM/model/pit/ndn-pit-impl.h"
#include <iomanip>
#include <boost/foreach.hpp>
// #include "a-ndn-simple-test.h"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

using namespace std;

namespace ns3 {
namespace ndn {
namespace fw {


class ndnLce : public ForwardingStrategy
{
public:
	static TypeId GetTypeId ();
	ndnLce();
 void OnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest);
 void DoOnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest);
	void OnData (Ptr<Face> inFace,Ptr<Data> data);
	// void DoOnData (Ptr<Face> inFace,Ptr<Data> data);
  void SatisfyPendingInterest (Ptr<Face> inFace,
                                            Ptr<const Data> data,
                                            Ptr<pit::Entry> pitEntry);

  bool DoPropagateInterest (Ptr<Face> inFace,
                       		  Ptr<const Interest> interest,
                            Ptr<pit::Entry> pitEntry);

  bool TrySendOutInterest (Ptr<Face> inFace,
                          Ptr<Face> outFace,
                          Ptr<const Interest> interest,
                          Ptr<pit::Entry> pitEntry);
  void DidReceiveSolicitedData (Ptr<Face> inFace,
                                               Ptr<const Data> data,
                                               bool didCreateCacheEntry);
  void DidReceiveUnsolicitedData (Ptr<Face> inFace,
                                               Ptr<const Data> data,
                                               bool didCreateCacheEntry);
  bool CanSendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry);

private:

	static const int m_size = 10000;
	//Ptr<Interest> m_interest;

	uint32_t dataNum;
	uint32_t interestNum;
	uint32_t consumerID;

	//Name perReceived[m_size];

};

NS_OBJECT_ENSURE_REGISTERED (ndnLce);
//-------------------------------------------------
TypeId
ndnLce::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::ndnLce")
    .SetParent<ForwardingStrategy> ()
    .SetGroupName ("Ndn")
    .AddConstructor<ndnLce> ()
    .AddAttribute ("ID",
                   "ID",
                   UintegerValue (0),
                   MakeUintegerAccessor (&ndnLce::consumerID),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

ndnLce::ndnLce():dataNum(0), interestNum(0)
{	
}

void
ndnLce::DoOnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest)
{
  CacheTag cacheTag;
  if (interest->GetPayload()->PeekPacketTag(cacheTag))
      return;


  Ptr<pit::Entry> pitEntry = m_pit->Lookup (*interest);
  bool similarInterest = true;
  if (pitEntry == 0)
    {
      similarInterest = false;

      pitEntry = m_pit->Create (interest);
      if (pitEntry != 0)
        {
          DidCreatePitEntry (inFace, interest, pitEntry);
        }
      else
        { 

          FailedToCreatePitEntry (inFace, interest);
          return;
        }
    }

  bool isDuplicated = true;
  if (!pitEntry->IsNonceSeen (interest->GetNonce ()))
    {
      pitEntry->AddSeenNonce (interest->GetNonce ());
      isDuplicated = false;
    }

  Ptr<Data> contentObject;
  contentObject = m_contentStore->Lookup (interest);
  if (contentObject != 0)
    { 
      //cout<<"NODE "<<i<<" NO CONTENT"<<endl;
      FwHopCountTag hopCountTag;
      if (interest->GetPayload ()->PeekPacketTag (hopCountTag))
        {
          contentObject->GetPayload ()->AddPacketTag (hopCountTag);
        }

    SourceTag sourceTag;
   if (interest->GetPayload ()->PeekPacketTag (sourceTag))
  {
    contentObject->GetPayload ()->AddPacketTag (sourceTag);
  }

   IndexTag indexTag;
   if (interest->GetPayload ()->PeekPacketTag (indexTag) ){
    contentObject->GetPayload ()->AddPacketTag (indexTag);
  }

      pitEntry->AddIncoming (inFace/*, Seconds (1.0)*/);

  Ptr<Node> node = inFace->GetNode();
  uint32_t i = node->GetId();
      std::cout<<"hittttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt---------------------"<<i<<std::endl;

      SatisfyPendingInterest (0, contentObject, pitEntry);
      return;
    }

  if (similarInterest && ShouldSuppressIncomingInterest (inFace, interest, pitEntry))
    {
      pitEntry->AddIncoming (inFace/*, interest->GetInterestLifetime ()*/);
      pitEntry->UpdateLifetime (interest->GetInterestLifetime ());

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
ndnLce::SatisfyPendingInterest (Ptr<Face> inFace,
                                            Ptr<const Data> data,
                                            Ptr<pit::Entry> pitEntry)
{
  if (inFace != 0)
    pitEntry->RemoveIncoming (inFace);
  //satisfy all pending incoming Interests
  BOOST_FOREACH (const pit::IncomingFace &incoming, pitEntry->GetIncoming ())
    {
      bool ok = incoming.m_face->SendData (data);

      DidSendOutData (inFace, incoming.m_face, data, pitEntry);
      NS_LOG_DEBUG ("Satisfy " << *incoming.m_face);

      if (!ok)
        {
          m_dropData (data, incoming.m_face);
          NS_LOG_DEBUG ("Cannot satisfy data to " << *incoming.m_face);
        }
    }

  // All incoming interests are satisfied. Remove them
  pitEntry->ClearIncoming ();

  // Remove all outgoing faces
  pitEntry->ClearOutgoing ();

  // Set pruning timout on PIT entry (instead of deleting the record)
//  m_pit->MarkErased (pitEntry);
}


void
ndnLce::OnInterest (Ptr<Face> inFace,
                                Ptr<Interest> interest)
{
 
  //cout<<"NODE "<<i<<" SEND"<<endl;

  DoOnInterest(inFace, interest);
}

void
ndnLce::DidReceiveUnsolicitedData (Ptr<Face> inFace,
                                               Ptr<const Data> data,
                                               bool didCreateCacheEntry)
{
  Ptr<Node> node = inFace->GetNode();
  uint32_t i = node->GetId();
  if (i!=consumerID)
  {
  inFace->SendData (data);
  }
}

void
ndnLce::DidReceiveSolicitedData (Ptr<Face> inFace,
                                               Ptr<const Data> data,
                                               bool didCreateCacheEntry)
{
  Ptr<Node> node = inFace->GetNode();
  uint32_t i = node->GetId();
  if (i!=consumerID)
  {
  inFace->SendData (data);
  }
}



void
ndnLce::OnData (Ptr<Face> inFace, Ptr<Data> data)
{	
  CacheTag cacheTag;
  if (!data->GetPayload()->PeekPacketTag(cacheTag))
  {
  Ptr<pit::Entry> pitEntry = m_pit->Lookup (*data);
  DidReceiveSolicitedData (inFace, data, true);
//  bool cached = m_contentStore->Add (data);
  while (pitEntry != 0)
    {
      // Do data plane performance measurements
      WillSatisfyPendingInterest (inFace, pitEntry);

      // Actually satisfy pending interest
      SatisfyPendingInterest (inFace, data, pitEntry);

      // Lookup another PIT entry
      pitEntry = m_pit->Lookup (*data);
    }
  }else {
      Ptr<Packet> copy =  data->GetPayload()->Copy();
      copy->RemoveAllPacketTags();
      data->SetPayload(copy);
      bool cached = m_contentStore->Add (data);
  }
}


bool
ndnLce::TrySendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry)
{
  if (!CanSendOutInterest (inFace, outFace, interest, pitEntry))
    { 
      cout<<"false!!";
      return false;
    }
  // Ptr<Node> node = inFace->GetNode();
  // uint32_t i = node->GetId();
  // cout<<"NODE "<<i<<" TRY SEND"<<endl;

  pitEntry->AddOutgoing (outFace);

  //transmission--------------------------

  //transmission---------------------------
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
ndnLce::CanSendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry)
{
  // if (outFace == inFace)
  //   {
  //     // NS_LOG_DEBUG ("Same as incoming");
  //     //cout<<"SANME"<<endl;
  //     return false; // same face as incoming, don't forward
  //   }

  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outFace);

  if (outgoing != pitEntry->GetOutgoing ().end ())
    {
      if (!m_detectRetransmissions)
       {cout<<"same"<<endl;
        return false; // suppress
      }
      else if (outgoing->m_retxCount >= pitEntry->GetMaxRetxCount ())
        {
  //        cout<<"same2"<<endl;
          // NS_LOG_DEBUG ("Already forwarded before during this retransmission cycle (" <<outgoing->m_retxCount << " >= " << pitEntry->GetMaxRetxCount () << ")");
  //        return false; // already forwarded before during this retransmission cycle
        }
   }

  return true;
}


bool
ndnLce::DoPropagateInterest (Ptr<Face> inFace,
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
