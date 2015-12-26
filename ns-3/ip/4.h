#include "ns3/object.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include <iostream>

using namespace ns3;

class MyObject1: public Object
{
public:
   static TypeId GetTypeId (void);
    MyObject1 ();
    ~MyObject1();
   TracedValue<int32_t> m_myInt;
};
