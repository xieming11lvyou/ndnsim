################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/stats/test/average-test-suite.cc \
../ns-3/src/stats/test/basic-data-calculators-test-suite.cc \
../ns-3/src/stats/test/double-probe-test-suite.cc 

OBJS += \
./ns-3/src/stats/test/average-test-suite.o \
./ns-3/src/stats/test/basic-data-calculators-test-suite.o \
./ns-3/src/stats/test/double-probe-test-suite.o 

CC_DEPS += \
./ns-3/src/stats/test/average-test-suite.d \
./ns-3/src/stats/test/basic-data-calculators-test-suite.d \
./ns-3/src/stats/test/double-probe-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/stats/test/%.o: ../ns-3/src/stats/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


