################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/build/.conf_check_efd20fc635aad783e4d3953701740533/test.cc 

OBJS += \
./ns-3/build/.conf_check_efd20fc635aad783e4d3953701740533/test.o 

CC_DEPS += \
./ns-3/build/.conf_check_efd20fc635aad783e4d3953701740533/test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/build/.conf_check_efd20fc635aad783e4d3953701740533/%.o: ../ns-3/build/.conf_check_efd20fc635aad783e4d3953701740533/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


