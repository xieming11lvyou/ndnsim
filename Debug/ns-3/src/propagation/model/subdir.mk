################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/propagation/model/cost231-propagation-loss-model.cc \
../ns-3/src/propagation/model/itu-r-1411-los-propagation-loss-model.cc \
../ns-3/src/propagation/model/itu-r-1411-nlos-over-rooftop-propagation-loss-model.cc \
../ns-3/src/propagation/model/jakes-process.cc \
../ns-3/src/propagation/model/jakes-propagation-loss-model.cc \
../ns-3/src/propagation/model/kun-2600-mhz-propagation-loss-model.cc \
../ns-3/src/propagation/model/okumura-hata-propagation-loss-model.cc \
../ns-3/src/propagation/model/propagation-delay-model.cc \
../ns-3/src/propagation/model/propagation-loss-model.cc 

OBJS += \
./ns-3/src/propagation/model/cost231-propagation-loss-model.o \
./ns-3/src/propagation/model/itu-r-1411-los-propagation-loss-model.o \
./ns-3/src/propagation/model/itu-r-1411-nlos-over-rooftop-propagation-loss-model.o \
./ns-3/src/propagation/model/jakes-process.o \
./ns-3/src/propagation/model/jakes-propagation-loss-model.o \
./ns-3/src/propagation/model/kun-2600-mhz-propagation-loss-model.o \
./ns-3/src/propagation/model/okumura-hata-propagation-loss-model.o \
./ns-3/src/propagation/model/propagation-delay-model.o \
./ns-3/src/propagation/model/propagation-loss-model.o 

CC_DEPS += \
./ns-3/src/propagation/model/cost231-propagation-loss-model.d \
./ns-3/src/propagation/model/itu-r-1411-los-propagation-loss-model.d \
./ns-3/src/propagation/model/itu-r-1411-nlos-over-rooftop-propagation-loss-model.d \
./ns-3/src/propagation/model/jakes-process.d \
./ns-3/src/propagation/model/jakes-propagation-loss-model.d \
./ns-3/src/propagation/model/kun-2600-mhz-propagation-loss-model.d \
./ns-3/src/propagation/model/okumura-hata-propagation-loss-model.d \
./ns-3/src/propagation/model/propagation-delay-model.d \
./ns-3/src/propagation/model/propagation-loss-model.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/propagation/model/%.o: ../ns-3/src/propagation/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


