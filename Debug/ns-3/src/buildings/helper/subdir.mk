################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/buildings/helper/building-allocator.cc \
../ns-3/src/buildings/helper/building-container.cc \
../ns-3/src/buildings/helper/building-position-allocator.cc \
../ns-3/src/buildings/helper/buildings-helper.cc 

OBJS += \
./ns-3/src/buildings/helper/building-allocator.o \
./ns-3/src/buildings/helper/building-container.o \
./ns-3/src/buildings/helper/building-position-allocator.o \
./ns-3/src/buildings/helper/buildings-helper.o 

CC_DEPS += \
./ns-3/src/buildings/helper/building-allocator.d \
./ns-3/src/buildings/helper/building-container.d \
./ns-3/src/buildings/helper/building-position-allocator.d \
./ns-3/src/buildings/helper/buildings-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/buildings/helper/%.o: ../ns-3/src/buildings/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


