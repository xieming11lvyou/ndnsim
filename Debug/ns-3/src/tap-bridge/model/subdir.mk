################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/tap-bridge/model/tap-bridge.cc \
../ns-3/src/tap-bridge/model/tap-creator.cc \
../ns-3/src/tap-bridge/model/tap-encode-decode.cc 

OBJS += \
./ns-3/src/tap-bridge/model/tap-bridge.o \
./ns-3/src/tap-bridge/model/tap-creator.o \
./ns-3/src/tap-bridge/model/tap-encode-decode.o 

CC_DEPS += \
./ns-3/src/tap-bridge/model/tap-bridge.d \
./ns-3/src/tap-bridge/model/tap-creator.d \
./ns-3/src/tap-bridge/model/tap-encode-decode.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/tap-bridge/model/%.o: ../ns-3/src/tap-bridge/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


