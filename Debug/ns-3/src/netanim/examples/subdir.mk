################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/netanim/examples/dumbbell-animation.cc \
../ns-3/src/netanim/examples/dynamic_linknode.cc \
../ns-3/src/netanim/examples/grid-animation.cc \
../ns-3/src/netanim/examples/resources_demo.cc \
../ns-3/src/netanim/examples/star-animation.cc \
../ns-3/src/netanim/examples/uan-animation.cc \
../ns-3/src/netanim/examples/wireless-animation.cc 

OBJS += \
./ns-3/src/netanim/examples/dumbbell-animation.o \
./ns-3/src/netanim/examples/dynamic_linknode.o \
./ns-3/src/netanim/examples/grid-animation.o \
./ns-3/src/netanim/examples/resources_demo.o \
./ns-3/src/netanim/examples/star-animation.o \
./ns-3/src/netanim/examples/uan-animation.o \
./ns-3/src/netanim/examples/wireless-animation.o 

CC_DEPS += \
./ns-3/src/netanim/examples/dumbbell-animation.d \
./ns-3/src/netanim/examples/dynamic_linknode.d \
./ns-3/src/netanim/examples/grid-animation.d \
./ns-3/src/netanim/examples/resources_demo.d \
./ns-3/src/netanim/examples/star-animation.d \
./ns-3/src/netanim/examples/uan-animation.d \
./ns-3/src/netanim/examples/wireless-animation.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/netanim/examples/%.o: ../ns-3/src/netanim/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


