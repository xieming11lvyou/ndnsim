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

#ifndef BITTORRENTCLIENT_UTILITIES_H_
#define BITTORRENTCLIENT_UTILITIES_H_

#include "ns3/object.h"

#include <list>
#include <set>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Provides commonly-needed functionality for permutations and character recoding.
 *
 * This class incorporates some methods that implement functionality needed throughout the RWTH Aachen University BitTorrent simulation model classes.
 *
 */
class Utilities : public Object
{
// Constructors etc. (singleton pattern)
private:
  Utilities ();
  ~Utilities ();

// Offered methods
public:
  /**
   * \brief Generate a random permutation of integers.
   *
   * The algorithm is taken from:\n
   *  J. Bentley with Special Guest O. B. Floyd\ņ
   *   "Programming Pearls" - "A Sample of Brilliance"\n
   *    Communications of the ACM, September 1987, Volume 30, Number 9, pp. 754-756
   *
   * @param m the number of elements in the permutation.
   * @param n the upper bound (inclusive) of the random elements.
   *
   * @returns a permutation of length m of elements taken from the range 0..n.
   */
  static std::list<uint32_t> GetPermutationP (uint32_t m, uint32_t n);

  /**
   * \brief Generate a random sample of integers.
   *
   * The algorithm is taken from:\n
   *  J. Bentley with Special Guest O. B. Floyd\ņ
   *   "Programming Pearls" - "A Sample of Brilliance"\n
   *    Communications of the ACM, September 1987, Volume 30, Number 9, pp. 754-756
   *
   * @param m the number of elements in the sample.
   * @param n the upper bound (inclusive) of the random elements.
   *
   * @returns a random sample of length m of elements taken from the range 0..n.
   */
  static std::set<uint32_t> GetRandomSampleF2 (uint32_t m, uint32_t n);

  /**
   * \brief Retrieve the "normalized" value of a hexadecimal character.
   *
   * @param c the character to convert.
   *
   * @returns an integer between 0 and 15 when the supplied character was a valid hexadecimal character; 16 in case of an error.
   */
  static uint32_t GetValueOfHexChar (char c);

  /**
   * \brief URL-encode a string.
   *
   * This method URL-encodes a string by escaping non-standard ASCII characters into an "%xy" escaped form.
   * See <a href="http://www.faqs.org/rfcs/rfc1738.html" target="_blank">RFC 1738</a> for details.
   *
   * @param s the string to URL-encode.
   *
   * @returns an URL-encoded variant of the input string.
   */
  static std::string EscapeHex (std::string s);

  /**
   * \brief URL-encode a string in a character buffer.
   *
   * @param input pointer to the start of the string to convert.
   * @param len the length of the string to convert.
   *
   * @returns an URL-encoded variant of the string found in the given location.
   */
  static std::string EscapeHex (const uint8_t* input, uint32_t len);

  /**
   * \brief Transform a URL-encoded string into its non-encoded form.
   *
   * This method is the inverse function for the two variants of EscapeHex.
   *
   * @param s the string to decode. Must contain a valid encoding.
   *
   * @returns the decoded variant of the supplied string; an empty string in case of an error.
   */
  static std::string UnescapeHex (std::string s);

  /**
   * \brief Transform an URL-encoded info-hash into its non-encoded form.
   *
   * This method works similarly to the UnescapeHex method.
   *
   * @param s the info hash to decode. Must contain a valid encoding.
   *
   * @returns the decoded variant of the supplied info hash.
   */
  static std::string DecodeInfoHash (std::string s);
};

// One of the most useful templates: The lexical cast.
template<typename T,typename R>
T lexical_cast (const R &r)
{

  std::stringstream ss;

  T t;
  ss << r;
  ss >> t;
  return t;
}

} // ns bittorrent
} // ns ns3

#endif /* BITTORRENTCLIENT_UTILITIES_H_ */
