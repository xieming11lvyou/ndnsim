################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/utils/bench-packets.cc \
../ns-3/utils/bench-simulator.cc \
../ns-3/utils/print-introspected-doxygen.cc \
../ns-3/utils/test-runner.cc 

OBJS += \
./ns-3/utils/bench-packets.o \
./ns-3/utils/bench-simulator.o \
./ns-3/utils/print-introspected-doxygen.o \
./ns-3/utils/test-runner.o 

CC_DEPS += \
./ns-3/utils/bench-packets.d \
./ns-3/utils/bench-simulator.d \
./ns-3/utils/print-introspected-doxygen.d \
./ns-3/utils/test-runner.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/utils/%.o: ../ns-3/utils/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


