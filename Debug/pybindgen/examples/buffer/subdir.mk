################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../pybindgen/examples/buffer/c.cc 

OBJS += \
./pybindgen/examples/buffer/c.o 

CC_DEPS += \
./pybindgen/examples/buffer/c.d 


# Each subdirectory must supply rules for building sources it contributes
pybindgen/examples/buffer/%.o: ../pybindgen/examples/buffer/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


