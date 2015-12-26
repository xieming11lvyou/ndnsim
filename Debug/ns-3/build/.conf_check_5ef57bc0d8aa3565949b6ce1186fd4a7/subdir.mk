################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ns-3/build/.conf_check_5ef57bc0d8aa3565949b6ce1186fd4a7/test.cpp 

OBJS += \
./ns-3/build/.conf_check_5ef57bc0d8aa3565949b6ce1186fd4a7/test.o 

CPP_DEPS += \
./ns-3/build/.conf_check_5ef57bc0d8aa3565949b6ce1186fd4a7/test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/build/.conf_check_5ef57bc0d8aa3565949b6ce1186fd4a7/%.o: ../ns-3/build/.conf_check_5ef57bc0d8aa3565949b6ce1186fd4a7/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


