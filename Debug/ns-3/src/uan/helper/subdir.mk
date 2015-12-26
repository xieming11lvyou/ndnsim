################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/uan/helper/acoustic-modem-energy-model-helper.cc \
../ns-3/src/uan/helper/uan-helper.cc 

OBJS += \
./ns-3/src/uan/helper/acoustic-modem-energy-model-helper.o \
./ns-3/src/uan/helper/uan-helper.o 

CC_DEPS += \
./ns-3/src/uan/helper/acoustic-modem-energy-model-helper.d \
./ns-3/src/uan/helper/uan-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/uan/helper/%.o: ../ns-3/src/uan/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


