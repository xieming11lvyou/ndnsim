################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/aodv/test/aodv-id-cache-test-suite.cc \
../ns-3/src/aodv/test/aodv-regression.cc \
../ns-3/src/aodv/test/aodv-test-suite.cc \
../ns-3/src/aodv/test/bug-772.cc \
../ns-3/src/aodv/test/loopback.cc 

OBJS += \
./ns-3/src/aodv/test/aodv-id-cache-test-suite.o \
./ns-3/src/aodv/test/aodv-regression.o \
./ns-3/src/aodv/test/aodv-test-suite.o \
./ns-3/src/aodv/test/bug-772.o \
./ns-3/src/aodv/test/loopback.o 

CC_DEPS += \
./ns-3/src/aodv/test/aodv-id-cache-test-suite.d \
./ns-3/src/aodv/test/aodv-regression.d \
./ns-3/src/aodv/test/aodv-test-suite.d \
./ns-3/src/aodv/test/bug-772.d \
./ns-3/src/aodv/test/loopback.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/aodv/test/%.o: ../ns-3/src/aodv/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


