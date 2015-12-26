################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/buildings/test/building-position-allocator-test.cc \
../ns-3/src/buildings/test/buildings-helper-test.cc \
../ns-3/src/buildings/test/buildings-pathloss-test.cc \
../ns-3/src/buildings/test/buildings-shadowing-test.cc 

OBJS += \
./ns-3/src/buildings/test/building-position-allocator-test.o \
./ns-3/src/buildings/test/buildings-helper-test.o \
./ns-3/src/buildings/test/buildings-pathloss-test.o \
./ns-3/src/buildings/test/buildings-shadowing-test.o 

CC_DEPS += \
./ns-3/src/buildings/test/building-position-allocator-test.d \
./ns-3/src/buildings/test/buildings-helper-test.d \
./ns-3/src/buildings/test/buildings-pathloss-test.d \
./ns-3/src/buildings/test/buildings-shadowing-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/buildings/test/%.o: ../ns-3/src/buildings/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


