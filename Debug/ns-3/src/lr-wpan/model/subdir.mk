################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/lr-wpan/model/lr-wpan-csmaca.cc \
../ns-3/src/lr-wpan/model/lr-wpan-error-model.cc \
../ns-3/src/lr-wpan/model/lr-wpan-interference-helper.cc \
../ns-3/src/lr-wpan/model/lr-wpan-lqi-tag.cc \
../ns-3/src/lr-wpan/model/lr-wpan-mac-header.cc \
../ns-3/src/lr-wpan/model/lr-wpan-mac-trailer.cc \
../ns-3/src/lr-wpan/model/lr-wpan-mac.cc \
../ns-3/src/lr-wpan/model/lr-wpan-net-device.cc \
../ns-3/src/lr-wpan/model/lr-wpan-phy.cc \
../ns-3/src/lr-wpan/model/lr-wpan-spectrum-signal-parameters.cc \
../ns-3/src/lr-wpan/model/lr-wpan-spectrum-value-helper.cc 

OBJS += \
./ns-3/src/lr-wpan/model/lr-wpan-csmaca.o \
./ns-3/src/lr-wpan/model/lr-wpan-error-model.o \
./ns-3/src/lr-wpan/model/lr-wpan-interference-helper.o \
./ns-3/src/lr-wpan/model/lr-wpan-lqi-tag.o \
./ns-3/src/lr-wpan/model/lr-wpan-mac-header.o \
./ns-3/src/lr-wpan/model/lr-wpan-mac-trailer.o \
./ns-3/src/lr-wpan/model/lr-wpan-mac.o \
./ns-3/src/lr-wpan/model/lr-wpan-net-device.o \
./ns-3/src/lr-wpan/model/lr-wpan-phy.o \
./ns-3/src/lr-wpan/model/lr-wpan-spectrum-signal-parameters.o \
./ns-3/src/lr-wpan/model/lr-wpan-spectrum-value-helper.o 

CC_DEPS += \
./ns-3/src/lr-wpan/model/lr-wpan-csmaca.d \
./ns-3/src/lr-wpan/model/lr-wpan-error-model.d \
./ns-3/src/lr-wpan/model/lr-wpan-interference-helper.d \
./ns-3/src/lr-wpan/model/lr-wpan-lqi-tag.d \
./ns-3/src/lr-wpan/model/lr-wpan-mac-header.d \
./ns-3/src/lr-wpan/model/lr-wpan-mac-trailer.d \
./ns-3/src/lr-wpan/model/lr-wpan-mac.d \
./ns-3/src/lr-wpan/model/lr-wpan-net-device.d \
./ns-3/src/lr-wpan/model/lr-wpan-phy.d \
./ns-3/src/lr-wpan/model/lr-wpan-spectrum-signal-parameters.d \
./ns-3/src/lr-wpan/model/lr-wpan-spectrum-value-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/lr-wpan/model/%.o: ../ns-3/src/lr-wpan/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


