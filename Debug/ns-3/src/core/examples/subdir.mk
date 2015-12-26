################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/core/examples/command-line-example.cc \
../ns-3/src/core/examples/hash-example.cc \
../ns-3/src/core/examples/main-callback.cc \
../ns-3/src/core/examples/main-ptr.cc \
../ns-3/src/core/examples/main-random-variable-stream.cc \
../ns-3/src/core/examples/main-random-variable.cc \
../ns-3/src/core/examples/main-test-sync.cc \
../ns-3/src/core/examples/sample-random-variable-stream.cc \
../ns-3/src/core/examples/sample-random-variable.cc \
../ns-3/src/core/examples/sample-simulator.cc 

OBJS += \
./ns-3/src/core/examples/command-line-example.o \
./ns-3/src/core/examples/hash-example.o \
./ns-3/src/core/examples/main-callback.o \
./ns-3/src/core/examples/main-ptr.o \
./ns-3/src/core/examples/main-random-variable-stream.o \
./ns-3/src/core/examples/main-random-variable.o \
./ns-3/src/core/examples/main-test-sync.o \
./ns-3/src/core/examples/sample-random-variable-stream.o \
./ns-3/src/core/examples/sample-random-variable.o \
./ns-3/src/core/examples/sample-simulator.o 

CC_DEPS += \
./ns-3/src/core/examples/command-line-example.d \
./ns-3/src/core/examples/hash-example.d \
./ns-3/src/core/examples/main-callback.d \
./ns-3/src/core/examples/main-ptr.d \
./ns-3/src/core/examples/main-random-variable-stream.d \
./ns-3/src/core/examples/main-random-variable.d \
./ns-3/src/core/examples/main-test-sync.d \
./ns-3/src/core/examples/sample-random-variable-stream.d \
./ns-3/src/core/examples/sample-random-variable.d \
./ns-3/src/core/examples/sample-simulator.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/core/examples/%.o: ../ns-3/src/core/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


