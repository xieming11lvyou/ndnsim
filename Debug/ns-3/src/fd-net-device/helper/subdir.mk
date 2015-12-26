################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/fd-net-device/helper/creator-utils.cc \
../ns-3/src/fd-net-device/helper/emu-fd-net-device-helper.cc \
../ns-3/src/fd-net-device/helper/encode-decode.cc \
../ns-3/src/fd-net-device/helper/fd-net-device-helper.cc \
../ns-3/src/fd-net-device/helper/planetlab-fd-net-device-helper.cc \
../ns-3/src/fd-net-device/helper/planetlab-tap-creator.cc \
../ns-3/src/fd-net-device/helper/raw-sock-creator.cc \
../ns-3/src/fd-net-device/helper/tap-device-creator.cc \
../ns-3/src/fd-net-device/helper/tap-fd-net-device-helper.cc 

OBJS += \
./ns-3/src/fd-net-device/helper/creator-utils.o \
./ns-3/src/fd-net-device/helper/emu-fd-net-device-helper.o \
./ns-3/src/fd-net-device/helper/encode-decode.o \
./ns-3/src/fd-net-device/helper/fd-net-device-helper.o \
./ns-3/src/fd-net-device/helper/planetlab-fd-net-device-helper.o \
./ns-3/src/fd-net-device/helper/planetlab-tap-creator.o \
./ns-3/src/fd-net-device/helper/raw-sock-creator.o \
./ns-3/src/fd-net-device/helper/tap-device-creator.o \
./ns-3/src/fd-net-device/helper/tap-fd-net-device-helper.o 

CC_DEPS += \
./ns-3/src/fd-net-device/helper/creator-utils.d \
./ns-3/src/fd-net-device/helper/emu-fd-net-device-helper.d \
./ns-3/src/fd-net-device/helper/encode-decode.d \
./ns-3/src/fd-net-device/helper/fd-net-device-helper.d \
./ns-3/src/fd-net-device/helper/planetlab-fd-net-device-helper.d \
./ns-3/src/fd-net-device/helper/planetlab-tap-creator.d \
./ns-3/src/fd-net-device/helper/raw-sock-creator.d \
./ns-3/src/fd-net-device/helper/tap-device-creator.d \
./ns-3/src/fd-net-device/helper/tap-fd-net-device-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/fd-net-device/helper/%.o: ../ns-3/src/fd-net-device/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


