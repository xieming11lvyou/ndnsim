################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/wimax/examples/wimax-ipv4.cc \
../ns-3/src/wimax/examples/wimax-multicast.cc \
../ns-3/src/wimax/examples/wimax-simple.cc 

OBJS += \
./ns-3/src/wimax/examples/wimax-ipv4.o \
./ns-3/src/wimax/examples/wimax-multicast.o \
./ns-3/src/wimax/examples/wimax-simple.o 

CC_DEPS += \
./ns-3/src/wimax/examples/wimax-ipv4.d \
./ns-3/src/wimax/examples/wimax-multicast.d \
./ns-3/src/wimax/examples/wimax-simple.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/wimax/examples/%.o: ../ns-3/src/wimax/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


