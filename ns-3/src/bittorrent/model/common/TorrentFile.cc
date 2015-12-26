/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010, 2012 ComSys, RWTH Aachen University
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
 * Authors: Martin Lang
 */

#include "TorrentFile.h"

#include "BitTorrentUtilities.h"

#include "ns3/log.h"

namespace ns3 {
namespace bittorrent {

NS_LOG_COMPONENT_DEFINE ("TorrentFileParser");

Ptr<TorrentData> TorrentData::ReadBencodedData (std::istream &istr)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<TorrentData> rootNode (new TorrentDataDictonary ());
  rootNode->ReadFromStream (istr);
  return rootNode;
}

TorrentData::TorrentDataType TorrentData::GetNextDataType (std::istream &file)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (file.bad ())
    {
      return DATA_EOF;
    }

  int nextSign = file.peek ();

  if (nextSign == 'i')
    {
      return DATA_INT;
    }
  else if (nextSign == 'l')
    {
      return DATA_LIST;
    }
  else if (nextSign == 'd')
    {
      return DATA_DICT;
    }
  else if (nextSign == 'e')
    {
      return DATA_ENDMARKER;
    }
  else if (std::isdigit (nextSign))
    {
      return DATA_STRING;
    }
  else
    {
      return DATA_UNKNOWN;
    }

}

Ptr<TorrentData> TorrentData::CreateObject (TorrentDataType type)
{
  NS_LOG_FUNCTION_NOARGS ();
  // i am a mini factory
  switch (type)
    {
    case DATA_STRING:
      return Create<TorrentDataString> ();
    case DATA_INT:
      return Create<TorrentDataInt> ();
    case DATA_LIST:
      return Create<TorrentDataList> ();
    case DATA_DICT:
      return Create<TorrentDataDictonary> ();
    default:
      return Ptr<TorrentData> (0);
    }
}

long long TorrentData::ReadNumber (std::istream &file)
{
  NS_LOG_FUNCTION_NOARGS ();
  std::string num;
  char c;

  file.get (c);
  while (std::isdigit (c) || c == '-')
    {
      num = num + c;

      file.get (c);
    }

  return lexical_cast<long long> (num);
}

bool TorrentDataString::ReadFromStream (std::istream &file)
{
  NS_LOG_FUNCTION ("String");
  long long length = ReadNumber (file);

  char *rawData = new char[length];

  file.read (rawData,length);
  m_data.assign (rawData,length);

  delete [] rawData;

  return true;
}

bool TorrentDataInt::ReadFromStream (std::istream &file)
{
  NS_LOG_FUNCTION ("Int");
  // first drop the "i"
  file.get ();

  // now read number
  m_data = ReadNumber (file);      // notice that the e at the end is dropped

  //file.get(); // drop the "e"

  NS_LOG_DEBUG ("Next Sign is " << file.peek ());

  return true;
}

bool TorrentDataList::ReadFromStream (std::istream &file)
{
  NS_LOG_FUNCTION ("List");
  // drop the "l"
  file.get ();

  TorrentDataType nextType = GetNextDataType (file);
  while (nextType != DATA_ENDMARKER && nextType != DATA_EOF && nextType != DATA_UNKNOWN)
    {
      Ptr<TorrentData> currData = CreateObject (nextType);
      currData->ReadFromStream (file);
      m_data.push_back (currData);
      nextType = GetNextDataType (file);
    }

  if (nextType == DATA_ENDMARKER)
    {
      // drop the endmarker
      file.get ();
      return true;
    }
  else
    {
      NS_LOG_WARN ("Unexpected end of list while parsing torrent file");
      return false;           // something weird occurred
    }
}

bool TorrentDataDictonary::ReadFromStream (std::istream &file)
{
  NS_LOG_FUNCTION ("Dict");
  // drop the "d"
  file.get ();

  NS_LOG_DEBUG ("Reading Dictionary Entry, String is" << DATA_STRING);

  TorrentDataType nextType = GetNextDataType (file);

  NS_LOG_DEBUG ("Next Entry Type: " << nextType);

  while (nextType == DATA_STRING)
    {
      NS_LOG_DEBUG ("Read Entry");
      // the type should be a string
      TorrentDataString name;
      name.ReadFromStream (file);

      NS_LOG_DEBUG ("Entry Name " << name.GetData ());

      nextType = GetNextDataType (file);
      if (nextType == DATA_ENDMARKER || nextType == DATA_UNKNOWN || nextType == DATA_EOF)
        {
          // something is wrong here
          return false;
        }

      NS_LOG_DEBUG ("Read Data");

      Ptr<TorrentData> currData = CreateObject (nextType);
      currData->ReadFromStream (file);

      // OLD
      // m_data[name.GetData ()] = currData;
      
      // MEMORY
      std::string key = name.GetData ();
      m_data.insert(std::pair<std::string, Ptr<TorrentData> >(key, currData));
      // /MEMORY

      nextType = GetNextDataType (file);
    }

  if (nextType == DATA_ENDMARKER)
    {
      NS_LOG_DEBUG ("Found Endmarker");
      // drop the endmarker
      file.get ();
      return true;
    }
  else
    {
      NS_LOG_WARN ("Unexpected end of dictionary while reading torrent file. nextType is " << nextType );
      NS_LOG_WARN ("Content now is ");
      Dump ();
      return false;                   // something weird occured
    }

}

Ptr<TorrentData> TorrentDataDictonary::GetData (const std::string &entry)
{
  std::map<std::string,Ptr<TorrentData> >::iterator iter = m_data.find (entry);

  if (iter == m_data.end ())
    {
      return Ptr<TorrentData> ();
    }
  else
    {
      return iter->second;
    }
}

void TorrentDataDictonary::Dump () const
{
  std::map<std::string,Ptr<TorrentData> >::const_iterator iter = m_data.begin ();

  NS_LOG_DEBUG ("Dict Map:");
  for (; iter != m_data.end (); ++iter)
    {
      NS_LOG_DEBUG ("Entry: " << iter->first);
    }
  NS_LOG_DEBUG ("/Dict Map");
}

} // ns bittorrent
} // ns ns3

