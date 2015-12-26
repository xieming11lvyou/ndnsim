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

#ifndef BITTORRENTVIDEOMETRICS_H_
#define BITTORRENTVIDEOMETRICS_H_

#include "AbstractStrategy.h"
#include "BitTorrentVideoClient.h"

#include "ns3/nstime.h"

#include <map>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Gathers client-local information about a simulated BitTorrent-based Video-on-Demand (VOD) process.
 *
 * This class collects information from one client in a simulated BitTorrent Video-on-Demand swarm for further analysis or the implementation of (theoretical)
 * protocols that use statistical information for parameter adaption.
 *
 * The class as an example includes an implementation of the Fluency metric introduced by Huang et.al. in their <a href="http://dx.doi.org/10.1145/1402958.1403001" target="_blank">2008 SIGCOMM paper</a>.
 */
class BitTorrentVideoMetricsBase : public AbstractStrategy
{
// Fields
private:
  // The BitTorrentVideoClient this instance is associated with
  Ptr<BitTorrentVideoClient> m_myVideoClient;

  // Was a periodic position change announced? (Important for some metrics)
  bool m_periodicPositionChangeAnnounced;

  // Fluency metric
  Time m_bufferStart, m_bufferEnd, m_playbackStart, m_playbackEnd;
  Time m_fluencyNumerator, m_fluencyDenominator;

// Constructors etc.
public:
  BitTorrentVideoMetricsBase (Ptr<BitTorrentClient> myClient);
  virtual ~BitTorrentVideoMetricsBase ();
  virtual void DoInitialize ();

// Event listeners etc.
public:
  void ProcessPlaybackPositionWillChangePeriodicallyEvent ();
  virtual void ProcessPlaybackPositionChangedEvent (Time newPosition);

  /**
   * \brief React to a change of the playback state of a client.
   */
  virtual void ProcessPlaybackStateChangedEvent ();

// Public methods for accessing the measured metrics
public:
  /**
   * \brief Return the collected periodic metrics.
   *
   * The up-to-dateness of the periodic metrics depends on their specific implementation.
   *
   * @returns a map containing the last values of the collected periodic metrics
   */
  std::map<std::string, std::string> ReturnPeriodicMetrics ();
};

} // ns bittorrent
} // ns ns3

#endif /* BITTORRENTVIDEOMETRICS_H_ */
