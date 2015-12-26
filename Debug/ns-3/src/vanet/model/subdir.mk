################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/vanet/model/controller.cc \
../ns-3/src/vanet/model/highway.cc \
../ns-3/src/vanet/model/lane-change.cc \
../ns-3/src/vanet/model/model.cc \
../ns-3/src/vanet/model/obstacle.cc \
../ns-3/src/vanet/model/vehicle.cc 

OBJS += \
./ns-3/src/vanet/model/controller.o \
./ns-3/src/vanet/model/highway.o \
./ns-3/src/vanet/model/lane-change.o \
./ns-3/src/vanet/model/model.o \
./ns-3/src/vanet/model/obstacle.o \
./ns-3/src/vanet/model/vehicle.o 

CC_DEPS += \
./ns-3/src/vanet/model/controller.d \
./ns-3/src/vanet/model/highway.d \
./ns-3/src/vanet/model/lane-change.d \
./ns-3/src/vanet/model/model.d \
./ns-3/src/vanet/model/obstacle.d \
./ns-3/src/vanet/model/vehicle.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/vanet/model/%.o: ../ns-3/src/vanet/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


