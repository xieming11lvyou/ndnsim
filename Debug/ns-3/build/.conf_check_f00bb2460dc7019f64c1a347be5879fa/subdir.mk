################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ns-3/build/.conf_check_f00bb2460dc7019f64c1a347be5879fa/test.cpp 

OBJS += \
./ns-3/build/.conf_check_f00bb2460dc7019f64c1a347be5879fa/test.o 

CPP_DEPS += \
./ns-3/build/.conf_check_f00bb2460dc7019f64c1a347be5879fa/test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/build/.conf_check_f00bb2460dc7019f64c1a347be5879fa/%.o: ../ns-3/build/.conf_check_f00bb2460dc7019f64c1a347be5879fa/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


