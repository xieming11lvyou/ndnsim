################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/olsr/test/bug780-test.cc \
../ns-3/src/olsr/test/hello-regression-test.cc \
../ns-3/src/olsr/test/olsr-header-test-suite.cc \
../ns-3/src/olsr/test/olsr-routing-protocol-test-suite.cc \
../ns-3/src/olsr/test/regression-test-suite.cc \
../ns-3/src/olsr/test/tc-regression-test.cc 

OBJS += \
./ns-3/src/olsr/test/bug780-test.o \
./ns-3/src/olsr/test/hello-regression-test.o \
./ns-3/src/olsr/test/olsr-header-test-suite.o \
./ns-3/src/olsr/test/olsr-routing-protocol-test-suite.o \
./ns-3/src/olsr/test/regression-test-suite.o \
./ns-3/src/olsr/test/tc-regression-test.o 

CC_DEPS += \
./ns-3/src/olsr/test/bug780-test.d \
./ns-3/src/olsr/test/hello-regression-test.d \
./ns-3/src/olsr/test/olsr-header-test-suite.d \
./ns-3/src/olsr/test/olsr-routing-protocol-test-suite.d \
./ns-3/src/olsr/test/regression-test-suite.d \
./ns-3/src/olsr/test/tc-regression-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/olsr/test/%.o: ../ns-3/src/olsr/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


