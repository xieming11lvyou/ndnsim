/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
// custom-strategy.cc

#include "ns3/ndn-fib.h"
#include "ns3/ndn-fib-entry.h"
#include "ns3/ndn-pit-entry.h"
#include "ns3/ndn-interest.h"
#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ns3 {
namespace ndn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED(EnhenceFlood);

TypeId
EnHenceFlood::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::EnHenceFlood")
    .SetGroupName ("Ndn")
    .SetParent <ForwardingStrategy> ()
    .AddConstructor <EnHenceFlood> ()

    // .AddAttribute ("Attribute", "Attribute spec",
    //                         StringValue ("DefaultValue"),
    //                         MakeStringAccessor (&BaseStrategy::m_variable),
    //                         MakeStringChecker ())
    ;
  return tid;
}

EnHenceFlood::EnHenceFlood ()
{
  m_count = 0;
}

EnHenceFlood::~EnHenceFlood ()
{
}
bool
EnHenceFlood::DoPropagateInterest (Ptr<Face> inFace,
                                     Ptr<const Interest> interest,
                                     Ptr<pit::Entry> pitEntry)
{

  int propagatedCount = 0;

  BOOST_FOREACH (const fib::FaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<fib::i_metric> ())
    {
      NS_LOG_DEBUG ("Trying " << boost::cref(metricFace));
      if (metricFace.GetStatus () == fib::FaceMetric::NDN_FIB_RED) // all non-read faces are in the front of the list
        break;

      if (!TrySendOutInterest (inFace, metricFace.GetFace (), interest, pitEntry))
        {
          continue;
        }

      propagatedCount++;
    }

  std::cout<<"Propagated to " << propagatedCount << " faces"<<std::endl;
  return propagatedCount > 0;
}





} // namespace fw
} // namespace ndn
} // namespace ns3
