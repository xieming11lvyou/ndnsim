################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/stats/model/boolean-probe.cc \
../ns-3/src/stats/model/data-calculator.cc \
../ns-3/src/stats/model/data-collection-object.cc \
../ns-3/src/stats/model/data-collector.cc \
../ns-3/src/stats/model/data-output-interface.cc \
../ns-3/src/stats/model/double-probe.cc \
../ns-3/src/stats/model/file-aggregator.cc \
../ns-3/src/stats/model/get-wildcard-matches.cc \
../ns-3/src/stats/model/gnuplot-aggregator.cc \
../ns-3/src/stats/model/gnuplot.cc \
../ns-3/src/stats/model/omnet-data-output.cc \
../ns-3/src/stats/model/probe.cc \
../ns-3/src/stats/model/sqlite-data-output.cc \
../ns-3/src/stats/model/time-data-calculators.cc \
../ns-3/src/stats/model/time-series-adaptor.cc \
../ns-3/src/stats/model/uinteger-16-probe.cc \
../ns-3/src/stats/model/uinteger-32-probe.cc \
../ns-3/src/stats/model/uinteger-8-probe.cc 

OBJS += \
./ns-3/src/stats/model/boolean-probe.o \
./ns-3/src/stats/model/data-calculator.o \
./ns-3/src/stats/model/data-collection-object.o \
./ns-3/src/stats/model/data-collector.o \
./ns-3/src/stats/model/data-output-interface.o \
./ns-3/src/stats/model/double-probe.o \
./ns-3/src/stats/model/file-aggregator.o \
./ns-3/src/stats/model/get-wildcard-matches.o \
./ns-3/src/stats/model/gnuplot-aggregator.o \
./ns-3/src/stats/model/gnuplot.o \
./ns-3/src/stats/model/omnet-data-output.o \
./ns-3/src/stats/model/probe.o \
./ns-3/src/stats/model/sqlite-data-output.o \
./ns-3/src/stats/model/time-data-calculators.o \
./ns-3/src/stats/model/time-series-adaptor.o \
./ns-3/src/stats/model/uinteger-16-probe.o \
./ns-3/src/stats/model/uinteger-32-probe.o \
./ns-3/src/stats/model/uinteger-8-probe.o 

CC_DEPS += \
./ns-3/src/stats/model/boolean-probe.d \
./ns-3/src/stats/model/data-calculator.d \
./ns-3/src/stats/model/data-collection-object.d \
./ns-3/src/stats/model/data-collector.d \
./ns-3/src/stats/model/data-output-interface.d \
./ns-3/src/stats/model/double-probe.d \
./ns-3/src/stats/model/file-aggregator.d \
./ns-3/src/stats/model/get-wildcard-matches.d \
./ns-3/src/stats/model/gnuplot-aggregator.d \
./ns-3/src/stats/model/gnuplot.d \
./ns-3/src/stats/model/omnet-data-output.d \
./ns-3/src/stats/model/probe.d \
./ns-3/src/stats/model/sqlite-data-output.d \
./ns-3/src/stats/model/time-data-calculators.d \
./ns-3/src/stats/model/time-series-adaptor.d \
./ns-3/src/stats/model/uinteger-16-probe.d \
./ns-3/src/stats/model/uinteger-32-probe.d \
./ns-3/src/stats/model/uinteger-8-probe.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/stats/model/%.o: ../ns-3/src/stats/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


