################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/spectrum/test/spectrum-ideal-phy-test.cc \
../ns-3/src/spectrum/test/spectrum-interference-test.cc \
../ns-3/src/spectrum/test/spectrum-value-test.cc 

OBJS += \
./ns-3/src/spectrum/test/spectrum-ideal-phy-test.o \
./ns-3/src/spectrum/test/spectrum-interference-test.o \
./ns-3/src/spectrum/test/spectrum-value-test.o 

CC_DEPS += \
./ns-3/src/spectrum/test/spectrum-ideal-phy-test.d \
./ns-3/src/spectrum/test/spectrum-interference-test.d \
./ns-3/src/spectrum/test/spectrum-value-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/spectrum/test/%.o: ../ns-3/src/spectrum/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


