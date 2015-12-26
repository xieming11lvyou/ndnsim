################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/energy/test/basic-energy-harvester-test.cc \
../ns-3/src/energy/test/basic-energy-model-test.cc \
../ns-3/src/energy/test/li-ion-energy-source-test.cc \
../ns-3/src/energy/test/rv-battery-model-test.cc 

OBJS += \
./ns-3/src/energy/test/basic-energy-harvester-test.o \
./ns-3/src/energy/test/basic-energy-model-test.o \
./ns-3/src/energy/test/li-ion-energy-source-test.o \
./ns-3/src/energy/test/rv-battery-model-test.o 

CC_DEPS += \
./ns-3/src/energy/test/basic-energy-harvester-test.d \
./ns-3/src/energy/test/basic-energy-model-test.d \
./ns-3/src/energy/test/li-ion-energy-source-test.d \
./ns-3/src/energy/test/rv-battery-model-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/energy/test/%.o: ../ns-3/src/energy/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


