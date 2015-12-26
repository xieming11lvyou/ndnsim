################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/bittorrent/model/client/AbstractStrategy.cc \
../ns-3/src/bittorrent/model/client/BitTorrentClient.cc \
../ns-3/src/bittorrent/model/client/BitTorrentHttpClient.cc \
../ns-3/src/bittorrent/model/client/BitTorrentPacket.cc \
../ns-3/src/bittorrent/model/client/BitTorrentPeer.cc \
../ns-3/src/bittorrent/model/client/BitTorrentVideoClient.cc \
../ns-3/src/bittorrent/model/client/BitTorrentVideoMetricsBase.cc \
../ns-3/src/bittorrent/model/client/ChokeUnChokeStrategyBase.cc \
../ns-3/src/bittorrent/model/client/PartSelectionStrategyBase.cc \
../ns-3/src/bittorrent/model/client/PeerConnectorStrategyBase.cc \
../ns-3/src/bittorrent/model/client/ProtocolFactory.cc \
../ns-3/src/bittorrent/model/client/RequestSchedulingStrategyBase.cc \
../ns-3/src/bittorrent/model/client/StorageManager.cc 

OBJS += \
./ns-3/src/bittorrent/model/client/AbstractStrategy.o \
./ns-3/src/bittorrent/model/client/BitTorrentClient.o \
./ns-3/src/bittorrent/model/client/BitTorrentHttpClient.o \
./ns-3/src/bittorrent/model/client/BitTorrentPacket.o \
./ns-3/src/bittorrent/model/client/BitTorrentPeer.o \
./ns-3/src/bittorrent/model/client/BitTorrentVideoClient.o \
./ns-3/src/bittorrent/model/client/BitTorrentVideoMetricsBase.o \
./ns-3/src/bittorrent/model/client/ChokeUnChokeStrategyBase.o \
./ns-3/src/bittorrent/model/client/PartSelectionStrategyBase.o \
./ns-3/src/bittorrent/model/client/PeerConnectorStrategyBase.o \
./ns-3/src/bittorrent/model/client/ProtocolFactory.o \
./ns-3/src/bittorrent/model/client/RequestSchedulingStrategyBase.o \
./ns-3/src/bittorrent/model/client/StorageManager.o 

CC_DEPS += \
./ns-3/src/bittorrent/model/client/AbstractStrategy.d \
./ns-3/src/bittorrent/model/client/BitTorrentClient.d \
./ns-3/src/bittorrent/model/client/BitTorrentHttpClient.d \
./ns-3/src/bittorrent/model/client/BitTorrentPacket.d \
./ns-3/src/bittorrent/model/client/BitTorrentPeer.d \
./ns-3/src/bittorrent/model/client/BitTorrentVideoClient.d \
./ns-3/src/bittorrent/model/client/BitTorrentVideoMetricsBase.d \
./ns-3/src/bittorrent/model/client/ChokeUnChokeStrategyBase.d \
./ns-3/src/bittorrent/model/client/PartSelectionStrategyBase.d \
./ns-3/src/bittorrent/model/client/PeerConnectorStrategyBase.d \
./ns-3/src/bittorrent/model/client/ProtocolFactory.d \
./ns-3/src/bittorrent/model/client/RequestSchedulingStrategyBase.d \
./ns-3/src/bittorrent/model/client/StorageManager.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/bittorrent/model/client/%.o: ../ns-3/src/bittorrent/model/client/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


