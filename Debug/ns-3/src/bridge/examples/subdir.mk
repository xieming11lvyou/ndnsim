################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/bridge/examples/csma-bridge-one-hop.cc \
../ns-3/src/bridge/examples/csma-bridge.cc 

OBJS += \
./ns-3/src/bridge/examples/csma-bridge-one-hop.o \
./ns-3/src/bridge/examples/csma-bridge.o 

CC_DEPS += \
./ns-3/src/bridge/examples/csma-bridge-one-hop.d \
./ns-3/src/bridge/examples/csma-bridge.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/bridge/examples/%.o: ../ns-3/src/bridge/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


