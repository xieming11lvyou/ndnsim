################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/wifi/helper/athstats-helper.cc \
../ns-3/src/wifi/helper/ht-wifi-mac-helper.cc \
../ns-3/src/wifi/helper/nqos-wifi-mac-helper.cc \
../ns-3/src/wifi/helper/qos-wifi-mac-helper.cc \
../ns-3/src/wifi/helper/wifi-helper.cc \
../ns-3/src/wifi/helper/yans-wifi-helper.cc 

OBJS += \
./ns-3/src/wifi/helper/athstats-helper.o \
./ns-3/src/wifi/helper/ht-wifi-mac-helper.o \
./ns-3/src/wifi/helper/nqos-wifi-mac-helper.o \
./ns-3/src/wifi/helper/qos-wifi-mac-helper.o \
./ns-3/src/wifi/helper/wifi-helper.o \
./ns-3/src/wifi/helper/yans-wifi-helper.o 

CC_DEPS += \
./ns-3/src/wifi/helper/athstats-helper.d \
./ns-3/src/wifi/helper/ht-wifi-mac-helper.d \
./ns-3/src/wifi/helper/nqos-wifi-mac-helper.d \
./ns-3/src/wifi/helper/qos-wifi-mac-helper.d \
./ns-3/src/wifi/helper/wifi-helper.d \
./ns-3/src/wifi/helper/yans-wifi-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/wifi/helper/%.o: ../ns-3/src/wifi/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


