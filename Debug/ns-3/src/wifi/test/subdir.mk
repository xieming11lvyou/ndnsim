################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/wifi/test/block-ack-test-suite.cc \
../ns-3/src/wifi/test/dcf-manager-test.cc \
../ns-3/src/wifi/test/tx-duration-test.cc \
../ns-3/src/wifi/test/wifi-test.cc 

OBJS += \
./ns-3/src/wifi/test/block-ack-test-suite.o \
./ns-3/src/wifi/test/dcf-manager-test.o \
./ns-3/src/wifi/test/tx-duration-test.o \
./ns-3/src/wifi/test/wifi-test.o 

CC_DEPS += \
./ns-3/src/wifi/test/block-ack-test-suite.d \
./ns-3/src/wifi/test/dcf-manager-test.d \
./ns-3/src/wifi/test/tx-duration-test.d \
./ns-3/src/wifi/test/wifi-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/wifi/test/%.o: ../ns-3/src/wifi/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


