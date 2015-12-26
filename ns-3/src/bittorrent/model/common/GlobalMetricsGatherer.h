/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2012 ComSys, RWTH Aachen University
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

#ifndef GLOBALMETRICSGATHERER_H_
#define GLOBALMETRICSGATHERER_H_


#include "ns3/BitTorrentClient.h"
#include "ns3/BitTorrentPeer.h"

#include "ns3/application-container.h"

#include <map>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Gathers simulation-global information about a simulated BitTorrent swarm.
 *
 * This class collects information from all clients in a simulated BitTorrent swarm for further analysis or the implementation of (theoretical)
 * protocols that have a consistent, global view on the swarm.
 *
 * Currently, this class only supports one (anonymous) swarm, regardless of how many actual swarms (i.e., shared files) are simulated.
 */
class GlobalMetricsGatherer
{
// Fields
private:
  // The prefix for appended files
  std::string m_fileNamePrefix;

  // Whether file output is enabled or not
  bool m_fileOutputEnabled;

  // Applications already registered
  ApplicationContainer m_registeredWith;

  // Application statistics
  uint32_t m_appCount;                 // The number of applications registered with the global metrics gatherer
  uint32_t m_externalAppCount;         // The number of external applications (real-world cllients) registered
  uint32_t m_finishedAppCount;         // The number of applications which have announced to have finished a download
  uint32_t m_finishedExternalAppCount; // The number of external applications which have announced to have finished a download

  // Automatic exiting of simulations when a specific number of finished applications has been reached
  // int64_t m_stopThreshold;             // The number of applications that should have finished before the GMG stops the simulation
  double  m_stopFraction;              // The fraction of clients that should have finished before the GMG stops the simulation
  double  m_externalStopFraction;      // The fraction of external clients that should have finished before the GMG stops the simulation

  // For simulation-wide piece distribution analysis
  std::map<uint32_t, uint32_t> m_pieceCount;     // How often a piece is present within the clients
  std::map<uint32_t, uint32_t> m_pieceDemand;    // How many clients have currently expressed interest in a piece

// Constructors etc. (singleton pattern)
private:
  GlobalMetricsGatherer ();
  virtual ~GlobalMetricsGatherer ();
public:
  static GlobalMetricsGatherer* GetInstance ()
  {
    static GlobalMetricsGatherer s_instance;

    return &s_instance;
  }

// Control functions
public:
  /**
   * \brief Set a general prefix for the names of output files.
   *
   * This method sets a string that is prefixed to any name of files that the global instance of this class writes to.
   * The GlobalMetricsGatherer appends information to files and it is imperative to call this function with a meaningful
   * value to ensure that the correct files are appended.
   *
   * @param fileNamePrefix the desired prefix. May contain slashes to denote paths.
   * @param enableLogging whether to enable logging to a file or to disable it.
   *  If logging to file is disabled, all logging output is redirected to the standard output.
   */
  void SetFileNamePrefix (const std::string fileNamePrefix, bool enableLogging);

  /**
   * @returns the general prefix for the names of output files. File output is enabled if the returned value is not equal to "undefined".
   */
  std::string GetFileNamePrefix () const;

  /**
   * @returns true, if the metrics gatherer is currently logging to files. The prefix of these files can be obtained via the GetFileNamePrefix method.
   */
  bool GetLoggingToFileEnabled () const;

  /**
   * \brief Register the global metrics gatherer with the supplied instances of the BitTorrentClient class.
   *
   * This method registers the global metrics gatherer as a listener for certain events within the supplied BitTorrent clients.
   * You can override this method to add further metrics to be calculated for the given clients. Clients need not be added all at once,
   * the gatherer simply attaches itself to additionally-supplied clients.
   *
   * @param appContainer the applications that the global metrics gatherer should register with.
   */
  virtual void RegisterWithApplications (ApplicationContainer appContainer);

  /**
   * \brief Announce the existence of external clients to the global metrics gatherer.
   *
   * In some situations, the global metrics gatherer may be interested in knowing how many external clients are existing in an
   * emulation scenario (i.e., when a TapBridge or related method is used to connect ns3 to the executing system), e.g., to
   * evaluate the stop fraction (see the SetStopFraction method) correctly.
   *
   * @param count the number of external clients to announce. The number is added to the number of already-announced external clients.
   */
  void AnnounceExternalClients (uint32_t count);

  /**
   * \brief Set the fraction of clients that have to finish their download before the simulation is stopped.
   *
   * The global metrics gatherer can automatically stop the simulation when a certain fraction of clients
   * have completed their download. This function can be used to set that fraction.
   *
   * The fraction must take account of the clients starting off as seeders, i.e., they will be included in the clients that
   * have completed their download.
   *
   * Note: You must call the SetStopFraction method again after issuing a call to this method.
   *
   * @param stopFraction the desired fraction of clients (0.0 <= stopFraction <= 1.0). If the parameter is set to -1.0, the simulation
   *  is not stopped after a specific threshold.
   * @param stopFractionExternal the same as stopFraction, but applied to external clients that were announced to the global metrics
   *  gatherer (if any).
   */
  virtual void SetStopFraction (double stopFraction, double stopFractionExternal);

// Output generation
/**
 * \brief Write information for a certain metric to a file.
 *
 * This method appends a file with a file name of the structure GetFileNamePrefix()-metricName.dat with the supplied string.
 *
 * @param metricName the name of the metric for which there is new information available.
 * @param metricString the information to write.
 * @param timestamp whether to add a timestamp ("<milliseconds of simulation time>ms: ") to the line at the beginning.
 */
  void WriteToFile (const std::string metricName, const std::string metricString, bool timestamp) const;

  /**
   * \brief Retrieve the current wallclock time.
   *
   * @returns the current wallclock date and time in a printable format.
   */
  static std::string GetWallclockTime ();

// Event listeners
public:
  /**
   * \brief Announce that one application has finished its download.
   *
   * This method increases the internal counter of finished applications by 1 and subsequently checks whether the number of finished applications
   * has reached the threshold controlled by the SetStopFraction method. If so, it issues a call to the Simulator class to end the simulation.
   */
  void UpdateFinishedApps ();

  /**
   * \brief Announce that one external client has finished its download.
   *
   * This method is the analogous version of the UpdateFinishedApps method for external clients.
   * See the documentation for UpdateFinishedApps for further information.
   *
   * Note: This method cannot be used as an event listener but has to be called explicitly. If you use the default tracker model implementation
   * provided by the BitTorrentTracker class, this method is called automatically upon the reception of a "finished" announce.
   */
  void AnnounceFinishedExternalClient ();

  /**
   * \brief Update the health index metric after a client has been started.
   *
   * This method updates the availability of each piece in the surveilled swarm by taking into account the initial bitfield of a newly-started client.
   *
   * @param client the newly-started client of which the bitfield should be analyzed
   */
  void UpdateHealthIndexAppStart (Ptr<BitTorrentClient> client);

  /**
   * \brief Update the health index metric after a client has been stopped or has left the swarm.
   *
   * This method updates the availability of each piece in the surveilled swarm by taking into account the bitfield of the client leaving the swarm.
   *
   * @param client the client of which the bitfield should be analyzed
   */
  void UpdateHealthIndexAppStop (Ptr<BitTorrentClient> client);

  /**
   * \brief Update the health index metric after a piece has been downloaded by a client.
   *
   * Should be used as an event listener only.
   *
   * The availability of the given piece is increased by one.
   *
   * @param peer the peer from which the piece was successfully downloaded.
   * @param pieceIndex the index of the piece for which availability information should be updated.
   */
  void UpdateHealthIndexPieceCompleted (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief Update demand metric for one piece.
   *
   * Should be used as an event listener only.
   *
   * The demand metric of the given piece is increased by one.
   *
   * @param peer the peer from which the piece is requested. This is NOT the client requesting the piece.
   * @param pieceIndex the index of the piece for which demand information should be updated.
   */
  void UpdateDemandUp (Ptr<Peer> peer, uint32_t pieceIndex);

  /**
   * \brief Update demand metric for one piece.
   *
   * Should be used as an event listener only.
   *
   * The demand metric of the given piece is decreased by one.
   *
   * @param peer the peer from which the piece is requested. This is NOT the client requesting the piece.
   * @param pieceIndex the index of the piece for which demand information should be updated.
   */
  void UpdateDemandDown (Ptr<Peer> peer, uint32_t pieceIndex);

// Getter methods for certain global metrics
public:
  /**
   * \brief Retrieve the availability of all pieces.
   *
   * @returns a pointer to a map that contains the availability of each piece in the swarm.
   */
  const std::map<uint32_t, uint32_t>& GetPieceCount () const;

  /**
   * \brief Retrieve the current demand for all pieces.
   *
   * @returns a pointer to a map that contains the current demand (i.e., number of requests sent) for each piece in the swarm.
   */
  const std::map<uint32_t, uint32_t>& GetPieceDemand () const;
};

} // ns bittorrent
} // ns ns3

#endif /* GLOBALMETRICSGATHERER_H_ */
