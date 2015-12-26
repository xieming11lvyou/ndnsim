################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/lte/examples/lena-cqi-threshold.cc \
../ns-3/src/lte/examples/lena-distributed-ffr.cc \
../ns-3/src/lte/examples/lena-dual-stripe.cc \
../ns-3/src/lte/examples/lena-fading.cc \
../ns-3/src/lte/examples/lena-frequency-reuse.cc \
../ns-3/src/lte/examples/lena-intercell-interference.cc \
../ns-3/src/lte/examples/lena-pathloss-traces.cc \
../ns-3/src/lte/examples/lena-profiling.cc \
../ns-3/src/lte/examples/lena-rem-sector-antenna.cc \
../ns-3/src/lte/examples/lena-rem.cc \
../ns-3/src/lte/examples/lena-rlc-traces.cc \
../ns-3/src/lte/examples/lena-simple-epc-emu.cc \
../ns-3/src/lte/examples/lena-simple-epc.cc \
../ns-3/src/lte/examples/lena-simple.cc \
../ns-3/src/lte/examples/lena-uplink-power-control.cc \
../ns-3/src/lte/examples/lena-x2-handover-measures.cc \
../ns-3/src/lte/examples/lena-x2-handover.cc 

OBJS += \
./ns-3/src/lte/examples/lena-cqi-threshold.o \
./ns-3/src/lte/examples/lena-distributed-ffr.o \
./ns-3/src/lte/examples/lena-dual-stripe.o \
./ns-3/src/lte/examples/lena-fading.o \
./ns-3/src/lte/examples/lena-frequency-reuse.o \
./ns-3/src/lte/examples/lena-intercell-interference.o \
./ns-3/src/lte/examples/lena-pathloss-traces.o \
./ns-3/src/lte/examples/lena-profiling.o \
./ns-3/src/lte/examples/lena-rem-sector-antenna.o \
./ns-3/src/lte/examples/lena-rem.o \
./ns-3/src/lte/examples/lena-rlc-traces.o \
./ns-3/src/lte/examples/lena-simple-epc-emu.o \
./ns-3/src/lte/examples/lena-simple-epc.o \
./ns-3/src/lte/examples/lena-simple.o \
./ns-3/src/lte/examples/lena-uplink-power-control.o \
./ns-3/src/lte/examples/lena-x2-handover-measures.o \
./ns-3/src/lte/examples/lena-x2-handover.o 

CC_DEPS += \
./ns-3/src/lte/examples/lena-cqi-threshold.d \
./ns-3/src/lte/examples/lena-distributed-ffr.d \
./ns-3/src/lte/examples/lena-dual-stripe.d \
./ns-3/src/lte/examples/lena-fading.d \
./ns-3/src/lte/examples/lena-frequency-reuse.d \
./ns-3/src/lte/examples/lena-intercell-interference.d \
./ns-3/src/lte/examples/lena-pathloss-traces.d \
./ns-3/src/lte/examples/lena-profiling.d \
./ns-3/src/lte/examples/lena-rem-sector-antenna.d \
./ns-3/src/lte/examples/lena-rem.d \
./ns-3/src/lte/examples/lena-rlc-traces.d \
./ns-3/src/lte/examples/lena-simple-epc-emu.d \
./ns-3/src/lte/examples/lena-simple-epc.d \
./ns-3/src/lte/examples/lena-simple.d \
./ns-3/src/lte/examples/lena-uplink-power-control.d \
./ns-3/src/lte/examples/lena-x2-handover-measures.d \
./ns-3/src/lte/examples/lena-x2-handover.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/lte/examples/%.o: ../ns-3/src/lte/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


