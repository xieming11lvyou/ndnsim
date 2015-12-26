################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../pybindgen/tests/boost/bar.cc 

OBJS += \
./pybindgen/tests/boost/bar.o 

CC_DEPS += \
./pybindgen/tests/boost/bar.d 


# Each subdirectory must supply rules for building sources it contributes
pybindgen/tests/boost/%.o: ../pybindgen/tests/boost/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


