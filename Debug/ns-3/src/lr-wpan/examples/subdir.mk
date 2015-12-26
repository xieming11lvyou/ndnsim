################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/lr-wpan/examples/lr-wpan-data.cc \
../ns-3/src/lr-wpan/examples/lr-wpan-error-distance-plot.cc \
../ns-3/src/lr-wpan/examples/lr-wpan-error-model-plot.cc \
../ns-3/src/lr-wpan/examples/lr-wpan-packet-print.cc \
../ns-3/src/lr-wpan/examples/lr-wpan-phy-test.cc 

OBJS += \
./ns-3/src/lr-wpan/examples/lr-wpan-data.o \
./ns-3/src/lr-wpan/examples/lr-wpan-error-distance-plot.o \
./ns-3/src/lr-wpan/examples/lr-wpan-error-model-plot.o \
./ns-3/src/lr-wpan/examples/lr-wpan-packet-print.o \
./ns-3/src/lr-wpan/examples/lr-wpan-phy-test.o 

CC_DEPS += \
./ns-3/src/lr-wpan/examples/lr-wpan-data.d \
./ns-3/src/lr-wpan/examples/lr-wpan-error-distance-plot.d \
./ns-3/src/lr-wpan/examples/lr-wpan-error-model-plot.d \
./ns-3/src/lr-wpan/examples/lr-wpan-packet-print.d \
./ns-3/src/lr-wpan/examples/lr-wpan-phy-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/lr-wpan/examples/%.o: ../ns-3/src/lr-wpan/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


