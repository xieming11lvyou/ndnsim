#include "ns3/ndn-forwarding-strategy.h"
#include "ns3/ndn-name.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndnSIM/model/pit/ndn-pit-impl.h"
#include <iomanip>
#include <boost/foreach.hpp>
#include "a-ndn-simple-test.h"

using namespace std;
using namespace pit;

NS_LOG_COMPONENT_DEFINE ("ndn.fw.MyFw");


namespace ns3 {
namespace ndn {
namespace fw {


class MyFw : public ForwardingStrategy
{
public:
	static TypeId GetTypeId ();
	MyFw();
	void OnInterest (Ptr<Face> inFace,Ptr<Interest> interest);
	void DoOnInterest (Ptr<Face> inFace,Ptr<Interest> interest);
	void Delay (Ptr<Face> inFace,Ptr<Interest> interest);
	void OnData (Ptr<Face> inFace,Ptr<Data> data);
	// void DoOnData (Ptr<Face> inFace,Ptr<Data> data);
	bool CanSendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry);

  	bool DoPropagateInterest (Ptr<Face> inFace,
                       		  Ptr<const Interest> interest,
                              Ptr<pit::Entry> pitEntry);
  	bool TrySendOutInterest (Ptr<Face> inFace,
                             Ptr<Face> outFace,
                             Ptr<const Interest> interest,
                             Ptr<pit::Entry> pitEntry);  	

  	bool DoTrySendOutInterest (Ptr<Face> inFace,
                             Ptr<Face> outFace,
                             Ptr<const Interest> interest,
                             Ptr<pit::Entry> pitEntry);
  	void RandSeq (uint32_t num, uint32_t arr[]);


private:
	uint32_t m_seq;
	static const int m_size = 10000;
	//Ptr<Interest> m_interest;
	Ptr<Name> nameWithSequence[m_size];

	//Ptr<Name> firstNameWithSequence[m_size];
	uint32_t m_array[m_size];

	//uint32_t first_array[m_size];
	bool firstTime;

	uint32_t dataNum;
	uint32_t interestNum;

	//Name perReceived[m_size];

};

NS_OBJECT_ENSURE_REGISTERED (MyFw);
//-------------------------------------------------
TypeId
MyFw::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::MyFw")
    .SetParent<ForwardingStrategy> ()
    .SetGroupName ("Ndn")
    .AddConstructor<MyFw> ();
  return tid;
}

MyFw::MyFw():m_seq(0), dataNum(0), interestNum(0)
{	


	//cout<<"Constructor!"<<endl;
	Name interestName("/Prefix");
	for (int i = 0; i<m_size; i++)
	{
		nameWithSequence[i] = Create<Name> (interestName);
		m_array[i] = i;
	}
	RandSeq (m_size-1, m_array);

	// for (int i = 0; i<m_size; i++)
	// {
	// 	firstNameWithSequence[i] = Create<Name> (interestName);
	// 	first_array[i] = m_size-1-i;
	// }

	firstTime = true;
}


void
MyFw::OnInterest (Ptr<Face> inFace,
                  Ptr<Interest> interest)
{	
	// interest->Print(cout);
	// cout<<endl;
	//ForwardingStrategy::OnInterest (inFace, interest);
	
	//cout<<endl;
	//cout<<"asd";
	// Ptr<Node> node = inFace->GetNode();
	// uint32_t i = node->GetId();


 //  Ptr<Name> firstName = Create<Name> ("/Prefix/");
 //  //Name firstName("/Prefix/");
 //  firstName->appendSeqNum(0);
 //  if (i==0||interest->GetName () == *firstName)
 //  {	


	// cout<<"On Interest " <<i<<endl;
	
 	
	//m_interest = interest;
	
	//Delay (inFace, interest);
  Simulator::Schedule(Seconds(0.0), &MyFw::Delay, this, inFace, interest);
  // }
	
}

void
MyFw::Delay (Ptr<Face> inFace,
             Ptr<Interest> interest)
{	
	Ptr<Node> node = inFace->GetNode();
  	uint32_t i = node->GetId();

  	if (i==0 && firstTime==false)
  	{	
  		//cout<<"Delay"<<endl;
  		UniformVariable m_rand (0, std::numeric_limits<uint32_t>::max ());
		Ptr<Interest> t_interest = Create<Interest>();
		t_interest = interest;
		nameWithSequence[0]->appendSeqNum(0);
		t_interest->SetName(nameWithSequence[0]);
		t_interest->SetNonce(m_rand.GetValue ());
  		
  		DoOnInterest(inFace, t_interest);
		Name interestName("/Prefix");
		nameWithSequence[0] = Create<Name> (interestName);

  	}

  // 	else if (i==0 && firstTime==true)
  // 	{
 	// 	UniformVariable m_rand (0, std::numeric_limits<uint32_t>::max ());

		// //cout<<"m_seq"<< m_seq<<endl;
		// Ptr<Interest> t_interest = Create<Interest>();
		
		// t_interest = interest;

		// firstNameWithSequence[m_seq]->appendSeqNum (first_array[m_seq]);
		// //cout<<*firstNameWithSequence[m_seq];
		// t_interest->SetName(firstNameWithSequence[m_seq]);
		// t_interest->SetNonce(m_rand.GetValue ());
	 //    //t_interest->Print(cout);
	 //    //cout<<" OnInterest"<<" the node is "<<i<<" m_seq "<<m_array[m_seq]<<endl;

	 //    DoOnInterest(inFace, t_interest);

	 //    m_seq++;

	 //    if (m_seq < m_size)
		// {	
		
		//     if (i==0)
		// 		Simulator::Schedule(Seconds(0.0), &MyFw::Delay, this, inFace, t_interest);
		// 	else	
		// 		Simulator::Schedule(Seconds(0.01), &MyFw::Delay, this, inFace, t_interest);//0.1s
		// 		//DoOnInterest (inFace, t_interest);
		// }
		// else if (m_seq == m_size)
		// {
		// 	if (i==0)
		// 	{	m_seq = 0;
		// 		firstTime = false;
		// 		Name interestName("/Prefix");
		// 		for (int i = 0; i<m_size; i++)
		// 		{
		// 			nameWithSequence[i] = Create<Name> (interestName);
		// 			m_array[i] = i;
		// 		}
		// 		RandSeq (m_size-1, m_array);
		// 	}
		// }
	
  // 	}

  	else
  	{

		UniformVariable m_rand (0, std::numeric_limits<uint32_t>::max ());

		//cout<<"m_seq"<< m_seq<<endl;
		Ptr<Interest> t_interest = Create<Interest>();
		
		t_interest = interest;

		nameWithSequence[m_seq]->appendSeqNum (m_array[m_seq]);
		t_interest->SetName(nameWithSequence[m_seq]);
		t_interest->SetNonce(m_rand.GetValue ());
	    //t_interest->Print(cout);
	    //cout<<" OnInterest"<<" the node is "<<i<<" m_seq "<<m_array[m_seq]<<endl;

	    DoOnInterest(inFace, t_interest);

      //cout<<m_seq;
	    m_seq++;

	    if (m_seq < m_size)
		{	
		
		    if (i==0)
				Simulator::Schedule(Seconds(0.0), &MyFw::Delay, this, inFace, t_interest);
			else	
				Simulator::Schedule(Seconds(0.01), &MyFw::Delay, this, inFace, t_interest);//0.1s
				//DoOnInterest (inFace, t_interest);
		}
		else if (m_seq == m_size)
		{
			if (i==0)
			{	firstTime = false;
      }
        m_seq = 0;
				
				Name interestName("/Prefix");
				for (int i = 0; i<m_size; i++)
				{
					nameWithSequence[i] = Create<Name> (interestName);
					m_array[i] = i;
				}
				RandSeq (m_size-1, m_array);

        Simulator::Schedule(Seconds(0.01), &MyFw::Delay, this, inFace, t_interest);

			

		}
	}
	

}


void 
MyFw::DoOnInterest (Ptr<Face> inFace,
                  Ptr<Interest> interest)
{
 // NS_LOG_FUNCTION (inFace << interest->GetName ());
 // m_inInterests (interest, inFace);

	//cout<<"START!";
  Ptr<pit::Entry> pitEntry = m_pit->Lookup (*interest);
  bool similarInterest = true;
  if (pitEntry == 0)
    {
      similarInterest = false;
      pitEntry = m_pit->Create (interest);
      //cout<<"Pit "<< pitEntry -> GetPrefix();
      if (pitEntry != 0)
        {	
        //  cout<<"YES!";
          ForwardingStrategy::DidCreatePitEntry (inFace, interest, pitEntry);
        }
      else
        {
       //   cout<<"no"<<endl;
          ForwardingStrategy::FailedToCreatePitEntry (inFace, interest);
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
    //  cout<<"DID";
      ForwardingStrategy::DidReceiveDuplicateInterest (inFace, interest, pitEntry);
      return;
    }

  Ptr<Data> contentObject;
  contentObject = m_contentStore->Lookup (interest);
  //if (i==0 && firstTime==true)
 // {
//  	firstTime = false;
  if (contentObject != 0)
    {
    //  cout<<"oh!"<<endl;
      FwHopCountTag hopCountTag;
      if (interest->GetPayload ()->PeekPacketTag (hopCountTag))
        {
          contentObject->GetPayload ()->AddPacketTag (hopCountTag);
        }

      pitEntry->AddIncoming (inFace/*, Seconds (1.0)*/);

      // Do data plane performance measurements
      ForwardingStrategy::WillSatisfyPendingInterest (0, pitEntry);

      // Actually satisfy pending interest
      ForwardingStrategy::SatisfyPendingInterest (0, contentObject, pitEntry);
      return;
    }
   //}

  if (similarInterest && ForwardingStrategy::ShouldSuppressIncomingInterest (inFace, interest, pitEntry))
    {
      pitEntry->AddIncoming (inFace/*, interest->GetInterestLifetime ()*/);
      // update PIT entry lifetime
      pitEntry->UpdateLifetime (interest->GetInterestLifetime ());

      // Suppress this interest if we're still expecting data from some other face
     // cout<<"Suppress interests";
      m_dropInterests (interest, inFace);

     // cout<<"Suppress";

      ForwardingStrategy::DidSuppressSimilarInterest (inFace, interest, pitEntry);
      return;
    }

  if (similarInterest)
    {	
    	
     //cout<<"Find";
     //cout<<"Find a similarInterest"<<endl;
      ForwardingStrategy::DidForwardSimilarInterest (inFace, interest, pitEntry);
    }
 ///cout<<"DO!";

  ForwardingStrategy::PropagateInterest (inFace, interest, pitEntry);

}

void
MyFw::OnData (Ptr<Face> inFace, Ptr<Data> data)
{	
	Ptr<Node> node = inFace->GetNode();
	uint32_t i = node->GetId();
	//cout<<"On Data "<<"Node "<<i<<endl;
	//cout<<m_pit<<endl;
	if (i==0)
	{	
		dataNum++;
    	cout<<Simulator::Now().GetSeconds()<<" ";
    	cout<<(double)(m_size-m_pit->GetSize())*100/(double)m_size<<" ";
    	cout<<interestNum<<" ";
    	cout<<dataNum<<endl;
  //   cout<<node->GetObject<ns3::ndn::MyNetDeviceFace>()->interestNum<<" ";
  //   cout<<node->GetObject<ns3::ndn::MyNetDeviceFace>()->m_number<<" "<<endl;
		// //cout<<m_pit->GetSize()<<" ";
		if (m_pit->GetSize() < m_size*0.05)
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
MyFw::DoPropagateInterest (Ptr<Face> inFace,
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
 		
      if (!MyFw::TrySendOutInterest (inFace, metricFace.GetFace (), interest, pitEntry))
        {
          continue;
        }

      propagatedCount++;
      break; // propagate only one interest
    }

  return propagatedCount > 0;
}

bool 
MyFw::TrySendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry)
{
	Ptr<Node> node = inFace->GetNode();
  	uint32_t i = node->GetId();
  	//cout<<"the node is "<<i<<endl;
  	if (i==0)
  	{
  		return DoTrySendOutInterest (inFace, outFace, interest, pitEntry);
  	}
  	else
  	{
  		return DoTrySendOutInterest (inFace, outFace, interest, pitEntry);//Simulator::ScheduleNow (&MyFw::DoTrySendOutInterest, this, inFace, outFace, pitEntry);
  	}
	
}







bool
MyFw::DoTrySendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry)
{	
  
  //cout<<inFace<<"  "<<outFace<<endl;
  Ptr<Node> node = inFace->GetNode();
  uint32_t i = node->GetId();
  // cout<<"the node is "<<i<<endl;

  if (!CanSendOutInterest (inFace, outFace, interest, pitEntry))
    {
      
      return false;
    }

  // cout<<inFace<<"    "<<outFace;
  // cout<<endl;
  pitEntry->AddOutgoing (outFace);



  Ptr<Name> firstName = Create<Name> ("/Prefix/");
  //Name firstName("/Prefix/");
  firstName->appendSeqNum(0);

  //cout<<"Node "<<i<<" "<<m_pit->GetSize()<<endl;


  //Ptr<Name> interestName = Create<Name> (interest->GetName ());
  // cout<<*interestName<<endl;
  // cout<<*firstName<<" "<<interest->GetName ();
  // cout<<(firstName==*interestName)<<endl;//"real send"<<*interestName;

  if (i==0)
  {

	  if (interest->GetName () == *firstName)
	  {	
	  	interestNum++;

	  //firstName.rbegin();
	  	//cout<<"At: "<<Simulator::Now()<<" Pit: "<<m_pit->GetSize()<<endl;
	  	//cout<<"real send"<<*interestName;
	  	bool successSend = outFace->SendInterest (interest);
	  	if (!successSend)
	    {
	      m_dropInterests (interest, outFace);
	    }

	   // cout<<m_pit->GetSize();

	    ForwardingStrategy::DidSendOutInterest (inFace, outFace, interest, pitEntry);

	    return true;

	  }
	  else
	  	//cout<<"not equal"<<endl;
	  	//firstName.rbegin();
	  	return false;
   }
   else
   {	

	  	bool successSend = outFace->SendInterest (interest);
	  	if (!successSend)
	    {	
	      m_dropInterests (interest, outFace);
	    }
	    ForwardingStrategy::DidSendOutInterest (inFace, outFace, interest, pitEntry);

	    //Simulator::Schedule (Seconds(0.1), &MyFw::DoTrySendOutInterest, this, inFace, outFace, pitEntry);
	    return true;
	}



  //transmission


}

bool
MyFw::CanSendOutInterest (Ptr<Face> inFace,
                                        Ptr<Face> outFace,
                                        Ptr<const Interest> interest,
                                        Ptr<pit::Entry> pitEntry)
{
  if (outFace == inFace)
    {
      // NS_LOG_DEBUG ("Same as incoming");
      //cout<<"same"<<endl;
      return false; // same face as incoming, don't forward
    }

  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outFace);

  if (outgoing != pitEntry->GetOutgoing ().end ())
    {
      if (!m_detectRetransmissions)
        return false; // suppress
      else if (outgoing->m_retxCount >= pitEntry->GetMaxRetxCount ())
        {
          // NS_LOG_DEBUG ("Already forwarded before during this retransmission cycle (" <<outgoing->m_retxCount << " >= " << pitEntry->GetMaxRetxCount () << ")");
          return false; // already forwarded before during this retransmission cycle
        }
   }

  return true;
}

void 
MyFw::RandSeq (uint32_t n,uint32_t arr[])
{
	 uint32_t i;
	 uint32_t p;
	 uint32_t tmp;

	 for (i=n;i>0;i--)
	 {
		  UniformVariable m_rand1;
		  p = m_rand1.GetInteger(1,i);
		  //cout<<"rand: "<<" "<<0<<" "<<i<<endl;
		  //cout<<p<<endl;
		  tmp=arr[p];
		  arr[p]=arr[i];
		  arr[i]=tmp;
		  //cout<<endl;
	 }
}


}
}
}