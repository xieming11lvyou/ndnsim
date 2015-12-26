################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/ip/4.cc \
../ns-3/ip/request-tag.cc \
../ns-3/ip/udp-echo-client1024.cc \
../ns-3/ip/udp-echo-server1024.cc 

OBJS += \
./ns-3/ip/4.o \
./ns-3/ip/request-tag.o \
./ns-3/ip/udp-echo-client1024.o \
./ns-3/ip/udp-echo-server1024.o 

CC_DEPS += \
./ns-3/ip/4.d \
./ns-3/ip/request-tag.d \
./ns-3/ip/udp-echo-client1024.d \
./ns-3/ip/udp-echo-server1024.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/ip/%.o: ../ns-3/ip/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


