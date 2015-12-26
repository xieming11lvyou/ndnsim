################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/olsr/model/olsr-header.cc \
../ns-3/src/olsr/model/olsr-routing-protocol.cc \
../ns-3/src/olsr/model/olsr-state.cc 

OBJS += \
./ns-3/src/olsr/model/olsr-header.o \
./ns-3/src/olsr/model/olsr-routing-protocol.o \
./ns-3/src/olsr/model/olsr-state.o 

CC_DEPS += \
./ns-3/src/olsr/model/olsr-header.d \
./ns-3/src/olsr/model/olsr-routing-protocol.d \
./ns-3/src/olsr/model/olsr-state.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/olsr/model/%.o: ../ns-3/src/olsr/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


