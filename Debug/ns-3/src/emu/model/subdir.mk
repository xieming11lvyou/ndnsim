################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/emu/model/emu-encode-decode.cc \
../ns-3/src/emu/model/emu-net-device.cc \
../ns-3/src/emu/model/emu-sock-creator.cc 

OBJS += \
./ns-3/src/emu/model/emu-encode-decode.o \
./ns-3/src/emu/model/emu-net-device.o \
./ns-3/src/emu/model/emu-sock-creator.o 

CC_DEPS += \
./ns-3/src/emu/model/emu-encode-decode.d \
./ns-3/src/emu/model/emu-net-device.d \
./ns-3/src/emu/model/emu-sock-creator.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/emu/model/%.o: ../ns-3/src/emu/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


