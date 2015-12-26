################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mobility/examples/bonnmotion-ns2-example.cc \
../ns-3/src/mobility/examples/main-grid-topology.cc \
../ns-3/src/mobility/examples/main-random-topology.cc \
../ns-3/src/mobility/examples/main-random-walk.cc \
../ns-3/src/mobility/examples/mobility-trace-example.cc \
../ns-3/src/mobility/examples/ns2-mobility-trace.cc 

OBJS += \
./ns-3/src/mobility/examples/bonnmotion-ns2-example.o \
./ns-3/src/mobility/examples/main-grid-topology.o \
./ns-3/src/mobility/examples/main-random-topology.o \
./ns-3/src/mobility/examples/main-random-walk.o \
./ns-3/src/mobility/examples/mobility-trace-example.o \
./ns-3/src/mobility/examples/ns2-mobility-trace.o 

CC_DEPS += \
./ns-3/src/mobility/examples/bonnmotion-ns2-example.d \
./ns-3/src/mobility/examples/main-grid-topology.d \
./ns-3/src/mobility/examples/main-random-topology.d \
./ns-3/src/mobility/examples/main-random-walk.d \
./ns-3/src/mobility/examples/mobility-trace-example.d \
./ns-3/src/mobility/examples/ns2-mobility-trace.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mobility/examples/%.o: ../ns-3/src/mobility/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


