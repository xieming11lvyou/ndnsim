################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/ipv6/fragmentation-ipv6-two-MTU.cc \
../ns-3/examples/ipv6/fragmentation-ipv6.cc \
../ns-3/examples/ipv6/icmpv6-redirect.cc \
../ns-3/examples/ipv6/loose-routing-ipv6.cc \
../ns-3/examples/ipv6/ping6.cc \
../ns-3/examples/ipv6/radvd-two-prefix.cc \
../ns-3/examples/ipv6/radvd.cc \
../ns-3/examples/ipv6/test-ipv6.cc \
../ns-3/examples/ipv6/wsn-ping6.cc 

OBJS += \
./ns-3/examples/ipv6/fragmentation-ipv6-two-MTU.o \
./ns-3/examples/ipv6/fragmentation-ipv6.o \
./ns-3/examples/ipv6/icmpv6-redirect.o \
./ns-3/examples/ipv6/loose-routing-ipv6.o \
./ns-3/examples/ipv6/ping6.o \
./ns-3/examples/ipv6/radvd-two-prefix.o \
./ns-3/examples/ipv6/radvd.o \
./ns-3/examples/ipv6/test-ipv6.o \
./ns-3/examples/ipv6/wsn-ping6.o 

CC_DEPS += \
./ns-3/examples/ipv6/fragmentation-ipv6-two-MTU.d \
./ns-3/examples/ipv6/fragmentation-ipv6.d \
./ns-3/examples/ipv6/icmpv6-redirect.d \
./ns-3/examples/ipv6/loose-routing-ipv6.d \
./ns-3/examples/ipv6/ping6.d \
./ns-3/examples/ipv6/radvd-two-prefix.d \
./ns-3/examples/ipv6/radvd.d \
./ns-3/examples/ipv6/test-ipv6.d \
./ns-3/examples/ipv6/wsn-ping6.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/ipv6/%.o: ../ns-3/examples/ipv6/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


