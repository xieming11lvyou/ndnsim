################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/network/utils/address-utils.cc \
../ns-3/src/network/utils/ascii-file.cc \
../ns-3/src/network/utils/crc32.cc \
../ns-3/src/network/utils/data-rate.cc \
../ns-3/src/network/utils/drop-tail-queue.cc \
../ns-3/src/network/utils/error-model.cc \
../ns-3/src/network/utils/ethernet-header.cc \
../ns-3/src/network/utils/ethernet-trailer.cc \
../ns-3/src/network/utils/flow-id-tag.cc \
../ns-3/src/network/utils/inet-socket-address.cc \
../ns-3/src/network/utils/inet6-socket-address.cc \
../ns-3/src/network/utils/ipv4-address.cc \
../ns-3/src/network/utils/ipv6-address.cc \
../ns-3/src/network/utils/llc-snap-header.cc \
../ns-3/src/network/utils/mac16-address.cc \
../ns-3/src/network/utils/mac48-address.cc \
../ns-3/src/network/utils/mac64-address.cc \
../ns-3/src/network/utils/output-stream-wrapper.cc \
../ns-3/src/network/utils/packet-burst.cc \
../ns-3/src/network/utils/packet-data-calculators.cc \
../ns-3/src/network/utils/packet-probe.cc \
../ns-3/src/network/utils/packet-socket-address.cc \
../ns-3/src/network/utils/packet-socket-client.cc \
../ns-3/src/network/utils/packet-socket-factory.cc \
../ns-3/src/network/utils/packet-socket-server.cc \
../ns-3/src/network/utils/packet-socket.cc \
../ns-3/src/network/utils/packetbb.cc \
../ns-3/src/network/utils/pcap-file-wrapper.cc \
../ns-3/src/network/utils/pcap-file.cc \
../ns-3/src/network/utils/queue.cc \
../ns-3/src/network/utils/radiotap-header.cc \
../ns-3/src/network/utils/red-queue.cc \
../ns-3/src/network/utils/simple-channel.cc \
../ns-3/src/network/utils/simple-net-device.cc 

OBJS += \
./ns-3/src/network/utils/address-utils.o \
./ns-3/src/network/utils/ascii-file.o \
./ns-3/src/network/utils/crc32.o \
./ns-3/src/network/utils/data-rate.o \
./ns-3/src/network/utils/drop-tail-queue.o \
./ns-3/src/network/utils/error-model.o \
./ns-3/src/network/utils/ethernet-header.o \
./ns-3/src/network/utils/ethernet-trailer.o \
./ns-3/src/network/utils/flow-id-tag.o \
./ns-3/src/network/utils/inet-socket-address.o \
./ns-3/src/network/utils/inet6-socket-address.o \
./ns-3/src/network/utils/ipv4-address.o \
./ns-3/src/network/utils/ipv6-address.o \
./ns-3/src/network/utils/llc-snap-header.o \
./ns-3/src/network/utils/mac16-address.o \
./ns-3/src/network/utils/mac48-address.o \
./ns-3/src/network/utils/mac64-address.o \
./ns-3/src/network/utils/output-stream-wrapper.o \
./ns-3/src/network/utils/packet-burst.o \
./ns-3/src/network/utils/packet-data-calculators.o \
./ns-3/src/network/utils/packet-probe.o \
./ns-3/src/network/utils/packet-socket-address.o \
./ns-3/src/network/utils/packet-socket-client.o \
./ns-3/src/network/utils/packet-socket-factory.o \
./ns-3/src/network/utils/packet-socket-server.o \
./ns-3/src/network/utils/packet-socket.o \
./ns-3/src/network/utils/packetbb.o \
./ns-3/src/network/utils/pcap-file-wrapper.o \
./ns-3/src/network/utils/pcap-file.o \
./ns-3/src/network/utils/queue.o \
./ns-3/src/network/utils/radiotap-header.o \
./ns-3/src/network/utils/red-queue.o \
./ns-3/src/network/utils/simple-channel.o \
./ns-3/src/network/utils/simple-net-device.o 

CC_DEPS += \
./ns-3/src/network/utils/address-utils.d \
./ns-3/src/network/utils/ascii-file.d \
./ns-3/src/network/utils/crc32.d \
./ns-3/src/network/utils/data-rate.d \
./ns-3/src/network/utils/drop-tail-queue.d \
./ns-3/src/network/utils/error-model.d \
./ns-3/src/network/utils/ethernet-header.d \
./ns-3/src/network/utils/ethernet-trailer.d \
./ns-3/src/network/utils/flow-id-tag.d \
./ns-3/src/network/utils/inet-socket-address.d \
./ns-3/src/network/utils/inet6-socket-address.d \
./ns-3/src/network/utils/ipv4-address.d \
./ns-3/src/network/utils/ipv6-address.d \
./ns-3/src/network/utils/llc-snap-header.d \
./ns-3/src/network/utils/mac16-address.d \
./ns-3/src/network/utils/mac48-address.d \
./ns-3/src/network/utils/mac64-address.d \
./ns-3/src/network/utils/output-stream-wrapper.d \
./ns-3/src/network/utils/packet-burst.d \
./ns-3/src/network/utils/packet-data-calculators.d \
./ns-3/src/network/utils/packet-probe.d \
./ns-3/src/network/utils/packet-socket-address.d \
./ns-3/src/network/utils/packet-socket-client.d \
./ns-3/src/network/utils/packet-socket-factory.d \
./ns-3/src/network/utils/packet-socket-server.d \
./ns-3/src/network/utils/packet-socket.d \
./ns-3/src/network/utils/packetbb.d \
./ns-3/src/network/utils/pcap-file-wrapper.d \
./ns-3/src/network/utils/pcap-file.d \
./ns-3/src/network/utils/queue.d \
./ns-3/src/network/utils/radiotap-header.d \
./ns-3/src/network/utils/red-queue.d \
./ns-3/src/network/utils/simple-channel.d \
./ns-3/src/network/utils/simple-net-device.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/network/utils/%.o: ../ns-3/src/network/utils/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


