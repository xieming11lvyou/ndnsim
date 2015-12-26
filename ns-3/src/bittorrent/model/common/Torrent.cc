/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2013 ComSys, RWTH Aachen University
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

#include "Torrent.h"

#include "TorrentFile.h"
#include "3rd-party/sha1.h"

#include "ns3/log.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <ios>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::Torrent");

Torrent::Torrent ()
{

}

Torrent::~Torrent ()
{
}

TypeId Torrent::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::bittorrent::Torrent").SetParent<Object> ();

  return tid;
}

bool Torrent::ReadTorrentFile (std::string path)
{
  // First, we read the torrent file
  std::ifstream torrentFile (path.c_str (),std::ios_base::in | std::ios_base::binary);

  if (!torrentFile.is_open ())
    {
      NS_LOG_ERROR ("Error: Could not open torrent file \"" << path << "\".");
      return false;
    }

  std::string wholeTorrentFile;
  torrentFile.seekg (0, std::ios::end);
  wholeTorrentFile.reserve (static_cast<uint32_t> (torrentFile.tellg ()));
  torrentFile.seekg (0, std::ios::beg);
  wholeTorrentFile.assign ((std::istreambuf_iterator<char> (torrentFile)), std::istreambuf_iterator<char> ());

  size_t infoPos = wholeTorrentFile.find (":infod") + 5;
  size_t eeePos = wholeTorrentFile.find ("eee");
  
  // In case the torrent file finishes with "ee" instead of "eee"
  if (eeePos == std::string::npos)
    {
      eeePos = wholeTorrentFile.rfind ("ee");
      if (eeePos == std::string::npos)
        {
          NS_LOG_ERROR ("Error: Could not find the 'ee' sequence near the end of \"" << path << "\". Aborting.");
        }
      else
        eeePos += 1;
    }
  else
    {
      eeePos += 2;
    }

  std::string infoValue = wholeTorrentFile.substr (infoPos, eeePos - infoPos);

  // Next, we calculate the SHA1 hash over the content of the torrent file
  unsigned char newSHA[20];
  char SHAhexstring[41];
  sha1::calc(&infoValue[0], eeePos - infoPos, newSHA);
  sha1::toHexString(newSHA, SHAhexstring);

  std::stringstream ss1;
  ss1 << std::uppercase << SHAhexstring;
  std::string sha1Hash = ss1.str();
  std::transform(sha1Hash.begin(), sha1Hash.end(), sha1Hash.begin(), ::toupper);
  m_infoHash = sha1Hash;
  memcpy (m_byteValueInfoHash, newSHA, 20);

  // Output has byte value string
  std::memcpy (m_byteValueInfoHash, newSHA, 20);

  // Output as hex string
  std::stringstream ss;
  ss << SHAhexstring;
  m_infoHash = ss.str ();

  // Output as binary URL encoded hex
  std::stringstream encHashStream;
  for (uint32_t i = 0; i < 20; ++i)
    {
      if ((newSHA[i] <= 44) || (newSHA[i] == 47) || (newSHA[i] >= 58 && newSHA[i] <= 64) || (newSHA[i] >= 91 && newSHA[i] <= 96) || (newSHA[i] >= 123 && newSHA[i] != 126))
        {
          encHashStream
          << "%"
          << std::uppercase << std::right << std::setw (2) << std::setfill ('0') << std::hex
          << static_cast<uint16_t> (static_cast<uint8_t> (newSHA[i]));
        }
      else if ((newSHA[i] >= 97 && newSHA[i] <= 127) || (newSHA[i] >= 48 && newSHA[i] <= 57) || (newSHA[i] >= 65 && newSHA[i] <= 90))
        {
          std::string strConverter = "";
          strConverter += newSHA[i];
          encHashStream << strConverter;
        }
      else
        {
          encHashStream << newSHA[i];
        }
    }
  m_encodedInfoHash = encHashStream.str ();

  // Now, read and apply the content of the torrent file
  torrentFile.seekg (0, std::ios::beg); // return get pointer to start of inputstream
  Ptr<TorrentData> root = TorrentData::ReadBencodedData (torrentFile);
  Ptr<TorrentDataDictonary> rootDict = DynamicCast<TorrentDataDictonary> (root);

  NS_ASSERT (rootDict);

  rootDict->Dump ();

  // now copy the mandatory attributes in our data structure

  Ptr<TorrentDataString> annURL = DynamicCast<TorrentDataString> (rootDict->GetData ("announce"));
  NS_ASSERT (annURL);
  m_announceURL = annURL->GetData ();

  Ptr<TorrentDataDictonary> infoDict = DynamicCast<TorrentDataDictonary> (rootDict->GetData ("info"));
  NS_ASSERT (infoDict);

  Ptr<TorrentDataString> torrentName = DynamicCast<TorrentDataString> (infoDict->GetData ("name"));
  NS_ASSERT (torrentName);

  m_fileName = torrentName->GetData ();

  Ptr<TorrentDataInt> torrentfileSize = DynamicCast<TorrentDataInt> (infoDict->GetData ("length"));
  NS_ASSERT (torrentfileSize);

  m_fileLength = torrentfileSize->GetData ();

  Ptr<TorrentDataInt> torrentPieceSize = DynamicCast<TorrentDataInt> (infoDict->GetData ("piece length"));
  NS_ASSERT (torrentPieceSize);

  m_pieceLength = torrentPieceSize->GetData ();

  // calculate the number of pieces
  m_numberOfPieces = static_cast<uint32_t> (m_fileLength / m_pieceLength);

  // if it does not fit exactly we have a trailing piece
  m_trailingPieceLength = m_fileLength % m_pieceLength;
  if (m_trailingPieceLength > 0)
    {
      ++m_numberOfPieces;
    }

  // Set the bitfield size so we can easily travere the bitfield in applications without having to store this value for each app instance
  m_bitfieldSize = m_numberOfPieces / 8;
  if ((m_numberOfPieces % 8) > 0)
    {
      ++m_bitfieldSize;
    }

  // now read all the hashes and store them
  Ptr<TorrentDataString> hashes = DynamicCast<TorrentDataString> (infoDict->GetData ("pieces"));
  NS_ASSERT (hashes);

  NS_ASSERT (hashes->GetData ().size () == 20 * m_numberOfPieces);

  const char *data = hashes->GetData ().data ();

  m_pieces.reserve (m_numberOfPieces);

  for (unsigned int i = 0; i < m_numberOfPieces; ++i)
    {
      std::memcpy (m_pieces[i].sha_hash,data,20);
      data += 20;
    }

  // now we read the optional parameters
  Ptr<TorrentDataInt> cdate = DynamicCast<TorrentDataInt> (rootDict->GetData ("creation date"));
  if (cdate)
    {
      m_creationDate = static_cast<time_t> (cdate->GetData ());
    }

  Ptr<TorrentDataString> torrentComment = DynamicCast<TorrentDataString> (rootDict->GetData ("comment"));
  if (torrentComment)
    {
      m_comment = torrentComment->GetData ();
    }
  else
    {
      Ptr<TorrentDataString> torrentComment2 = DynamicCast<TorrentDataString> (rootDict->GetData ("comment.utf-8"));
      if (torrentComment2)
        {
          m_comment = torrentComment2->GetData ();
        }
    }

  Ptr<TorrentDataString> torrentEnconding = DynamicCast<TorrentDataString> (rootDict->GetData ("encoding"));
  if (torrentEnconding)
    {
      m_encoding = torrentEnconding->GetData ();
    }
  else
    {
      m_encoding = "utf8";           // this is standard
    }

  return true;
}

void Torrent::SetDataPath (std::string dataPath)
{
  m_dataPath = dataPath;
}

std::string Torrent::GetDataPath () const
{
  return m_dataPath;
}

std::string Torrent::GetAnnounceURL () const
{
  return m_announceURL;
}

void Torrent::SetAnnounceURL (std::string announceURL)
{
  m_announceURL = announceURL;
}

std::string Torrent::GetInfoHash () const
{
  return m_infoHash;
}
std::string Torrent::GetEncodedInfoHash () const
{
  return m_encodedInfoHash;
}
const char* Torrent::GetByteValueInfoHash () const
{
  return (const char*) &m_byteValueInfoHash[0];
}

time_t Torrent::GetCreationDate () const
{
  return m_creationDate;
}

std::string Torrent::GetComment () const
{
  return m_comment;
}

std::string Torrent::GetEncoding () const
{
  return m_encoding;
}

const char* Torrent::GetPieces () const
{
  return (const char*) &m_pieces[0].sha_hash;
}

uint8_t Torrent::IsPrivateTorrent () const
{
  return m_privateTorrent;
}

uint8_t Torrent::GetFileMode () const
{
  return m_fileMode;
}

std::string  Torrent::GetFileName () const
{
  return m_fileName;
}

uint64_t Torrent::GetFileLength () const
{
  return m_fileLength;
}

bool Torrent::HasTrailingPiece () const
{
  return m_trailingPieceLength > 0;
}

uint8_t Torrent::GetNumberOfFiles () const
{
  return m_numberOfiles;
}

uint32_t Torrent::GetBitfieldSize () const
{
  return m_bitfieldSize;
}

uint32_t Torrent::GetTrailingPieceLength () const
{
  return m_trailingPieceLength;
}

} // ns bittorrent
} // ns ns3
