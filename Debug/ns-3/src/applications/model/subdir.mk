################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/applications/model/application-packet-probe.cc \
../ns-3/src/applications/model/bulk-send-application.cc \
../ns-3/src/applications/model/onoff-application.cc \
../ns-3/src/applications/model/packet-loss-counter.cc \
../ns-3/src/applications/model/packet-sink.cc \
../ns-3/src/applications/model/ping6.cc \
../ns-3/src/applications/model/radvd-interface.cc \
../ns-3/src/applications/model/radvd-prefix.cc \
../ns-3/src/applications/model/radvd.cc \
../ns-3/src/applications/model/seq-ts-header.cc \
../ns-3/src/applications/model/udp-client.cc \
../ns-3/src/applications/model/udp-echo-client.cc \
../ns-3/src/applications/model/udp-echo-server.cc \
../ns-3/src/applications/model/udp-server.cc \
../ns-3/src/applications/model/udp-trace-client.cc \
../ns-3/src/applications/model/v4ping.cc 

OBJS += \
./ns-3/src/applications/model/application-packet-probe.o \
./ns-3/src/applications/model/bulk-send-application.o \
./ns-3/src/applications/model/onoff-application.o \
./ns-3/src/applications/model/packet-loss-counter.o \
./ns-3/src/applications/model/packet-sink.o \
./ns-3/src/applications/model/ping6.o \
./ns-3/src/applications/model/radvd-interface.o \
./ns-3/src/applications/model/radvd-prefix.o \
./ns-3/src/applications/model/radvd.o \
./ns-3/src/applications/model/seq-ts-header.o \
./ns-3/src/applications/model/udp-client.o \
./ns-3/src/applications/model/udp-echo-client.o \
./ns-3/src/applications/model/udp-echo-server.o \
./ns-3/src/applications/model/udp-server.o \
./ns-3/src/applications/model/udp-trace-client.o \
./ns-3/src/applications/model/v4ping.o 

CC_DEPS += \
./ns-3/src/applications/model/application-packet-probe.d \
./ns-3/src/applications/model/bulk-send-application.d \
./ns-3/src/applications/model/onoff-application.d \
./ns-3/src/applications/model/packet-loss-counter.d \
./ns-3/src/applications/model/packet-sink.d \
./ns-3/src/applications/model/ping6.d \
./ns-3/src/applications/model/radvd-interface.d \
./ns-3/src/applications/model/radvd-prefix.d \
./ns-3/src/applications/model/radvd.d \
./ns-3/src/applications/model/seq-ts-header.d \
./ns-3/src/applications/model/udp-client.d \
./ns-3/src/applications/model/udp-echo-client.d \
./ns-3/src/applications/model/udp-echo-server.d \
./ns-3/src/applications/model/udp-server.d \
./ns-3/src/applications/model/udp-trace-client.d \
./ns-3/src/applications/model/v4ping.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/applications/model/%.o: ../ns-3/src/applications/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


