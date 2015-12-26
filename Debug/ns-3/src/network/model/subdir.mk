################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/network/model/address.cc \
../ns-3/src/network/model/application.cc \
../ns-3/src/network/model/buffer.cc \
../ns-3/src/network/model/byte-tag-list.cc \
../ns-3/src/network/model/channel-list.cc \
../ns-3/src/network/model/channel.cc \
../ns-3/src/network/model/chunk.cc \
../ns-3/src/network/model/header.cc \
../ns-3/src/network/model/net-device.cc \
../ns-3/src/network/model/nix-vector.cc \
../ns-3/src/network/model/node-list.cc \
../ns-3/src/network/model/node.cc \
../ns-3/src/network/model/packet-metadata.cc \
../ns-3/src/network/model/packet-tag-list.cc \
../ns-3/src/network/model/packet.cc \
../ns-3/src/network/model/socket-factory.cc \
../ns-3/src/network/model/socket.cc \
../ns-3/src/network/model/tag-buffer.cc \
../ns-3/src/network/model/tag.cc \
../ns-3/src/network/model/trailer.cc 

OBJS += \
./ns-3/src/network/model/address.o \
./ns-3/src/network/model/application.o \
./ns-3/src/network/model/buffer.o \
./ns-3/src/network/model/byte-tag-list.o \
./ns-3/src/network/model/channel-list.o \
./ns-3/src/network/model/channel.o \
./ns-3/src/network/model/chunk.o \
./ns-3/src/network/model/header.o \
./ns-3/src/network/model/net-device.o \
./ns-3/src/network/model/nix-vector.o \
./ns-3/src/network/model/node-list.o \
./ns-3/src/network/model/node.o \
./ns-3/src/network/model/packet-metadata.o \
./ns-3/src/network/model/packet-tag-list.o \
./ns-3/src/network/model/packet.o \
./ns-3/src/network/model/socket-factory.o \
./ns-3/src/network/model/socket.o \
./ns-3/src/network/model/tag-buffer.o \
./ns-3/src/network/model/tag.o \
./ns-3/src/network/model/trailer.o 

CC_DEPS += \
./ns-3/src/network/model/address.d \
./ns-3/src/network/model/application.d \
./ns-3/src/network/model/buffer.d \
./ns-3/src/network/model/byte-tag-list.d \
./ns-3/src/network/model/channel-list.d \
./ns-3/src/network/model/channel.d \
./ns-3/src/network/model/chunk.d \
./ns-3/src/network/model/header.d \
./ns-3/src/network/model/net-device.d \
./ns-3/src/network/model/nix-vector.d \
./ns-3/src/network/model/node-list.d \
./ns-3/src/network/model/node.d \
./ns-3/src/network/model/packet-metadata.d \
./ns-3/src/network/model/packet-tag-list.d \
./ns-3/src/network/model/packet.d \
./ns-3/src/network/model/socket-factory.d \
./ns-3/src/network/model/socket.d \
./ns-3/src/network/model/tag-buffer.d \
./ns-3/src/network/model/tag.d \
./ns-3/src/network/model/trailer.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/network/model/%.o: ../ns-3/src/network/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


