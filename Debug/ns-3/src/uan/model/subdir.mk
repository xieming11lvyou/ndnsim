################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/uan/model/acoustic-modem-energy-model.cc \
../ns-3/src/uan/model/uan-address.cc \
../ns-3/src/uan/model/uan-channel.cc \
../ns-3/src/uan/model/uan-header-common.cc \
../ns-3/src/uan/model/uan-header-rc.cc \
../ns-3/src/uan/model/uan-mac-aloha.cc \
../ns-3/src/uan/model/uan-mac-cw.cc \
../ns-3/src/uan/model/uan-mac-rc-gw.cc \
../ns-3/src/uan/model/uan-mac-rc.cc \
../ns-3/src/uan/model/uan-mac.cc \
../ns-3/src/uan/model/uan-net-device.cc \
../ns-3/src/uan/model/uan-noise-model-default.cc \
../ns-3/src/uan/model/uan-noise-model.cc \
../ns-3/src/uan/model/uan-phy-dual.cc \
../ns-3/src/uan/model/uan-phy-gen.cc \
../ns-3/src/uan/model/uan-phy.cc \
../ns-3/src/uan/model/uan-prop-model-ideal.cc \
../ns-3/src/uan/model/uan-prop-model-thorp.cc \
../ns-3/src/uan/model/uan-prop-model.cc \
../ns-3/src/uan/model/uan-transducer-hd.cc \
../ns-3/src/uan/model/uan-transducer.cc \
../ns-3/src/uan/model/uan-tx-mode.cc 

OBJS += \
./ns-3/src/uan/model/acoustic-modem-energy-model.o \
./ns-3/src/uan/model/uan-address.o \
./ns-3/src/uan/model/uan-channel.o \
./ns-3/src/uan/model/uan-header-common.o \
./ns-3/src/uan/model/uan-header-rc.o \
./ns-3/src/uan/model/uan-mac-aloha.o \
./ns-3/src/uan/model/uan-mac-cw.o \
./ns-3/src/uan/model/uan-mac-rc-gw.o \
./ns-3/src/uan/model/uan-mac-rc.o \
./ns-3/src/uan/model/uan-mac.o \
./ns-3/src/uan/model/uan-net-device.o \
./ns-3/src/uan/model/uan-noise-model-default.o \
./ns-3/src/uan/model/uan-noise-model.o \
./ns-3/src/uan/model/uan-phy-dual.o \
./ns-3/src/uan/model/uan-phy-gen.o \
./ns-3/src/uan/model/uan-phy.o \
./ns-3/src/uan/model/uan-prop-model-ideal.o \
./ns-3/src/uan/model/uan-prop-model-thorp.o \
./ns-3/src/uan/model/uan-prop-model.o \
./ns-3/src/uan/model/uan-transducer-hd.o \
./ns-3/src/uan/model/uan-transducer.o \
./ns-3/src/uan/model/uan-tx-mode.o 

CC_DEPS += \
./ns-3/src/uan/model/acoustic-modem-energy-model.d \
./ns-3/src/uan/model/uan-address.d \
./ns-3/src/uan/model/uan-channel.d \
./ns-3/src/uan/model/uan-header-common.d \
./ns-3/src/uan/model/uan-header-rc.d \
./ns-3/src/uan/model/uan-mac-aloha.d \
./ns-3/src/uan/model/uan-mac-cw.d \
./ns-3/src/uan/model/uan-mac-rc-gw.d \
./ns-3/src/uan/model/uan-mac-rc.d \
./ns-3/src/uan/model/uan-mac.d \
./ns-3/src/uan/model/uan-net-device.d \
./ns-3/src/uan/model/uan-noise-model-default.d \
./ns-3/src/uan/model/uan-noise-model.d \
./ns-3/src/uan/model/uan-phy-dual.d \
./ns-3/src/uan/model/uan-phy-gen.d \
./ns-3/src/uan/model/uan-phy.d \
./ns-3/src/uan/model/uan-prop-model-ideal.d \
./ns-3/src/uan/model/uan-prop-model-thorp.d \
./ns-3/src/uan/model/uan-prop-model.d \
./ns-3/src/uan/model/uan-transducer-hd.d \
./ns-3/src/uan/model/uan-transducer.d \
./ns-3/src/uan/model/uan-tx-mode.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/uan/model/%.o: ../ns-3/src/uan/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


