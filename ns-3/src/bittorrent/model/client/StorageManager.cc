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
 * Authors: Martin Lang (principal author), Rene Glebke
 */

#include "StorageManager.h"

#include "ns3/abort.h"
#include "ns3/log.h"

#include <cstring> // for memset, memcpy
#include <fstream>
#include <sys/stat.h>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::StorageManager");

StorageManager::StorageManager ()
{
  m_useFakeData = false;
  m_fakeDataCounter = 0;
}

StorageManager::~StorageManager ()
{
  std::map<std::string,FileInfo>::iterator iter = m_fileMap.begin ();

  for (; iter != m_fileMap.end (); ++iter)
    {
      delete [] iter->second.m_dataBuffer;
    }
}

bool StorageManager::GetUseFakeData() const
{
  return m_useFakeData;
}

void StorageManager::SetUseFakeData (bool useFakeData)
{
  if (m_useFakeData && useFakeData)
    {
      return;
    }
  else if (useFakeData)
    {
      m_useFakeData = true;

      std::map<std::string,FileInfo>::iterator iter = m_fileMap.begin ();
      for (; iter != m_fileMap.end (); ++iter)
        {
          // We do not delete the buffer here to allow direct access without having to deal with intermediate fake data situations
          // delete [] iter->second.m_dataBuffer;
        }
    }
  else if (!useFakeData)
    {
      m_useFakeData = false;

      std::map<std::string,FileInfo>::iterator iter = m_fileMap.begin ();
      for (; iter != m_fileMap.end (); ++iter)
        {
          EnsureFileLoaded (iter->first);
        }
    }
}

uint64_t StorageManager::GetFakeDataCounter() const
{
  return m_fakeDataCounter;
}

void StorageManager::EnsureFileLoaded (const std::string &path)
{
  std::map<std::string,FileInfo>::iterator iter = m_fileMap.find (path);

  if (iter == m_fileMap.end ())
    {
      // this file has to be loaded
      // get information on its size

      struct stat fileStat;
      stat (path.c_str (),&fileStat);
      uint32_t fileSize = static_cast<uint32_t> (fileStat.st_size);

      std::ifstream theFile (path.c_str (), std::ios_base::binary | std::ios_base::in);

      if (!theFile.is_open () || theFile.bad ())
        {
          NS_ABORT_MSG ("StorageManager: Could not open file with path \"" << path << "\".");
        }

      uint8_t *fileBuffer = new (std::nothrow) uint8_t[fileSize];
      if (fileBuffer == NULL)
        {
          NS_ABORT_MSG ("StorageManager: Could not allocate " << fileSize << " bytes for file \"" << path << "\".");
        }

      theFile.read (reinterpret_cast<char*> (fileBuffer),fileSize);
      if (theFile.fail () || theFile.eof ())
        {
          NS_ABORT_MSG ("StorageManager: Could not read " << fileSize << " bytes from file \"" << path << "\".");
        }
      
      FileInfo fileInfoStruct;
      fileInfoStruct.m_fileSize = fileSize;
      fileInfoStruct.m_dataBuffer = fileBuffer;

      m_fileMap[path] = fileInfoStruct;
    }
}

void StorageManager::CopyFileIntoBuffer (const std::string &path,uint64_t offset, uint64_t length, uint8_t *buffer)
{
  // If we use fake data, we simply fill the buffer with FF's and adjust the first 8 bytes to the value counting the number of calls to this function
  if (m_useFakeData)
    {
      std::memset (buffer, 0xFF, length);

      reinterpret_cast<uint64_t*> (buffer)[0] = ++m_fakeDataCounter;
    }
  else       // We return the actual data
    {
      std::map<std::string,FileInfo>::const_iterator iter = m_fileMap.find (path);

      if (iter == m_fileMap.end ())
        {
          NS_LOG_ERROR ("StorageManager: File \"" << path << "\" was not found amongst the loaded files.");
          return;
        }

      const uint8_t *filePtr = iter->second.m_dataBuffer;
      if (offset + length > iter->second.m_fileSize)
        {
          NS_LOG_ERROR ("StorageManager: CopyFileIntoBuffer: Requested data out of bounds. Offset: " << offset << "; length: " << length << "; filesize : " << iter->second.m_fileSize << ".");
          return;
        }

      std::memcpy (buffer,filePtr + offset,length);
    }
}

const uint8_t* StorageManager::GetBufferForFile(const std::string &path) const
{
  std::map<std::string,FileInfo>::const_iterator iter = m_fileMap.find (path);

  if (iter == m_fileMap.end ())
    {
      NS_LOG_ERROR ("StorageManager: File \"" << path << "\" was not found amongst the loaded files.");
      return 0;
    }
  else
    {
      return iter->second.m_dataBuffer;
    }
}


uint64_t StorageManager::GetBufferSizeForFile(const std::string &path) const
{
  std::map<std::string,FileInfo>::const_iterator iter = m_fileMap.find (path);

  if (iter == m_fileMap.end ())
    {
      NS_LOG_ERROR ("StorageManager: File \"" << path << "\" was not found amongst the loaded files.");
      return 0;
    }
  else
    {
      return iter->second.m_fileSize;
    }
}

} // ns bittorrent
} // ns ns3
