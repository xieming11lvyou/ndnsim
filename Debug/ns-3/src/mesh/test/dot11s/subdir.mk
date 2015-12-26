################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mesh/test/dot11s/dot11s-test-suite.cc \
../ns-3/src/mesh/test/dot11s/hwmp-proactive-regression.cc \
../ns-3/src/mesh/test/dot11s/hwmp-reactive-regression.cc \
../ns-3/src/mesh/test/dot11s/hwmp-simplest-regression.cc \
../ns-3/src/mesh/test/dot11s/hwmp-target-flags-regression.cc \
../ns-3/src/mesh/test/dot11s/pmp-regression.cc \
../ns-3/src/mesh/test/dot11s/regression.cc 

OBJS += \
./ns-3/src/mesh/test/dot11s/dot11s-test-suite.o \
./ns-3/src/mesh/test/dot11s/hwmp-proactive-regression.o \
./ns-3/src/mesh/test/dot11s/hwmp-reactive-regression.o \
./ns-3/src/mesh/test/dot11s/hwmp-simplest-regression.o \
./ns-3/src/mesh/test/dot11s/hwmp-target-flags-regression.o \
./ns-3/src/mesh/test/dot11s/pmp-regression.o \
./ns-3/src/mesh/test/dot11s/regression.o 

CC_DEPS += \
./ns-3/src/mesh/test/dot11s/dot11s-test-suite.d \
./ns-3/src/mesh/test/dot11s/hwmp-proactive-regression.d \
./ns-3/src/mesh/test/dot11s/hwmp-reactive-regression.d \
./ns-3/src/mesh/test/dot11s/hwmp-simplest-regression.d \
./ns-3/src/mesh/test/dot11s/hwmp-target-flags-regression.d \
./ns-3/src/mesh/test/dot11s/pmp-regression.d \
./ns-3/src/mesh/test/dot11s/regression.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mesh/test/dot11s/%.o: ../ns-3/src/mesh/test/dot11s/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


