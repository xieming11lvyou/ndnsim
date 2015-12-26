################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/click/model/ipv4-click-routing.cc \
../ns-3/src/click/model/ipv4-l3-click-protocol.cc 

OBJS += \
./ns-3/src/click/model/ipv4-click-routing.o \
./ns-3/src/click/model/ipv4-l3-click-protocol.o 

CC_DEPS += \
./ns-3/src/click/model/ipv4-click-routing.d \
./ns-3/src/click/model/ipv4-l3-click-protocol.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/click/model/%.o: ../ns-3/src/click/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


