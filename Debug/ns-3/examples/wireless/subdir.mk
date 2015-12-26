################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/wireless/ht-wifi-network.cc \
../ns-3/examples/wireless/mixed-wireless.cc \
../ns-3/examples/wireless/multirate.cc \
../ns-3/examples/wireless/ofdm-validation.cc \
../ns-3/examples/wireless/simple-wifi-frame-aggregation.cc \
../ns-3/examples/wireless/wifi-adhoc.cc \
../ns-3/examples/wireless/wifi-ap.cc \
../ns-3/examples/wireless/wifi-blockack.cc \
../ns-3/examples/wireless/wifi-clear-channel-cmu.cc \
../ns-3/examples/wireless/wifi-hidden-terminal.cc \
../ns-3/examples/wireless/wifi-simple-adhoc-grid.cc \
../ns-3/examples/wireless/wifi-simple-adhoc.cc \
../ns-3/examples/wireless/wifi-simple-infra.cc \
../ns-3/examples/wireless/wifi-simple-interference.cc \
../ns-3/examples/wireless/wifi-sleep.cc \
../ns-3/examples/wireless/wifi-wired-bridging.cc 

OBJS += \
./ns-3/examples/wireless/ht-wifi-network.o \
./ns-3/examples/wireless/mixed-wireless.o \
./ns-3/examples/wireless/multirate.o \
./ns-3/examples/wireless/ofdm-validation.o \
./ns-3/examples/wireless/simple-wifi-frame-aggregation.o \
./ns-3/examples/wireless/wifi-adhoc.o \
./ns-3/examples/wireless/wifi-ap.o \
./ns-3/examples/wireless/wifi-blockack.o \
./ns-3/examples/wireless/wifi-clear-channel-cmu.o \
./ns-3/examples/wireless/wifi-hidden-terminal.o \
./ns-3/examples/wireless/wifi-simple-adhoc-grid.o \
./ns-3/examples/wireless/wifi-simple-adhoc.o \
./ns-3/examples/wireless/wifi-simple-infra.o \
./ns-3/examples/wireless/wifi-simple-interference.o \
./ns-3/examples/wireless/wifi-sleep.o \
./ns-3/examples/wireless/wifi-wired-bridging.o 

CC_DEPS += \
./ns-3/examples/wireless/ht-wifi-network.d \
./ns-3/examples/wireless/mixed-wireless.d \
./ns-3/examples/wireless/multirate.d \
./ns-3/examples/wireless/ofdm-validation.d \
./ns-3/examples/wireless/simple-wifi-frame-aggregation.d \
./ns-3/examples/wireless/wifi-adhoc.d \
./ns-3/examples/wireless/wifi-ap.d \
./ns-3/examples/wireless/wifi-blockack.d \
./ns-3/examples/wireless/wifi-clear-channel-cmu.d \
./ns-3/examples/wireless/wifi-hidden-terminal.d \
./ns-3/examples/wireless/wifi-simple-adhoc-grid.d \
./ns-3/examples/wireless/wifi-simple-adhoc.d \
./ns-3/examples/wireless/wifi-simple-infra.d \
./ns-3/examples/wireless/wifi-simple-interference.d \
./ns-3/examples/wireless/wifi-sleep.d \
./ns-3/examples/wireless/wifi-wired-bridging.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/wireless/%.o: ../ns-3/examples/wireless/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


