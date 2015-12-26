################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/network/examples/droptail_vs_red.cc \
../ns-3/src/network/examples/main-packet-header.cc \
../ns-3/src/network/examples/main-packet-tag.cc \
../ns-3/src/network/examples/packet-socket-apps.cc \
../ns-3/src/network/examples/red-tests.cc 

OBJS += \
./ns-3/src/network/examples/droptail_vs_red.o \
./ns-3/src/network/examples/main-packet-header.o \
./ns-3/src/network/examples/main-packet-tag.o \
./ns-3/src/network/examples/packet-socket-apps.o \
./ns-3/src/network/examples/red-tests.o 

CC_DEPS += \
./ns-3/src/network/examples/droptail_vs_red.d \
./ns-3/src/network/examples/main-packet-header.d \
./ns-3/src/network/examples/main-packet-tag.d \
./ns-3/src/network/examples/packet-socket-apps.d \
./ns-3/src/network/examples/red-tests.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/network/examples/%.o: ../ns-3/src/network/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


