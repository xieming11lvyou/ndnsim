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

#include "BitTorrentVideoClient.h"

#include "ns3/BitTorrentUtilities.h"

#include "ns3/abort.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"

#include <cmath>

namespace ns3 {
namespace bittorrent {
NS_LOG_COMPONENT_DEFINE ("bittorrent::BitTorrentVideoClient");
NS_OBJECT_ENSURE_REGISTERED (BitTorrentVideoClient);

BitTorrentVideoClient::BitTorrentVideoClient ()
{
  m_totalLength = MilliSeconds (0);
  m_milliSecondsPerPiece = 0;

  m_preBufferingTime = Seconds (10);
  m_autoPlay = false;
  m_autoPlayFromRight = false;

  m_piecesMissable = false;
  m_skipTolerance = Seconds (1);
  m_continousPlaybackAfterSkip = Seconds (15);

  m_playbackPosition = MilliSeconds (0);
  m_lastPlaybackChangeTime = Simulator::Now ();
  m_playing = false;
  m_paused = false;
  m_buffering = false;
  m_pausedUntil = MilliSeconds (0);
}

BitTorrentVideoClient::~BitTorrentVideoClient ()
{
}

TypeId BitTorrentVideoClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BitTorrentVideoClient")
    .SetParent<BitTorrentClient> ()
    .AddConstructor<BitTorrentVideoClient> ()
  ;
  return tid;
}

void BitTorrentVideoClient::StopApplication ()
{
  BitTorrentClient::StopApplication ();
}

void BitTorrentVideoClient::StartApplication ()
{
  // Step 1: Initialize the underyling BitTorrent client, also intializing the used protocol
  BitTorrentClient::StartApplication ();

  // TODO: Shift this into a new Torrent class derivate?
  std::string comment = GetTorrent ()->GetComment ();
  if (comment.size () > 0)
    {
      double lengthInMs = lexical_cast<double> (comment.substr (0, comment.find ("ms")));

      m_milliSecondsPerPiece = std::ceil (lengthInMs / GetTorrent ()->GetNumberOfPieces ());
      m_totalLength = MilliSeconds (m_milliSecondsPerPiece * GetTorrent ()->GetNumberOfPieces ());
      // std::cout << m_milliSecondsPerPiece << " ms / piece; " << GetTorrent()->GetNumberOfPieces() << " pieces" << std::endl;
    }
  else
    {
	  // TODO: distinguish between video and normal client for use of torrent comment field
      // NS_LOG_WARN ("Warning: BitTorrentVideoClient could not figure out length and fps of video to be downloaded.");
    }

  // Step 2: If auto-playback was configured, start playback
  if (m_autoPlay)
    {
      Play ();
    }
  if (m_autoPlayFromRight)
    {
      SetPlaybackPositionToTheRight (true);
    }
}

Time BitTorrentVideoClient::GetTotalLength () const
{
 return m_totalLength;
}

Time BitTorrentVideoClient::GetPlaybackPosition () const
{
  return m_playbackPosition;
}

Time BitTorrentVideoClient::GetPreBufferingTime () const
{
  return m_preBufferingTime;
}

void BitTorrentVideoClient::SetPreBufferingTime (Time preBufferingTime)
{
  if (preBufferingTime.IsPositive ())
    {
      m_preBufferingTime = preBufferingTime;
    }
}

bool BitTorrentVideoClient::GetAutoPlay () const
{
  return m_autoPlay;
}

void BitTorrentVideoClient::SetAutoPlay (bool autoPlay)
{
  m_autoPlay = autoPlay;
}

bool BitTorrentVideoClient::GetAutoPlayFromRight () const
{
 return m_autoPlayFromRight;
}

void BitTorrentVideoClient::SetAutoPlayFromRight (bool autoPlayFromRight)
{
 m_autoPlayFromRight = autoPlayFromRight;
}

bool BitTorrentVideoClient::GetPiecesMissable () const
{
  return m_piecesMissable;
}

void BitTorrentVideoClient::SetPiecesMissable (bool piecesMissable)
{
  m_piecesMissable = piecesMissable;
}

Time BitTorrentVideoClient::GetSkipTolerance () const
{
  return m_skipTolerance;
}

void BitTorrentVideoClient::SetSkipTolerance (Time skipTolerance)
{
  if (skipTolerance.IsPositive ())
    {
      m_skipTolerance = skipTolerance;
    }
}

Time BitTorrentVideoClient::GetRequiredContinousPlaybackAfterSkip () const
{
  return m_continousPlaybackAfterSkip;
}

void BitTorrentVideoClient::SetRequiredContinousPlaybackAfterSkip (Time continousPlaybackAfterSkip)
{
  if (continousPlaybackAfterSkip.IsPositive ())
    {
      m_continousPlaybackAfterSkip = continousPlaybackAfterSkip;
    }
}

uint32_t BitTorrentVideoClient::GetCurrentPiece () const
{
  return TimeToPiece (m_playbackPosition);
}

bool BitTorrentVideoClient::IsPlaying () const
{
  return m_playing;
}

bool  BitTorrentVideoClient::IsPaused () const
{
  return m_paused;
}

Time BitTorrentVideoClient::GetRemainingPause () const
{
  Time remainingPause = m_pausedUntil - Simulator::Now ();

  if (remainingPause.IsStrictlyPositive ())
    {
      return remainingPause;
    }
  else
    {
      return MilliSeconds (0);
    }
}

uint32_t BitTorrentVideoClient::GetContinousPiecesFromPiece (uint32_t piece) const
{
  uint32_t result = 0;

  if (piece >= GetTorrent ()->GetNumberOfPieces ())
    {
      return 0;
    }

  uint32_t currentBitfieldIndex = piece / 8;

  // Step 1: Go through the probably only partially filled bitfield entry containing this piece
  uint8_t currentPiece = m_bitfield[currentBitfieldIndex];
  currentPiece >>= piece % 8;       // Shift index of the requested piece to the beginning of currentPiece for easier counting (big endian!)
  while (currentPiece & 0x01)      // While the most significant (big endian!) bit is set
    {
      ++result;
      currentPiece >>= 1;           // Shift the 2nd most significant bit to the front, filling with a 0 (big endian!)
    }
  if (result != (8 - piece % 8))
    {
      return result;
    }

  // Step 2: Go through the full bitfield entries in larger steps
  ++currentBitfieldIndex;
  if (currentBitfieldIndex < GetTorrent ()->GetBitfieldSize () - 1)
    {
      for (; currentBitfieldIndex < GetTorrent ()->GetBitfieldSize () - 1 && m_bitfield[currentBitfieldIndex] == 0xFF; )
        {
          result += 8;
          ++currentBitfieldIndex;
        }
    }

  // Step 3: Go through the probably only paritally filled last continously-reachable bitfield entry
  currentPiece = m_bitfield[currentBitfieldIndex];
  while (currentPiece & 0x01)
    {
      ++result;
      currentPiece >>= 1;
    }

  return result;
}

uint32_t BitTorrentVideoClient::GetContinousMissingPiecesFromPiece (uint32_t piece) const
{
  // NOTE: This is roughly the same function as GetContinousPiecesFromPiece; it mainly inverts the bitfield entries for calculation!
  uint32_t result = 0;

  if (piece >= GetTorrent ()->GetNumberOfPieces ())
    {
      return 0;
    }

  uint32_t currentBitfieldIndex = piece / 8;

  // Step 1: Go through the probably only partially filled bitfield entry containing this piece
  uint8_t currentPiece = ~m_bitfield[currentBitfieldIndex];
  currentPiece >>= piece % 8;       // Shift index of the requested piece to the beginning of currentPiece for easier counting (big endian!)
  while (currentPiece & 0x01)      // While the most significant (big endian!) bit is set
    {
      ++result;
      currentPiece >>= 1;           // Shift the 2nd most significant bit to the front, filling with a 0 (big endian!)
    }
  if (result != (8 - piece % 8))
    {
      return result;
    }

  // Step 2: Go through the full bitfield entries in larger steps
  ++currentBitfieldIndex;
  if (currentBitfieldIndex < GetTorrent ()->GetBitfieldSize () - 1)
    {
      //                                            We've got to check for the inverted value here as well --v
      for (; currentBitfieldIndex < GetTorrent ()->GetBitfieldSize () - 1 && m_bitfield[currentBitfieldIndex] == 0x00; )
        {
          result += 8;
          ++currentBitfieldIndex;
        }
    }

  // Step 3: Go through the probably only paritally filled last continously-reachable bitfield entry
  currentPiece = ~m_bitfield[currentBitfieldIndex];
  while (currentPiece & 0x01)
    {
      ++result;
      currentPiece >>= 1;
    }

  return result;
}

Time BitTorrentVideoClient::GetTimeUntilPiece (uint32_t piece) const
{
  if (PieceToTime (piece) >= m_playbackPosition)
    {
      return PieceToTime (piece) - m_playbackPosition + GetRemainingPause ();
    }
  else
    {
      return MilliSeconds (0);
    }
}

void BitTorrentVideoClient::Play ()
{
  NS_LOG_INFO ("Playing.");

  // Step 1: Reset the playback-related settings
  m_lastPlaybackChangeTime = Simulator::Now ();
  m_playing = true;
  m_paused = false;
  m_buffering = false;

  // Step 2: Reset the playback position
  SetPlaybackPosition (MilliSeconds (0));

  // Step 3: Inform other components that playback state has changed
  PlaybackStateChangedEvent ();

  /*
   * Step 4:
   * In case the user has set a pre-buffering phase, we have to adjust the pre-buffering time accordingly
   * and make sure that other events (probably from a previous play period) are probably unscheduled beforehand
   * (Play is meant as a reset to the system)
   */
  Simulator::Cancel (m_nextBufferCheckEvent);
  Simulator::Cancel (m_nextBufferForElapsedEvent);

  Time preBufferingTimeLeft = m_preBufferingTime - PieceToTime (GetContinousPiecesFromPiece (0));

  if (preBufferingTimeLeft.IsStrictlyPositive ())
    {
      NS_LOG_INFO ("Need to buffer " << preBufferingTimeLeft.GetSeconds () << " additional seconds before playback.");
      BufferFor (preBufferingTimeLeft);
    }

  // Step 5: Start the playback again
  Simulator::Cancel (m_nextAdvancePlaybackEvent);
  AdvancePlayback ();
}

void BitTorrentVideoClient::Pause ()
{
  if (m_playing && !m_buffering)      // Users shall not interfere with the buffering of the application - at least not by unpausing it.
    {
      NS_LOG_INFO ("Paused.");

      m_lastPlaybackChangeTime = Simulator::Now ();
      m_paused = true;
      m_pausedUntil = Simulator::Now () + m_totalLength;

      Simulator::Cancel (m_nextAdvancePlaybackEvent);
      PlaybackStateChangedEvent ();
    }
}

void BitTorrentVideoClient::UnPause ()
{
  if (m_playing && !m_buffering)      // Users shall not interfere with the buffering of the application - at least not by unpausing it.
    {
      NS_LOG_INFO ("Playing again.");

      m_lastPlaybackChangeTime = Simulator::Now ();
      m_paused = false;
      m_pausedUntil = MilliSeconds (0);
      PlaybackStateChangedEvent ();

      Simulator::Cancel (m_nextAdvancePlaybackEvent);
      AdvancePlayback ();
    }
}

void BitTorrentVideoClient::Stop ()
{
  NS_LOG_INFO ("Playback stop requested.");

  if (m_playing)
    {
      NS_LOG_INFO ("Stopped playing the video.");

      m_lastPlaybackChangeTime = Simulator::Now ();
      m_playing = false;
      m_paused = false;

      SetPlaybackPosition (MilliSeconds (0));
      PlaybackStateChangedEvent ();
    }

  Simulator::Cancel (m_nextAdvancePlaybackEvent);
  Simulator::Cancel (m_nextBufferCheckEvent);
  Simulator::Cancel (m_nextBufferForElapsedEvent);
}

void BitTorrentVideoClient::SetPlaybackPosition (Time position)
{
  if (position > m_totalLength)
    {
      Stop ();
      PlaybackFinishedEvent ();
    }
  else if (m_playing)
    {
      m_lastPlaybackChangeTime = Simulator::Now ();

      if (position.IsStrictlyNegative ())
        {
          position = MilliSeconds (0);
        }

      m_playbackPosition = position;
      PlaybackPositionChangedEvent ();
    }
}

void BitTorrentVideoClient::SetPlaybackPositionRelative (Time change)
{
  if (m_playbackPosition + change > m_totalLength)      // If we would go beyond the "end time" of the video, we stop playback
    {
      Stop ();
      PlaybackFinishedEvent ();
    }
  else if (m_playing)
    {
      m_lastPlaybackChangeTime = Simulator::Now ();

      if (Time (m_playbackPosition + change).IsStrictlyNegative ())        // If we would go past the beginning of the video...
        {
          change = MilliSeconds (-m_playbackPosition.GetMilliSeconds ());             // ... we go right to the beginning only

        }
      m_playbackPosition = PieceToTime (TimeToPiece (Time (m_playbackPosition + change)));
      PlaybackPositionChangedEvent ();
    }
}

void BitTorrentVideoClient::SetPlaybackPositionToTheRight (bool keepPreBufferingPhase)
{
  SetPlaybackPosition (PieceToTime (GetContinousPiecesFromPiece (0)));

  if (keepPreBufferingPhase)
    {
      SetPlaybackPositionRelative (MilliSeconds (-1 * m_preBufferingTime.GetMilliSeconds ()));
    }
}

void BitTorrentVideoClient::BufferFor (Time bufferingPeriod)
{
  if (!m_playing)
    {
      NS_LOG_WARN ("BufferFor called while the client was NOT actually playing back. Skipped command.");
      return;
    }

  // Step 1: If playback is not paused, pause it
  if (!m_paused)
    {
      Pause ();
      m_buffering = true;
    }

  // Step 2: Cancel previously scheduled events that might disrupt our buffering phase
  Simulator::Cancel (m_nextBufferCheckEvent);
  Simulator::Cancel (m_nextBufferForElapsedEvent);

  // Step 3: Schedule the unpausing of our video once the buffering period has ended
  m_nextBufferForElapsedEvent = Simulator::Schedule (bufferingPeriod, &BitTorrentVideoClient::UnPauseAfterBuffering, this);
  m_pausedUntil = Simulator::Now () + bufferingPeriod;
}

void BitTorrentVideoClient::BufferNext (Time period)
{
  BufferUntil (m_playbackPosition + period);
}

void BitTorrentVideoClient::BufferUntil (Time position)
{
  if (position > m_totalLength)
    {
      position = m_totalLength;
    }

  if (!m_playing)
    {
      NS_LOG_WARN ("BufferUntil called while the client was NOT actually playing back. Skipped command.");
      return;
    }

  if (position < m_playbackPosition)
    {
      NS_LOG_WARN ("BufferUntil called with already elapsed playback position as argument. Skipped command.");
      return;
    }

  // Step 1: Check whether we actually have to buffer (i.e., whether there are some pieces missing)
  if (GetContinousPiecesFromPiece (TimeToPiece (m_playbackPosition)) < TimeToPiece (position) - TimeToPiece (m_playbackPosition) + 1)
    {
      NS_LOG_INFO ("Buffering until continous playback up to piece " << TimeToPiece (position) << " is possible.");

      // Step 1a: If playback is not paused, pause it
      if (!m_paused)
        {
          Pause ();
          m_buffering = true;
        }

      // Step 1b: Cancel previously scheduled events that might disrupt our buffering phase
      Simulator::Cancel (m_nextBufferCheckEvent);
      Simulator::Cancel (m_nextBufferForElapsedEvent);

      // Step 1c: Schedule the regular check for the appearance of the needed pieces
      m_nextBufferCheckEvent = Simulator::Schedule (MilliSeconds (m_milliSecondsPerPiece), &BitTorrentVideoClient::BufferUntil, this, position);
      m_pausedUntil = Simulator::Now () + m_totalLength;
    }
  else
    {
      if (!m_playing)
        {
          Play ();
        }
      else if (m_paused)          // When playback is paused but enough pieces are available, resume playback
        {
          UnPauseAfterBuffering ();
        }
    }
}

void BitTorrentVideoClient::BufferRange (uint32_t rangeStart, uint32_t rangeEnd)
{
  if (rangeStart > GetTorrent ()->GetNumberOfPieces ())
    {
      rangeStart = GetTorrent ()->GetNumberOfPieces ();
    }

  if (!m_playing)
    {
      NS_LOG_WARN ("BufferRange called while the client was NOT actually playing back. Skipped command.");
      return;
    }

  // Step 1: Check whether the requested range actually contains gaps
  if (GetContinousPiecesFromPiece (rangeStart) < rangeEnd - rangeStart)
    {
      // Step 1a: If playback is not paused, pause it
      if (!m_paused)
        {
          Pause ();
          m_buffering = true;
        }

      // Step 1b: Cancel previously scheduled events that might disrupt our buffering phase
      Simulator::Cancel (m_nextBufferCheckEvent);
      Simulator::Cancel (m_nextBufferForElapsedEvent);

      // Step 1c: Schedule the regular check for the appearance of the needed pieces
      m_nextBufferCheckEvent = Simulator::Schedule (MilliSeconds (m_milliSecondsPerPiece), &BitTorrentVideoClient::BufferRange, this, rangeStart, rangeEnd);
      m_pausedUntil = Simulator::Now () + m_totalLength;
    }
  else
    {
      if (!m_playing)
        {
          Play ();
        }
      else if (m_paused)          // When playback is paused but enough pieces are available, resume playback
        {
          UnPauseAfterBuffering ();
        }
    }
}

Time BitTorrentVideoClient::PieceToTime (uint32_t piece) const
{
  return MilliSeconds (static_cast<uint64_t> (ceil (piece * m_milliSecondsPerPiece)));
}

uint32_t BitTorrentVideoClient::TimeToPiece (Time time) const
{
  return floor (time.GetMilliSeconds () / m_milliSecondsPerPiece);
}

void BitTorrentVideoClient::PlaybackStateChangedEvent ()
{
  std::list<Callback<void> >::iterator iter = m_playbackStateChangedEventListeners.begin ();
  for (; iter != m_playbackStateChangedEventListeners.end (); ++iter)
    {
      (*iter)();
    }
}

void BitTorrentVideoClient::PlaybackPositionChangedEvent ()
{
  std::list<Callback<void, Time> >::iterator iter = m_playbackPositionChangedEventListeners.begin ();
  for (; iter != m_playbackPositionChangedEventListeners.end (); ++iter)
    {
      (*iter)(m_playbackPosition);
    }
}

void BitTorrentVideoClient::PlaybackPositionWillChangePeriodicallyEvent ()
{
  std::list<Callback<void> >::iterator iter = m_playbackStateWillChangePeriodicallyEventListeners.begin ();
  for (; iter != m_playbackStateWillChangePeriodicallyEventListeners.end (); ++iter)
    {
      (*iter)();
    }
}

void BitTorrentVideoClient::CannotAdvancePlaybackEvent ()
{
  std::list<Callback<void> >::iterator iter = m_CannotAdvancePlaybackEventListeners.begin ();
  for (; iter != m_CannotAdvancePlaybackEventListeners.end (); ++iter)
    {
      (*iter)();
    }
}

void BitTorrentVideoClient::SkippedPiecesInPlaybackEvent (uint32_t oldPiece, uint32_t newPiece)
{
  std::list<Callback<void, uint32_t, uint32_t> >::iterator iter = m_skippedPiecesInPlaybackEventListeners.begin ();
  for (; iter != m_skippedPiecesInPlaybackEventListeners.end (); ++iter)
    {
      (*iter)(oldPiece, newPiece);
    }
}

void BitTorrentVideoClient::PlaybackFinishedEvent ()
{
  std::list<Callback<void> >::iterator iter = m_playbackFinishedEventListeners.begin ();
  for (; iter != m_playbackFinishedEventListeners.end (); ++iter)
    {
      (*iter)();
    }
}

void BitTorrentVideoClient::RegisterCallbackPlaybackStateChangedEvent (Callback<void> eventCallback)
{
  m_playbackStateChangedEventListeners.push_back (eventCallback);
}

void BitTorrentVideoClient::UnregisterCallbackPlaybackStateChangedEvent (Callback<void> eventCallback)
{
  std::list<Callback<void> >::iterator iter = m_playbackStateChangedEventListeners.begin ();
  for (; iter != m_playbackStateChangedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_playbackStateChangedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentVideoClient::RegisterCallbackPlaybackPositionChangedEvent (Callback<void, Time> eventCallback)
{
  m_playbackPositionChangedEventListeners.push_back (eventCallback);
}

void BitTorrentVideoClient::UnregisterCallbackPlaybackPositionChangedEvent (Callback<void, Time> eventCallback)
{
  std::list<Callback<void, Time> >::iterator iter = m_playbackPositionChangedEventListeners.begin ();
  for (; iter != m_playbackPositionChangedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_playbackPositionChangedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentVideoClient::RegisterCallbackPlaybackPositionWillChangePeriodicallyEvent (Callback<void> eventCallback)
{
  m_playbackStateWillChangePeriodicallyEventListeners.push_back (eventCallback);
}

void BitTorrentVideoClient::UnregisterCallbackPlaybackPositionWillChangePeriodicallyEvent (Callback<void> eventCallback)
{
  std::list<Callback<void> >::iterator iter = m_playbackStateWillChangePeriodicallyEventListeners.begin ();
  for (; iter != m_playbackStateWillChangePeriodicallyEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_playbackStateWillChangePeriodicallyEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentVideoClient::RegisterCallbackCannotAdvancePlaybackEvent (Callback<void> eventCallback)
{
  m_CannotAdvancePlaybackEventListeners.push_back (eventCallback);
}

void BitTorrentVideoClient::UnregisterCallbackCannotAdvancePlaybackEvent (Callback<void> eventCallback)
{
  std::list<Callback<void> >::iterator iter = m_CannotAdvancePlaybackEventListeners.begin ();
  for (; iter != m_CannotAdvancePlaybackEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_CannotAdvancePlaybackEventListeners.erase (iter);
          break;
        }
    }
}



void BitTorrentVideoClient::RegisterCallbackSkippedPiecesInPlaybackEvent (Callback<void, uint32_t, uint32_t > eventCallback)
{
  m_skippedPiecesInPlaybackEventListeners.push_back (eventCallback);
}

void BitTorrentVideoClient::UnregisterCallbackSkippedPiecesInPlaybackEvent (Callback<void, uint32_t, uint32_t > eventCallback)
{
  std::list<Callback<void, uint32_t, uint32_t> >::iterator iter = m_skippedPiecesInPlaybackEventListeners.begin ();
  for (; iter != m_skippedPiecesInPlaybackEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_skippedPiecesInPlaybackEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentVideoClient::RegisterCallbackPlaybackFinishedEvent (Callback<void> eventCallback)
{
  m_playbackFinishedEventListeners.push_back (eventCallback);
}

void BitTorrentVideoClient::UnregisterCallbackPlaybackFinishedEvent (Callback<void> eventCallback)
{
  std::list<Callback<void> >::iterator iter = m_playbackFinishedEventListeners.begin ();
  for (; iter != m_playbackFinishedEventListeners.end (); ++iter)
    {
      if (iter->IsEqual (eventCallback))
        {
          m_playbackFinishedEventListeners.erase (iter);
          break;
        }
    }
}

void BitTorrentVideoClient::UnPauseAfterBuffering ()
{
  if (m_playing)
    {
      NS_LOG_INFO ("Playing again after buffering.");

      m_lastPlaybackChangeTime = Simulator::Now ();
      m_paused = false;
      m_buffering = false;
      m_pausedUntil = MilliSeconds (0);
      PlaybackStateChangedEvent ();

      Simulator::Cancel (m_nextAdvancePlaybackEvent);
      AdvancePlayback ();
    }
}

void BitTorrentVideoClient::AdvancePlayback ()
{
  // Playback can only be advanced if the client is playing and not paused (like in DVDs: Playing ~ picture visible, Paused ~ Freezed picture)
  if (m_playing && !m_paused)
    {
      // Step 1: Get the current and needed pieces for playback
      bool cantAdvancePlayback = false;
      uint32_t currentPiece = TimeToPiece (m_playbackPosition);
      uint32_t neededPiece = TimeToPiece (m_playbackPosition + Simulator::Now () - m_lastPlaybackChangeTime);

      // Step 2: If we need another piece, try to advance playback to that position
      if (neededPiece > currentPiece)
        {
          // Step 2a: Advancing over the end of the video file means finishing playback
          if (neededPiece >= GetTorrent ()->GetNumberOfPieces ())
            {
              NS_LOG_INFO ("Finished playback.");

              Stop ();
              PlaybackFinishedEvent ();
              return;
            }

          // Step 2b: Calculate the number of continously available pieces from the current position and compare this value to the number of needed pieces
          uint32_t availablePieces = GetContinousPiecesFromPiece (currentPiece + 1);

          if (availablePieces >= neededPiece - currentPiece)
            {
              NS_LOG_INFO ("Advancing playback from position " << currentPiece << " to piece " << neededPiece << ".");

              PlaybackPositionWillChangePeriodicallyEvent ();
              SetPlaybackPosition (PieceToTime (neededPiece));
            }
          else
            {
              cantAdvancePlayback = true;
            }
        }
      else if (GetContinousPiecesFromPiece (currentPiece) == 0)
        {
          cantAdvancePlayback = true;
        }

      // Step 3: If we cannot advance playback, we may have to pause, buffer, or do something else...
      if (cantAdvancePlayback)
        {
          // Step 3a: We may be able to skip a section of the video during playback...
          if (m_piecesMissable)
            {
              // Step 3a1: Get the number of pieces we may have to skip and compare them to the maximum duration of the part we may skip
              uint32_t piecesToSkip = GetContinousMissingPiecesFromPiece (currentPiece + 1);

              if (PieceToTime (piecesToSkip) <= m_skipTolerance)
                {
                  // Step 3a2: Since we may be able to skip, we also have to check the length of the part of the video available after the skip
                  uint32_t piecesAfterwards = GetContinousPiecesFromPiece (currentPiece + piecesToSkip + 1);

                  if (PieceToTime (piecesAfterwards) >= m_continousPlaybackAfterSkip)
                    {
                      NS_LOG_INFO ("Skipping playback from piece " << currentPiece << " to piece " << currentPiece + piecesToSkip << ":");

                      SetPlaybackPosition (PieceToTime (currentPiece + piecesToSkip + 1));
                      SkippedPiecesInPlaybackEvent (currentPiece, currentPiece + piecesToSkip + 1);

                      //                                                              v-- This is the resolution that we typically have
                      m_nextAdvancePlaybackEvent = Simulator::Schedule (MilliSeconds (m_milliSecondsPerPiece), &BitTorrentVideoClient::AdvancePlayback, this);
                    }
                  // Step 3a3: We cannot skip because of the "afterward condition" and hence have to buffer until at least enough pieces are available after(!) the skip
                  else
                    {
                      NS_LOG_INFO ("Cannot skip because continous playback after the skip would have been too short. Buffering until skip is possible.");

                      BufferRange (currentPiece + piecesToSkip + 1, currentPiece + piecesToSkip + 1 + TimeToPiece (m_continousPlaybackAfterSkip));
                      CannotAdvancePlaybackEvent ();
                    }
                }
              // Step 3a4: We cannot skip and have to buffer
              else
                {
                  Time preBufferingTimeLeft = m_preBufferingTime - PieceToTime (GetContinousPiecesFromPiece (currentPiece));

                  if (preBufferingTimeLeft.IsStrictlyPositive ())
                    {
                      NS_LOG_INFO ("Cannot advance playback. Buffering for " << preBufferingTimeLeft.GetMilliSeconds () << " ms.");

                      BufferFor (preBufferingTimeLeft);
                      CannotAdvancePlaybackEvent ();
                    }
                }
            }
          // Step 3b: We must not skip any pieces - and hence have to buffer until we have received the needed pice
          else               //if(piecesMissable)
            {
              NS_LOG_INFO ("Cannot advance playback. Buffering until piece " << neededPiece << " is available.");

              BufferUntil (PieceToTime (neededPiece));
              CannotAdvancePlaybackEvent ();
            }
          // Step 3c: If we could simply continue our playback, we schedule the next periodic playback advancement check
        }         // if(cantAdvancePlayback)
      else
        {
          //                                                              v-- This is the maximum resolution that typically have
          m_nextAdvancePlaybackEvent = Simulator::Schedule (MilliSeconds (m_milliSecondsPerPiece), &BitTorrentVideoClient::AdvancePlayback, this);
        }
    }
  else
    {
      NS_LOG_INFO ("Playback is paused.");
    }
}

} // namespace bittorrent
} //namespace ns3
