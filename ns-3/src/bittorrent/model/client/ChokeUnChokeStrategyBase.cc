/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 ComSys, RWTH Aachen University
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
 * Authors: Rene Glebke
 */

#include "ChokeUnChokeStrategyBase.h"

#include "BitTorrentClient.h"
#include "BitTorrentPeer.h"


#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/pointer.h"
#include "ns3/random-variable.h"

#include <list>
#include <set>
#include <stack>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::CokeUnChokeStrategyBase");

ChokeUnChokeStrategyBase::ChokeUnChokeStrategyBase (Ptr<BitTorrentClient> myClient) : AbstractStrategy (myClient)
{
  m_periodicInterval = Seconds (10);       // see wiki.theory.org

  m_optimisticMultiple = 3;
  m_periodicScheduleRound = 0;

  m_newlyConnectedPeersSinceLastPeriod = 0;

  m_optimisticallyUnchokedPeer = PointerValue (0);
}

ChokeUnChokeStrategyBase::~ChokeUnChokeStrategyBase ()
{
  Simulator::Cancel (m_nextPeriodicEvent);
}


void ChokeUnChokeStrategyBase::DoInitialize ()
{
  // Step 1: Register for all needed events within the client
  m_myClient->RegisterCallbackBitfieldReceivedEvent (MakeCallback (&ChokeUnChokeStrategyBase::ProcessBitfieldReceivedEvent, this));

  m_myClient->RegisterCallbackInterestedChangingEvent (MakeCallback (&ChokeUnChokeStrategyBase::ProcessPeerInterestedChangingEvent,this));
  m_myClient->RegisterCallbackConnectionCloseEvent (MakeCallback (&ChokeUnChokeStrategyBase::ProcessPeerConnectionCloseEvent,this));

  m_myClient->RegisterCallbackGatherMetricsEvent (MakeCallback (&ChokeUnChokeStrategyBase::ProcessGatherMetricsEvent, this));

  // Step 2: Start the strategy by issuing the first periodic schedule processing in m_periodicInterval seconds
  m_nextPeriodicEvent = Simulator::ScheduleNow (&ChokeUnChokeStrategyBase::ProcessPeriodicSchedule, this);
}

Time ChokeUnChokeStrategyBase::GetPeriodicInterval () const
{
  return m_periodicInterval;
}

void ChokeUnChokeStrategyBase::SetPeriodicInterval (Time periodicInterval)
{
  if (periodicInterval.IsStrictlyPositive ())
    {
      Simulator::Cancel (m_nextPeriodicEvent);
      m_periodicInterval = periodicInterval;
      m_nextPeriodicEvent = Simulator::Schedule (m_periodicInterval, &ChokeUnChokeStrategyBase::ProcessPeriodicSchedule, this);
    }
}

uint32_t ChokeUnChokeStrategyBase::GetOptimisticMultiple () const
{
  return m_optimisticMultiple;
}

void ChokeUnChokeStrategyBase::SetOptimisticMultiple (uint32_t optimisticMultiple)
{
  if(optimisticMultiple > 0)
    {
      m_optimisticMultiple = optimisticMultiple;
    }
}

void ChokeUnChokeStrategyBase::ProcessBitfieldReceivedEvent (Ptr<Peer> peer)
{
  m_newlyConnectedPeers.push_back (peer);
  ++m_newlyConnectedPeersSinceLastPeriod;

  // Each new peer starts off choked
  peer->SetAmChoking (true);
}

void ChokeUnChokeStrategyBase::ProcessPeerInterestedChangingEvent (Ptr<Peer> peer)
{
  // Put the peer into the right interested state when it sends an according message
  if (peer->IsInterested ())
    {
      m_interestedPeers.insert (peer);
      m_uninterestedPeers.erase (peer);

      // If less than the maximum of clients is currently unchoked - and this one is interested in being unchoked - trigger the periodic event immediately
      uint16_t max = m_myClient->GetMaxUnchokedPeers ();
      if (max == 0)
        {
          max = m_myClient->GetMaxPeers ();
        }
      if (m_downloadingPeers.size () < max)
        {
          Simulator::Cancel (m_nextPeriodicEvent);
          ProcessPeriodicSchedule ();
        }
    }
  else
    {
      m_uninterestedPeers.insert (peer);
      m_interestedPeers.erase (peer);
      m_downloadingPeers.erase (peer);          // Uninterested peers don't appear amongst the downloading peers
    }
}

void ChokeUnChokeStrategyBase::ProcessPeerConnectionCloseEvent (Ptr<Peer> peer)
{
  m_unchokedPeers.erase (peer);
  m_downloadingPeers.erase (peer);
  m_interestedPeers.erase (peer);
  m_uninterestedPeers.erase (peer);
  m_newlyConnectedPeers.remove (peer);

  // If some unchoked peer has closed the connection, trigger the assignment process
  if (peer == m_optimisticallyUnchokedPeer)
    {
      m_optimisticallyUnchokedPeer = PointerValue (0);
    }
  uint16_t max = m_myClient->GetMaxUnchokedPeers ();
  if (max == 0)
    {
      max = m_myClient->GetMaxPeers ();
    }
  if (m_downloadingPeers.size () < max)
    {
      Simulator::Cancel (m_nextPeriodicEvent);
      ProcessPeriodicSchedule ();
    }
}

void ChokeUnChokeStrategyBase::ProcessPeriodicSchedule ()
{
  // A new periodic schedule round
  m_periodicScheduleRound = (m_periodicScheduleRound + 1) % 6;

  // Correct the list of newly connected peers so it only includes the peers connected within the last 60 seconds
  if (m_periodicScheduleRound == 0)
    {
      for (uint16_t i = std::min (m_newlyConnectedPeersSinceLastPeriod, static_cast<uint16_t> (m_newlyConnectedPeers.size ())); i > 0; --i)
        {
          m_newlyConnectedPeers.pop_front ();
        }
      m_newlyConnectedPeersSinceLastPeriod = 0;
    }

  std::set<Ptr<Peer> > peersToChoke;
  std::set<Ptr<Peer> > peersToUnchoke;
  std::set<Ptr<Peer> > topInterestedPeers;              // Candidates for insertion into the set of downloading peers
  std::set<Ptr<Peer> > badDownloadingPeers;             // Candidates for removal from the set of downloading peers
  std::stack<Ptr<Peer> > goodDownloadingPeersShelter;       // Temporary container for downloading peers that should not be touched

  // First, we insert the currently downloading peers to their respective categories
  for (std::set<Ptr<Peer> >::iterator it = m_downloadingPeers.begin (); it != m_downloadingPeers.end (); )
    {
      std::set<Ptr<Peer> >::iterator it2 = it;
      ++it2;

      if ((*it)->IsInterested ())
        {
          m_interestedPeers.insert (*it);
          m_uninterestedPeers.erase (*it);
        }
      else
        {
          m_uninterestedPeers.insert (*it);
          m_interestedPeers.erase (*it);

          badDownloadingPeers.insert (*it);              // This peer is a good replacement for better (and interested) peers
          m_downloadingPeers.erase (*it);              // Uninterested peers cannot be amongst the downloading peers
        }

      peersToChoke.insert (*it);          // Generally choke all unchoked peers (revise this decision in the latter course of the method)
      // DO NOT REMOVE THE PEERS HERE GENERALLY - Otherwise, they may be informed again of being unchoked whilst they already are

      it = it2;
    }

  // Then, we do the same thing with the currently unchoked peers
  for (std::set<Ptr<Peer> >::iterator it = m_unchokedPeers.begin (); it != m_unchokedPeers.end (); )
    {
      std::set<Ptr<Peer> >::iterator it2 = it;
      ++it2;

      if ((*it)->IsInterested ())
        {
          m_interestedPeers.insert (*it);
          m_uninterestedPeers.erase (*it);
        }
      else
        {
          m_uninterestedPeers.insert (*it);
          m_interestedPeers.erase (*it);
        }

      peersToChoke.insert (*it);          // Generally choke all unchoked peers (revise this decision in the latter course of the method)
      m_unchokedPeers.erase (*it);

      it = it2;
    }

  // Now, the same thing with our optimistically unchoked peer (but only every m_optimisticMultiple rounds)
  if (m_optimisticallyUnchokedPeer != NULL && m_periodicScheduleRound % m_optimisticMultiple == 0)
    {
      if (m_optimisticallyUnchokedPeer->IsInterested ())
        {
          m_uninterestedPeers.erase (m_optimisticallyUnchokedPeer);
          m_interestedPeers.insert (m_optimisticallyUnchokedPeer);
        }
      else
        {
          m_interestedPeers.erase (m_optimisticallyUnchokedPeer);
          m_uninterestedPeers.insert (m_optimisticallyUnchokedPeer);
        }

      peersToChoke.insert (m_optimisticallyUnchokedPeer);
      // peersToUnchoke.erase(m_optimisticallyUnchokedPeer);
    }

  // Now, we get the top interested peers to unchoke (number set in the client)
  uint16_t max = m_myClient->GetMaxUnchokedPeers ();
  if (max == 0)
    {
      max = m_myClient->GetMaxPeers ();
    }
  while (topInterestedPeers.size () < max && !m_interestedPeers.empty ())
    {
      Ptr<Peer> newCandidate = GetCandidateForInsertion ().second;

      // All found peers are equally bad; we stick with the current ones to avoid fibrillation
      // Note: This may lead to worse peer performance but sticks to the "protocol definition" on wiki.theory.org a little more...
      if (newCandidate == NULL)
        {
          break;
        }

      peersToUnchoke.insert (newCandidate);          // We want to unchoke this peer...
      peersToChoke.erase (newCandidate);          // ... and not choke it

      m_interestedPeers.erase (newCandidate);          // We've assigned this peer a new role. It will be downloading, not interested anymore
      m_newlyConnectedPeers.remove (newCandidate);          // So we cannot get this peer again later when optimistically unchoking

      topInterestedPeers.insert (newCandidate);
    }

  // Now, for already-downloading peers among the top 4 interested peers, make sure that:
  // a) They won't be chosen as a replacement for new top 4 peers by deleting them from m_downloadingPeers
  // b) They won't be informed about unchoking status again by deleting them from peersToUnchoke
  // c) They won't be choked by deleting them from peersToChoke
  // d) They won't be visited again to see whether they indicate any possible replacement to be found
  for (std::set<Ptr<Peer> >::iterator it = topInterestedPeers.begin (); it != topInterestedPeers.end (); )
    {
      std::set<Ptr<Peer> >::iterator it2 = it;
      ++it2;

      if (m_downloadingPeers.find (*it) != m_downloadingPeers.end ())
        {
          goodDownloadingPeersShelter.push (*it);              // d)(2) // Moved here to calm valgrind
          m_downloadingPeers.erase (*it);              // a)
          peersToUnchoke.erase (*it);              // b)
          peersToChoke.erase (*it);              // c)
          topInterestedPeers.erase (*it);              // d)(1)
        }
      it = it2;
    }

  // Now, we get as many bad downloading peers as we need to exchange them with the better ones from above
  std::pair<double, Ptr<Peer> > worstDownloadingPeer = GetCandidateForRemoval ();       // We need this afterwards

  if (!m_downloadingPeers.empty ())
    {
      while ((badDownloadingPeers.size () < topInterestedPeers.size ()) && !m_downloadingPeers.empty ())
        {
          Ptr<Peer> newCandidate = GetCandidateForRemoval ().second;

          NS_ASSERT (newCandidate != NULL);

          peersToChoke.insert (newCandidate);              // This peer is worse than one of the top 4 peers, so we choke ...
          peersToUnchoke.erase (newCandidate);              // ... and not unchoke it

          m_downloadingPeers.erase (newCandidate);              // When we choke it, we also cannot have it as a downloading peer

          badDownloadingPeers.insert (newCandidate);
        }
    }

  // Now, we have chosen our peers to unchoke because they behave well and the peers to choke in exchange. Let's find the other peers
  // to unchoke because their upload / download behavior is still good and they are not interested at the moment (see wiki.theory.org)
  if (worstDownloadingPeer.second != NULL)
    {
      for (std::set<Ptr<Peer> >::iterator it = m_uninterestedPeers.begin (); it != m_uninterestedPeers.end (); )
        {
          std::set<Ptr<Peer> >::iterator it2 = it;
          ++it2;

          bool unchoke = false;

          if (m_myClient->GetDownloadCompleted ())
            {
              unchoke = ((*it)->GetBpsDownload () >= worstDownloadingPeer.first);
            }
          else
            {
              unchoke = ((*it)->GetBpsUpload () >= worstDownloadingPeer.first);
            }

          if (unchoke)
            {
              peersToUnchoke.insert (*it);
              peersToChoke.erase (*it);

              //m_unchokedPeers.insert(*it);
              m_uninterestedPeers.erase (*it);
            }

          it = it2;
          // There should be no "else" needed here -- uninterested peers are generally choked (even the former unchoked ones, see above)
        }
    }

  // Finally, choose our new optimistcally unchoked peer
  Ptr<Peer> newOptimisticallyUnchokedPeer = PointerValue (0);
  if (m_periodicScheduleRound % m_optimisticMultiple == 0)      // Only optimistically unchoke every m_optimisticMultiple rounds
    {
      bool newOptimisticallyUnchokedPeerFound = false;

      // New peers are 3 times more probable to be optimistically unchoked than older peers (see wiki.theory.org)
      UniformVariable uv;
      uint8_t newPeerChooser = uv.GetInteger (0,2);

      if (newPeerChooser < 2 && m_newlyConnectedPeers.size () > 0)         // but since they may also be included in the list of "interested" peers, we only need to check for a "2" instead of a "3" here
        {
          // Approach: Try out finding a suitable sample in a list of length n by taking n random samples.
          // RENE: This usually works well but can become horribly inefficient, we shall try and find a better solution for this...
          uint8_t maxTries = m_newlyConnectedPeers.size ();
          while (maxTries > 0 && !newOptimisticallyUnchokedPeerFound)
            {
              std::list<Ptr<Peer> >::iterator it = m_newlyConnectedPeers.begin ();
              for (uint16_t unchokeChoice = uv.GetInteger (0, m_newlyConnectedPeers.size () - 1); unchokeChoice > 0; --unchokeChoice)
                {
                  ++it;
                }

              if ((*it)->IsInterested ())                 // Should mean "interested and not yet unchoked"
                {
                  newOptimisticallyUnchokedPeer = (*it);
                  m_optimisticallyUnchokedPeer = newOptimisticallyUnchokedPeer;

                  peersToUnchoke.insert (newOptimisticallyUnchokedPeer);
                  peersToChoke.erase (newOptimisticallyUnchokedPeer);

                  m_interestedPeers.erase (*it);
                  m_newlyConnectedPeers.remove (*it);

                  newOptimisticallyUnchokedPeerFound = true;
                }

              --maxTries;
            }
        }

      // If not using the newly connected peers (or if no interested newly connected peer is found), use one of the normal interested peers for optimistic unchoking
      if (!newOptimisticallyUnchokedPeerFound && m_interestedPeers.size () > 0)
        {
          std::set<Ptr<Peer> >::iterator it = m_interestedPeers.begin ();
          for (uint16_t unchokeChoice = uv.GetInteger (0, m_interestedPeers.size () - 1); unchokeChoice > 0; --unchokeChoice)
            {
              ++it;
            }

          newOptimisticallyUnchokedPeer = (*it);
          m_optimisticallyUnchokedPeer = newOptimisticallyUnchokedPeer;

          peersToUnchoke.insert (newOptimisticallyUnchokedPeer);
          peersToChoke.erase (newOptimisticallyUnchokedPeer);

          m_newlyConnectedPeers.remove (*it);
          m_interestedPeers.erase (*it);

          newOptimisticallyUnchokedPeerFound = true;
        }
    }

  // AT LAST, execute the choking and unchoking by issuing the corresponsing messages
  for (std::set<Ptr<Peer> >::iterator it = peersToChoke.begin (); it != peersToChoke.end (); ++it)
    {
      NS_LOG_INFO ("ChokeUnChokeStrategyBase: Choked " << (*it)->GetRemoteIp () << " (connected for " << (Simulator::Now () - (*it)->GetConnectionEstablishmentTime ()).GetSeconds () << " seconds)" << "; upload rate: " << (*it)->GetBpsUpload () << ", download rate: " << (*it)->GetBpsDownload () << "; interested: " << (*it)->IsInterested ());

      if ((*it)->IsInterested ())
        {
          m_interestedPeers.insert (*it);
          m_uninterestedPeers.erase (*it);
        }
      else
        {
          m_uninterestedPeers.insert (*it);
          m_interestedPeers.erase (*it);
        }

      m_unchokedPeers.erase (*it);
      m_downloadingPeers.erase (*it);

      (*it)->SetAmChoking (true);

    }

  for (std::set<Ptr<Peer> >::iterator it = peersToUnchoke.begin (); it != peersToUnchoke.end (); ++it)
    {
      NS_LOG_INFO ("ChokeUnChokeStrategyBase: Unchoked " << (*it)->GetRemoteIp () << " (connected for " << (Simulator::Now () - (*it)->GetConnectionEstablishmentTime ()).GetSeconds () << " seconds)" << "; upload rate: " << (*it)->GetBpsUpload () << ", download rate: " << (*it)->GetBpsDownload () << "; interested: " << (*it)->IsInterested () << "; optimistically unchoked: " << ((*it) == newOptimisticallyUnchokedPeer));

      if ((*it)->IsInterested ())
        {
          // When a peer is interested (and if it is not the optimistically unchoked peer), it is DOWNLOADING, not just interested or unchoked
          if ((*it) != newOptimisticallyUnchokedPeer)
            {
              m_downloadingPeers.insert (*it);
            }

          m_unchokedPeers.erase (*it);
          m_interestedPeers.erase (*it);
        }
      else
        {
          m_unchokedPeers.insert (*it);
          m_downloadingPeers.erase (*it);
          m_uninterestedPeers.erase (*it);
        }

      (*it)->SetAmChoking (false);
    }

  // To end this, add the sheltered downloading peers into the set of downloading peers again
  while (!goodDownloadingPeersShelter.empty ())
    {
      NS_LOG_INFO ("ChokeUnChokeStrategyBase: Left good peer " << goodDownloadingPeersShelter.top ()->GetRemoteIp () << " unchoked.");

      m_downloadingPeers.insert (goodDownloadingPeersShelter.top ());
      goodDownloadingPeersShelter.pop ();
    }

  // Schedule the next periodic call of all that cra... I mean, this nice routine ;)
  m_nextPeriodicEvent = Simulator::Schedule (m_periodicInterval, &ChokeUnChokeStrategyBase::ProcessPeriodicSchedule, this);
}

std::pair<double, Ptr<Peer> > ChokeUnChokeStrategyBase::GetCandidateForRemoval () const
{
  std::pair<double, Ptr<Peer> > worstDownloadingPeer = std::pair<double, Ptr<Peer> > (std::numeric_limits<double>::max (), PointerValue (0));

  if (!m_myClient->GetDownloadCompleted ())
    {
      for (std::set<Ptr<Peer> >::const_iterator it = m_downloadingPeers.begin (); it != m_downloadingPeers.end (); ++it)
        {
          if ((*it)->GetBpsDownload () <= worstDownloadingPeer.first)
            {
              worstDownloadingPeer = std::pair<double, Ptr<Peer> > ((*it)->GetBpsDownload (), (*it));
            }
        }
    }
  else
    {
      for (std::set<Ptr<Peer> >::const_iterator it = m_downloadingPeers.begin (); it != m_downloadingPeers.end (); ++it)
        {
          if ((*it)->GetBpsUpload () <= worstDownloadingPeer.first)
            {
              worstDownloadingPeer = std::pair<double, Ptr<Peer> > ((*it)->GetBpsUpload (), (*it));
            }
        }
    }

  return worstDownloadingPeer;
}

std::pair<double, Ptr<Peer> > ChokeUnChokeStrategyBase::GetCandidateForInsertion () const
{
  std::pair<double, Ptr<Peer> > bestInterestedPeer = std::pair<double, Ptr<Peer> > (-1.0, PointerValue (0));

  if (!m_myClient->GetDownloadCompleted ())
    {
      for (std::set<Ptr<Peer> >::const_iterator it = m_interestedPeers.begin (); it != m_interestedPeers.end (); ++it)
        {
          if ((*it)->GetBpsDownload () > bestInterestedPeer.first)
            {
              bestInterestedPeer = std::pair<double, Ptr<Peer> > ((*it)->GetBpsDownload (), (*it));
            }
        }
    }
  else
    {
      for (std::set<Ptr<Peer> >::const_iterator it = m_interestedPeers.begin (); it != m_interestedPeers.end (); ++it)
        {
          if ((*it)->GetBpsUpload () > bestInterestedPeer.first)
            {
              bestInterestedPeer = std::pair<double, Ptr<Peer> > ((*it)->GetBpsUpload (), (*it));
            }
        }
    }

  return bestInterestedPeer;
}

std::map<std::string, std::string> ChokeUnChokeStrategyBase::ProcessGatherMetricsEvent ()
{
  std::map<std::string, std::string> result;

  double uploadSpeed = 0, downloadSpeed = 0;
  for (std::vector<Ptr<Peer> >::const_iterator it = m_myClient->GetPeerListIterator (); it != m_myClient->GetPeerListEnd (); ++it)
    {
      uploadSpeed += (*it)->GetBpsUpload ();
      downloadSpeed += (*it)->GetBpsDownload ();
    }

  result["upload-speed"] = lexical_cast<std::string> (uploadSpeed) + "bps";
  result["download-speed"] = lexical_cast<std::string> (downloadSpeed) + "bps";

  return result;
}

} // ns bittorrent
} // ns ns3
