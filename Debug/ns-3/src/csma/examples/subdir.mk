################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/csma/examples/csma-broadcast.cc \
../ns-3/src/csma/examples/csma-multicast.cc \
../ns-3/src/csma/examples/csma-one-subnet.cc \
../ns-3/src/csma/examples/csma-packet-socket.cc \
../ns-3/src/csma/examples/csma-ping.cc \
../ns-3/src/csma/examples/csma-raw-ip-socket.cc 

OBJS += \
./ns-3/src/csma/examples/csma-broadcast.o \
./ns-3/src/csma/examples/csma-multicast.o \
./ns-3/src/csma/examples/csma-one-subnet.o \
./ns-3/src/csma/examples/csma-packet-socket.o \
./ns-3/src/csma/examples/csma-ping.o \
./ns-3/src/csma/examples/csma-raw-ip-socket.o 

CC_DEPS += \
./ns-3/src/csma/examples/csma-broadcast.d \
./ns-3/src/csma/examples/csma-multicast.d \
./ns-3/src/csma/examples/csma-one-subnet.d \
./ns-3/src/csma/examples/csma-packet-socket.d \
./ns-3/src/csma/examples/csma-ping.d \
./ns-3/src/csma/examples/csma-raw-ip-socket.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/csma/examples/%.o: ../ns-3/src/csma/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


