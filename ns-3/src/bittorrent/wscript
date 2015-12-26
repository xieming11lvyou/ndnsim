## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    if 'tap-bridge' in bld.env['MODULES_NOT_BUILT']:
      module = bld.create_ns3_module('bittorrent', ['core', 'internet', 'network', 'nix-vector-routing', 'point-to-point', 'topology-read', 'csma', 
'applications'])
    else:
      module = bld.create_ns3_module('bittorrent', ['core', 'internet', 'network', 'nix-vector-routing', 'point-to-point', 'topology-read', 'csma', 'applications', 'tap-bridge'])
      # module = bld.create_ns3_module('bittorrent', ['core', 'internet', 'network', 'nix-vector-routing', 'point-to-point', 'topology-read', 'csma', 'applications', 'tap-bridge', 'slicetime'])
    module.source = [
        ## Common ##
	'model/common/3rd-party/sha1.cc',
        'model/common/BitTorrentUtilities.cc',
        'model/common/GlobalMetricsGatherer.cc',
        'model/common/Torrent.cc',
        'model/common/TorrentFile.cc',
        ## Client ##
    	'model/client/AbstractStrategy.cc',
        'model/client/BitTorrentClient.cc',
        'model/client/BitTorrentVideoClient.cc',
        'model/client/BitTorrentHttpClient.cc',
        'model/client/BitTorrentPacket.cc',
        'model/client/BitTorrentPeer.cc',
        'model/client/BitTorrentVideoMetricsBase.cc',
        'model/client/ChokeUnChokeStrategyBase.cc',
        'model/client/PartSelectionStrategyBase.cc',
        'model/client/PeerConnectorStrategyBase.cc',
        'model/client/ProtocolFactory.cc',
        'model/client/RequestSchedulingStrategyBase.cc',
        'model/client/StorageManager.cc',
        'model/client/strategies/RarestFirstPartSelectionStrategy.cc',
        #'model/client/strategies/vod/bitos/BiToS-PartSelectionStrategy.cc',
        #'model/client/strategies/vod/gtg/GTG-ChokeUnChokeStrategy.cc',
        #'model/client/strategies/vod/gtg/GTG-PartSelectionStrategy.cc',
        #'model/client/strategies/vod/rf-vod/RF-VoD-PartSelectionStrategy.cc',
		## Tracker ##
		'model/tracker/BitTorrentTracker.cc',
		'model/tracker/BitTorrentHttpServer.cc',
		## Helpers ##
		'helper/brite-topology-helper.cc',
        'helper/Story.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'bittorrent'
    headers.source = [
        ## Common ##
	'model/common/3rd-party/sha1.h',
        'model/common/BitTorrentDefines.h',
        'model/common/BitTorrentUtilities.h',
        'model/common/GlobalMetricsGatherer.h',
        'model/common/Torrent.h',
        'model/common/TorrentFile.h',
        ## Client ##
    	'model/client/AbstractStrategy.h',
        'model/client/BitTorrentClient.h',
        'model/client/BitTorrentVideoClient.h',
        'model/client/BitTorrentHttpClient.h',
        'model/client/BitTorrentPacket.h',
        'model/client/BitTorrentPeer.h',
        'model/client/BitTorrentVideoMetricsBase.h',
        'model/client/ChokeUnChokeStrategyBase.h',
        'model/client/PartSelectionStrategyBase.h',
        'model/client/PeerConnectorStrategyBase.h',
        'model/client/ProtocolFactory.h',
        'model/client/RequestSchedulingStrategyBase.h',       
        'model/client/StorageManager.h',
        'model/client/strategies/RarestFirstPartSelectionStrategy.h',
        #'model/client/strategies/vod/bitos/BiToS-PartSelectionStrategy.h',
        #'model/client/strategies/vod/gtg/GTG-ChokeUnChokeStrategy.h',
        #'model/client/strategies/vod/gtg/GTG-PartSelectionStrategy.h',
        #'model/client/strategies/vod/rf-vod/RF-VoD-PartSelectionStrategy.h',
        ## Tracker ##
		'model/tracker/BitTorrentTracker.h',
		'model/tracker/BitTorrentHttpServer.h',
		## Helpers ##
        'helper/brite-topology-helper.h',
        'helper/Story.h',
        ]
        
    if 'tap-bridge' in bld.env['MODULES_NOT_BUILT']:
      module.source.append('helper/no-tap/brite-tap-helper.cc')
      headers.source.append('helper/no-tap/brite-tap-helper.h')
    else:
      module.source.append('helper/brite-tap-helper.cc')
      headers.source.append('helper/brite-tap-helper.h')
      
    bld.recurse('examples')
