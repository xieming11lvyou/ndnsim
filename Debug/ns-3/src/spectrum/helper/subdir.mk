################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/spectrum/helper/adhoc-aloha-noack-ideal-phy-helper.cc \
../ns-3/src/spectrum/helper/spectrum-analyzer-helper.cc \
../ns-3/src/spectrum/helper/spectrum-helper.cc \
../ns-3/src/spectrum/helper/waveform-generator-helper.cc 

OBJS += \
./ns-3/src/spectrum/helper/adhoc-aloha-noack-ideal-phy-helper.o \
./ns-3/src/spectrum/helper/spectrum-analyzer-helper.o \
./ns-3/src/spectrum/helper/spectrum-helper.o \
./ns-3/src/spectrum/helper/waveform-generator-helper.o 

CC_DEPS += \
./ns-3/src/spectrum/helper/adhoc-aloha-noack-ideal-phy-helper.d \
./ns-3/src/spectrum/helper/spectrum-analyzer-helper.d \
./ns-3/src/spectrum/helper/spectrum-helper.d \
./ns-3/src/spectrum/helper/waveform-generator-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/spectrum/helper/%.o: ../ns-3/src/spectrum/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


