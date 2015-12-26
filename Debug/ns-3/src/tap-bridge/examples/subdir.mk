################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/tap-bridge/examples/tap-csma-virtual-machine.cc \
../ns-3/src/tap-bridge/examples/tap-csma.cc \
../ns-3/src/tap-bridge/examples/tap-wifi-dumbbell.cc \
../ns-3/src/tap-bridge/examples/tap-wifi-virtual-machine.cc 

OBJS += \
./ns-3/src/tap-bridge/examples/tap-csma-virtual-machine.o \
./ns-3/src/tap-bridge/examples/tap-csma.o \
./ns-3/src/tap-bridge/examples/tap-wifi-dumbbell.o \
./ns-3/src/tap-bridge/examples/tap-wifi-virtual-machine.o 

CC_DEPS += \
./ns-3/src/tap-bridge/examples/tap-csma-virtual-machine.d \
./ns-3/src/tap-bridge/examples/tap-csma.d \
./ns-3/src/tap-bridge/examples/tap-wifi-dumbbell.d \
./ns-3/src/tap-bridge/examples/tap-wifi-virtual-machine.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/tap-bridge/examples/%.o: ../ns-3/src/tap-bridge/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


