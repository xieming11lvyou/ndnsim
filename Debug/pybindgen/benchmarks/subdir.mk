################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../pybindgen/benchmarks/testapi.cc \
../pybindgen/benchmarks/testapi_boost.cc 

OBJS += \
./pybindgen/benchmarks/testapi.o \
./pybindgen/benchmarks/testapi_boost.o 

CC_DEPS += \
./pybindgen/benchmarks/testapi.d \
./pybindgen/benchmarks/testapi_boost.d 


# Each subdirectory must supply rules for building sources it contributes
pybindgen/benchmarks/%.o: ../pybindgen/benchmarks/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


