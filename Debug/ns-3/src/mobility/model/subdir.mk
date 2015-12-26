################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mobility/model/box.cc \
../ns-3/src/mobility/model/constant-acceleration-mobility-model.cc \
../ns-3/src/mobility/model/constant-position-mobility-model.cc \
../ns-3/src/mobility/model/constant-velocity-helper.cc \
../ns-3/src/mobility/model/constant-velocity-mobility-model.cc \
../ns-3/src/mobility/model/gauss-markov-mobility-model.cc \
../ns-3/src/mobility/model/hierarchical-mobility-model.cc \
../ns-3/src/mobility/model/mobility-model.cc \
../ns-3/src/mobility/model/position-allocator.cc \
../ns-3/src/mobility/model/random-direction-2d-mobility-model.cc \
../ns-3/src/mobility/model/random-walk-2d-mobility-model.cc \
../ns-3/src/mobility/model/random-waypoint-mobility-model.cc \
../ns-3/src/mobility/model/rectangle.cc \
../ns-3/src/mobility/model/steady-state-random-waypoint-mobility-model.cc \
../ns-3/src/mobility/model/waypoint-mobility-model.cc \
../ns-3/src/mobility/model/waypoint.cc 

OBJS += \
./ns-3/src/mobility/model/box.o \
./ns-3/src/mobility/model/constant-acceleration-mobility-model.o \
./ns-3/src/mobility/model/constant-position-mobility-model.o \
./ns-3/src/mobility/model/constant-velocity-helper.o \
./ns-3/src/mobility/model/constant-velocity-mobility-model.o \
./ns-3/src/mobility/model/gauss-markov-mobility-model.o \
./ns-3/src/mobility/model/hierarchical-mobility-model.o \
./ns-3/src/mobility/model/mobility-model.o \
./ns-3/src/mobility/model/position-allocator.o \
./ns-3/src/mobility/model/random-direction-2d-mobility-model.o \
./ns-3/src/mobility/model/random-walk-2d-mobility-model.o \
./ns-3/src/mobility/model/random-waypoint-mobility-model.o \
./ns-3/src/mobility/model/rectangle.o \
./ns-3/src/mobility/model/steady-state-random-waypoint-mobility-model.o \
./ns-3/src/mobility/model/waypoint-mobility-model.o \
./ns-3/src/mobility/model/waypoint.o 

CC_DEPS += \
./ns-3/src/mobility/model/box.d \
./ns-3/src/mobility/model/constant-acceleration-mobility-model.d \
./ns-3/src/mobility/model/constant-position-mobility-model.d \
./ns-3/src/mobility/model/constant-velocity-helper.d \
./ns-3/src/mobility/model/constant-velocity-mobility-model.d \
./ns-3/src/mobility/model/gauss-markov-mobility-model.d \
./ns-3/src/mobility/model/hierarchical-mobility-model.d \
./ns-3/src/mobility/model/mobility-model.d \
./ns-3/src/mobility/model/position-allocator.d \
./ns-3/src/mobility/model/random-direction-2d-mobility-model.d \
./ns-3/src/mobility/model/random-walk-2d-mobility-model.d \
./ns-3/src/mobility/model/random-waypoint-mobility-model.d \
./ns-3/src/mobility/model/rectangle.d \
./ns-3/src/mobility/model/steady-state-random-waypoint-mobility-model.d \
./ns-3/src/mobility/model/waypoint-mobility-model.d \
./ns-3/src/mobility/model/waypoint.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mobility/model/%.o: ../ns-3/src/mobility/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


