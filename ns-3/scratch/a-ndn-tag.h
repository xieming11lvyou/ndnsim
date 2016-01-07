#ifndef A_NDN_TAG_H_
#define A_NDN_TAG_H_

#include "ns3/tag.h"
#include "ns3/packet.h"

using namespace ns3;

class MobilityTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetMobility (double mobility);
  double GetMobility (void) const;

  void Print (std::ostream &os) const;

private:
  double m_mobility;

  // end class 
};


class SourceTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetSource (double mobility);
  double GetSource (void) const;

  void Print (std::ostream &os) const;

private:
  double m_source;

  // end class
};

//
class VelocityTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetVelocity (double velocity);
  double GetVelocity (void) const;

  void Print (std::ostream &os) const;

private:
  double m_velocity;

  // end class 
};

class ObjectSizeTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetSize (double size);
  double GetSize (void) const;

  void Print (std::ostream &os) const;

private:
  double m_objectSize;
};


class HopTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void UpdateHop ();
  void SetHop (uint32_t hop);
  uint32_t GetHop (void) const;

  void UpdateOverHead ();
  void SetOverHead (uint32_t hop);
  uint32_t GetOverHead (void) const;

  void Print (std::ostream &os) const;

private:
  uint32_t m_hop;
  uint32_t m_overHead;
};


//------------------------------------------------

class IndexTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetIndex (double mobility);
  double GetIndex (void) const;

  void Print (std::ostream &os) const;

private:
  double m_index;

  // end class
};

///////////////////////////////////////////////////////////
TypeId 
MobilityTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("MobilityTag")
    .SetParent<Tag> ()
    .AddConstructor<MobilityTag> ()
    // .AddAttribute ("Mobility",
    //                "The Mobility",
    //                EmptyAttributeValue (),
    //                MakeTimeAccessor (&TimestampTag::GetTimestamp),
    //                MakeTimeChecker ())
  ;
  return tid;
}

//--------------------------------------------------

//--------------------------------------------------
TypeId 
MobilityTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
MobilityTag::GetSerializedSize (void) const
{
  return 8;
}
void 
MobilityTag::Serialize (TagBuffer i) const
{
  double t = m_mobility;
  i.Write ((const uint8_t *)&t, 8);
}
void 
MobilityTag::Deserialize (TagBuffer i)
{
  double t;
  i.Read ((uint8_t *)&t, 8);
  m_mobility = t;
}

void
MobilityTag::SetMobility (double mobility)
{
  m_mobility = mobility;
}
double
MobilityTag::GetMobility (void) const
{
  return m_mobility;
}

void 
MobilityTag::Print (std::ostream &os) const
{
  os << "v=" << m_mobility;
}

//------------------------------------------------


TypeId
SourceTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("SourceTag")
    .SetParent<Tag> ()
    .AddConstructor<SourceTag> ()
    // .AddAttribute ("Mobility",
    //                "The Mobility",
    //                EmptyAttributeValue (),
    //                MakeTimeAccessor (&TimestampTag::GetTimestamp),
    //                MakeTimeChecker ())
  ;
  return tid;
}

TypeId
SourceTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
SourceTag::GetSerializedSize (void) const
{
  return 8;
}
void
SourceTag::Serialize (TagBuffer i) const
{
  double t = m_source;
  i.Write ((const uint8_t *)&t, 8);
}
void
SourceTag::Deserialize (TagBuffer i)
{
  double t;
  i.Read ((uint8_t *)&t, 8);
  m_source = t;
}

void
SourceTag::SetSource (double mobility)
{
  m_source = mobility;
}
double
SourceTag::GetSource (void) const
{
  return m_source;
}

void
SourceTag::Print (std::ostream &os) const
{
  os << "v=" << m_source;
}

////////////////////////////////////////////////////////////////------------------------------------------------



TypeId 
VelocityTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("VelocityTag")
    .SetParent<Tag> ()
    .AddConstructor<VelocityTag> ()
    // .AddAttribute ("Mobility",
    //                "The Mobility",
    //                EmptyAttributeValue (),
    //                MakeTimeAccessor (&TimestampTag::GetTimestamp),
    //                MakeTimeChecker ())
  ;
  return tid;
}
TypeId 
VelocityTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
VelocityTag::GetSerializedSize (void) const
{
  return 8;
}
void 
VelocityTag::Serialize (TagBuffer i) const
{
  double t = m_velocity;
  i.Write ((const uint8_t *)&t, 8);
}
void 
VelocityTag::Deserialize (TagBuffer i)
{
  double t;
  i.Read ((uint8_t *)&t, 8);
  m_velocity = t;
}

void
VelocityTag::SetVelocity (double velocity)
{
  m_velocity = velocity;
}
double
VelocityTag::GetVelocity (void) const
{
  return m_velocity;
}

void 
VelocityTag::Print (std::ostream &os) const
{
  os << "v=" << m_velocity;
}


//--------------------------------------------------
TypeId 
ObjectSizeTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ObjectSizeTag")
    .SetParent<Tag> ()
    .AddConstructor<ObjectSizeTag> ()
    // .AddAttribute ("Mobility",
    //                "The Mobility",
    //                EmptyAttributeValue (),
    //                MakeTimeAccessor (&TimestampTag::GetTimestamp),
    //                MakeTimeChecker ())
  ;
  return tid;
}
TypeId 
ObjectSizeTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
ObjectSizeTag::GetSerializedSize (void) const
{
  return 8;
}
void 
ObjectSizeTag::Serialize (TagBuffer i) const
{
  double t = m_objectSize;
  i.Write ((const uint8_t *)&t, 8);
}
void 
ObjectSizeTag::Deserialize (TagBuffer i)
{
  double t;
  i.Read ((uint8_t *)&t, 8);
  m_objectSize = t;
}

void
ObjectSizeTag::SetSize (double size)
{
  m_objectSize = size;
}
double
ObjectSizeTag::GetSize (void) const
{
  return m_objectSize;
}

void 
ObjectSizeTag::Print (std::ostream &os) const
{
  os << "v=" << m_objectSize;
}

//----------------------------------------------------------
TypeId 
HopTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("HopTag")
    .SetParent<Tag> ()
    .AddConstructor<HopTag> ()
    // .AddAttribute ("Mobility",
    //                "The Mobility",
    //                EmptyAttributeValue (),
    //                MakeTimeAccessor (&TimestampTag::GetTimestamp),
    //                MakeTimeChecker ())
  ;
  return tid;
}

//--------------------------------------------------

//--------------------------------------------------
TypeId 
HopTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
HopTag::GetSerializedSize (void) const
{
  return 8;
}
void 
HopTag::Serialize (TagBuffer i) const
{
  i.WriteU32 (m_hop);
  i.WriteU32(m_overHead);
}
void 
HopTag::Deserialize (TagBuffer i)
{
  m_hop = i.ReadU32 ();
  m_overHead = i.ReadU32 ();
}

void
HopTag::UpdateHop ()
{
  m_hop++;
}
void
HopTag::UpdateOverHead()
{
  m_overHead++;
}
void
HopTag::SetHop(uint32_t hop)
{
  m_hop = hop;
}
void
HopTag::SetOverHead (uint32_t hop)
{
  m_overHead = hop;
}
uint32_t
HopTag::GetHop (void) const
{
  return m_hop;
}
uint32_t
HopTag::GetOverHead (void) const
{
  return m_overHead;
}

void 
HopTag::Print (std::ostream &os) const
{
  os <<m_hop;
}


//--------------------------------------------------
TypeId
IndexTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("IndexTag")
    .SetParent<Tag> ()
    .AddConstructor<IndexTag> ()
    // .AddAttribute ("Mobility",
    //                "The Mobility",
    //                EmptyAttributeValue (),
    //                MakeTimeAccessor (&TimestampTag::GetTimestamp),
    //                MakeTimeChecker ())
  ;
  return tid;
}
TypeId
IndexTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
IndexTag::GetSerializedSize (void) const
{
  return 8;
}
void
IndexTag::Serialize (TagBuffer i) const
{
  double t = m_index;
  i.Write ((const uint8_t *)&t, 8);
}
void
IndexTag::Deserialize (TagBuffer i)
{
  double t;
  i.Read ((uint8_t *)&t, 8);
  m_index = t;
}

void
IndexTag::SetIndex (double mobility)
{
  m_index = mobility;
}
double
IndexTag::GetIndex (void) const
{
  return m_index;
}

void
IndexTag::Print (std::ostream &os) const
{
  os << "v=" << m_index;
}

//------------------------------------------------
class CacheTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetHop (uint32_t hop);
  uint32_t GetHop (void) const;


  void Print (std::ostream &os) const;

private:
  uint32_t m_hop;
};

TypeId 
CacheTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("CacheTag")
    .SetParent<Tag> ()
    .AddConstructor<CacheTag> ()
  ;
  return tid;
}
TypeId 
CacheTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
CacheTag::GetSerializedSize (void) const
{
  return 8;
}
void 
CacheTag::Serialize (TagBuffer i) const
{
  i.WriteU32 (m_hop);
}
void 
CacheTag::Deserialize (TagBuffer i)
{
  m_hop = i.ReadU32 ();
}

void
CacheTag::SetHop(uint32_t hop)
{
  m_hop = hop;
}
uint32_t
CacheTag::GetHop (void) const
{
  return m_hop;
}

void 
CacheTag::Print (std::ostream &os) const
{
  os <<m_hop;
}

class ProducerTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetHop (uint32_t hop);
  uint32_t GetHop (void) const;


  void Print (std::ostream &os) const;

private:
  uint32_t m_hop;
};

TypeId 
ProducerTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ProducerTag")
    .SetParent<Tag> ()
    .AddConstructor<ProducerTag> ()
  ;
  return tid;
}
TypeId 
ProducerTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
ProducerTag::GetSerializedSize (void) const
{
  return 8;
}
void 
ProducerTag::Serialize (TagBuffer i) const
{
  i.WriteU32 (m_hop);
}
void 
ProducerTag::Deserialize (TagBuffer i)
{
  m_hop = i.ReadU32 ();
}

void
ProducerTag::SetHop(uint32_t hop)
{
  m_hop = hop;
}
uint32_t
ProducerTag::GetHop (void) const
{
  return m_hop;
}

void 
ProducerTag::Print (std::ostream &os) const
{
  os <<m_hop;
}

#endif
