/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2012, 2014 ComSys, RWTH Aachen University.
 * Partially copyright (c) 2014 National and Kapodistrian University of Athens.
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
 * Authors: Rene Glebke, Martin Lang (principal authors), Elias Weingaertner (initial author), Alexander Hocks
 * Contributors: Aristotelis Glentis (U Athens)
 */
#include "BitTorrentClient.h"

#include "ns3/BitTorrentDefines.h"
#include "ns3/BitTorrentUtilities.h"
#include "AbstractStrategy.h"
#include "ns3/GlobalMetricsGatherer.h"
#include "StorageManager.h"
#include "ProtocolFactory.h"
#include "ns3/TorrentFile.h"

#include "ns3/address.h"
#include "ns3/application.h"
#include "ns3/channel.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-interface-address.h"
#include "ns3/log.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/pointer.h"
#include "ns3/random-variable.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/mpi-interface.h"

#include <cmath>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::BitTorrentClient");
NS_OBJECT_ENSURE_REGISTERED (BitTorrentClient);

BitTorrentClient::BitTorrentClient ()
{
  m_protocol = "rarest-first";
  m_bitfieldFillType = "empty";
  m_gatherMetricsEventPeriodicity = Seconds (60);

  m_interfaceId = 1;
  m_port = BT_PROTOCOL_LISTENER_PORT;

  m_autoConnect = true;
  m_seedingDuration = MilliSeconds (0) - MilliSeconds (1);     // Should be negative; expressed like this to mute a certain version of the Intel C++ compiler

  m_desiredPeers = 30;
  m_maxPeers = 55;
  m_maxUnchokedPeers = 4;

  m_maxRequestsPerPeer = 16;
  m_maxRequestsPerPiece = 8;
  m_maxRequestsPerBlock = 1;
  m_maxRequestsPerPeerPerPiece = 8;

  m_requestBlockSize = 16384;
  m_sendBlockSize = 16384;

  m_pieceTimeout = Seconds (30);

  m_checkDownloadedData = false;

  m_downloadCompleted = false;

  m_connectedToCloud = false;
  m_connectionToCloudSuspended = false;
}

BitTorrentClient::~BitTorrentClient ()
{
}

TypeId BitTorrentClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BitTorrentClient")
    .SetParent<Application> ()
    .AddConstructor<BitTorrentClient> ()
  ;
  return tid;
}

void BitTorrentClient::StopApplication ()
{
  // Clear the list of strategies
  m_strategyList.clear ();
}

void BitTorrentClient::DoDispose ()
{
  Application::DoDispose ();
}

// Initializes the application upon startup
void BitTorrentClient::StartApplication ()
{
  // Step 0: Set up a unique client id (peerId)
  UniformVariable uv;
  std::string peerId = "VODSim-" + lexical_cast<std::string> (GetNode ()->GetId ());
  peerId += "-" + lexical_cast<std::string> (uv.GetInteger (0, 0xffffffff - 1));
  peerId.resize (20, '.');
  m_peerId = peerId;

  // Step 1: Find out our connection settings (IP address) and store them for easy access
  m_ip = GetNode ()->GetObject<Ipv4> ()->GetAddress (m_interfaceId, 0).GetLocal ();
  m_interface = GetNode() ->GetDevice (m_interfaceId);

  // Step 2: Check whether the needed torrent is loaded correctly and set the data retrieval pointer accordingly
  StorageManager::GetInstance ()->EnsureFileLoaded (m_torrent->GetDataPath () + "/" + m_torrent->GetFileName ());
  m_torrentDataPtr = StorageManager::GetInstance ()->GetBufferForFile (m_torrent->GetDataPath () + "/" + m_torrent->GetFileName ());

  // Step 3: Set up the bitfield
  // Step 3a: Calculate its size
  uint32_t bitfieldSize = m_torrent->GetNumberOfPieces () / 8;
  if ((m_torrent->GetNumberOfPieces () % 8) > 0)
    {
      bitfieldSize++;
    }
  m_bitfield.resize (bitfieldSize);

  // Step 3b: Zero it and then fill it according to the provided settings -->
  for (uint32_t i = 0; i < m_torrent->GetBitfieldSize (); ++i)
    {
      m_bitfield[i] = 0;
    }

  RandomVariable bitfieldFiller;
  uint32_t randomEndStart = bitfieldSize;
  switch (m_bitfieldFillType[0])
    {
    case 'e':
      bitfieldFiller = ConstantVariable (0.0);   
      for (uint8_t i = 0; i < randomEndStart; ++i)
      {
         m_bitfield[i] = static_cast<uint8_t> (bitfieldFiller.GetInteger ());
      }      
      break;
    case 'f':
      bitfieldFiller = ConstantVariable (255.0);     
      for (uint8_t i = 0; i < randomEndStart; ++i)
      {
         m_bitfield[i] = static_cast<uint8_t> (bitfieldFiller.GetInteger ());
      }            
      break;
    case 'r':
    {
      float percentage = lexical_cast<float>(m_bitfieldFillType.substr(7, 4));
      
      std::set<uint32_t> indices = Utilities::GetRandomSampleF2((int)round(GetTorrent ()->GetNumberOfPieces () * percentage), GetTorrent ()->GetNumberOfPieces ());
      
      for(std::set<uint32_t>::const_iterator it = indices.begin(); it != indices.end(); ++it)
      {
         m_bitfield[*it / 8] += 1 << (*it % 8);
      }
      break;
    }
    default:
      bitfieldFiller = ConstantVariable (255.0);
      randomEndStart = bitfieldSize - static_cast<uint32_t> (ceil (bitfieldSize * lexical_cast<double> (m_bitfieldFillType.substr (1, m_bitfieldFillType.size () - 1))));
      
      for (uint8_t i = 0; i < randomEndStart; ++i)
      {
         m_bitfield[i] = static_cast<uint8_t> (bitfieldFiller.GetInteger ());
      }            
      break;
    }

  bitfieldFiller = UniformVariable (0.0, 255.0);

  for (uint8_t i = randomEndStart; i < bitfieldSize; ++i)
    {
      m_bitfield[i] = static_cast<uint8_t> (bitfieldFiller.GetInteger ());
    }
  // <-- Step 3b

  // Step 3c: Apply the post-filling changes
  if (m_bitfieldManipulations.size () > 0)
    {
      for (std::map<uint32_t, uint8_t>::const_iterator it = m_bitfieldManipulations.begin (); it != m_bitfieldManipulations.end (); ++it)
        {
          if ((*it).first < m_torrent->GetNumberOfPieces ())
            {
              m_bitfield[(*it).first] = (*it).second;
            }
        }

      m_bitfieldManipulations.clear ();
    }

  // Step 3d: Fill up the rest of the bitfield so that the calculation of a completed download is easier
  uint8_t leftBits = 0;       // Step 3e depends on this and latter changes to it, do not remove if you don't also remove step 4e!

  if (m_torrent->HasTrailingPiece ())
    {
      leftBits = 7 - ((m_torrent->GetNumberOfPieces () - 1) % 8);
      for (int i = 0; i < leftBits; ++i)
        {
          m_bitfield[m_torrent->GetBitfieldSize () - 1] |= (1 << i);
        }
    }

  // Step 3e: Calculate the number of set bits in the bitfield to speed up the calculation of downloaded bytes for tracker announcements -->
  uint64_t bitsSet = 0;

  for (uint32_t pos = 0; pos < m_torrent->GetBitfieldSize (); ++pos)
    {
      uint8_t byte = m_bitfield[pos];
      if (byte == 255)
        {
          bitsSet += 8;
        }
      else
        {
          /*
           * Adaption of a "bit counting" algorithm by Brian Kernighan,
           * see http://www-graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
           */
          while (byte)
            {
              byte &= byte - 1;                               // Clear the least significant bit set
              ++bitsSet;
            }
        }
    }
  bitsSet -= leftBits;

  m_piecesCompleted = bitsSet;

  m_bytesCompleted = bitsSet * m_torrent->GetPieceLength ();
  if (m_torrent->HasTrailingPiece ())
    {
      if (((m_bitfield[m_torrent->GetBitfieldSize () - 1] >> leftBits) % 2) == 1)
        {
          m_bytesCompleted = m_torrent->GetTrailingPieceLength () + m_bytesCompleted - m_torrent->GetPieceLength ();
        }
    }
  // <-- Step 3e

  // Step 3f: Check whether the download was already completed
  m_downloadCompleted = true;
  for (uint32_t current = 0; current < m_torrent->GetBitfieldSize (); ++current)
    {
      if (m_bitfield[current] != 0xFF)
        {
          m_downloadCompleted = false;
          break;
        }
    }

  // Step 4: Initialize the desired protocol (also referred to as a "strategy bundle")
  Ptr<PeerConnectorStrategyBase> pcs; // Retrieves the peer connector strategy returned by the protocol
  ProtocolFactory::CreateStrategyBundle (m_protocol, this, m_strategyList, pcs);
  pcs->StartListening (GetPort ()); // Initialize the returned peer connector strategy

  // Step 5: If we should connect to the cloud automatically, we now do so
  if (GetAutoConnect ())
    {
      Simulator::ScheduleNow (&BitTorrentClient::JoinCloud, this);
    }

  // Step 6: Initialize the periodic gathering of metrics
  Simulator::Schedule (m_gatherMetricsEventPeriodicity, &BitTorrentClient::GatherMetricsEvent, this);

  // Step 7: Announce this instance to the user
  std::stringstream outputIpStream;
  GetIp ().Print (outputIpStream);
  std::string outputIpString = outputIpStream.str ();
  GlobalMetricsGatherer::GetInstance ()->WriteToFile (
    "active-clients",
    GetPeerId ()
    + " on node " + lexical_cast<std::string> (GetNode ()->GetId ())
#ifdef NS3_MPI
	  + " (MPI system ID " + lexical_cast<std::string> (GetNode ()->GetSystemId ()) + " )"
#endif	      
    + " (address " + outputIpString + ":" + lexical_cast<std::string> (GetPort ()) + ")"
    + " using protocol " + m_protocol
    + " starting with " + lexical_cast<std::string> (m_piecesCompleted) + " pieces completed"
    + " and auto-connect " + ( GetAutoConnect () ? "enabled" : "disabled"),
    true
    );

  // Step 8: Finish initialization by issuing an ApplicationInitializedEvent
  ApplicationInitializedEvent ();
}

void BitTorrentClient::SetTorrent (Ptr<Torrent> torrent)
{
  if (m_torrent == NULL)
    {
      m_torrent = torrent;
    }
}

void BitTorrentClient::SetProtocol (std::string protocol)
{
  if (m_protocol == "")
    {
      m_protocol = protocol;
    }
}

std::string BitTorrentClient::GetInitialBitfield () const
{
  return m_bitfieldFillType;
}

void BitTorrentClient::SetInitialBitfield (std::string bitfieldFillType)
{
  m_bitfieldFillType = bitfieldFillType;
}

void BitTorrentClient::ManipulateInitialBitfield (uint32_t bitfieldIndex, uint8_t value)
{
  if (bitfieldIndex < m_torrent->GetNumberOfPieces ())
    {
      m_bitfieldManipulations[bitfieldIndex] = value;
    }
}

Time BitTorrentClient::GetGatherMetricsEventPeriodicity () const
{
  return m_gatherMetricsEventPeriodicity;
}

void BitTorrentClient::SetGatherMetricsEventPeriodicity (Time gatherMetricsEventPeriodicity)
{
  if (gatherMetricsEventPeriodicity.IsStrictlyPositive ())
    {
      m_gatherMetricsEventPeriodicity = gatherMetricsEventPeriodicity;
    }
}

const uint8_t* BitTorrentClient::GetCurrentInfoHash () const
{
  return reinterpret_cast<const uint8_t*> (m_torrent->GetByteValueInfoHash ());
}

std::string BitTorrentClient::GetPeerId () const
{
  return m_peerId;
}

void BitTorrentClient::SetInterfaceId (uint16_t interfaceId)
{
  m_interfaceId = interfaceId;
  m_interface = GetNode() ->GetDevice (m_interfaceId);
  m_ip = GetNode ()->GetObject<Ipv4> ()->GetAddress (m_interfaceId, 0).GetLocal ();
}

uint16_t BitTorrentClient::GetInterfaceId () const
{
  return m_interfaceId;
}

Ptr<NetDevice> BitTorrentClient::GetInterface () const
{
  return m_interface;
}

Ipv4Address BitTorrentClient::GetIp () const
{
  return m_ip;
}

uint16_t BitTorrentClient::GetPort () const
{
  return m_port;
}

void BitTorrentClient::SetPort (uint16_t port)
{
  m_port = port;
}

DataRate BitTorrentClient::GetBpsDownlink () const
{
  DataRateValue result;
  GetNode()->GetDevice (m_interfaceId)->GetChannel ()->GetDevice (1)->GetAttribute ("DataRate", result);
  return result.Get ();
}

DataRate BitTorrentClient::GetBpsUplink () const
{
  DataRateValue result;
  GetNode()->GetDevice (m_interfaceId)->GetChannel ()->GetDevice (0)->GetAttribute ("DataRate", result);
  return result.Get ();
}

bool BitTorrentClient::GetAutoConnect () const
{
  return m_autoConnect;
}

void BitTorrentClient::SetAutoConnect (bool autoConnect)
{
  CHANGED_OPTION ("auto_connect", m_autoConnect, autoConnect);
  m_autoConnect = autoConnect;
}

Time BitTorrentClient::GetSeedingDuration () const
{
  return m_seedingDuration;
}

void BitTorrentClient::SetSeedingDuration (Time seedingDuration)
{
  m_seedingDuration = seedingDuration;
}

uint16_t BitTorrentClient::GetDesiredPeers () const
{
  return m_desiredPeers;
}

void BitTorrentClient::SetDesiredPeers (uint16_t desiredPeers)
{
  CHANGED_OPTION ("desired_peers", m_desiredPeers, desiredPeers);
  m_desiredPeers = desiredPeers;
}

void BitTorrentClient::SetMaxPeers (uint16_t maxPeers)
{
  CHANGED_OPTION ("max_peers", m_maxPeers, maxPeers);
  m_maxPeers = maxPeers;
}

uint16_t BitTorrentClient::GetMaxUnchokedPeers () const
{
  return m_maxUnchokedPeers;
}

void BitTorrentClient::SetMaxUnchokedPeers (uint16_t maxUnchokedPeers)
{
  CHANGED_OPTION ("max_unchoked_peers", m_maxUnchokedPeers, maxUnchokedPeers);
  m_maxUnchokedPeers = maxUnchokedPeers;
}                                                                                                                                                                                                                    // 0 => No limitation

void BitTorrentClient::SetMaxRequestsPerPeer (uint16_t maxRequestsPerPeer)
{
  CHANGED_OPTION ("max_requests_per_peer", m_maxRequestsPerPeer, maxRequestsPerPeer);
  m_maxRequestsPerPeer = maxRequestsPerPeer;
}

void BitTorrentClient::SetMaxRequestsPerPiece (uint16_t maxRequestsPerPiece)
{
  CHANGED_OPTION ("max_requests_per_piece", m_maxRequestsPerPiece, maxRequestsPerPiece);
  m_maxRequestsPerPiece = maxRequestsPerPiece;
}

void BitTorrentClient::SetMaxRequestsPerBlock (uint16_t maxRequestsPerBlock)
{
  CHANGED_OPTION ("max_requests_per_block",  m_maxRequestsPerBlock, maxRequestsPerBlock);
  m_maxRequestsPerBlock = maxRequestsPerBlock;
}

void BitTorrentClient::SetMaxRequestsPerPeerPerPiece (uint16_t maxRequestsPerPeerPerPiece)
{
  CHANGED_OPTION ("max_requests_per_peer_per_piece", m_maxRequestsPerPeerPerPiece, maxRequestsPerPeerPerPiece);
  m_maxRequestsPerPeerPerPiece = maxRequestsPerPeerPerPiece;
}

void BitTorrentClient::SetRequestBlockSize (uint32_t requestBlockSize)
{
  if (requestBlockSize > 0)
    {
      CHANGED_OPTION ("request_block_size", m_requestBlockSize, requestBlockSize);
      m_requestBlockSize = requestBlockSize;
    }
}

void BitTorrentClient::SetSendBlockSize (uint32_t sendBlockSize)
{
  if (sendBlockSize > 0)
    {
      CHANGED_OPTION ("send_block_size", m_sendBlockSize, sendBlockSize);
      m_sendBlockSize = sendBlockSize;
    }
}

void BitTorrentClient::SetPieceTimeout (Time pieceTimeout)
{
  if (pieceTimeout.GetMilliSeconds () > 0)
    {
      CHANGED_OPTION ("pieceTimeout", m_pieceTimeout, pieceTimeout);
      m_pieceTimeout = pieceTimeout;
    }
}

void BitTorrentClient::SetCheckDownloadedData (bool checkDownloadedData)
{
  CHANGED_OPTION ("check_downloaded_data", m_checkDownloadedData, checkDownloadedData);
  m_checkDownloadedData = checkDownloadedData;
}

void BitTorrentClient::SetPieceComplete (uint32_t pieceIndex)
{
  m_bitfield[pieceIndex / 8] |= (1 << (7 - (pieceIndex % 8)));

  if (pieceIndex != m_torrent->GetNumberOfPieces () - 1)
    {
      m_bytesCompleted += m_torrent->GetPieceLength ();
    }
  else
    {
      m_bytesCompleted += m_torrent->GetTrailingPieceLength ();
    }

  ++m_piecesCompleted;
}

const std::list<Ptr<AbstractStrategy> > & BitTorrentClient::GetStrategies () const
{
  return m_strategyList;
}

std::list<Ptr<AbstractStrategy> >::const_iterator BitTorrentClient::GetStrategyListIterator () const
{
  return m_strategyList.begin ();
}

std::list<Ptr<AbstractStrategy> >::const_iterator BitTorrentClient::GetStrategyListEnd () const
{
  return m_strategyList.end ();
}

const std::map<std::string, std::string >& BitTorrentClient::GetStrategyOptions () const
{
  return m_strategyOptions;
}

const std::pair<std::string, std::string> BitTorrentClient::GetStrategyOptionChangePair (std::string name) const
{
  std::map<std::string, std::pair<std::string, std::string> >::const_iterator result = m_lastChangedStrategyOptions.find (name);

  if (result == m_lastChangedStrategyOptions.end ())
    {
      return std::pair<std::string, std::string> ();
    }
  else
    {
      return (*result).second;
    }
}

std::string BitTorrentClient::GetLastChangedStrategyOptionName () const
{
  return m_lastChangedStrategyOptionName;
}

void BitTorrentClient::SetStrategyOptions (std::map<std::string, std::string> strategyOptions)
{
  if (strategyOptions.empty ())
    {
      return;
    }

  std::map<std::string, std::string>::const_iterator it;
  for (it = strategyOptions.begin (); it != strategyOptions.end (); ++it)
    {
      m_lastChangedStrategyOptions[(*it).first] = std::pair<std::string, std::string> (m_strategyOptions[(*it).first], (*it).second);
      m_strategyOptions[(*it).first] = (*it).second;

    }
  if (strategyOptions.size () != 1)
    {
      m_lastChangedStrategyOptionName = "";
    }
  else
    {
      std::map<std::string, std::pair<std::string, std::string> >::const_iterator it2 = m_lastChangedStrategyOptions.begin ();
      m_lastChangedStrategyOptionName = (*it2).first;
    }

  StrategyOptionsChangedEvent ();
  m_lastChangedStrategyOptions.clear ();
  m_lastChangedStrategyOptionName = "";
}

std::vector<Ptr<Peer> >::const_iterator BitTorrentClient::GetPeerListIterator () const
{
  return m_peerList.begin ();
}

std::vector<Ptr<Peer> >::const_iterator BitTorrentClient::GetPeerListEnd () const
{
  return m_peerList.end ();
}

void BitTorrentClient::RegisterPeer (Ptr<Peer> peer)
{
  m_peerList.push_back (peer);
}

void BitTorrentClient::UnregisterPeer (Ptr<Peer> peer)
{
  for (std::vector<Ptr<Peer> >::iterator it = m_peerList.begin (); it != m_peerList.end (); )
    {
      std::vector<Ptr<Peer> >::iterator it2 = it;
      ++it2;
      if ((*it) == peer)
        {
          m_peerList.erase (it);
          break;
        }
      it = it2;
    }
}

bool BitTorrentClient::GetConnectedToCloud () const
{
  return m_connectedToCloud;
}

void BitTorrentClient::SetConnectedToCloud (bool connectedToCloud)
{
  m_connectedToCloud = connectedToCloud;
}

bool BitTorrentClient::GetConnectionToCloudSuspended () const
{
  return m_connectionToCloudSuspended;
}

void BitTorrentClient::SetConnectionToCloudSuspended (bool connectionToCloudSuspended)
{
  m_connectionToCloudSuspended = connectionToCloudSuspended;
}

void BitTorrentClient::ApplicationInitializedEvent ()
{
  std::list<Callback<void, Ptr<BitTorrentClient > > >::iterator iter = m_applicationInitializedEventListeners.begin ();
  for (; iter != m_applicationInitializedEventListeners.end (); ++iter)
    {
      (*iter);
    }
}

void BitTorrentClient::TrackerResponseReceivedEvent ()
{
  std::list<Callback<void> >::iterator iter = m_trackerResponseReceivedListerners.begin ();
  for (; iter != m_trackerResponseReceivedListerners.end (); ++iter)
    {
      (*iter)();
    }
}

void BitTorrentClient::PeerConnectionEstablishedEvent (Ptr<Peer> peer)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_establishedEventListeners.begin ();
  for (; iter != m_establishedEventListeners.end (); ++iter)
    {
      (*iter)(peer);
    }
}

void BitTorrentClient::PeerConnectionFailEvent (Ptr<Peer> peer)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_failEventListeners.begin ();
  for (; iter != m_failEventListeners.end (); ++iter)
    {
      (*iter)(peer);
    }
}

void BitTorrentClient::PeerConnectionCloseEvent (Ptr<Peer> peer)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_closeEventListeners.begin ();
  for (; iter != m_closeEventListeners.end (); ++iter)
    {
      (*iter)(peer);
    }
}

void BitTorrentClient::CloudConnectionEstablishedEvent ()
{
  std::list<Callback<void, Ptr<BitTorrentClient> > >::iterator iter = m_cloudConnectionEstablishedEventListeners.begin ();
  for (; iter != m_cloudConnectionEstablishedEventListeners.end (); ++iter)
    {
      (*iter)(Ptr<BitTorrentClient> (this));
    }
}

void BitTorrentClient::CloudConnectionSuspendedEvent ()
{
  std::list<Callback<void, Ptr<BitTorrentClient> > >::iterator iter = m_cloudConnectionSuspendedEventListeners.begin ();
  for (; iter != m_cloudConnectionSuspendedEventListeners.end (); ++iter)
    {
      (*iter)(Ptr<BitTorrentClient> (this));
    }
}

void BitTorrentClient::PeerChokeChangingEvent (Ptr<Peer> peer)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_chokeEventListeners.begin ();
  for (; iter != m_chokeEventListeners.end (); ++iter)
    {
      (*iter)(peer);
    }
}

void BitTorrentClient::PeerInterestedChangingEvent (Ptr<Peer> peer)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_interedEventListeners.begin ();
  for (; iter != m_interedEventListeners.end (); ++iter)
    {
      (*iter)(peer);
    }
}

void BitTorrentClient::PeerHaveEvent (Ptr<Peer> peer, uint32_t pieceIndex)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t> >::iterator iter = m_haveEventListeners.begin ();
  for (; iter != m_haveEventListeners.end (); ++iter)
    {
      (*iter)(peer,pieceIndex);
    }
}

void BitTorrentClient::PeerBitfieldReceivedEvent (Ptr<Peer> peer)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_bitfieldEventListeners.begin ();
  for (; iter != m_bitfieldEventListeners.end (); ++iter)
    {
      (*iter)(peer);
    }
}

void BitTorrentClient::PeerRequestEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_requestEventListeners.begin ();
  for (; iter != m_requestEventListeners.end (); ++iter)
    {
      (*iter)(peer,pieceIndex,blockOffset,blockLength);
    }
}

void BitTorrentClient::PeerCancelEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_cancelEventListeners.begin ();
  for (; iter != m_cancelEventListeners.end (); ++iter)
    {
      (*iter)(peer,pieceIndex,blockOffset,blockLength);
    }
}

void BitTorrentClient::PeerPortMessageEvent (Ptr<Peer> peer, uint16_t port)
{
  std::list<Callback<void, Ptr<Peer>,uint16_t> >::iterator iter = m_portMessageEventListeners.begin ();
  for (; iter != m_portMessageEventListeners.end (); ++iter)
    {
      (*iter)(peer,port);
    }
}

void BitTorrentClient::PeerExtensionMessageEvent (Ptr<Peer> peer, uint8_t messageId, std::string message)
{
  std::list<Callback<void, Ptr<Peer>, const std::string& > >::iterator iter = m_extensionMessageListeners[messageId].begin ();
  for (; iter != m_extensionMessageListeners[messageId].end (); ++iter)
    {
      (*iter)(peer, message);
    }
}

void BitTorrentClient::PieceRequestedEvent (Ptr<Peer> peer, uint32_t pieceIndex)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t > >::iterator iter = m_pieceRequestedEventListeners.begin ();
  for (; iter != m_pieceRequestedEventListeners.end (); ++iter)
    {
      (*iter)(peer, pieceIndex);
    }
}


void BitTorrentClient::PeerBlockCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_blockCompleteEventListeners.begin ();
  for (; iter != m_blockCompleteEventListeners.end (); ++iter)
    {
      (*iter)(peer,pieceIndex,blockOffset,blockLength);
    }
}

void BitTorrentClient::PieceCancelledEvent (Ptr<Peer> peer, uint32_t pieceIndex)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t > >::iterator iter = m_pieceCancelledEventListeners.begin ();
  for (; iter != m_pieceCancelledEventListeners.end (); ++iter)
    {
      (*iter)(peer, pieceIndex);
    }
}

void BitTorrentClient::PieceTimeoutEvent (Ptr<Peer> peer, uint32_t pieceIndex)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t > >::iterator iter = m_pieceTimeoutEventListeners.begin ();
  for (; iter != m_pieceTimeoutEventListeners.end (); ++iter)
    {
      (*iter)(peer, pieceIndex);
    }
}

void BitTorrentClient::PieceCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t > >::iterator iter = m_pieceCompleteEventListeners.begin ();
  for (; iter != m_pieceCompleteEventListeners.end (); ++iter)
    {
      (*iter)(peer, pieceIndex);
    }
}

void BitTorrentClient::DownloadCompleteEvent ()
{
  m_downloadCompleted = true;

  TriggerCallbackAnnounceAsSeeder ();

  std::list<Callback<void> >::iterator iter = m_downloadCompleteEventListeners.begin ();
  for (; iter != m_downloadCompleteEventListeners.end (); ++iter)
    {
      (*iter)();
    }

  if (m_seedingDuration.IsPositive ())
    {
      Simulator::Schedule (m_seedingDuration, &BitTorrentClient::DisconnectFromCloud, this);
    }
}


void BitTorrentClient::PeerBlockUploadCompleteEvent (Ptr<Peer> peer, uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_blockUploadCompleteEventListeners.begin ();
  for (; iter != m_blockUploadCompleteEventListeners.end (); ++iter)
    {
      (*iter)(peer,pieceIndex,blockOffset,blockLength);
    }
}

// Used internally
void BitTorrentClient::StrategyOptionsChangedEvent (std::string optionName, std::string oldValue, std::string newValue)
{
  m_lastChangedStrategyOptionName = optionName;
  m_lastChangedStrategyOptions[optionName] = std::pair<std::string, std::string> (oldValue, newValue);
  StrategyOptionsChangedEvent ();
  m_lastChangedStrategyOptionName = "";
  m_lastChangedStrategyOptions.clear ();
}

void BitTorrentClient::StrategyOptionsChangedEvent ()
{
  std::list<Callback<void> >::iterator iter = m_strategyOptionsChangedEventListeners.begin ();
  for (; iter != m_strategyOptionsChangedEventListeners.end (); ++iter)
    {
      (*iter)();
    }
}

void BitTorrentClient::GatherMetricsEvent ()
{
  std::multimap<std::string, std::string> allMetrics;
  std::list<Callback<std::map<std::string, std::string> > >::iterator iter = m_gatherMetricsEventListeners.begin ();

  for (; iter != m_gatherMetricsEventListeners.end (); ++iter)
    {
      std::map<std::string, std::string> metrics = (*iter)();
      for (std::map<std::string, std::string>::const_iterator it = metrics.begin (); it != metrics.end (); ++it)
        {
          allMetrics.insert (std::pair<std::string, std::string> ((*it).first, (*it).second));
        }
    }

  AnnounceMetrics (allMetrics);

  Simulator::Schedule (m_gatherMetricsEventPeriodicity, &BitTorrentClient::GatherMetricsEvent, this);
}

void BitTorrentClient::AnnounceMetrics (std::multimap<std::string, std::string> metrics)
{
  if (!metrics.empty ())
    {
      for (std::multimap<std::string, std::string>::const_iterator it = metrics.begin (); it != metrics.end (); ++it)
        {
          std::cout << Simulator::Now ().GetMilliSeconds () << ": Node " << GetNode ()->GetId () << ": " << (*it).first << "=" << (*it).second << std::endl;
          // GlobalMetricsGatherer::GetInstance ()->WriteToFile ((*it).first, "Node " + lexical_cast<std::string> (GetNode ()->GetId ()) + ": " + (*it).second, true);
        }
    }
}

void BitTorrentClient::RegisterCallbackChokeChangingEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  m_chokeEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackChokeChangingEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_chokeEventListeners.begin ();
  for (; iter != m_chokeEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_chokeEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackInterestedChangingEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  m_interedEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackInterestedChangingEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_interedEventListeners.begin ();
  for (; iter != m_interedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_interedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackPeerHaveEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback)
{
  m_haveEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackPeerHaveEvent (Callback<void, Ptr<Peer>, uint32_t> eventCallback)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t> >::iterator iter = m_haveEventListeners.begin ();
  for (; iter != m_haveEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_haveEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackBitfieldReceivedEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  m_bitfieldEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackBitfieldReceivedEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_bitfieldEventListeners.begin ();
  for (; iter != m_bitfieldEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_bitfieldEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackRequestEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  m_requestEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackRequestEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_requestEventListeners.begin ();
  for (; iter != m_requestEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_requestEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackCancelEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  m_cancelEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackCancelEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_cancelEventListeners.begin ();
  for (; iter != m_cancelEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_cancelEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackPortMessageEvent (Callback<void, Ptr<Peer>,uint16_t> eventCallback)
{
  m_portMessageEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackPortMessageEvent (Callback<void, Ptr<Peer>,uint16_t> eventCallback)
{
  std::list<Callback<void, Ptr<Peer>,uint16_t> >::iterator iter = m_portMessageEventListeners.begin ();
  for (; iter != m_portMessageEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_portMessageEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackExtensionMessageEvent (uint8_t messageId, Callback<void, Ptr<Peer>, const std::string& > eventCallback)
{
  m_extensionMessageListeners[messageId].push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackExtensionMessageEvent (uint8_t messageId, Callback<void, Ptr<Peer>, const std::string& > eventCallback)
{
  std::list<Callback<void, Ptr<Peer>, const std::string& > >::iterator iter = m_extensionMessageListeners[messageId].begin ();
  for (; iter != m_extensionMessageListeners[messageId].end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_extensionMessageListeners[messageId].erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackConnectionEstablishedEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  m_establishedEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackConnectionEstablishedEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_establishedEventListeners.begin ();
  for (; iter != m_establishedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_establishedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackConnectionFailEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  m_failEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackConnectionFailEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_failEventListeners.begin ();
  for (; iter != m_failEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_failEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackConnectionCloseEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  m_closeEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackConnectionCloseEvent (Callback<void, Ptr<Peer> > eventCallback)
{
  std::list<Callback<void, Ptr<Peer> > >::iterator iter = m_closeEventListeners.begin ();
  for (; iter != m_closeEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_closeEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackCloudConnectionEstablishedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback)
{
  m_cloudConnectionEstablishedEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackCloudConnectionEstablishedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback)
{
  std::list<Callback<void, Ptr<BitTorrentClient> > >::iterator iter = m_cloudConnectionEstablishedEventListeners.begin ();
  for (; iter != m_cloudConnectionEstablishedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_cloudConnectionEstablishedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackCloudConnectionSuspendedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback)
{
  m_cloudConnectionSuspendedEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackCloudConnectionSuspendedEvent (Callback<void, Ptr<BitTorrentClient> > eventCallback)
{
  std::list<Callback<void, Ptr<BitTorrentClient> > >::iterator iter = m_cloudConnectionSuspendedEventListeners.begin ();
  for (; iter != m_cloudConnectionSuspendedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_cloudConnectionSuspendedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackPieceRequestedEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  m_pieceRequestedEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackPieceRequestedEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t> >::iterator iter = m_pieceRequestedEventListeners.begin ();
  for (; iter != m_pieceRequestedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_pieceRequestedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackBlockCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  m_blockCompleteEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackBlockCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_blockCompleteEventListeners.begin ();
  for (; iter != m_blockCompleteEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_blockCompleteEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackPieceCancelledEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  m_pieceCancelledEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackPieceCancelledEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t> >::iterator iter = m_pieceCancelledEventListeners.begin ();
  for (; iter != m_pieceCancelledEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_pieceCancelledEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackPieceCompleteEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  m_pieceCompleteEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackPieceCompleteEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t> >::iterator iter = m_pieceCompleteEventListeners.begin ();
  for (; iter != m_pieceCompleteEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_pieceCompleteEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackDownloadCompleteEvent (Callback<void> eventCallback)
{
  m_downloadCompleteEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackDownloadCompleteEvent (Callback<void> eventCallback)
{
  std::list<Callback<void> >::iterator iter = m_downloadCompleteEventListeners.begin ();
  for (; iter != m_downloadCompleteEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_downloadCompleteEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackPieceTimeoutEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  m_pieceTimeoutEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackPieceTimeoutEvent (Callback<void, Ptr<Peer>, uint32_t > eventCallback)
{
  std::list<Callback<void, Ptr<Peer>, uint32_t> >::iterator iter = m_pieceTimeoutEventListeners.begin ();
  for (; iter != m_pieceTimeoutEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_pieceTimeoutEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackBlockUploadCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  m_blockUploadCompleteEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackBlockUploadCompleteEvent (Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> eventCallback)
{
  std::list<Callback<void, Ptr<Peer>,uint32_t,uint32_t,uint32_t> >::iterator iter = m_blockUploadCompleteEventListeners.begin ();
  for (; iter != m_blockUploadCompleteEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_blockUploadCompleteEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackTrackerResponseReceivedEvent (Callback<void> eventCallback)
{
  m_trackerResponseReceivedListerners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackTrackerResponseReceivedEvent (Callback<void> eventCallback)
{
  std::list<Callback<void> >::iterator iter = m_trackerResponseReceivedListerners.begin ();
  for (; iter != m_trackerResponseReceivedListerners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_trackerResponseReceivedListerners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackApplicationInitializedEvent (Callback<void, Ptr<BitTorrentClient > > eventCallback)
{
  m_applicationInitializedEventListeners.push_back (eventCallback);

}
void BitTorrentClient::UnregisterCallbackApplicationInitializedEvent (Callback <void, Ptr<BitTorrentClient > > eventCallback)
{
  std::list<Callback<void, Ptr<BitTorrentClient > > >::iterator iter = m_applicationInitializedEventListeners.begin ();
  for (; iter != m_applicationInitializedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_applicationInitializedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackStrategyOptionsChangedEvent (Callback<void> eventCallback)
{
  m_strategyOptionsChangedEventListeners.push_back (eventCallback);
}

void BitTorrentClient::UnregisterCallbackStrategyOptionsChangedEvent (Callback<void> eventCallback)
{
  std::list<Callback<void> >::iterator iter = m_strategyOptionsChangedEventListeners.begin ();
  for (; iter != m_strategyOptionsChangedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_strategyOptionsChangedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::RegisterCallbackGatherMetricsEvent (Callback<std::map<std::string, std::string> > eventCallback)
{
  m_gatherMetricsEventListeners.push_back (eventCallback);

}

void BitTorrentClient::UnregisterCallbackGatherMetricsEvent (Callback<std::map<std::string, std::string> > eventCallback)
{
  std::list<Callback<std::map<std::string, std::string> > >::iterator iter = m_gatherMetricsEventListeners.begin ();
  for (; iter != m_gatherMetricsEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_gatherMetricsEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentClient::JoinCloud ()
{
  if (!GetConnectedToCloud ())
    {
      m_connectToCloud ();
    }
}

void BitTorrentClient::DisconnectFromCloud ()
{
  m_disconnectFromCloud ();
}

void BitTorrentClient::TriggerCallbackAnnounceAsSeeder ()
{
  m_announceAsSeeder ();
}

void BitTorrentClient::TriggerCallbackConnectToPeers (uint16_t count)
{
  m_connectToPeers (count);
}

void BitTorrentClient::TriggerCallbackConnectToPeer (Ipv4Address address, uint16_t port)
{
  m_connectToPeer (address, port);
}

void BitTorrentClient::TriggerCallbackDisconnectPeers (int32_t count)
{
  m_disconnectPeers (count);
}

void BitTorrentClient::TriggerCallbackDisconnectPeer (Ptr<Peer> peer)
{
  m_disconnectPeer (peer);
}

uint16_t BitTorrentClient::TriggerCallbackGetPeerCount ()
{
  return m_peerCount ();
}

void BitTorrentClient::SetCallbackConnectToCloud (Callback<void > eventCallback)
{
  m_connectToCloud = eventCallback;
}

void BitTorrentClient::SetCallbackDisconnectFromCloud (Callback<void > eventCallback)
{
  m_disconnectFromCloud = eventCallback;
}

void BitTorrentClient::SetCallbackAnnounceAsSeeder (Callback<void > eventCallback)
{
  m_announceAsSeeder = eventCallback;
}

void BitTorrentClient::SetCallbackConnectToPeers (Callback<uint16_t, uint16_t> eventCallback)
{
  m_connectToPeers = eventCallback;
}

void BitTorrentClient::SetCallbackConnectToPeer (Callback<void, Ipv4Address, uint16_t> eventCallback)
{
  m_connectToPeer = eventCallback;
}

void BitTorrentClient::SetCallbackDisconnectPeers (Callback<void, int32_t> eventCallback)
{
  m_disconnectPeers = eventCallback;
}

void BitTorrentClient::SetCallbackDisconnectPeer (Callback<void, Ptr<Peer> > eventCallback)
{
  m_disconnectPeer = eventCallback;
}

void BitTorrentClient::SetCallbackGetPeerCount (Callback<uint16_t> eventCallback)
{
  m_peerCount = eventCallback;
}

} // ns bittorrent
} //namespace ns3
