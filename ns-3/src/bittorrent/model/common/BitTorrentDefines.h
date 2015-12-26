/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 ComSys, RWTH Aachen University
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

/**
 * \ingroup BitTorrent
 *
 * @namespace bittorrent
 * A BitTorrent client and tracker model for ns-3.
 */

#ifndef BITTORRENTCLIENT_DEFINES_H_
#define BITTORRENTCLIENT_DEFINES_H_

#define BT_PEER_SOCKET_TRANSMIT_BUFFER_SIZE 65536 // In bytes; per client connection
#define BT_PEER_SOCKET_RECEIVE_BUFFER_SIZE 65536 // In bytes; per client connection 125000000 = Maximum bytes in Gigabit Ethernet per second
#define BT_PEER_SOCKET_TCP_SEGMENT_SIZE_MAX 1452 // In bytes; per client connection; 1452 = Maximum segment size for most access providers (PPPoE); 536 = Default for many systems
#define BT_PEER_DOWNLOADUPLOADRATE_ROLLING_AVERAGE_SECONDS 20 // Rolling down/upload rate is calculated over the given last seconds; 20 = Standard

#define BT_PEER_PIECE_RECEPTION_NOT_RECEIVED 0
#define BT_PEER_PIECE_RECEPTION_CHECKSUM_OK 255
#define BT_PEER_PIECE_RECEPTION_CHECKSUM_NOT_OK 1

#define BT_PROTOCOL_MESSAGES_LENGTHHEADER_LENGTH 4 // Only update for general revisions of the BT protocol; 4 = Standard 32-bit integer
#define BT_PROTOCOL_MESSAGES_LENGTHHEADER_THRESHOLD_PIECE_OR_EXTENSIONPROTOCOL_MESSAGE BT_PROTOCOL_MESSAGES_PIECE_LENGTH_MIN // Packets of larger size should contain piece or extension message data (used for bandwidth estimation)
#define BT_PROTOCOL_MESSAGES_HANDSHAKE_LENGTH_MIN 49 // Only update for general revisions of the BT protocol; 49 = Standard
#define BT_PROTOCOL_MESSAGES_HANDSHAKE_PROTOCOL_STRING "BitTorrent protocol" // // Only update for general revisions of the BT protocol; "BitTorrent protocol" = standard
#define BT_PROTOCOL_MESSAGES_HANDSHAKE_PROTOCOL_STRING_LENGTH 19 // Length of above BT_PROTOCOL_MESSAGES_HANDSHAKE_PROTOCOL_STRING string (8-bit characters)
#define BT_PROTOCOL_MESSAGES_HANDSHAKE_PEERID_LENGTH_MAX 20 // Only update for general revisions of the BT protocol; 20 = Standard
#define BT_PROTOCOL_MESSAGES_KEEPALIVE_LENGTH 0
#define BT_PROTOCOL_MESSAGES_CHOKE_LENGTH 1
#define BT_PROTOCOL_MESSAGES_UNCHOKE_LENGTH 1
#define BT_PROTOCOL_MESSAGES_INTERESTED_LENGTH 1
#define BT_PROTOCOL_MESSAGES_UNINTERESTED_LENGTH 1
#define BT_PROTOCOL_MESSAGES_HAVE_LENGTH 5
#define BT_PROTOCOL_MESSAGES_BITFIELD_LENGTH_MIN 1
#define BT_PROTOCOL_MESSAGES_REQUEST_LENGTH 13
#define BT_PROTOCOL_MESSAGES_PIECE_LENGTH_MIN 9
#define BT_PROTOCOL_MESSAGES_CANCEL_LENGTH 13
#define BT_PROTOCOL_MESSAGES_PORT_LENGTH 3
#define BT_PROTOCOL_MESSAGES_EXTENSIONPROTOCOL_LENGTH_MIN 1

#define BT_PROTOCOL_LISTENER_PORT 6881

#define BT_PEER_CONNECTOR_CONNECTION_ACCEPTANCE_DELAY 10000 // In milliseconds; Usually, 10 seconds should be enough

#endif /* BITTORRENTCLIENT_DEFINES_H_ */
