################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy-matrix-propagation-loss-model.cc \
../ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy-with-microwave-oven.cc \
../ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy.cc 

OBJS += \
./ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy-matrix-propagation-loss-model.o \
./ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy-with-microwave-oven.o \
./ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy.o 

CC_DEPS += \
./ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy-matrix-propagation-loss-model.d \
./ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy-with-microwave-oven.d \
./ns-3/src/spectrum/examples/adhoc-aloha-ideal-phy.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/spectrum/examples/%.o: ../ns-3/src/spectrum/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


