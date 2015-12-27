

#include "ns3/ndnSIM/apps/ndn-producer.h"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

#include "a-ndn-tag.h"

using namespace std;
using namespace ns3;
using namespace ndn;

class MyProducer : public Producer
{
public:
  static TypeId
  GetTypeId (void);

  MyProducer ();

  // inherited from NdnApp
  void OnInterest (Ptr<const Interest> interest);
  void ScheduleNextPacket (Ptr<const Interest> interest);

  //void ScheduleNextPacket ();

private:
  uint32_t m_objectSize;
  uint32_t m_seq;
  Name m_prefix;

  Name m_postfix;
  uint32_t m_virtualPayloadSize;
  Time m_freshness;

  uint32_t m_signature;
  Name m_keyLocator;


};

//---------------------------------implements--------------------------
TypeId
MyProducer::GetTypeId (void)
{
  static TypeId tid = TypeId ("MyProducer")
    .SetParent<Producer> ()
    .AddConstructor<MyProducer> ()

    .AddAttribute ("ObjectSize","OZ,the size of the obejct",
                   UintegerValue (10),
                   MakeUintegerAccessor (&MyProducer::m_objectSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MyPrefix","Prefix, for which producer has the data",
                   StringValue ("/"),
                   MakeNameAccessor (&MyProducer::m_prefix),
                   MakeNameChecker ())
    .AddAttribute ("MyPostfix", "Postfix that is added to the output data (e.g., for adding producer-uniqueness)",
                   StringValue ("/"),
                   MakeNameAccessor (&MyProducer::m_postfix),
                   MakeNameChecker ())
    .AddAttribute ("MyPayloadSize", "Virtual payload size for Content packets",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&MyProducer::m_virtualPayloadSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MyFreshness", "Freshness of data packets, if 0, then unlimited freshness",
                   TimeValue (Seconds (0)),
                   MakeTimeAccessor (&MyProducer::m_freshness),
                   MakeTimeChecker ())
    .AddAttribute ("MySignature", "Fake signature, 0 valid signature (default), other values application-specific",
                   UintegerValue (0),
                   MakeUintegerAccessor (&MyProducer::m_signature),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MyKeyLocator", "Name to be used for key locator.  If root, then key locator is not used",
                   NameValue (),
                   MakeNameAccessor (&MyProducer::m_keyLocator),
                   MakeNameChecker ())
    ;
  return tid;
}

MyProducer::MyProducer():m_seq(0)
{

}

void
MyProducer::OnInterest (Ptr<const Interest> interest)
{
  App::OnInterest (interest); // tracing inside

  if (!m_active) return;

  // Ptr<const Interest> m_interest = interest;

  Simulator::ScheduleNow (&MyProducer::ScheduleNextPacket,this,interest);
}

void
MyProducer::ScheduleNextPacket (Ptr<const Interest> interest)
{ 
  //cout<<"haha"<<m_seq<<m_objectSize<<endl;

  Ptr<Data> data = Create<Data> (Create<Packet> (m_virtualPayloadSize));
  // if(m_seq != -1)
  // {
 
  Name interestName("/Prefix");
  //Ptr<Name> nameWithSequence = Create<Name> (interestName);

  Ptr<Name> dataName = Create<Name> (interestName);

  dataName->appendSeqNum (m_seq);
  dataName->append (m_postfix);
  data->SetName (dataName);
  data->SetFreshness (m_freshness);
  data->SetTimestamp (Simulator::Now());

  data->SetSignature (m_signature);
  if (m_keyLocator.size () > 0)
    {
      data->SetKeyLocator (Create<Name> (m_keyLocator));
    }

  //NS_LOG_INFO ("node("<< GetNode()->GetId() <<") respodning with Data: " << data->GetName ());

  // Echo back FwHopCountTag if exists
  FwHopCountTag hopCountTag;
  if (interest->GetPayload ()->PeekPacketTag (hopCountTag))
    {
      data->GetPayload ()->AddPacketTag (hopCountTag);
    }

  ObjectSizeTag objectSizeTag;
  objectSizeTag.SetSize(m_objectSize);
  data->GetPayload ()->AddPacketTag (objectSizeTag);

  data->Print(cout);
  cout<<endl;


  m_face->ReceiveData (data);
  m_transmittedDatas (data, this, m_face);

//  }
  m_seq++;

  if(m_seq<=m_objectSize)
  {
    Simulator::Schedule (Seconds(0.1),&MyProducer::ScheduleNextPacket,this,interest);
     // ScheduleNextPacket();
  }

}