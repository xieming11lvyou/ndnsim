################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/fd-net-device/examples/dummy-network.cc \
../ns-3/src/fd-net-device/examples/fd-emu-onoff.cc \
../ns-3/src/fd-net-device/examples/fd-emu-ping.cc \
../ns-3/src/fd-net-device/examples/fd-emu-udp-echo.cc \
../ns-3/src/fd-net-device/examples/fd-planetlab-ping.cc \
../ns-3/src/fd-net-device/examples/fd-tap-ping.cc \
../ns-3/src/fd-net-device/examples/fd-tap-ping6.cc \
../ns-3/src/fd-net-device/examples/fd2fd-onoff.cc \
../ns-3/src/fd-net-device/examples/realtime-dummy-network.cc \
../ns-3/src/fd-net-device/examples/realtime-fd2fd-onoff.cc 

OBJS += \
./ns-3/src/fd-net-device/examples/dummy-network.o \
./ns-3/src/fd-net-device/examples/fd-emu-onoff.o \
./ns-3/src/fd-net-device/examples/fd-emu-ping.o \
./ns-3/src/fd-net-device/examples/fd-emu-udp-echo.o \
./ns-3/src/fd-net-device/examples/fd-planetlab-ping.o \
./ns-3/src/fd-net-device/examples/fd-tap-ping.o \
./ns-3/src/fd-net-device/examples/fd-tap-ping6.o \
./ns-3/src/fd-net-device/examples/fd2fd-onoff.o \
./ns-3/src/fd-net-device/examples/realtime-dummy-network.o \
./ns-3/src/fd-net-device/examples/realtime-fd2fd-onoff.o 

CC_DEPS += \
./ns-3/src/fd-net-device/examples/dummy-network.d \
./ns-3/src/fd-net-device/examples/fd-emu-onoff.d \
./ns-3/src/fd-net-device/examples/fd-emu-ping.d \
./ns-3/src/fd-net-device/examples/fd-emu-udp-echo.d \
./ns-3/src/fd-net-device/examples/fd-planetlab-ping.d \
./ns-3/src/fd-net-device/examples/fd-tap-ping.d \
./ns-3/src/fd-net-device/examples/fd-tap-ping6.d \
./ns-3/src/fd-net-device/examples/fd2fd-onoff.d \
./ns-3/src/fd-net-device/examples/realtime-dummy-network.d \
./ns-3/src/fd-net-device/examples/realtime-fd2fd-onoff.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/fd-net-device/examples/%.o: ../ns-3/src/fd-net-device/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


