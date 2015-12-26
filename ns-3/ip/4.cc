#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object.h"
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("fourTH");
NS_OBJECT_ENSURE_REGISTERED (MyObject1);

TypeId
MyObject1::GetTypeId (void)
{
  static TypeId tid = TypeId ("MyObject1")
    .SetParent (Object::GetTypeId ())
    .AddConstructor<MyObject1> ()
    .AddTraceSource ("MyInteger",
                     "An integer value to trace.",
                     MakeTraceSourceAccessor (&MyObject1::m_myInt))
  ;
  return tid;
}

MyObject1::MyObject1()
{
 // NS_LOG_INFO("sdicajiod");
}
MyObject1::~MyObject1()
{}
