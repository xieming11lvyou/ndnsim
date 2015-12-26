################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/topology-read/model/brite-topology-reader.cc \
../ns-3/src/topology-read/model/inet-topology-reader.cc \
../ns-3/src/topology-read/model/orbis-topology-reader.cc \
../ns-3/src/topology-read/model/rocketfuel-topology-reader.cc \
../ns-3/src/topology-read/model/topology-reader.cc 

OBJS += \
./ns-3/src/topology-read/model/brite-topology-reader.o \
./ns-3/src/topology-read/model/inet-topology-reader.o \
./ns-3/src/topology-read/model/orbis-topology-reader.o \
./ns-3/src/topology-read/model/rocketfuel-topology-reader.o \
./ns-3/src/topology-read/model/topology-reader.o 

CC_DEPS += \
./ns-3/src/topology-read/model/brite-topology-reader.d \
./ns-3/src/topology-read/model/inet-topology-reader.d \
./ns-3/src/topology-read/model/orbis-topology-reader.d \
./ns-3/src/topology-read/model/rocketfuel-topology-reader.d \
./ns-3/src/topology-read/model/topology-reader.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/topology-read/model/%.o: ../ns-3/src/topology-read/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


