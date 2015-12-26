################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../pybindgen/examples/a/a.cc 

OBJS += \
./pybindgen/examples/a/a.o 

CC_DEPS += \
./pybindgen/examples/a/a.d 


# Each subdirectory must supply rules for building sources it contributes
pybindgen/examples/a/%.o: ../pybindgen/examples/a/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


