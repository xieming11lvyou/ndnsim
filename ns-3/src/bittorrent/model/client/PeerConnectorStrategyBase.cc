/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2014 ComSys, RWTH Aachen University
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
 * Authors: Rene Glebke, Martin Lang (principal authors), Alexander Hocks
 */

#include "PeerConnectorStrategyBase.h"

#include "ns3/BitTorrentDefines.h"

#include "BitTorrentClient.h"
#include "BitTorrentHttpClient.h"
#include "BitTorrentPeer.h"
#include "ns3/TorrentFile.h"

#include "ns3/address.h"
#include "ns3/event-id.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/random-variable.h"
#include "ns3/ptr.h"
#include "ns3/socket.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/uinteger.h"

#include <map>
#include <set>
#include <utility>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::PeerConnectorStrategyBase");
NS_OBJECT_ENSURE_REGISTERED (PeerConnectorStrategyBase);

PeerConnectorStrategyBase::PeerConnectorStrategyBase (Ptr<BitTorrentClient> myClient) : AbstractStrategy (myClient)
{
  m_periodicInterval = Seconds (10);
  m_reannouncementInterval = Seconds (60);
  AddConnection (m_myClient->GetIp ().Get ());

  m_timeout = Seconds (15);
  m_clientUpdateCycles = 3;
  m_currentClientUpdateCycle = 0;

  m_trackerId = m_myClient->GetPeerId ();
}

PeerConnectorStrategyBase::~PeerConnectorStrategyBase ()
{
}

void PeerConnectorStrategyBase::DoInitialize ()
{
  m_myClient->RegisterCallbackConnectionEstablishedEvent (MakeCallback (&PeerConnectorStrategyBase::ProcessPeerConnectionEstablishedEvent,this));
  m_myClient->RegisterCallbackConnectionFailEvent (MakeCallback (&PeerConnectorStrategyBase::ProcessPeerConnectionFailEvent,this));
  m_myClient->RegisterCallbackConnectionCloseEvent (MakeCallback (&PeerConnectorStrategyBase::ProcessConnectionCloseEvent, this));
  m_myClient->RegisterCallbackDownloadCompleteEvent (MakeCallback (&PeerConnectorStrategyBase::ProcessDownloadCompleteEvent, this));
  m_myClient->RegisterCallbackBitfieldReceivedEvent (MakeCallback (&PeerConnectorStrategyBase::ProcessPeerBitfieldReceivedEvent,this));

  // Announce special callbacks to the client so it can pass calls through to this class
  m_myClient->SetCallbackConnectToCloud (MakeCallback (&PeerConnectorStrategyBase::ConnectToCloud, this));
  m_myClient->SetCallbackDisconnectFromCloud (MakeCallback (&PeerConnectorStrategyBase::DisconnectFromCloud, this));
  m_myClient->SetCallbackAnnounceAsSeeder (MakeCallback (&PeerConnectorStrategyBase::AnnounceAsSeeder, this));
  m_myClient->SetCallbackConnectToPeers (MakeCallback (&PeerConnectorStrategyBase::ConnectToPeers, this));
  m_myClient->SetCallbackDisconnectPeers (MakeCallback (&PeerConnectorStrategyBase::DisconnectPeers, this));
  m_myClient->SetCallbackDisconnectPeer (MakeCallback (&PeerConnectorStrategyBase::DisconnectPeer, this));
  m_myClient->SetCallbackGetPeerCount (MakeCallback (&PeerConnectorStrategyBase::GetPeerCount, this));
  m_myClient->SetCallbackConnectToPeer (MakeCallback (&PeerConnectorStrategyBase::ConnectToPeer, this));

  if (!m_myClient->GetDownloadCompleted ())
    {
      m_nextPeriodicEvent = Simulator::Schedule (m_periodicInterval,&PeerConnectorStrategyBase::ProcessPeriodicSchedule,this);
    }
}

const std::set<std::pair<uint32_t, uint16_t> > & PeerConnectorStrategyBase::GetPotentialClients() const
{
  return m_potentialClients;
}

Time PeerConnectorStrategyBase::GetPeriodicInterval() const
{
  return m_periodicInterval;
}

void PeerConnectorStrategyBase::SetPeriodicInterval(Time periodicInterval)
{
  if(periodicInterval.IsStrictlyPositive())
    {
      m_periodicInterval = periodicInterval;
    }
}

Time PeerConnectorStrategyBase::GetReannouncementInterval() const
{
  return m_reannouncementInterval;
}

void PeerConnectorStrategyBase::SetReannouncementInterval(Time reannouncementInterval)
{
  if(reannouncementInterval.IsStrictlyPositive())
    {
      m_reannouncementInterval = reannouncementInterval;
    }
}

Time PeerConnectorStrategyBase::GetTimeoutPeriod() const
{
  return m_timeout;
}

void PeerConnectorStrategyBase::SetTimeoutPeriod(Time timeoutPeriod)
{
  m_timeout = timeoutPeriod;
}

uint16_t PeerConnectorStrategyBase::GetPeerCount() const
{
  return m_connectedTo.size() - 1 + m_pendingConnections.size();
}

void PeerConnectorStrategyBase::ConnectToCloud ()
{
  NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ": Contacting tracker to join cloud...");

  std::map<std::string, std::string> additionalParameters;
  ContactTracker (PeerConnectorStrategyBase::STARTED, 0, additionalParameters, true);
  m_myClient->SetConnectionToCloudSuspended (false);
  m_myClient->CloudConnectionEstablishedEvent ();

  ProcessPeriodicReannouncements ();
}

void PeerConnectorStrategyBase::AnnounceAsSeeder ()
{
  NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ":  Contacting tracker to report completed download.");

  Simulator::ScheduleNow (&PeerConnectorStrategyBase::ContactTrackerWrapper, this, PeerConnectorStrategyBase::COMPLETED, 1, true);
}

void PeerConnectorStrategyBase::DisconnectFromCloud ()
{
  NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ":  Disconnecting all peers and leaving cloud.");

  DisconnectPeers (-1);

  std::map<std::string, std::string> additionalParameters;
  ContactTracker (PeerConnectorStrategyBase::STOPPED, 0, additionalParameters, true);

  m_myClient->SetConnectedToCloud (false);
  m_myClient->SetConnectionToCloudSuspended (true);
  m_myClient->CloudConnectionSuspendedEvent ();
}

void PeerConnectorStrategyBase::StartListening (uint16_t port)
{
  // Create and open a TCP socket to listen at for incoming connection attempts
  TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
  m_serverSocket = Socket::CreateSocket (m_myClient->GetNode (), tid);
  m_serverSocket->SetAcceptCallback (MakeCallback (&PeerConnectorStrategyBase::AcceptCallback,this),MakeCallback (&PeerConnectorStrategyBase::NewConnectionCreatedCallback,this));
  m_serverSocket->SetAttribute ("SegmentSize", UintegerValue (BT_PEER_SOCKET_TCP_SEGMENT_SIZE_MAX));
  m_serverSocket->Bind (InetSocketAddress (m_myClient->GetIp(), port));
  m_serverSocket->Listen ();
}

void PeerConnectorStrategyBase::DisconnectPeer (Ptr<Peer> peer)
{
  peer->CloseConnection (false);      // false => NOT slient, i.e., issue events to the client and associated strategies
}

void PeerConnectorStrategyBase::DisconnectPeerSilent (Ptr<Peer> peer)
{
  peer->CloseConnection (true);
}

void PeerConnectorStrategyBase::ConnectToPeer (Ipv4Address address, uint16_t port)
{
  NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ":  Explicitly connecting to " << address << ":" << port << ".");

  // Step 0: Check if we are already connected to this peer (or have a pending connection). In this case, return.
  if (m_connectedTo.find (address.Get ()) != m_connectedTo.end () || m_pendingConnections.find (address.Get ()) != m_pendingConnections.end ())
    {
      return;
    }

  // Step 1: In case we are not supplied a valid port, we use the pre-defined listener port
  if (port == 0)
    {
      port = BT_PROTOCOL_LISTENER_PORT;
    }

  // Step 2: We create a peer class instance and register the necessary functions
  Ptr<Peer> newPeer = CreateObject<Peer> (m_myClient);
  Simulator::ScheduleNow (&Peer::ConnectToPeer, newPeer, address, port);
  Simulator::Schedule (MilliSeconds (BT_PEER_CONNECTOR_CONNECTION_ACCEPTANCE_DELAY), &PeerConnectorStrategyBase::CheckAndDisconnectIfRejected, this, newPeer);

  m_pendingConnections.insert (address.Get ());
}

uint16_t PeerConnectorStrategyBase::ConnectToPeers (uint16_t count)
{
  // We only need to do this if we have not yet completed our download
  if (m_myClient->GetDownloadCompleted ())
    {
      return 0;
    }

  // Step 1: Get the clients that we MAY connect to
  const std::set<std::pair<uint32_t, uint16_t> >& potentialPeers = GetPotentialClients ();
  if (potentialPeers.size () == 0)     // No one we could connect to
    {
      return 0;
    }

  // Step 2a: If no count of peers to connect to is specified, we want to connect to as many peers as we are allowed to
  if (count == 0)
    {
      count = m_myClient->GetDesiredPeers ();
    }

  // Step 2b: We don't actively connect to more peers than we are allowed to
  int32_t connectToNPeers = std::min (static_cast<int32_t> (count), m_myClient->GetMaxPeers () - GetPeerCount ());
  if (connectToNPeers <= 0)
    {
      return 0;
    }

  // Step 3: Get an iterator through the data structure for our potential clients to connect to
  std::set<std::pair<uint32_t, uint16_t> >::const_iterator iter = potentialPeers.begin ();
  // This is a mini heuristic for random selection of peers to connect to.
  // One might want to implement a better one
  UniformVariable uv;
  for (uint32_t start = uv.GetInteger (0, potentialPeers.size () - 1); start > 0; --start)
    {
      ++iter;
    }

  uint16_t peersConnected = 0;       // i.e., in this call of ConnectToPeers()
  bool goOn =
    (iter != potentialPeers.end ())
    && (connectToNPeers > 0)
    && (GetPeerCount () < m_myClient->GetDesiredPeers ());

  // Step 4: Connect to peers
  while (goOn)
    {
      // Step 4a: Only connect if we are not already connected or have a pending connection
      if (m_connectedTo.find ((*iter).first) == m_connectedTo.end () && m_pendingConnections.find ((*iter).first) == m_pendingConnections.end ())
        {
          // Step 4a1: Re-convert the IP address in integer format to a Ipv4Address object
          // TODO: We should change this back to using Ipv4Address objets only
          Ipv4Address connectToAddress;
          connectToAddress.Set ((*iter).first);

          // Step 4a2: Create a Peer object for the new connection and schedule the connection to that peer by calling the appropriate method of the Peer class
          Ptr<Peer> newPeer = CreateObject<Peer> (m_myClient);
          Simulator::ScheduleNow (&Peer::ConnectToPeer, newPeer, connectToAddress, (*iter).second);
          Simulator::Schedule (MilliSeconds (BT_PEER_CONNECTOR_CONNECTION_ACCEPTANCE_DELAY), &PeerConnectorStrategyBase::CheckAndDisconnectIfRejected, this, newPeer);

          // Step 4a3: Insert this connection into the list of pending connections
          m_pendingConnections.insert ((*iter).first);

          // Step 4a4: Adjust our "counters"
          ++peersConnected;
          --connectToNPeers;

          NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ": Connecting to " << connectToAddress << ":" << (*iter).second << ".");
        }

      // Step 4b: Adjust our iterator and recalculate our "guard" goOn
      ++iter;
      goOn =
        (iter != potentialPeers.end ())
        && (connectToNPeers > 0)
        && (GetPeerCount () < m_myClient->GetDesiredPeers ());

    }

  return peersConnected;
}

void PeerConnectorStrategyBase::DisconnectPeers (int32_t count)
{
  if (count == -1)      // Close all connections
    {
      NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ":  Disconnecting all peers.");

      std::vector<Ptr<Peer> >::const_iterator it = m_myClient->GetPeerListIterator ();
      for (; it != m_myClient->GetPeerListEnd (); ++it)
        {
          Simulator::Schedule (MilliSeconds (1), &PeerConnectorStrategyBase::DisconnectPeer, this, *it);
        }
    }
  else
    {
      std::vector<Ptr<Peer> >::const_iterator it = m_myClient->GetPeerListIterator ();
      uint32_t disconnect = std::min (static_cast<uint32_t> (m_myClient->GetConnectedPeerCount ()), static_cast<uint32_t> (count));

      for(; disconnect > 0; --disconnect, ++it)
        {
          Simulator::Schedule (MilliSeconds (1), &PeerConnectorStrategyBase::DisconnectPeer, this, *it);
        }
    }
}

uint8_t PeerConnectorStrategyBase::AcceptConnection (Ptr<Socket> socket, Address addr)
{
  InetSocketAddress buf = InetSocketAddress::ConvertFrom (addr);

  // Only allow connections if we are under our maximum connection count
  if (GetPeerCount () < m_myClient->GetMaxPeers ())
    {
      // Only allow a connection if we are not already connected to that peer
      uint32_t foreignIP = buf.GetIpv4 ().Get ();

      // Do not allow a connection between two seeders
      if (m_myClient->GetDownloadCompleted () && m_knownSeeders.find (foreignIP) != m_knownSeeders.end ())
        {
          return 4;
        }

      // Only allow connections if we have not already scheduled a connection on our own
      if (m_connectedTo.find (foreignIP) == m_connectedTo.end ())
        {
          bool scheduledOneMyself = (m_pendingConnections.find (foreignIP) != m_pendingConnections.end ());

          if (scheduledOneMyself)
            {
              // The one with the higher IP wins
              bool iAmHigher = m_myClient->GetIp ().Get () > foreignIP;

              if (iAmHigher)
                {
                  return 0;                       // We win
                }
              else
                {
                  return 3;                       // The remote peer wins
                }
            }
          else
            {
              return 0;                   // Everything is fine
            }
        }
      else
        {
          return 2;               // We are already connected to this peer
        }
    }
  else
    {
      return 1;           // We already have enough peers connected with us
    }
}

void PeerConnectorStrategyBase::AddConnection(uint32_t address)
{
  m_connectedTo.insert(address);
}

void PeerConnectorStrategyBase::DeleteConnection (uint32_t address)
{
  std::set<uint32_t>::iterator it = m_connectedTo.find (address);
  if (it != m_connectedTo.end ())
    {
      m_connectedTo.erase (it);
    }
}

void PeerConnectorStrategyBase::ProcessPeriodicSchedule ()
{
  if (m_myClient->GetDownloadCompleted ())
    {
      return;
    }

  if (m_myClient->GetAutoConnect () && GetPeerCount () < m_myClient->GetDesiredPeers ())   // Still peers left that we want to have
    {
      ConnectToPeers (m_myClient->GetDesiredPeers () - GetPeerCount ());
    }

  Simulator::Cancel (m_nextPeriodicEvent);
  m_nextPeriodicEvent = Simulator::Schedule (m_periodicInterval,&PeerConnectorStrategyBase::ProcessPeriodicSchedule,this);
}

void PeerConnectorStrategyBase::ProcessPeriodicReannouncements ()
{
  if (m_myClient->GetConnectedToCloud ())
    {
      std::map<std::string, std::string> additionalParameters;
      // The last parameter in here is set to "false" so more important updates can override this one
      ContactTracker (PeerConnectorStrategyBase::REGULAR_UPDATE, 0, additionalParameters, false);
    }

  Simulator::Cancel (m_nextPeriodicReannouncement);
  m_nextPeriodicReannouncement = Simulator::Schedule (m_reannouncementInterval, &PeerConnectorStrategyBase::ProcessPeriodicReannouncements, this);
}

void PeerConnectorStrategyBase::ParseResponse (std::istream &response)
{
  if (m_myClient->GetConnectionToCloudSuspended ())
    {
      return;
    }

  // NS_LOG_DEBUG ("Parsing tracker response \"" << dynamic_cast<std::stringstream&> (response).str () << "\"." << std::endl);

  Ptr<TorrentData> root = TorrentData::ReadBencodedData (response);
  Ptr<TorrentDataDictonary> rootDict = DynamicCast<TorrentDataDictonary> (root);

  // NS_ASSERT(rootDict);
  if (!rootDict)
    {
      return;
    }

  // test if there was an error
  Ptr<TorrentData> errorMsg = rootDict->GetData ("failure reason");

  if (errorMsg)
    {
      Ptr<TorrentDataString> errorMsgStr = DynamicCast<TorrentDataString> (errorMsg);
      return;
    }

  // retrieve renewal interval
  // ALEX: Sometimes, the tracker response seems malformed. Check where this comes from (Sending? Receiving?)
  Ptr<TorrentDataInt> renewalInterval = DynamicCast<TorrentDataInt> (rootDict->GetData ("interval"));
  if (!renewalInterval)
    {
      return;
    }
  // NS_ASSERT(renewalInterval);
  m_reannouncementInterval = Seconds (static_cast<uint32_t> (renewalInterval->GetData ()));

  Ptr<TorrentDataInt> leechers = DynamicCast<TorrentDataInt> (rootDict->GetData ("incomplete"));
  if (!leechers)
    {
      return;
    }
  // NS_ASSERT(leechers);
  leechers->GetData ();

  Ptr<TorrentDataInt> seeders = DynamicCast<TorrentDataInt> (rootDict->GetData ("complete"));
  if (!seeders)
    {
      return;
    }
  // NS_ASSERT(seeders);
  seeders->GetData ();

  // update the tracker id if there is one
  Ptr<TorrentDataString> trackId = DynamicCast<TorrentDataString> (rootDict->GetData ("tracker id"));
  if (trackId)
    {
      m_trackerId = trackId->GetData ();
    }
  // retrieve the peer list from response
  Ptr<TorrentData> peers = rootDict->GetData ("peers");
  if (!peers)
    {
      return;
    }
  // NS_ASSERT(peers);

  // A mini heuristic against dead (inactive) peers in the set of potential peers
  if (m_currentClientUpdateCycle == m_clientUpdateCycles - 1)
    {
      m_potentialClients.clear ();
    }
  m_currentClientUpdateCycle = (m_currentClientUpdateCycle + 1) % m_clientUpdateCycles;

  // we have to distinguish which type of peerlist is coming in
  if (peers->GetDataType () == TorrentData::DATA_STRING)
    {
      // this is the plain style, the string contains peers in the format
      // 4 byte ipaddr, 2 byte port until the end
      Ptr<TorrentDataString> peerStringPtr = DynamicCast<TorrentDataString> (peers);
      if (!peerStringPtr)
        {
          return;
        }
      // NS_ASSERT(peerStringPtr);

      const char * peerString = peerStringPtr->GetData ().c_str ();

      size_t peerLen = peerStringPtr->GetData ().size ();
      if (!((peerLen % 6) == 0))
        {
          return;
        }
      // NS_ASSERT((peerLen % 6) == 0);

      // read data sequentially
      std::pair<uint32_t, uint16_t> peer;
      for (uint32_t peerNum = 0; peerNum < peerLen; peerNum += 2)
        {
          uint32_t ipaddr;
          ipaddr = peerString[peerNum++];
          ipaddr <<= 8;
          ipaddr |= peerString[peerNum++];
          ipaddr <<= 8;
          ipaddr |= peerString[peerNum++];
          ipaddr <<= 8;
          ipaddr |= peerString[peerNum++];

          // Skip own IP
          if (ipaddr == m_myClient->GetIp ().Get ())
            {
              continue;
            }

          peer.first = ipaddr;
          peer.second = (peerString[peerNum] << 8) | peerString[peerNum + 1];

          m_potentialClients.insert (peer);
        }
    }
  else
    {
      // this is decorated style
      // peers is a dictionary of peerid, ip, port,
      // ip is string with dotted addr, or dns name - DNS is not yet supported (TODO)
      Ptr<TorrentDataList> peerList = DynamicCast<TorrentDataList> (peers);
      if (!peerList)
        {
          return;
        }
      // NS_ASSERT(peerList);

      std::pair<uint32_t, uint16_t> peer;
      for (std::list<Ptr<TorrentData> >::const_iterator iter = peerList->GetIterator ();
           iter != peerList->GetListEnd (); ++iter)
        {

          Ptr<TorrentDataDictonary> currentPeer = DynamicCast<TorrentDataDictonary> (*iter);
          if (!currentPeer)
            {
              continue;
            }
          // NS_ASSERT(currentPeer);

          Ptr<TorrentDataString> peerAddr = DynamicCast<TorrentDataString> (currentPeer->GetData ("ip"));
          if (!peerAddr)
            {
              continue;
            }
          // NS_ASSERT(peerAddr);

          Ptr<TorrentDataInt> peerPort = DynamicCast<TorrentDataInt> (currentPeer->GetData ("port"));
          if (!peerPort)
            {
              continue;
            }
          // NS_ASSERT(peerPort);

          Ipv4Address buf;
          buf.Set (peerAddr->GetData ().c_str ());

          // Skip own IP
          if (buf == m_myClient->GetIp ())
            {
              continue;
            }

          peer.first = buf.Get ();
          peer.second = static_cast<uint16_t> (peerPort->GetData ());

          m_potentialClients.insert (peer);
        }

    }

  if(!m_myClient->GetConnectedToCloud ())
    m_myClient->CloudConnectionEstablishedEvent ();

  m_myClient->SetConnectedToCloud (true);
}

bool PeerConnectorStrategyBase::AcceptCallback (Ptr<Socket> sock, const Address &addr)
{
  // We only accept connections if we are currently connected to the cloud
  bool ok = m_myClient->GetConnectedToCloud ();
  if (ok)
    {
      // Additionally, we only accept connections only if we have less than the desired number of clients and not scheduled a connection on our own
      uint8_t acceptedState = AcceptConnection (sock, addr);
      ok = (acceptedState == 0);           // 0 ==> accepted

      if (ok)
        {
          NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ": Accepted connection with " << InetSocketAddress::ConvertFrom(addr).GetIpv4 () << ":" << InetSocketAddress::ConvertFrom(addr).GetPort () << ".");

          return true;
        }
      else
        {
          NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ": Accepted connection with " << InetSocketAddress::ConvertFrom(addr).GetIpv4 () << ":" << InetSocketAddress::ConvertFrom(addr).GetPort () << "." << ". Reason: " << acceptedState << ".");

          return false;
        }
    }
  return false;
}

void PeerConnectorStrategyBase::NewConnectionCreatedCallback (Ptr<Socket> sock, const  Address &addr)
{
  // Similar to connections initiated on our own, for each accepted connection, we crate a Peer object and feed it with the appropriate information and method calls
  InetSocketAddress buf = InetSocketAddress::ConvertFrom (addr);
  Ptr<Peer> newPeer = CreateObject<Peer> (m_myClient);
  Simulator::ScheduleNow (&Peer::ServeNewPeer, newPeer, sock, buf.GetIpv4 (), buf.GetPort() );
  Simulator::Schedule (MilliSeconds (BT_PEER_CONNECTOR_CONNECTION_ACCEPTANCE_DELAY), &PeerConnectorStrategyBase::CheckAndDisconnectIfRejected, this, newPeer);
}

void PeerConnectorStrategyBase::CheckAndDisconnectIfRejected (Ptr<Peer> peer)
{
  // If for some reason, the connection to the remote peer could not be established successfully, disconnect that peer
  if (peer->GetConnectionState () != Peer::CONN_STATE_CONNECTED)
    {
      DisconnectPeerSilent (peer);
      m_pendingConnections.erase (peer->GetRemoteIp ().Get ());
    }
  else       // Else, fully register the peer with the client
    {
      m_myClient->RegisterPeer (peer);
      m_myClient->PeerConnectionEstablishedEvent (peer);


      NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ": Fully established connection with " << peer->GetRemoteIp () << ":" << peer->GetRemotePort () << ".");
    }
}

void PeerConnectorStrategyBase::TrackerResponseEvent (Ptr<Socket> socket)
{
  size_t responseStart;
  bool finished = false;

  // Step 1: Read the response from the HTTP client
  m_trackerBuffer << m_httpCC.HttpReceiveReply (socket, true, true, responseStart, finished);

  // Step 2a: If the response was fully received, parse it and announce that we received a tracker response
  if (finished)
    {
      ParseResponse (m_trackerBuffer);
    
      m_httpCC.CloseAndReInit ();
      m_trackerBuffer.clear ();
      Simulator::Cancel (m_timeoutEvent);
      
      m_myClient->TrackerResponseReceivedEvent ();
    }
  else
    {
      // Step 2b1: If the response had a bad/error header, we forget about the response
      if (m_httpCC.HasBadHeader ())
        {
          m_httpCC.CloseAndReInit ();
          m_trackerBuffer.clear ();
          Simulator::Cancel (m_timeoutEvent);

          NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ":  Received error header from tracker.");
        }

      // Step 2b2: If the request has not yet been fully answered, we check back in 500ms again
      if (m_httpCC.GetRequestActive ())
        {
          Simulator::Schedule (MilliSeconds (500), &PeerConnectorStrategyBase::TrackerResponseEvent, this, socket);
        }
    }
}

void PeerConnectorStrategyBase::TrackerTimeout (TrackerContactReason reason, uint16_t numwant, bool closeCurrentConnection)
{
  if (closeCurrentConnection)
    {
      ContactTrackerWrapper (reason, numwant, closeCurrentConnection);
    }
}

bool PeerConnectorStrategyBase::ContactTracker (TrackerContactReason event, uint16_t numwant, std::map<std::string, std::string> additionalParameters, bool closeCurrentConnection)
{
  // Step 1: Initiate a connection with the tracker, close existing ones if required
  if (closeCurrentConnection)
    {
      m_httpCC.CloseAndReInit ();
    }
  else
    {
      if (m_httpCC.HasBadHeader ())
        {
          NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ":  Received bad header from tracker.");

          return false;
        }
      else if (m_httpCC.GetRequestActive ())
        {
          return false;
        }
    }
  Simulator::Cancel (m_timeoutEvent);

  // Step 2: Create our announce (HTTP GET request) -->
  std::stringstream request;

  std::string announceURL = m_myClient->GetTorrent ()->GetAnnounceURL ();
  std::string encodedInfoHash = m_myClient->GetTorrent ()->GetEncodedInfoHash ();

  request << announceURL.substr (announceURL.find ('/', 7), announceURL.size () - announceURL.find ('/', 7)) << "?";
  request << "info_hash=" << encodedInfoHash;
  request << "&peer_id=" << m_trackerId;       // TODO: URL encode?
  request << "&port=" << m_myClient->GetPort ();
  request << "&uploaded=0";       // TODO: Keep track of the amount of bytes uploaded!
  request << "&downloaded=" << m_myClient->GetBytesCompleted ();
  request << "&left=" << m_myClient->GetTorrent ()->GetFileLength () - m_myClient->GetBytesCompleted ();
  request << "&compact=0";

  switch (event)
    {
    case STARTED:
      request << "&event=started";
      break;
    case STOPPED:
      request << "&event=stopped";
      break;
    case COMPLETED:
      request << "&event=completed";
      break;
    default:
      break;
    }
  // v-- This is the standard value (see wiki.theory.org)
  request << "&numwant=" << ((numwant == 0) ? 50 : numwant);
  request << "&ip=" << m_myClient->GetIp ();

  std::map<std::string, std::string>::const_iterator it = additionalParameters.begin ();
  for (; it != additionalParameters.end (); ++it)
    {
      request << "&" << (*it).first << "=" << Utilities::EscapeHex ((*it).second);
    }
  // <-- Step 2: Create our announce (HTTP GET request)

  // Step 3: Contact the tracker, schedule appropriate timeouts or closings of connections if required
  Ipv4Address trackerAddress = Ipv4Address (announceURL.substr (7, announceURL.find (':', 7) - 7).c_str ());

  if (m_httpCC.HttpGetRequest (m_myClient->GetNode (), TcpSocketFactory::GetTypeId (), trackerAddress, 80, request.str (), MakeCallback (&PeerConnectorStrategyBase::TrackerResponseEvent, this)))
    {
      m_timeoutEvent = Simulator::Schedule (m_timeout, &PeerConnectorStrategyBase::TrackerTimeout, this, event, numwant, true);
    }
  else if (closeCurrentConnection)
    {
      m_timeoutEvent = Simulator::Schedule (MilliSeconds (500), &PeerConnectorStrategyBase::ContactTrackerWrapper, this, event, numwant, closeCurrentConnection);
    }

  return true;
}

bool PeerConnectorStrategyBase::ContactTrackerWrapper (TrackerContactReason reason, uint16_t numwant, bool closeCurrentConnection)
{
  std::map<std::string, std::string> additionalParameters;

  return ContactTracker (reason, numwant, additionalParameters, closeCurrentConnection);
}

void PeerConnectorStrategyBase::CancelCurrentTrackerRequest ()
{
  Simulator::Cancel (m_timeoutEvent);
  m_httpCC.CloseAndReInit ();
}

void PeerConnectorStrategyBase::ProcessPeerConnectionEstablishedEvent (Ptr<Peer> peer)
{
  AddConnection (peer->GetRemoteIp ().Get ());
  m_pendingConnections.erase (peer->GetRemoteIp ().Get ());
  peer->SendBitfield ();
}

void PeerConnectorStrategyBase::ProcessPeerConnectionFailEvent (Ptr<Peer> peer)
{
  m_pendingConnections.erase (peer->GetRemoteIp ().Get ());
  if (!m_myClient->GetDownloadCompleted ())
    {
      Simulator::ScheduleNow (&PeerConnectorStrategyBase::ProcessPeriodicSchedule, this);
    }
}

void PeerConnectorStrategyBase::ProcessConnectionCloseEvent (Ptr<Peer> peer)
{
  DeleteConnection (peer->GetRemoteIp ().Get ());    // Already deletes only 1 entry from the multimap
}

void PeerConnectorStrategyBase::ProcessPeerBitfieldReceivedEvent (Ptr<Peer> peer)
{
  // If we have not yet completed download, we parse the received bitfield
  if (m_myClient->GetDownloadCompleted ())
    {
      // Step 1: Check whether the remote peer has already finished download (i.e., whether we can call it a seeder)
      bool completed = true;
      uint32_t i = 0;
      while (completed && i < m_myClient->GetTorrent ()->GetNumberOfPieces ())
        {
          if (!(peer->HasPiece (i)))
            {
              completed = false;
            }
          ++i;
        }

      // Step 2: If the peer we have encountered is indeed a seeder and we also are, close the connection
      if (completed && m_myClient->GetIp ().Get () < peer->GetRemoteIp ().Get ())
        {
          m_knownSeeders.insert (peer->GetRemoteIp ().Get ());
          Simulator::Schedule (Seconds (5), &PeerConnectorStrategyBase::DisconnectPeer, this, peer);

          NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ": Closing attempted connection from also finished peer " << peer->GetRemoteIp () << ":" << peer->GetRemotePort () << ".");
        }
    }
}

void PeerConnectorStrategyBase::ProcessDownloadCompleteEvent ()
{
  // In case of a completed download, we close the connection to already-known seederss so we don't keep unnecessary connections
  // The calculation is the same as in ProcessPeerBitfieldReceivedEvent()
  for (std::vector<Ptr<Peer> >::const_iterator it = m_myClient->GetPeerListIterator (); it != m_myClient->GetPeerListEnd (); ++it)
    {
      bool completed = true;
      uint32_t i = 0;
      while (completed && i < m_myClient->GetTorrent ()->GetNumberOfPieces ())
        {
          if (!(*it)->HasPiece (i))
            {
              completed = false;
            }
          ++i;
        }

      if (completed)
        {
          m_knownSeeders.insert ((*it)->GetRemoteIp ().Get ());
          Simulator::Schedule (Seconds (5), &PeerConnectorStrategyBase::DisconnectPeer, this, *it);

          NS_LOG_INFO ("PeerConnectorStrategyBase: " << m_myClient->GetIp () << ": Disconnecting from also finished peer " << (*it)->GetRemoteIp () << ":" << (*it)->GetRemotePort () << ".");
        }
    }
}

} // ns bittorrent
} // ns ns3
