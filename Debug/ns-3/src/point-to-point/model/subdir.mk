################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/point-to-point/model/point-to-point-channel.cc \
../ns-3/src/point-to-point/model/point-to-point-net-device.cc \
../ns-3/src/point-to-point/model/point-to-point-remote-channel.cc \
../ns-3/src/point-to-point/model/ppp-header.cc 

OBJS += \
./ns-3/src/point-to-point/model/point-to-point-channel.o \
./ns-3/src/point-to-point/model/point-to-point-net-device.o \
./ns-3/src/point-to-point/model/point-to-point-remote-channel.o \
./ns-3/src/point-to-point/model/ppp-header.o 

CC_DEPS += \
./ns-3/src/point-to-point/model/point-to-point-channel.d \
./ns-3/src/point-to-point/model/point-to-point-net-device.d \
./ns-3/src/point-to-point/model/point-to-point-remote-channel.d \
./ns-3/src/point-to-point/model/ppp-header.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/point-to-point/model/%.o: ../ns-3/src/point-to-point/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


