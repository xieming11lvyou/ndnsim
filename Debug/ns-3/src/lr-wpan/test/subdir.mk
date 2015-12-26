################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/lr-wpan/test/lr-wpan-ack-test.cc \
../ns-3/src/lr-wpan/test/lr-wpan-cca-test.cc \
../ns-3/src/lr-wpan/test/lr-wpan-collision-test.cc \
../ns-3/src/lr-wpan/test/lr-wpan-ed-test.cc \
../ns-3/src/lr-wpan/test/lr-wpan-error-model-test.cc \
../ns-3/src/lr-wpan/test/lr-wpan-packet-test.cc \
../ns-3/src/lr-wpan/test/lr-wpan-pd-plme-sap-test.cc \
../ns-3/src/lr-wpan/test/lr-wpan-spectrum-value-helper-test.cc 

OBJS += \
./ns-3/src/lr-wpan/test/lr-wpan-ack-test.o \
./ns-3/src/lr-wpan/test/lr-wpan-cca-test.o \
./ns-3/src/lr-wpan/test/lr-wpan-collision-test.o \
./ns-3/src/lr-wpan/test/lr-wpan-ed-test.o \
./ns-3/src/lr-wpan/test/lr-wpan-error-model-test.o \
./ns-3/src/lr-wpan/test/lr-wpan-packet-test.o \
./ns-3/src/lr-wpan/test/lr-wpan-pd-plme-sap-test.o \
./ns-3/src/lr-wpan/test/lr-wpan-spectrum-value-helper-test.o 

CC_DEPS += \
./ns-3/src/lr-wpan/test/lr-wpan-ack-test.d \
./ns-3/src/lr-wpan/test/lr-wpan-cca-test.d \
./ns-3/src/lr-wpan/test/lr-wpan-collision-test.d \
./ns-3/src/lr-wpan/test/lr-wpan-ed-test.d \
./ns-3/src/lr-wpan/test/lr-wpan-error-model-test.d \
./ns-3/src/lr-wpan/test/lr-wpan-packet-test.d \
./ns-3/src/lr-wpan/test/lr-wpan-pd-plme-sap-test.d \
./ns-3/src/lr-wpan/test/lr-wpan-spectrum-value-helper-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/lr-wpan/test/%.o: ../ns-3/src/lr-wpan/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


