################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/network/test/buffer-test.cc \
../ns-3/src/network/test/drop-tail-queue-test-suite.cc \
../ns-3/src/network/test/error-model-test-suite.cc \
../ns-3/src/network/test/ipv6-address-test-suite.cc \
../ns-3/src/network/test/packet-metadata-test.cc \
../ns-3/src/network/test/packet-socket-apps-test-suite.cc \
../ns-3/src/network/test/packet-test-suite.cc \
../ns-3/src/network/test/packetbb-test-suite.cc \
../ns-3/src/network/test/pcap-file-test-suite.cc \
../ns-3/src/network/test/red-queue-test-suite.cc \
../ns-3/src/network/test/sequence-number-test-suite.cc 

OBJS += \
./ns-3/src/network/test/buffer-test.o \
./ns-3/src/network/test/drop-tail-queue-test-suite.o \
./ns-3/src/network/test/error-model-test-suite.o \
./ns-3/src/network/test/ipv6-address-test-suite.o \
./ns-3/src/network/test/packet-metadata-test.o \
./ns-3/src/network/test/packet-socket-apps-test-suite.o \
./ns-3/src/network/test/packet-test-suite.o \
./ns-3/src/network/test/packetbb-test-suite.o \
./ns-3/src/network/test/pcap-file-test-suite.o \
./ns-3/src/network/test/red-queue-test-suite.o \
./ns-3/src/network/test/sequence-number-test-suite.o 

CC_DEPS += \
./ns-3/src/network/test/buffer-test.d \
./ns-3/src/network/test/drop-tail-queue-test-suite.d \
./ns-3/src/network/test/error-model-test-suite.d \
./ns-3/src/network/test/ipv6-address-test-suite.d \
./ns-3/src/network/test/packet-metadata-test.d \
./ns-3/src/network/test/packet-socket-apps-test-suite.d \
./ns-3/src/network/test/packet-test-suite.d \
./ns-3/src/network/test/packetbb-test-suite.d \
./ns-3/src/network/test/pcap-file-test-suite.d \
./ns-3/src/network/test/red-queue-test-suite.d \
./ns-3/src/network/test/sequence-number-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/network/test/%.o: ../ns-3/src/network/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


