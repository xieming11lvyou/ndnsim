################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/point-to-point-layout/model/point-to-point-dumbbell.cc \
../ns-3/src/point-to-point-layout/model/point-to-point-grid.cc \
../ns-3/src/point-to-point-layout/model/point-to-point-star.cc 

OBJS += \
./ns-3/src/point-to-point-layout/model/point-to-point-dumbbell.o \
./ns-3/src/point-to-point-layout/model/point-to-point-grid.o \
./ns-3/src/point-to-point-layout/model/point-to-point-star.o 

CC_DEPS += \
./ns-3/src/point-to-point-layout/model/point-to-point-dumbbell.d \
./ns-3/src/point-to-point-layout/model/point-to-point-grid.d \
./ns-3/src/point-to-point-layout/model/point-to-point-star.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/point-to-point-layout/model/%.o: ../ns-3/src/point-to-point-layout/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


