################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mobility/helper/mobility-helper.cc \
../ns-3/src/mobility/helper/ns2-mobility-helper.cc 

OBJS += \
./ns-3/src/mobility/helper/mobility-helper.o \
./ns-3/src/mobility/helper/ns2-mobility-helper.o 

CC_DEPS += \
./ns-3/src/mobility/helper/mobility-helper.d \
./ns-3/src/mobility/helper/ns2-mobility-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mobility/helper/%.o: ../ns-3/src/mobility/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


