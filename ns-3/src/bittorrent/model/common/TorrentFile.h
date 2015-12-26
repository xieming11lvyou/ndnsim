/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 ComSys, RWTH Aachen University
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

#ifndef TORRENTFILE_H_
#define TORRENTFILE_H_

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

#include <list>
#include <map>
#include <string>

namespace ns3 {
namespace bittorrent {

/**
 * \ingroup BitTorrent
 *
 * \brief Reads and stores information from a bencoded dictionary.
 *
 * The TorrentData class and its derived classes are used to represent the data found in a bencoded dictionary,
 * such as the ".torrent" files specified in <a href="http://www.bittorrent.org/beps/bep_0003.html" target="_blank">Bram Cohen's official BitTorrent specification</a>.
 *
 * The root class provides some globally-usable functionality for handlinc bencoded data, while the derived classes deal with specific bencoded data types.
 *
 * The derived classes are TorrentDataString, TorrentDataInt, TorrentDataList and TorrentDataDictionary.
 * Each of these classes (except TorrentDataList) provides a GetData() function which can be used to access the data stored within the respective class instance.
 *
 * The TorrentDataList class defines two special methods, GetIterator() and GetListEnd(), which both return iterators to the elements in the list, which is of type std::list<Ptr<TorrentData> >::const_iterator.
 *
 * The suggested usage of these classes is to open an input file stream and do something like the following:
 * \code
 * Ptr<TorrentData> rootEntry = TorrentData::ReadBencodedData (input);
 * Ptr<TorrentDataDictonary> rootDictionary = DynamicCast<TorrentDataDictonary> (root);
 *     // v-- or any other fitting TorrentDataType --v                                             v-- name of the entry, if on top level of dictionary
 * Ptr<TorrentDataString> announceURLNode = DynamicCast<TorrentDataString> (rootDict->GetData ("announce"));
 * std::string announceURL = announceURLNode->GetData ();
 * \endcode
 */
class TorrentData : public SimpleRefCount<TorrentData>
{
// Constructors etc.
public:
	TorrentData() { }
	virtual ~TorrentData() {}

	/**
	 * Represents a data type which can be found in a beconded file.
	 */
	enum TorrentDataType {
		DATA_STRING,
		DATA_INT,
		DATA_LIST,
		DATA_DICT,
		DATA_ENDMARKER,
		DATA_EOF,
		DATA_UNKNOWN
	};

	/**
	 * @returns the data type contained within the object.
	 */
	virtual TorrentDataType GetDataType() const = 0;

	/**
	 * \brief Read the next element from the bencoded file.
	 *
	 * Recursively called by ReadBencodedData.
	 *
	 * @param stream from which the element should be read.
	 *
	 * @returns true, if no error occurred while reading.
	 */
	virtual bool ReadFromStream(std::istream &file) = 0;

	/**
	 * \brief Read a bencoded file.
	 *
	 * The file is expected to contain at least a dictionary entry.
	 *
	 * @param istr stream from which the data should be read.  Must be readable and already opened.
	 *
	 * @returns a pointer to a TorrentData instance representing the root entry of the read dictionary.
	 */
	static Ptr<TorrentData> ReadBencodedData(std::istream &istr);

protected:
	/**
	 * \brief Get the data type of the next element in the stream.
	 *
	 * @param file stream from which the data should be read.
	 *
	 * @returns DATA_EOF if the stream has no more entriesm, else the appropriate TorrentDataType that follows immediately.
	 */
	static TorrentDataType GetNextDataType(std::istream &file);

	/**
	 * \brief Create an instance of a class derived from the TorrentData class.
	 *
	 * @param type the type that the created class shall handle.
	 *
	 * @returns a pointer to a TorrentData instance capable of handling the specified type.
	 */
	static Ptr<TorrentData> CreateObject(TorrentDataType type);

	/**
	 * \brief Read a signed or unsigned integer value from the stream.
	 *
	 * Stops at the first non-digit element.
	 *
	 * @param file stream from whioch the data should be read.
	 *
	 * @returns the number read from the file.
	 */
	static long long ReadNumber(std::istream &file);
};

/// @cond HIDDEN
class TorrentDataString : public TorrentData
{
public:
	TorrentDataString() {}
	virtual ~TorrentDataString() {}

	std::string GetData() const {
		return m_data;
	}

	virtual TorrentDataType GetDataType() const {
		return DATA_STRING;
	}

	virtual bool ReadFromStream(std::istream &file);
private:
	std::string m_data;
};

class TorrentDataInt : public TorrentData
{
public:
	TorrentDataInt() {}
	virtual ~TorrentDataInt() {}

	long long GetData() const {
		return m_data;
	}

	virtual TorrentDataType GetDataType() const {
		return DATA_INT;
	}

	virtual bool ReadFromStream(std::istream &file);
private:
	long long m_data;
};

class TorrentDataList : public TorrentData
{
public:
	TorrentDataList() {}
	virtual ~TorrentDataList() {}

	std::list<Ptr<TorrentData> >::const_iterator GetIterator() const {
		return m_data.begin();
	}

	std::list<Ptr<TorrentData> >::const_iterator GetListEnd() const {
			return m_data.end();
		}

	virtual TorrentDataType GetDataType() const {
		return DATA_LIST;
	}

	virtual bool ReadFromStream(std::istream &file);
private:
	std::list<Ptr<TorrentData> > m_data;
};

class TorrentDataDictonary : public TorrentData
{
public:
	TorrentDataDictonary() {}
	virtual ~TorrentDataDictonary() {}

	Ptr<TorrentData> GetData(const std::string &entry);

	virtual TorrentDataType GetDataType() const {
		return DATA_DICT;
	}

	virtual bool ReadFromStream(std::istream &file);

	void Dump() const;
private:
	std::map<std::string,Ptr<TorrentData> > m_data;
};
/// @endcond HIDDEN

} // ns bittorrent
} // ns ns3
#endif /* TORRENTFILE_H_ */
