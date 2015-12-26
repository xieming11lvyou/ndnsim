################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../pybindgen/examples/d/d.cc 

OBJS += \
./pybindgen/examples/d/d.o 

CC_DEPS += \
./pybindgen/examples/d/d.d 


# Each subdirectory must supply rules for building sources it contributes
pybindgen/examples/d/%.o: ../pybindgen/examples/d/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


