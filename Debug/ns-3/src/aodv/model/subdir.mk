################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/aodv/model/aodv-dpd.cc \
../ns-3/src/aodv/model/aodv-id-cache.cc \
../ns-3/src/aodv/model/aodv-neighbor.cc \
../ns-3/src/aodv/model/aodv-packet.cc \
../ns-3/src/aodv/model/aodv-routing-protocol.cc \
../ns-3/src/aodv/model/aodv-rqueue.cc \
../ns-3/src/aodv/model/aodv-rtable.cc 

OBJS += \
./ns-3/src/aodv/model/aodv-dpd.o \
./ns-3/src/aodv/model/aodv-id-cache.o \
./ns-3/src/aodv/model/aodv-neighbor.o \
./ns-3/src/aodv/model/aodv-packet.o \
./ns-3/src/aodv/model/aodv-routing-protocol.o \
./ns-3/src/aodv/model/aodv-rqueue.o \
./ns-3/src/aodv/model/aodv-rtable.o 

CC_DEPS += \
./ns-3/src/aodv/model/aodv-dpd.d \
./ns-3/src/aodv/model/aodv-id-cache.d \
./ns-3/src/aodv/model/aodv-neighbor.d \
./ns-3/src/aodv/model/aodv-packet.d \
./ns-3/src/aodv/model/aodv-routing-protocol.d \
./ns-3/src/aodv/model/aodv-rqueue.d \
./ns-3/src/aodv/model/aodv-rtable.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/aodv/model/%.o: ../ns-3/src/aodv/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


