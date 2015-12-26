################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/stats/examples/double-probe-example.cc \
../ns-3/src/stats/examples/file-aggregator-example.cc \
../ns-3/src/stats/examples/file-helper-example.cc \
../ns-3/src/stats/examples/gnuplot-aggregator-example.cc \
../ns-3/src/stats/examples/gnuplot-example.cc \
../ns-3/src/stats/examples/gnuplot-helper-example.cc 

OBJS += \
./ns-3/src/stats/examples/double-probe-example.o \
./ns-3/src/stats/examples/file-aggregator-example.o \
./ns-3/src/stats/examples/file-helper-example.o \
./ns-3/src/stats/examples/gnuplot-aggregator-example.o \
./ns-3/src/stats/examples/gnuplot-example.o \
./ns-3/src/stats/examples/gnuplot-helper-example.o 

CC_DEPS += \
./ns-3/src/stats/examples/double-probe-example.d \
./ns-3/src/stats/examples/file-aggregator-example.d \
./ns-3/src/stats/examples/file-helper-example.d \
./ns-3/src/stats/examples/gnuplot-aggregator-example.d \
./ns-3/src/stats/examples/gnuplot-example.d \
./ns-3/src/stats/examples/gnuplot-helper-example.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/stats/examples/%.o: ../ns-3/src/stats/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


