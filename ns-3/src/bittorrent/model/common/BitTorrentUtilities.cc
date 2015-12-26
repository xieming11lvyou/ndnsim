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

#include "BitTorrentUtilities.h"

#include "ns3/log.h"
#include "ns3/random-variable.h"

#include <iomanip>
#include <list>
#include <set>
#include <sstream>

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("bittorrent::Utilities");

Utilities::Utilities ()
{
}

Utilities::~Utilities ()
{
}

std::list<uint32_t> Utilities::GetPermutationP (uint32_t m, uint32_t n)
{
  std::list<uint32_t> result;

  for (uint32_t j = n - m + 1; j <= n; ++j)
    {
      UniformVariable uv;
      uint32_t t = uv.GetInteger (1, j);

      std::list<uint32_t>::iterator it = find (result.begin (), result.end (), t);
      if (it == result.end ())
        {
          result.push_front (t);
        }
      else
        {
          ++it;
          if (it == result.end ())
            {
              result.push_back (j);
            }
          else
            {
              result.insert (it, j);
            }
        }
    }

  return result;
}

std::set<uint32_t> Utilities::GetRandomSampleF2 (uint32_t m, uint32_t n)
{
  std::set<uint32_t> result;

  m = std::min (n, m);

  for (uint32_t j = n - m + 1; j <= n; ++j)
    {
      UniformVariable uv;
      uint32_t t = uv.GetInteger (1, j);
      if (result.find (t) == result.end ())
        {
          result.insert (t);
        }
      else
        {
          result.insert (j);
        }
    }

  return result;
}

uint32_t Utilities::GetValueOfHexChar (char c)
{
  if (c >= '0' && c <= '9')
    {
      return c - '0';
    }
  else if (c >= 'A' && c <= 'F')
    {
      return c - 'A' + 10;
    }
  else if (c >= 'a' && c <= 'f')
    {
      return c - 'a' + 10;
    }

  return 16;
}

std::string Utilities::EscapeHex (std::string s)
{
  std::stringstream result;

  for (uint32_t i = 0; i < s.size (); ++i)
    {
      if ((s[i] <= 44) || (s[i] == 47) || (s[i] >= 58 && s[i] <= 64) || (s[i] >= 91 && s[i] <= 96) || (s[i] >= 123 && s[i] != 126))
        {
          result
          << "%"
          << std::uppercase << std::right << std::setw (2) << std::setfill ('0') << std::hex
          << static_cast<uint16_t> (static_cast<uint8_t> (s[i]));
        }
      else
        {
          result << s[i];
        }
    }

  return result.str ();
}

std::string Utilities::EscapeHex (const uint8_t* input, uint32_t len)
{
  std::string buffer;
  buffer.assign (reinterpret_cast<const char*> (input), 0, len);

  return EscapeHex (buffer);
}

std::string Utilities::UnescapeHex (std::string s)
{
  std::string result;
  uint32_t i = 0;
  uint32_t buffer;

  while (i < s.size ())
    {
      if (s[i] == 37)
        {
          if (i + 2 < s.size ())
            {
              std::stringstream converter;
              converter << s[i + 1] << s[i + 2];
              converter >> std::hex >> buffer;
              result += static_cast<char> (buffer);
              i += 3;
            }
          else
            {
              NS_LOG_DEBUG ("Unable to convert hex encoded string at position " << i << "; string = " << s);
              return "";
            }
        }
      else
        {
          result += s[i];
          ++i;
        }
    }

  return result;
}

std::string Utilities::DecodeInfoHash (std::string s)
{
  std::string result;
  uint32_t i = 0;

  while (i < s.size ())
    {
      if (s[i] == 37)
        {
          std::stringstream converter;
          converter << ((char)toupper (s[i + 1])) << ((char)toupper (s[i + 2]));
          result += converter.str ();
          i += 2;
        }
      else
        {
          std::stringstream converter;
          converter << std::hex << (int)s[i];
          result += converter.str ();
        }

      ++i;
    }

  return result;
}


} // ns bittorrent
} // ns ns3
