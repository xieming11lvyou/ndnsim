################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/antenna/test/test-angles.cc \
../ns-3/src/antenna/test/test-cosine-antenna.cc \
../ns-3/src/antenna/test/test-degrees-radians.cc \
../ns-3/src/antenna/test/test-isotropic-antenna.cc \
../ns-3/src/antenna/test/test-parabolic-antenna.cc 

OBJS += \
./ns-3/src/antenna/test/test-angles.o \
./ns-3/src/antenna/test/test-cosine-antenna.o \
./ns-3/src/antenna/test/test-degrees-radians.o \
./ns-3/src/antenna/test/test-isotropic-antenna.o \
./ns-3/src/antenna/test/test-parabolic-antenna.o 

CC_DEPS += \
./ns-3/src/antenna/test/test-angles.d \
./ns-3/src/antenna/test/test-cosine-antenna.d \
./ns-3/src/antenna/test/test-degrees-radians.d \
./ns-3/src/antenna/test/test-isotropic-antenna.d \
./ns-3/src/antenna/test/test-parabolic-antenna.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/antenna/test/%.o: ../ns-3/src/antenna/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


