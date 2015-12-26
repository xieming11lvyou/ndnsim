################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/energy/model/basic-energy-harvester.cc \
../ns-3/src/energy/model/basic-energy-source.cc \
../ns-3/src/energy/model/device-energy-model-container.cc \
../ns-3/src/energy/model/device-energy-model.cc \
../ns-3/src/energy/model/energy-harvester.cc \
../ns-3/src/energy/model/energy-source.cc \
../ns-3/src/energy/model/li-ion-energy-source.cc \
../ns-3/src/energy/model/rv-battery-model.cc \
../ns-3/src/energy/model/simple-device-energy-model.cc \
../ns-3/src/energy/model/wifi-radio-energy-model.cc \
../ns-3/src/energy/model/wifi-tx-current-model.cc 

OBJS += \
./ns-3/src/energy/model/basic-energy-harvester.o \
./ns-3/src/energy/model/basic-energy-source.o \
./ns-3/src/energy/model/device-energy-model-container.o \
./ns-3/src/energy/model/device-energy-model.o \
./ns-3/src/energy/model/energy-harvester.o \
./ns-3/src/energy/model/energy-source.o \
./ns-3/src/energy/model/li-ion-energy-source.o \
./ns-3/src/energy/model/rv-battery-model.o \
./ns-3/src/energy/model/simple-device-energy-model.o \
./ns-3/src/energy/model/wifi-radio-energy-model.o \
./ns-3/src/energy/model/wifi-tx-current-model.o 

CC_DEPS += \
./ns-3/src/energy/model/basic-energy-harvester.d \
./ns-3/src/energy/model/basic-energy-source.d \
./ns-3/src/energy/model/device-energy-model-container.d \
./ns-3/src/energy/model/device-energy-model.d \
./ns-3/src/energy/model/energy-harvester.d \
./ns-3/src/energy/model/energy-source.d \
./ns-3/src/energy/model/li-ion-energy-source.d \
./ns-3/src/energy/model/rv-battery-model.d \
./ns-3/src/energy/model/simple-device-energy-model.d \
./ns-3/src/energy/model/wifi-radio-energy-model.d \
./ns-3/src/energy/model/wifi-tx-current-model.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/energy/model/%.o: ../ns-3/src/energy/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


