################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/internet/helper/internet-stack-helper.cc \
../ns-3/src/internet/helper/internet-trace-helper.cc \
../ns-3/src/internet/helper/ipv4-address-helper.cc \
../ns-3/src/internet/helper/ipv4-global-routing-helper.cc \
../ns-3/src/internet/helper/ipv4-interface-container.cc \
../ns-3/src/internet/helper/ipv4-list-routing-helper.cc \
../ns-3/src/internet/helper/ipv4-routing-helper.cc \
../ns-3/src/internet/helper/ipv4-static-routing-helper.cc \
../ns-3/src/internet/helper/ipv6-address-helper.cc \
../ns-3/src/internet/helper/ipv6-interface-container.cc \
../ns-3/src/internet/helper/ipv6-list-routing-helper.cc \
../ns-3/src/internet/helper/ipv6-routing-helper.cc \
../ns-3/src/internet/helper/ipv6-static-routing-helper.cc \
../ns-3/src/internet/helper/ripng-helper.cc 

OBJS += \
./ns-3/src/internet/helper/internet-stack-helper.o \
./ns-3/src/internet/helper/internet-trace-helper.o \
./ns-3/src/internet/helper/ipv4-address-helper.o \
./ns-3/src/internet/helper/ipv4-global-routing-helper.o \
./ns-3/src/internet/helper/ipv4-interface-container.o \
./ns-3/src/internet/helper/ipv4-list-routing-helper.o \
./ns-3/src/internet/helper/ipv4-routing-helper.o \
./ns-3/src/internet/helper/ipv4-static-routing-helper.o \
./ns-3/src/internet/helper/ipv6-address-helper.o \
./ns-3/src/internet/helper/ipv6-interface-container.o \
./ns-3/src/internet/helper/ipv6-list-routing-helper.o \
./ns-3/src/internet/helper/ipv6-routing-helper.o \
./ns-3/src/internet/helper/ipv6-static-routing-helper.o \
./ns-3/src/internet/helper/ripng-helper.o 

CC_DEPS += \
./ns-3/src/internet/helper/internet-stack-helper.d \
./ns-3/src/internet/helper/internet-trace-helper.d \
./ns-3/src/internet/helper/ipv4-address-helper.d \
./ns-3/src/internet/helper/ipv4-global-routing-helper.d \
./ns-3/src/internet/helper/ipv4-interface-container.d \
./ns-3/src/internet/helper/ipv4-list-routing-helper.d \
./ns-3/src/internet/helper/ipv4-routing-helper.d \
./ns-3/src/internet/helper/ipv4-static-routing-helper.d \
./ns-3/src/internet/helper/ipv6-address-helper.d \
./ns-3/src/internet/helper/ipv6-interface-container.d \
./ns-3/src/internet/helper/ipv6-list-routing-helper.d \
./ns-3/src/internet/helper/ipv6-routing-helper.d \
./ns-3/src/internet/helper/ipv6-static-routing-helper.d \
./ns-3/src/internet/helper/ripng-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/internet/helper/%.o: ../ns-3/src/internet/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


