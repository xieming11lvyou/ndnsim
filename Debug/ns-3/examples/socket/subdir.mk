################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/socket/socket-bound-static-routing.cc \
../ns-3/examples/socket/socket-bound-tcp-static-routing.cc \
../ns-3/examples/socket/socket-options-ipv4.cc \
../ns-3/examples/socket/socket-options-ipv6.cc 

OBJS += \
./ns-3/examples/socket/socket-bound-static-routing.o \
./ns-3/examples/socket/socket-bound-tcp-static-routing.o \
./ns-3/examples/socket/socket-options-ipv4.o \
./ns-3/examples/socket/socket-options-ipv6.o 

CC_DEPS += \
./ns-3/examples/socket/socket-bound-static-routing.d \
./ns-3/examples/socket/socket-bound-tcp-static-routing.d \
./ns-3/examples/socket/socket-options-ipv4.d \
./ns-3/examples/socket/socket-options-ipv6.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/socket/%.o: ../ns-3/examples/socket/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


