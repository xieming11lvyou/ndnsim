################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/click/examples/nsclick-defines.cc \
../ns-3/src/click/examples/nsclick-raw-wlan.cc \
../ns-3/src/click/examples/nsclick-routing.cc \
../ns-3/src/click/examples/nsclick-simple-lan.cc \
../ns-3/src/click/examples/nsclick-udp-client-server-csma.cc \
../ns-3/src/click/examples/nsclick-udp-client-server-wifi.cc 

OBJS += \
./ns-3/src/click/examples/nsclick-defines.o \
./ns-3/src/click/examples/nsclick-raw-wlan.o \
./ns-3/src/click/examples/nsclick-routing.o \
./ns-3/src/click/examples/nsclick-simple-lan.o \
./ns-3/src/click/examples/nsclick-udp-client-server-csma.o \
./ns-3/src/click/examples/nsclick-udp-client-server-wifi.o 

CC_DEPS += \
./ns-3/src/click/examples/nsclick-defines.d \
./ns-3/src/click/examples/nsclick-raw-wlan.d \
./ns-3/src/click/examples/nsclick-routing.d \
./ns-3/src/click/examples/nsclick-simple-lan.d \
./ns-3/src/click/examples/nsclick-udp-client-server-csma.d \
./ns-3/src/click/examples/nsclick-udp-client-server-wifi.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/click/examples/%.o: ../ns-3/src/click/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


