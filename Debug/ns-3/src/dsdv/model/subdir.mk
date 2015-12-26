################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/dsdv/model/dsdv-packet-queue.cc \
../ns-3/src/dsdv/model/dsdv-packet.cc \
../ns-3/src/dsdv/model/dsdv-routing-protocol.cc \
../ns-3/src/dsdv/model/dsdv-rtable.cc 

OBJS += \
./ns-3/src/dsdv/model/dsdv-packet-queue.o \
./ns-3/src/dsdv/model/dsdv-packet.o \
./ns-3/src/dsdv/model/dsdv-routing-protocol.o \
./ns-3/src/dsdv/model/dsdv-rtable.o 

CC_DEPS += \
./ns-3/src/dsdv/model/dsdv-packet-queue.d \
./ns-3/src/dsdv/model/dsdv-packet.d \
./ns-3/src/dsdv/model/dsdv-routing-protocol.d \
./ns-3/src/dsdv/model/dsdv-rtable.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/dsdv/model/%.o: ../ns-3/src/dsdv/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


