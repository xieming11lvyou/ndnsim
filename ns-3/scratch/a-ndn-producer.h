#ifndef A_NDN_PRODUCER_H
#define A_NDN_PRODUCER_H

#include "ns3/ndnSIM/apps/ndn-producer.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/traced-value.h"
#include "a-ndn-tag.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * @brief A simple Interest-sink applia simple Interest-sink application
 *
 * A simple Interest-sink applia simple Interest-sink application,
 * which replying every incoming Interest with Data packet with a specified
 * size and name same as in Interest.cation, which replying every incoming Interest
 * with Data packet with a specified size and name same as in Interest.
 */
class V2VProducer : public Producer
{
public:
  static TypeId
  GetTypeId (void);

  V2VProducer ();
  void OnInterest (Ptr<const Interest> interest);

};

NS_OBJECT_ENSURE_REGISTERED (V2VProducer);

TypeId
V2VProducer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::V2VProducer")
    .SetGroupName ("Ndn")
    .SetParent<Producer> ()
    .AddConstructor<V2VProducer> ();
  return tid;
}

V2VProducer::V2VProducer ()
{
  // NS_LOG_FUNCTION_NOARGS ();
}

void
V2VProducer::OnInterest (Ptr<const Interest> interest)
{
	Producer::OnInterest(interest);

}



} // namespace ndn
} // namespace ns3

#endif
