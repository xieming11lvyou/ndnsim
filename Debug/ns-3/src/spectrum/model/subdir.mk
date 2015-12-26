################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/spectrum/model/aloha-noack-mac-header.cc \
../ns-3/src/spectrum/model/aloha-noack-net-device.cc \
../ns-3/src/spectrum/model/constant-spectrum-propagation-loss.cc \
../ns-3/src/spectrum/model/friis-spectrum-propagation-loss.cc \
../ns-3/src/spectrum/model/half-duplex-ideal-phy-signal-parameters.cc \
../ns-3/src/spectrum/model/half-duplex-ideal-phy.cc \
../ns-3/src/spectrum/model/microwave-oven-spectrum-value-helper.cc \
../ns-3/src/spectrum/model/multi-model-spectrum-channel.cc \
../ns-3/src/spectrum/model/non-communicating-net-device.cc \
../ns-3/src/spectrum/model/single-model-spectrum-channel.cc \
../ns-3/src/spectrum/model/spectrum-analyzer.cc \
../ns-3/src/spectrum/model/spectrum-channel.cc \
../ns-3/src/spectrum/model/spectrum-converter.cc \
../ns-3/src/spectrum/model/spectrum-error-model.cc \
../ns-3/src/spectrum/model/spectrum-interference.cc \
../ns-3/src/spectrum/model/spectrum-model-300kHz-300GHz-log.cc \
../ns-3/src/spectrum/model/spectrum-model-ism2400MHz-res1MHz.cc \
../ns-3/src/spectrum/model/spectrum-model.cc \
../ns-3/src/spectrum/model/spectrum-phy.cc \
../ns-3/src/spectrum/model/spectrum-propagation-loss-model.cc \
../ns-3/src/spectrum/model/spectrum-signal-parameters.cc \
../ns-3/src/spectrum/model/spectrum-value.cc \
../ns-3/src/spectrum/model/waveform-generator.cc \
../ns-3/src/spectrum/model/wifi-spectrum-value-helper.cc 

OBJS += \
./ns-3/src/spectrum/model/aloha-noack-mac-header.o \
./ns-3/src/spectrum/model/aloha-noack-net-device.o \
./ns-3/src/spectrum/model/constant-spectrum-propagation-loss.o \
./ns-3/src/spectrum/model/friis-spectrum-propagation-loss.o \
./ns-3/src/spectrum/model/half-duplex-ideal-phy-signal-parameters.o \
./ns-3/src/spectrum/model/half-duplex-ideal-phy.o \
./ns-3/src/spectrum/model/microwave-oven-spectrum-value-helper.o \
./ns-3/src/spectrum/model/multi-model-spectrum-channel.o \
./ns-3/src/spectrum/model/non-communicating-net-device.o \
./ns-3/src/spectrum/model/single-model-spectrum-channel.o \
./ns-3/src/spectrum/model/spectrum-analyzer.o \
./ns-3/src/spectrum/model/spectrum-channel.o \
./ns-3/src/spectrum/model/spectrum-converter.o \
./ns-3/src/spectrum/model/spectrum-error-model.o \
./ns-3/src/spectrum/model/spectrum-interference.o \
./ns-3/src/spectrum/model/spectrum-model-300kHz-300GHz-log.o \
./ns-3/src/spectrum/model/spectrum-model-ism2400MHz-res1MHz.o \
./ns-3/src/spectrum/model/spectrum-model.o \
./ns-3/src/spectrum/model/spectrum-phy.o \
./ns-3/src/spectrum/model/spectrum-propagation-loss-model.o \
./ns-3/src/spectrum/model/spectrum-signal-parameters.o \
./ns-3/src/spectrum/model/spectrum-value.o \
./ns-3/src/spectrum/model/waveform-generator.o \
./ns-3/src/spectrum/model/wifi-spectrum-value-helper.o 

CC_DEPS += \
./ns-3/src/spectrum/model/aloha-noack-mac-header.d \
./ns-3/src/spectrum/model/aloha-noack-net-device.d \
./ns-3/src/spectrum/model/constant-spectrum-propagation-loss.d \
./ns-3/src/spectrum/model/friis-spectrum-propagation-loss.d \
./ns-3/src/spectrum/model/half-duplex-ideal-phy-signal-parameters.d \
./ns-3/src/spectrum/model/half-duplex-ideal-phy.d \
./ns-3/src/spectrum/model/microwave-oven-spectrum-value-helper.d \
./ns-3/src/spectrum/model/multi-model-spectrum-channel.d \
./ns-3/src/spectrum/model/non-communicating-net-device.d \
./ns-3/src/spectrum/model/single-model-spectrum-channel.d \
./ns-3/src/spectrum/model/spectrum-analyzer.d \
./ns-3/src/spectrum/model/spectrum-channel.d \
./ns-3/src/spectrum/model/spectrum-converter.d \
./ns-3/src/spectrum/model/spectrum-error-model.d \
./ns-3/src/spectrum/model/spectrum-interference.d \
./ns-3/src/spectrum/model/spectrum-model-300kHz-300GHz-log.d \
./ns-3/src/spectrum/model/spectrum-model-ism2400MHz-res1MHz.d \
./ns-3/src/spectrum/model/spectrum-model.d \
./ns-3/src/spectrum/model/spectrum-phy.d \
./ns-3/src/spectrum/model/spectrum-propagation-loss-model.d \
./ns-3/src/spectrum/model/spectrum-signal-parameters.d \
./ns-3/src/spectrum/model/spectrum-value.d \
./ns-3/src/spectrum/model/waveform-generator.d \
./ns-3/src/spectrum/model/wifi-spectrum-value-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/spectrum/model/%.o: ../ns-3/src/spectrum/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


