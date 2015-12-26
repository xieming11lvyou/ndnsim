################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../pybindgen/examples/boost_shared_ptr/bsp.cc 

OBJS += \
./pybindgen/examples/boost_shared_ptr/bsp.o 

CC_DEPS += \
./pybindgen/examples/boost_shared_ptr/bsp.d 


# Each subdirectory must supply rules for building sources it contributes
pybindgen/examples/boost_shared_ptr/%.o: ../pybindgen/examples/boost_shared_ptr/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


