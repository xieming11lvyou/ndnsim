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
 * Authors: Martin Lang, Elias Weingaertner (principal & initial authors), Rene Glebke, Alexander Hocks
 */

#ifndef TORRENT_H_
#define TORRENT_H_

#include "BitTorrentDefines.h"

#include "ns3/object.h"

#include <vector>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Reads and stores information about a BitTorrent swarm read from a ".torrent" file.
 *
 * This class holds all information necessary for a client to participate in a BitTorrent swarm.
 * The information about a BitTorrent swarm is immutable and it is sufficient that only one
 * global object representing the information exists, although the co-existence of several objects
 * representing the same information is technically possible.
 *
 * See the <a href="http://wiki.theory.org/BitTorrentSpecification#Metainfo_File_Structure" target="_blank">BitTorrent Protocol Specification</a>
 * for details on the available information.
 */
class Torrent : public Object
{
// Internal definitions and types used
private:
  /// @cond HIDDEN
  static const uint8_t FILE_MODE_SINGLE = 1;
  static const uint8_t FILE_MODE_MULTI = 255;

  typedef struct
  {

    char sha_hash[20];

  } SHA_Hash_t;

  /// @endcond HIDDEN

// Fields
private:
  // Directly read from torrent
  std::string                m_announceURL;                // The URL of the tracker
  time_t                     m_creationDate;               // Creation date of torrent
  std::string                m_comment;                    // Some comment (optional, in VoD simulations: duration of the video file)
  std::string                m_encoding;                   // Character-encoding of the torrent meta-data
  uint64_t                   m_pieceLength;                // Length of pieces
  uint32_t                   m_numberOfPieces;             // Number of pieces this torrent contains
  uint8_t                    m_privateTorrent;             // Is this a private torrent?
  uint8_t                    m_fileMode;                   // Corresponds to const;ant; single-or multi-file mode
  std::string                m_fileName;                   // Filenames in the torrent
  uint64_t                   m_fileLength;                 // Total length of the files described by the torrent
  std::string                m_infoHash;                   // The info Hash
  std::string                m_encodedInfoHash;            // The URLencoded info Hash
  uint8_t                    m_byteValueInfoHash[20];      // Contains the infoHash as an array of 20 int-bytes
  std::vector<SHA_Hash_t>    m_pieces;                     // Char array of dynamic size holding the SHA1 hashes of the pieces
  uint8_t                    m_numberOfiles;               // Number of files in the torrent

  // Derived fields
  uint32_t                   m_bitfieldSize;               // Number of uint8_ts needed to hold the bitfield
  uint32_t                   m_trailingPieceLength;        // Number of bytes in the trailing piece(s)

  // System-specifc settings
  std::string                m_dataPath;                   // The path to the data associated with the Torrent relative to the exec path of ns3

// Constructors etc.
public:
  Torrent ();
  virtual ~Torrent ();
  static TypeId GetTypeId ();
private:
  Torrent (const Torrent&);
  Torrent& operator = (const Torrent);
public:
  // Reads a torrent file and fills the fields accordingly

  /**
   * \brief Read the contents of a ".torrent" file into the Torrent class.
   *
   * @param path the path (relative to the current execution directory) to the ".torrent" file to be loaded.
   *
   * @returns true, if the file could be successfully loaded.
   */
  bool ReadTorrentFile (std::string path);

// Getters, setters
public:
  // System-related

  /**
   * \brief Set the location of the data associated with the Torrent.
   *
   * @param dataPath the path to the data associated with this Torrent relative to the ns3 execution path.
   */
  void SetDataPath (std::string dataPath);

  /**
   * @returns the path to the data associated with this Torrent, relative to the ns3 execution path.
   */
  std::string GetDataPath () const;

  // Directly read from torrent

  std::string GetAnnounceURL () const;

  /**
   * \brief Override the announce URL of the torrent.
   *
   * This function can be used by a centralized, simulated tracker (such as the BitTorrentTracker application) to set the correct announce URL
   * of the torrent so that clients can find the tracker even if the IP is different from that saved in the ".torrent" file.
   *
   * @params announceURL the fully-qualified URL that can be used to reach the tracker's announce function via HTTP. Usually of the form "http://[ip]:80/announce".
   */
  void SetAnnounceURL (std::string announceURL);

  /**
   * @returns the info_hash of the torrent as string of a 40 upper case hexadecimal characters.
   */
  std::string GetInfoHash () const;

  /**
   * @returns the info_hash of the torrent as a string of URL encoded upper case hexadecimal characters.
   */
  std::string GetEncodedInfoHash () const;

  /**
   * @returns the info_hash of the torrent as a const pointer to an array of 20 8-bit values.
   */
  const char* GetByteValueInfoHash () const;

  /**
   * @returns the creation date of the ".torrent" file.
   */
  time_t GetCreationDate () const;

  /**
   * @returns the comment included within the ".torrent" file. This information may include further input, e.g., for the BitTorrentVideoClient class.
   */
  std::string GetComment () const;

  std::string GetEncoding () const;

  /**
   * @returns the length, in bytes, of a normal piece in this torrent. See the HasTrailingPiece method for details.
   */
  uint32_t GetPieceLength () const
  {
    return m_pieceLength;
  }

  /**
   * @returns the number of pieces the torrent consists of. See the HasTrailingPiece method for details.
   */
  uint32_t GetNumberOfPieces () const
  {
    return m_numberOfPieces;
  }

  /**
   * @returns a const pointer to the array of the SHA-1 hashes of the pieces in the torrent. The length of the array equals the result of the GetNumberOfPieces member function.
   */
  const char* GetPieces () const;

  uint8_t IsPrivateTorrent () const;

  uint8_t GetFileMode () const;

  /**
   * @returns the name of the file to create/read for this torrent in single-file mode, without path qualifiers.
   */
  std::string GetFileName () const;

  /**
   * @returns the length, in bytes, of the shared file(s).
   */
  uint64_t GetFileLength () const;

  /**
   * @returns true if the last piece of the file is not of full length. If so, the torrent consists of GetNumberOfPieces - 1 pieces of length GetPieceLength and 1 piece of GetTrailingPieceLength.
   */
  bool HasTrailingPiece () const;

  uint8_t GetNumberOfFiles () const;

  /**
   * @returns the length of the bitfield array
   */
  uint32_t GetBitfieldSize () const;

  /**
   * @returns the length (in bytes) of the trailing piece.
   */
  uint32_t GetTrailingPieceLength () const;
};

} // ns bittorrent
} // ns ns3
#endif /* TORRENT_H_ */
