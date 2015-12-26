################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/buildings/model/building-list.cc \
../ns-3/src/buildings/model/building.cc \
../ns-3/src/buildings/model/buildings-propagation-loss-model.cc \
../ns-3/src/buildings/model/hybrid-buildings-propagation-loss-model.cc \
../ns-3/src/buildings/model/itu-r-1238-propagation-loss-model.cc \
../ns-3/src/buildings/model/mobility-building-info.cc \
../ns-3/src/buildings/model/oh-buildings-propagation-loss-model.cc 

OBJS += \
./ns-3/src/buildings/model/building-list.o \
./ns-3/src/buildings/model/building.o \
./ns-3/src/buildings/model/buildings-propagation-loss-model.o \
./ns-3/src/buildings/model/hybrid-buildings-propagation-loss-model.o \
./ns-3/src/buildings/model/itu-r-1238-propagation-loss-model.o \
./ns-3/src/buildings/model/mobility-building-info.o \
./ns-3/src/buildings/model/oh-buildings-propagation-loss-model.o 

CC_DEPS += \
./ns-3/src/buildings/model/building-list.d \
./ns-3/src/buildings/model/building.d \
./ns-3/src/buildings/model/buildings-propagation-loss-model.d \
./ns-3/src/buildings/model/hybrid-buildings-propagation-loss-model.d \
./ns-3/src/buildings/model/itu-r-1238-propagation-loss-model.d \
./ns-3/src/buildings/model/mobility-building-info.d \
./ns-3/src/buildings/model/oh-buildings-propagation-loss-model.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/buildings/model/%.o: ../ns-3/src/buildings/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


