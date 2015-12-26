################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/flow-monitor/model/flow-classifier.cc \
../ns-3/src/flow-monitor/model/flow-monitor.cc \
../ns-3/src/flow-monitor/model/flow-probe.cc \
../ns-3/src/flow-monitor/model/histogram.cc \
../ns-3/src/flow-monitor/model/ipv4-flow-classifier.cc \
../ns-3/src/flow-monitor/model/ipv4-flow-probe.cc \
../ns-3/src/flow-monitor/model/ipv6-flow-classifier.cc \
../ns-3/src/flow-monitor/model/ipv6-flow-probe.cc 

OBJS += \
./ns-3/src/flow-monitor/model/flow-classifier.o \
./ns-3/src/flow-monitor/model/flow-monitor.o \
./ns-3/src/flow-monitor/model/flow-probe.o \
./ns-3/src/flow-monitor/model/histogram.o \
./ns-3/src/flow-monitor/model/ipv4-flow-classifier.o \
./ns-3/src/flow-monitor/model/ipv4-flow-probe.o \
./ns-3/src/flow-monitor/model/ipv6-flow-classifier.o \
./ns-3/src/flow-monitor/model/ipv6-flow-probe.o 

CC_DEPS += \
./ns-3/src/flow-monitor/model/flow-classifier.d \
./ns-3/src/flow-monitor/model/flow-monitor.d \
./ns-3/src/flow-monitor/model/flow-probe.d \
./ns-3/src/flow-monitor/model/histogram.d \
./ns-3/src/flow-monitor/model/ipv4-flow-classifier.d \
./ns-3/src/flow-monitor/model/ipv4-flow-probe.d \
./ns-3/src/flow-monitor/model/ipv6-flow-classifier.d \
./ns-3/src/flow-monitor/model/ipv6-flow-probe.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/flow-monitor/model/%.o: ../ns-3/src/flow-monitor/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


