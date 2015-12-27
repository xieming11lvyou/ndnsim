#include "ns3/random-variable.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "iostream"
#include "ns3/nstime.h"

using namespace std;
using namespace ns3;


void GetRand(int n,int arr[])
{
	 int i;
	 int p;
	 int tmp;
	 for (i=0;i<=n;i++)
	 {
	  arr[i]=i;
	 }

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

bool
GetProbability (double distance)
{ 
  // uint32_t upper 
  // UniformVariable probability;
  // for (uint32_t i = 0; i<=500; i++)
  // {
  //   if 
  // }
  // delayTime += probability.GetValue(0,waitTime);
  UniformVariable probability;

  uint32_t i = probability.GetInteger (0,1);
  if (i<=distance)
    return true;
  else
    return false;

}




int main(int argc, char *argv[])
{	
	CommandLine cmd;
	

	double n;

	cmd.AddValue ("n","n",n);

	cmd.Parse (argc, argv);

	RngSeedManager::SetSeed (n);

	cout<<GetProbability(0);
	cout<<GetProbability(0);
	cout<<GetProbability(0);

// 	const int size = 10;

// 	int array[size];

// 	int array1[size];

// 	GetRand (size-1, array);

// //	GetRand (size-1, array1);

// 	for (int i=0; i<size; i++)
// 	{
// 		cout<<array[i]<<"  ";
// 	}
// 	cout<<endl;

// 	for (int i=0; i<size; i++)
// 	{
// 		cout<<array1[i]<<"  ";
// 	}


// //	cout<<m_rand1.GetInteger(0,2);
	return 0;
}