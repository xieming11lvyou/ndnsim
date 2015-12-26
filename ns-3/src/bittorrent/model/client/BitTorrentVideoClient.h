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

#ifndef BITTORRENTVIDEOCLIENT_H_
#define BITTORRENTVIDEOCLIENT_H_

#include "BitTorrentClient.h"
#include "BitTorrentPeer.h"
#include "ns3/Torrent.h"

#include "ns3/log.h"

#include <list>
#include <map>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief A BitTorrent Video-on-Demand client application for ns-3.
 *
 * This class provides a Video-on-Demand simulation model based on the BitTorrent model provided by the BitTorrentClient class.
 * It uses meta-information stored in the comment section of the ".torrent" file (see the Torrent class for details) to obtain
 * information on the length of the video represented by the shared file and artificially plays that file by advancing a playback
 * position indicator. The class provides heuristics for data buffering and skipping of sections of unavailable data as well as
 * additional internal messages for Video-on-Demand enabled strategies.
 */
class BitTorrentVideoClient : public BitTorrentClient
{
// Fields
private:
  // Basic settings for the video client
  Time    m_totalLength;               // The total length of the video to play back
  double  m_milliSecondsPerPiece;      // How long a piece is in milli seconds

  // Auto-playback-related settings
  Time   m_preBufferingTime;           // The time that the client pre-buffers before first attempting to start playback
  bool   m_autoPlay;                   // Whether to automatically start pseudo-playback upon application startup (StartApplication())
  bool   m_autoPlayFromRight;          // Whether autoplay starts at the rightmost continously reachable position (starting from the left)

  // General playback-related settings
  bool  m_piecesMissable;              // Whether pieces may be missed (skipped) during playback. Otherwise, buffering periods are initiated
  Time  m_skipTolerance;               // How long a period of skipped video playback may be at most
  Time  m_continousPlaybackAfterSkip;  // How much continous playback time must be available in order to allow a skip

  // Internal playback variables
  Time   m_playbackPosition;           // The current position of pseudo-playback
  Time   m_lastPlaybackChangeTime;     // The last Simulator timestamp the psuedo-playback position was updated
  bool   m_playing;                    // Whether the client is currently playing. False => Stop downloading
  bool   m_paused;                     // Whether the client is currently paused (i.e., downloading but not playing)
  bool   m_buffering;                  // Whether the client is currently buffering
  Time   m_pausedUntil;                // If the current is currently buffering, this indicates how long the remaining time of buffering is

  // Events used to simulate playback; should not be touched if psuedo-playback is not intended to be changed
  EventId     m_nextAdvancePlaybackEvent;
  EventId     m_nextBufferCheckEvent;
  EventId     m_nextBufferForElapsedEvent;

private:
  // Listeners for playback-related events
  std::list<Callback<void> > m_playbackStateChangedEventListeners;
  std::list<Callback<void, Time> > m_playbackPositionChangedEventListeners;
  std::list<Callback<void> > m_playbackStateWillChangePeriodicallyEventListeners;
  std::list<Callback<void> > m_CannotAdvancePlaybackEventListeners;
  std::list<Callback<void, uint32_t, uint32_t > > m_skippedPiecesInPlaybackEventListeners;
  std::list<Callback<void> > m_playbackFinishedEventListeners;

// Constructors etc.
public:
  BitTorrentVideoClient ();
  virtual ~BitTorrentVideoClient ();
  static TypeId GetTypeId (void);
protected:
  virtual void StopApplication (void);
public:

  /**
   * \brief Initialize the application.
   *
   * A call to this method initializes the Video-on-Demand client. The initializer of the parent BitTorrentClient class is called and then
   * the meta-information from the Torrent is read out to initialize playback.
   *
   * The expected format for the comment section of the Torrent is "xyzms", where xyz reperesents the length of the video in milliseconds.
   *
   * If automatic playback is enabled, the application will try starting video playback. If it is not enabled, a call to the Play method
   * can be issued at any later time to start playback. If playback from the rightmost available position is enabled (see the SetAutoPlayFromRight
   * method), playback will start at that position.
   *
   * The application issues an ApplicationInitializedEvent to which all strategies may listen to after full initialization has taken place.
   *
   * Note: You must set the shared file the client shall download and the employed downloading strategies with the SetTorrent and SetProtocol
   * methods, respectively, before calling this method. Otherwise, initialization of the client will fail and the simulation may abort.
   */
  virtual void StartApplication (void);

// Getters, setters
public:
  // Basic settings for the video client

  /**
   * @returns the length of the shared video.
   */
  Time GetTotalLength () const;
  /**
   * @returns the current internal playback position.
   */
  Time GetPlaybackPosition () const;

  // Auto-playback-related settings

  /**
   * @returns the time span the video must be continously playable before playback starts.
   */
  Time GetPreBufferingTime () const;

  /**
   * \brief Set the time span the video must be continously playbable before playback starts or resumes.
   *
   * This setting affects automatic playback and playback that may be commenced after a buffering phase.
   *
   * @param preBufferingTime the desired pre
   * -buffering time.
   */
  void SetPreBufferingTime (Time preBufferingTime);

  /**
   * @returns true, if auto play is enabled.
   */
  bool GetAutoPlay () const;

  /**
   * \brief Enable or disable autoplay.
   *
   * If enabled, the client will automatically start playback upon initialization. Otherwise, playback must be started
   * using the Play method.
   *
   * @param autoPlay the desired auto play setting.
   */
  void SetAutoPlay (bool autoPlay);

  /**
   * @returns true, if auto play from the right is enabled.
   */
  bool GetAutoPlayFromRight () const;

  /**
   * \brief Enable or disable autoplay from the right.
   *
   * If enabled, the client will, when auto play is enabled, start its playback at the rightmost position that is continously
   * reachable from the beginning (= left) of the file, i.e., playback will start at the position within the bitfield which is
   * the rightmost contiously-reachable 1. This can be used to simulate clients which have already been playing back the video
   * for a time.
   *
   * Note that this setting is affected by the pre-buffering time (see the SetPreBufferingTime method), so that playback may start
   * a little "earlier" in the bitfield to allow for pre-buffering operations to be continued.
   *
   * @param autoPlayFromRight the desired auto play from right setting.
   */
  void  SetAutoPlayFromRight (bool autoPlayFromRight);

  // General playback-related settings

  /**
   * @returns true, if pieces may be missed during playback.
   */
  bool GetPiecesMissable () const;

  /**
   * \brief Allow or disallow missed pieces during playback.
   *
   * If enabled, the video player may skip playback over missing (i.e., not downloaded) pieces and continue at the next
   * possible playback position. If this setting is disabled, the player will engage a buffering phase until enough data
   * is available again for playback.
   *
   * This setting is affected by both the skip tolerance (see SetSkipTolerance) and the continuity of playback after the
   * skipping operation (see SetRequiredContinousPlaybackAfterSkip method).
   *
   * @param piecesMissable the desired setting (miss pieces / always buffer).
   */
  void SetPiecesMissable (bool piecesMissable);

  /**
   * @returns the time span skipped missing pieces may at most encompass.
   */
  Time GetSkipTolerance () const;

  /**
   * \brief Set the time span skipped missing pieces may at most encompass.
   *
   * If the skipping of pieces would yield to a playback position which is too far away from the current playback position,
   * a buffering period is started. Else, the pieces may be skipped (see the GetRequiredContinousPlaybackAfterSkip method for
   * a second condition that has to apply).
   *
   * @param skipTolerance the maximum video time that may be skipped. Ignored, if not positive.
   */
  void SetSkipTolerance (Time skipTolerance);

  /**
   * @returns the length of the video that has to be continously available after a skip.
   */
  Time GetRequiredContinousPlaybackAfterSkip () const;

  /**
   * \brief Set the length of the video that has to be continously available after a skip.
   *
   * A video section will only be skipped if after the skip, a number of pieces with a playback length of at least the time
   * span specified using this method are available (see the GetSkipTolerance method for a second condition that has to apply).
   *
   * @param continousPlaybackAfterSkip the length of the section that has to be available. Ignored, if not positive.
   */
  void SetRequiredContinousPlaybackAfterSkip (Time continousPlaybackAfterSkip);

  // Internal playback variables

  /**
   * @returns the current playback position converted into a piece in the b
   */
  uint32_t  GetCurrentPiece () const;

  /**
   * @returns true, if the client is currently playing the video. Use the IsPaused method to obtain further information.
   */
  bool IsPlaying () const;

  /**
   * @returns true, if video playback is currently paused. Playback is ongoing if the statement (IsPlaying () && !IsPaused ()) returns true.
   */
  bool IsPaused () const;

  /**
   * @returns the length of the remaining pause, if any. May be used to obtain the remaining time that a video is paused for buffering reasons.
   */
  Time GetRemainingPause () const;

  // Derived attributes

  /**
   * @returns the number of pieces available "to the right" in the bitfield starting from the piece given in the argument.
   *
   * @param piece the piece that counting available pieces shall start off from. Included in the result.
   */
  uint32_t GetContinousPiecesFromPiece (uint32_t piece) const;

  /**
   * @returns the number of missing pieces "to the right" in the bitfield starting from the piece given in the argument.
   *
   * @param piece the piece that counting unavailable pieces shall start off from. Included in the result.
   */
  uint32_t GetContinousMissingPiecesFromPiece (uint32_t piece) const;

  /**
   * @returns the expected simulation time that has to elapse until playback of this piece begins. This method assumes that all intermediate
   * pieces are present and regularly played, i.e., that there are neither buffering, pause or skipping phases.
   *
   * @param piece the piece the expected playback time shall be calculated for.
   */
  Time GetTimeUntilPiece (uint32_t piece) const;

// Interaction methods
public:

  /**
   * \brief Start playback at position 0 and re-initialize all playback-related events such as buffering phases.
   */
  void Play ();

  /**
   * \brief Pause playback until the UnPause method is called.
   */
  void Pause ();

  /**
   * \brief Unpause playback after it was paused.
   *
   * Equals pressing the "Pause" button on a VCR/Disc player a second time.
   */
  void UnPause ();

  /**
   * \brief Stop playback.
   *
   * Playback can subsequently be restarted only by a call to the Play method.
   */
  void Stop ();

  /**
   * \brief Set the playback position to the specified time within the video.
   *
   * The time is approximated using the beginning of the section encompassed by the piece that includes the given position.
   * Setting time to an off-limit value (negative or too high) will re-start or end playback, respectively.
   *
   * @param position the desired position.
   */
  void SetPlaybackPosition (Time position);

  /**
   * \brief Set the playback position relative tot he current position.
   *
   * You may specify both negative and positive changes. The time is approximated using the beginning of the piece that encompasses the new position.
   * If the resulting time is an off-limit value (negative or too high) will re-start or end playback, respectively.
   *
   * @param change the desired change.
   */
  void SetPlaybackPositionRelative (Time change);

  /**
   * \brief Advance playback position as far as continously possible from the beginning.
   *
   * @param keepPrebufferingPhase set to true to dedeuct the pre-buffering time from the right to allow pre-buffering to continue more smoothly.
   */
  void SetPlaybackPositionToTheRight (bool keepPreBufferingPhase);

  /**
   * \brief Enable a buffering period of a given length.
   *
   * Note that buffering only works if the client is already playing, i.e., a call to the Play method must have been issued before.
   *
   * @param period the length of the buffering period.
   */
  void BufferFor (Time bufferingPeriod);

  /**
   * \brief Enable a buffering period that lasts until data for playback of the given length is available.
   *
   * @param period the length of the time span that has to be continiusly available.
   */
  void BufferNext (Time period);

  /**
   * \brief Enable a buffering period that lasts until data for playback up to the given position is available.
   *
   * Note that buffering may not work if the specified position is before the current playback position.
   *
   * @param position the position within the video.
   */
  void BufferUntil (Time position);

  /**
   * \brief Buffer until the part of the video between start and end are fully available.
   *
   * @param rangeStart the lower bound of the part that has to be available.
   * @param rangeEnd the upper bound of the part that has to be available.
   */
  void BufferRange (uint32_t rangeStart, uint32_t rangeEnd);

// Publically-available helper methods
public:

  /**
   * @returns the first time stamp period represented by the given piece.
   */
  Time PieceToTime (uint32_t piece) const;

  /**
   * @returns the index of the piece that contains the given video playback time.
   */
  uint32_t TimeToPiece (Time time) const;

// Events (protected, as only triggered internally)
protected:

  /**
   * \brief This event is triggered when the playback state is changed, e.g., from playing to paused state.
   */
  void PlaybackStateChangedEvent ();

  /**
   * \brief This event informs interested strategies about a change in the playback position, either by automatic playback or user interaction.
   */
  void PlaybackPositionChangedEvent ();

  /**
   * \brief A normal change of the playback position.
   *
   * This event is triggered before then PlaybackPositionChangedEvent when the upcoming change (triggered immediately thereafter) is
   * due to a normal change by automatic playback.
   */
  void PlaybackPositionWillChangePeriodicallyEvent ();

  /**
   * \brief Playback problems.
   *
   * This event occurs when automatic playback is not able to advance the playback position, meaning that insufficient data is available.
   * Note that this method is only triggered if also skipping of pieces or buffering phases failed.
   */
  void CannotAdvancePlaybackEvent ();

  /**
   * \brief Informs interested strategies that during playback, a number of pieces was skipped.
   *
   * @param oldPiece the current playback position before the skip occurred.
   * @param newPiece the new playback position after the skip.
   */
  void SkippedPiecesInPlaybackEvent (uint32_t oldPiece, uint32_t newPiece);

  /**
   * \brief Finished playback.
   *
   * When the playback position indicator has reached the end of the video, this event is triggered.
   * Note that it is not necessary for playback to have started at position 0 for this event to be triggered; any starting position is sufficient.
   */
  void PlaybackFinishedEvent ();

// Event listeners
public:
  void RegisterCallbackPlaybackStateChangedEvent (Callback<void> eventCallback);
  void UnregisterCallbackPlaybackStateChangedEvent (Callback<void> eventCallback);
  void RegisterCallbackPlaybackPositionChangedEvent (Callback<void, Time> eventCallback);
  void UnregisterCallbackPlaybackPositionChangedEvent (Callback<void, Time> eventCallback);
  void RegisterCallbackPlaybackPositionWillChangePeriodicallyEvent (Callback<void> eventCallback);
  void UnregisterCallbackPlaybackPositionWillChangePeriodicallyEvent (Callback<void> eventCallback);
  void RegisterCallbackCannotAdvancePlaybackEvent (Callback<void> eventCallback);
  void UnregisterCallbackCannotAdvancePlaybackEvent (Callback<void> eventCallback);
  void RegisterCallbackSkippedPiecesInPlaybackEvent (Callback<void, uint32_t, uint32_t > eventCallback);
  void UnregisterCallbackSkippedPiecesInPlaybackEvent (Callback<void, uint32_t, uint32_t > eventCallback);
  void RegisterCallbackPlaybackFinishedEvent (Callback<void> eventCallback);
  void UnregisterCallbackPlaybackFinishedEvent (Callback<void> eventCallback);

// Internal methods
protected:
  /// @cond HIDDEN
  // A special version of the UnPause function called when a buffering phase ended
  void UnPauseAfterBuffering ();
  /// @endcond HIDDEN

  /**
   * \brief The main pseudo-play method.
   *
   * You may override this method to implement different playback and buffering heuristics. Called periodically, at least once "per piece"
   * (i.e., if each piece encompasses video data for a length of x ms, the method is triggered every x ms).
   */
  virtual void AdvancePlayback ();
};

} // ns bittorrent
} // ns ns3

#endif /* BITTORRENTVIDEOCLIENT_H_ */
