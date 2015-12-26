################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/propagation/examples/jakes-propagation-model-example.cc \
../ns-3/src/propagation/examples/main-propagation-loss.cc 

OBJS += \
./ns-3/src/propagation/examples/jakes-propagation-model-example.o \
./ns-3/src/propagation/examples/main-propagation-loss.o 

CC_DEPS += \
./ns-3/src/propagation/examples/jakes-propagation-model-example.d \
./ns-3/src/propagation/examples/main-propagation-loss.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/propagation/examples/%.o: ../ns-3/src/propagation/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


