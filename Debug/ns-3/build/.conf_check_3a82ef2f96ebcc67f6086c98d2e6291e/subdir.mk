################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ns-3/build/.conf_check_3a82ef2f96ebcc67f6086c98d2e6291e/test.cpp 

OBJS += \
./ns-3/build/.conf_check_3a82ef2f96ebcc67f6086c98d2e6291e/test.o 

CPP_DEPS += \
./ns-3/build/.conf_check_3a82ef2f96ebcc67f6086c98d2e6291e/test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/build/.conf_check_3a82ef2f96ebcc67f6086c98d2e6291e/%.o: ../ns-3/build/.conf_check_3a82ef2f96ebcc67f6086c98d2e6291e/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


