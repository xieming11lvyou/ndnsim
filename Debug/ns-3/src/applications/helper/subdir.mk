################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/applications/helper/bulk-send-helper.cc \
../ns-3/src/applications/helper/on-off-helper.cc \
../ns-3/src/applications/helper/packet-sink-helper.cc \
../ns-3/src/applications/helper/ping6-helper.cc \
../ns-3/src/applications/helper/radvd-helper.cc \
../ns-3/src/applications/helper/udp-client-server-helper.cc \
../ns-3/src/applications/helper/udp-echo-helper.cc \
../ns-3/src/applications/helper/v4ping-helper.cc 

OBJS += \
./ns-3/src/applications/helper/bulk-send-helper.o \
./ns-3/src/applications/helper/on-off-helper.o \
./ns-3/src/applications/helper/packet-sink-helper.o \
./ns-3/src/applications/helper/ping6-helper.o \
./ns-3/src/applications/helper/radvd-helper.o \
./ns-3/src/applications/helper/udp-client-server-helper.o \
./ns-3/src/applications/helper/udp-echo-helper.o \
./ns-3/src/applications/helper/v4ping-helper.o 

CC_DEPS += \
./ns-3/src/applications/helper/bulk-send-helper.d \
./ns-3/src/applications/helper/on-off-helper.d \
./ns-3/src/applications/helper/packet-sink-helper.d \
./ns-3/src/applications/helper/ping6-helper.d \
./ns-3/src/applications/helper/radvd-helper.d \
./ns-3/src/applications/helper/udp-client-server-helper.d \
./ns-3/src/applications/helper/udp-echo-helper.d \
./ns-3/src/applications/helper/v4ping-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/applications/helper/%.o: ../ns-3/src/applications/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


