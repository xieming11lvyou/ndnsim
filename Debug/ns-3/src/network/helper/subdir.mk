################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/network/helper/application-container.cc \
../ns-3/src/network/helper/delay-jitter-estimation.cc \
../ns-3/src/network/helper/net-device-container.cc \
../ns-3/src/network/helper/node-container.cc \
../ns-3/src/network/helper/packet-socket-helper.cc \
../ns-3/src/network/helper/simple-net-device-helper.cc \
../ns-3/src/network/helper/trace-helper.cc 

OBJS += \
./ns-3/src/network/helper/application-container.o \
./ns-3/src/network/helper/delay-jitter-estimation.o \
./ns-3/src/network/helper/net-device-container.o \
./ns-3/src/network/helper/node-container.o \
./ns-3/src/network/helper/packet-socket-helper.o \
./ns-3/src/network/helper/simple-net-device-helper.o \
./ns-3/src/network/helper/trace-helper.o 

CC_DEPS += \
./ns-3/src/network/helper/application-container.d \
./ns-3/src/network/helper/delay-jitter-estimation.d \
./ns-3/src/network/helper/net-device-container.d \
./ns-3/src/network/helper/node-container.d \
./ns-3/src/network/helper/packet-socket-helper.d \
./ns-3/src/network/helper/simple-net-device-helper.d \
./ns-3/src/network/helper/trace-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/network/helper/%.o: ../ns-3/src/network/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


