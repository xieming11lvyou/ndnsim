/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef REQUEST_TAG_H
#define REQUEST_TAG_H
#include "ns3/header.h"
#include "ns3/nstime.h"

namespace ns3 {
/**
 * \ingroup udpclientserver
 * \class SeqTsHeader
 * \brief Packet header for UDP client/server application.
 *
 * The header is made of a 32bits sequence number followed by
 * a 64bits time stamp.
 */
class RequestHeader : public Header
{
public:
  RequestHeader ();

  /**
   * \param seq the sequence number
   */
  void SetSeq (uint32_t seq);
  /**
   * \return the sequence number
   */
  uint32_t GetSeq (void) const;
  /**
   * \return the time stamp
   */

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  uint32_t m_seq; //!< Sequence number
};

NS_OBJECT_ENSURE_REGISTERED (RequestHeader);
RequestHeader::RequestHeader ()
  : m_seq (0)
{
}

void
RequestHeader::SetSeq (uint32_t seq)
{
  m_seq = seq;
}
uint32_t
RequestHeader::GetSeq (void) const
{
  return m_seq;
}

TypeId
RequestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RequestHeader")
    .SetParent<Header> ()
    .AddConstructor<RequestHeader> ()
  ;
  return tid;
}
TypeId
RequestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void
RequestHeader::Print (std::ostream &os) const
{
  os << "(seq=" << m_seq <<  ")";
}
uint32_t
RequestHeader::GetSerializedSize (void) const
{
  return 4;
}

void
RequestHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU32 (m_seq);
}
uint32_t
RequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_seq = i.ReadNtohU32 ();
  return GetSerializedSize ();
}
}
#endif
