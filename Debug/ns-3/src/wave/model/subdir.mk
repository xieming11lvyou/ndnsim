################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/wave/model/higher-tx-tag.cc \
../ns-3/src/wave/model/ocb-wifi-mac.cc \
../ns-3/src/wave/model/vendor-specific-action.cc \
../ns-3/src/wave/model/wave-mac-low.cc 

OBJS += \
./ns-3/src/wave/model/higher-tx-tag.o \
./ns-3/src/wave/model/ocb-wifi-mac.o \
./ns-3/src/wave/model/vendor-specific-action.o \
./ns-3/src/wave/model/wave-mac-low.o 

CC_DEPS += \
./ns-3/src/wave/model/higher-tx-tag.d \
./ns-3/src/wave/model/ocb-wifi-mac.d \
./ns-3/src/wave/model/vendor-specific-action.d \
./ns-3/src/wave/model/wave-mac-low.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/wave/model/%.o: ../ns-3/src/wave/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


