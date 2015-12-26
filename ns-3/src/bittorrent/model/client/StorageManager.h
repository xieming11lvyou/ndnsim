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

#ifndef STORAGEMANAGER_H_
#define STORAGEMANAGER_H_

#include <map>
#include <string>
#include <inttypes.h>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Provides central data storage functionality for the BitTorrentClient and its related classes.
 *
 * This class provides centralized access to data shared over one or simulated BitTorrent swarms. Data is only held in memory once, and for each
 * operation which needs this data, it is copied into the desired location for local use. This avoids overly high memory usage by the model.
 */
class StorageManager
{
// Fields
private:
  struct FileInfo // Holds information about a shared file
  {
    uint8_t  *m_dataBuffer;  // Pointer to the beginning of the array into which the shared file is loaded
    uint64_t m_fileSize;     // The length of the shared file
  };

  // RENE: Probably avoid the string indexing to make access to file easier
  std::map<std::string, FileInfo> m_fileMap; // All loaded shared files

  // Fake data: If fake data is enabled, the actual file will not be loaded and deterministic contents for packets will be generated
  bool m_useFakeData;             // Whether to use fake data or not
  uint64_t m_fakeDataCounter;     // Fake data contents = [m_fakeDataCounter++ value]0xFF0xFF0xFF...0xFF

// Constructors etc. (singleton pattern)
private:
  StorageManager ();
  ~StorageManager ();
public:
  /**
   * @returns the single instance of the StorageManager class per ns3 simulation run.
   */
  static StorageManager * GetInstance ()
  {
    static StorageManager inst;
    return &inst;
  }

// Getters, setters
public:
  bool GetUseFakeData () const;

  /**
   * \brief Enable or disable global usage of fake data.
   *
   * When fake data is enabled, all subsequent calls to the CopyFileIntoBuffer() method will return special, faked data. See its documentation for further details.
   * The loaded files stay in memory to guarantee correct results when directly accessing the data via the GetBufferForFile() method.
   *
   * When fake data is disabled, CopyFileIntoBuffer() will result in the actual contents of the data being copied into the buffer.
   *
   * @param useFakeData whether or not to globally use fake data.
   */
  void SetUseFakeData (bool useFakeData);

  /**
   * \brief Retrieve the current setting of the auto-incremented value placed into packets when fake data is enabled.
   *
   * May be useful for debugging.
   *
   * Note that the value is auto-incremented in the CopyFileIntoBuffer() method BEFORE the data is created, so the result of this method yields
   * the last value placed into a buffer.
   *
   * It is safe to call this function during a running simulations and changes take effect immediately.
   *
   * Note that when fake data is used, checksum calculations for the received data will fail, so checksum calculation must be switched off on all
   * clients.
   *
   * @return the current value of the auto-incremented value placed into buffers in fake data mode.
   */
  uint64_t GetFakeDataCounter () const;

// Main methods
public:
  /**
   * \brief Read a file into the internal file buffer.
   *
   * @param path the path (relative to the current execution directory) to the file to be loaded.
   */
  void EnsureFileLoaded (const std::string &path);

  /**
   * \brief Copy a part of a shared file into a supplied buffer.
   *
   * When fake data is enabled, all data loaded from files is freed from memory and for each subsequent call, the CopyFileIntoBuffer() method will
   * not copy the real content of the file into the buffer, but instead write an auto-incremented 64-bit value at the beginning of the packet and
   * fill the rest of the packet with 0xFF. The value written is incremented after each call of CopyFileIntoBuffer().
   *
   * When fake data is disabled, all shared files are read from the disk into the main memory and calls to the CopyFileIntoBuffer() method will
   * result in the actual contents of the data being copied into the buffer.
   *
   * @param the path (relative to the current execution directory) to the file. Should equal an argument once passed to the EnsureFileLoaded method.
   * @param offset the offset (in bytes) at which the desired part is located within the file.
   * @param length the length of the part (in bytes) to be copied.
   * @param buffer the buffer into which the desired should be copied.
   *
   * Note that no check is performed on the buffer argument, so the receiving buffer must be at least length bytes long. Range checking is,
   * however, applied on the internal buffers and no data is returned in case of an error.
  */
  void CopyFileIntoBuffer (const std::string &path, uint64_t offset, uint64_t length, uint8_t *buffer);

  /**
   * \brief Directly access the internal data buffer for a shared file.
   *
   * To speed up the packet creation process, this method gives direct access to the internal data buffers for a file.
   * The returned value can be used to directly access the (consecutive) buffer in which a file is stored. The length of each buffer equals the
   * value of the GetBufferSizeForFile method.
   *
   * This method is not influenced by the usage of fake data.
   *
   * @param path the path (relative to the current execution directory) to the file. Should equal an argument once passed to the EnsureFileLoaded method.
   *
   * @returns a pointer to the beginning of the associated with the shared file.
   *
   * Note: The null pointer is returned if the file could not be found. You can check and load a file that is not loaded using the EnsureFileLoaded method.
   */
  const uint8_t* GetBufferForFile (const std::string &path) const;

  /**
   * \brief Return the size of the internal buffer for a shared file.
   *
   * @param path the path (relative to the current execution directory) to the file. Should equal an argument once passed to the EnsureFileLoaded method.
   *
   * @returns the size of the internal buffer associated with a shared file (in bytes).
   */
  uint64_t GetBufferSizeForFile (const std::string &path) const;
};

} // ns bittorrent
} // ns ns3

#endif /* STORAGEMANAGER_H_ */
