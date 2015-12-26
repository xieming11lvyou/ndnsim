################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mobility/test/mobility-test-suite.cc \
../ns-3/src/mobility/test/mobility-trace-test-suite.cc \
../ns-3/src/mobility/test/ns2-mobility-helper-test-suite.cc \
../ns-3/src/mobility/test/steady-state-random-waypoint-mobility-model-test.cc \
../ns-3/src/mobility/test/waypoint-mobility-model-test.cc 

OBJS += \
./ns-3/src/mobility/test/mobility-test-suite.o \
./ns-3/src/mobility/test/mobility-trace-test-suite.o \
./ns-3/src/mobility/test/ns2-mobility-helper-test-suite.o \
./ns-3/src/mobility/test/steady-state-random-waypoint-mobility-model-test.o \
./ns-3/src/mobility/test/waypoint-mobility-model-test.o 

CC_DEPS += \
./ns-3/src/mobility/test/mobility-test-suite.d \
./ns-3/src/mobility/test/mobility-trace-test-suite.d \
./ns-3/src/mobility/test/ns2-mobility-helper-test-suite.d \
./ns-3/src/mobility/test/steady-state-random-waypoint-mobility-model-test.d \
./ns-3/src/mobility/test/waypoint-mobility-model-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mobility/test/%.o: ../ns-3/src/mobility/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


