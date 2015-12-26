################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/core/test/attribute-test-suite.cc \
../ns-3/src/core/test/callback-test-suite.cc \
../ns-3/src/core/test/command-line-test-suite.cc \
../ns-3/src/core/test/config-test-suite.cc \
../ns-3/src/core/test/event-garbage-collector-test-suite.cc \
../ns-3/src/core/test/global-value-test-suite.cc \
../ns-3/src/core/test/hash-test-suite.cc \
../ns-3/src/core/test/int64x64-test-suite.cc \
../ns-3/src/core/test/many-uniform-random-variables-one-get-value-call-test-suite.cc \
../ns-3/src/core/test/names-test-suite.cc \
../ns-3/src/core/test/object-test-suite.cc \
../ns-3/src/core/test/one-uniform-random-variable-many-get-value-calls-test-suite.cc \
../ns-3/src/core/test/ptr-test-suite.cc \
../ns-3/src/core/test/random-variable-stream-test-suite.cc \
../ns-3/src/core/test/random-variable-test-suite.cc \
../ns-3/src/core/test/rng-test-suite.cc \
../ns-3/src/core/test/sample-test-suite.cc \
../ns-3/src/core/test/simulator-test-suite.cc \
../ns-3/src/core/test/threaded-test-suite.cc \
../ns-3/src/core/test/time-test-suite.cc \
../ns-3/src/core/test/timer-test-suite.cc \
../ns-3/src/core/test/traced-callback-test-suite.cc \
../ns-3/src/core/test/type-id-test-suite.cc \
../ns-3/src/core/test/type-traits-test-suite.cc \
../ns-3/src/core/test/watchdog-test-suite.cc 

OBJS += \
./ns-3/src/core/test/attribute-test-suite.o \
./ns-3/src/core/test/callback-test-suite.o \
./ns-3/src/core/test/command-line-test-suite.o \
./ns-3/src/core/test/config-test-suite.o \
./ns-3/src/core/test/event-garbage-collector-test-suite.o \
./ns-3/src/core/test/global-value-test-suite.o \
./ns-3/src/core/test/hash-test-suite.o \
./ns-3/src/core/test/int64x64-test-suite.o \
./ns-3/src/core/test/many-uniform-random-variables-one-get-value-call-test-suite.o \
./ns-3/src/core/test/names-test-suite.o \
./ns-3/src/core/test/object-test-suite.o \
./ns-3/src/core/test/one-uniform-random-variable-many-get-value-calls-test-suite.o \
./ns-3/src/core/test/ptr-test-suite.o \
./ns-3/src/core/test/random-variable-stream-test-suite.o \
./ns-3/src/core/test/random-variable-test-suite.o \
./ns-3/src/core/test/rng-test-suite.o \
./ns-3/src/core/test/sample-test-suite.o \
./ns-3/src/core/test/simulator-test-suite.o \
./ns-3/src/core/test/threaded-test-suite.o \
./ns-3/src/core/test/time-test-suite.o \
./ns-3/src/core/test/timer-test-suite.o \
./ns-3/src/core/test/traced-callback-test-suite.o \
./ns-3/src/core/test/type-id-test-suite.o \
./ns-3/src/core/test/type-traits-test-suite.o \
./ns-3/src/core/test/watchdog-test-suite.o 

CC_DEPS += \
./ns-3/src/core/test/attribute-test-suite.d \
./ns-3/src/core/test/callback-test-suite.d \
./ns-3/src/core/test/command-line-test-suite.d \
./ns-3/src/core/test/config-test-suite.d \
./ns-3/src/core/test/event-garbage-collector-test-suite.d \
./ns-3/src/core/test/global-value-test-suite.d \
./ns-3/src/core/test/hash-test-suite.d \
./ns-3/src/core/test/int64x64-test-suite.d \
./ns-3/src/core/test/many-uniform-random-variables-one-get-value-call-test-suite.d \
./ns-3/src/core/test/names-test-suite.d \
./ns-3/src/core/test/object-test-suite.d \
./ns-3/src/core/test/one-uniform-random-variable-many-get-value-calls-test-suite.d \
./ns-3/src/core/test/ptr-test-suite.d \
./ns-3/src/core/test/random-variable-stream-test-suite.d \
./ns-3/src/core/test/random-variable-test-suite.d \
./ns-3/src/core/test/rng-test-suite.d \
./ns-3/src/core/test/sample-test-suite.d \
./ns-3/src/core/test/simulator-test-suite.d \
./ns-3/src/core/test/threaded-test-suite.d \
./ns-3/src/core/test/time-test-suite.d \
./ns-3/src/core/test/timer-test-suite.d \
./ns-3/src/core/test/traced-callback-test-suite.d \
./ns-3/src/core/test/type-id-test-suite.d \
./ns-3/src/core/test/type-traits-test-suite.d \
./ns-3/src/core/test/watchdog-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/core/test/%.o: ../ns-3/src/core/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


