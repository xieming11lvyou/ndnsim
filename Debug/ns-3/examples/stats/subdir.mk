################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/stats/wifi-example-apps.cc \
../ns-3/examples/stats/wifi-example-sim.cc 

OBJS += \
./ns-3/examples/stats/wifi-example-apps.o \
./ns-3/examples/stats/wifi-example-sim.o 

CC_DEPS += \
./ns-3/examples/stats/wifi-example-apps.d \
./ns-3/examples/stats/wifi-example-sim.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/stats/%.o: ../ns-3/examples/stats/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


