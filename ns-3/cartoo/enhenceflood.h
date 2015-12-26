/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */

// custom-strategy.h
        
#ifndef ENHENCE_FLOOD_H
#define ENHENCE_FLOOD_H

#include "ns3/log.h"
#include "ns3/ndn-forwarding-strategy.h"
#include "ns3/ndn-l3-protocol.h"
                
namespace ns3 {
namespace ndn {
namespace fw {

class EnhenceFlood:
    public ForwardingStrategy
{
public:
  static TypeId
  GetTypeId ();

  EnhenceFlood ();
  ~EnhenceFlood ();
protected:
  virtual bool
  DoPropagateInterest (Ptr<Face> incomingFace,
                       Ptr<const Interest> interest,
                       Ptr<pit::Entry> pitEntry);

private:
  uint32_t m_counter;
};
        
        
        
} // namespace fw
} // namespace ndn
} // namespace ns3
                
#endif // CUSTOM_STRATEGY_H

