################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/bittorrent/model/common/BitTorrentUtilities.cc \
../ns-3/src/bittorrent/model/common/GlobalMetricsGatherer.cc \
../ns-3/src/bittorrent/model/common/Torrent.cc \
../ns-3/src/bittorrent/model/common/TorrentFile.cc 

OBJS += \
./ns-3/src/bittorrent/model/common/BitTorrentUtilities.o \
./ns-3/src/bittorrent/model/common/GlobalMetricsGatherer.o \
./ns-3/src/bittorrent/model/common/Torrent.o \
./ns-3/src/bittorrent/model/common/TorrentFile.o 

CC_DEPS += \
./ns-3/src/bittorrent/model/common/BitTorrentUtilities.d \
./ns-3/src/bittorrent/model/common/GlobalMetricsGatherer.d \
./ns-3/src/bittorrent/model/common/Torrent.d \
./ns-3/src/bittorrent/model/common/TorrentFile.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/bittorrent/model/common/%.o: ../ns-3/src/bittorrent/model/common/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


