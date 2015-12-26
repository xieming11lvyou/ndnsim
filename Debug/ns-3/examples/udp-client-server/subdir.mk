################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/udp-client-server/udp-client-server.cc \
../ns-3/examples/udp-client-server/udp-trace-client-server.cc 

OBJS += \
./ns-3/examples/udp-client-server/udp-client-server.o \
./ns-3/examples/udp-client-server/udp-trace-client-server.o 

CC_DEPS += \
./ns-3/examples/udp-client-server/udp-client-server.d \
./ns-3/examples/udp-client-server/udp-trace-client-server.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/udp-client-server/%.o: ../ns-3/examples/udp-client-server/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


