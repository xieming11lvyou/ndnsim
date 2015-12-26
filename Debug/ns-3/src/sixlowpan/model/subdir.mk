################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/sixlowpan/model/sixlowpan-header.cc \
../ns-3/src/sixlowpan/model/sixlowpan-net-device.cc 

OBJS += \
./ns-3/src/sixlowpan/model/sixlowpan-header.o \
./ns-3/src/sixlowpan/model/sixlowpan-net-device.o 

CC_DEPS += \
./ns-3/src/sixlowpan/model/sixlowpan-header.d \
./ns-3/src/sixlowpan/model/sixlowpan-net-device.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/sixlowpan/model/%.o: ../ns-3/src/sixlowpan/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


