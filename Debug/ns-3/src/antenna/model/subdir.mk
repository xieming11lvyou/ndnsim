################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/antenna/model/angles.cc \
../ns-3/src/antenna/model/antenna-model.cc \
../ns-3/src/antenna/model/cosine-antenna-model.cc \
../ns-3/src/antenna/model/isotropic-antenna-model.cc \
../ns-3/src/antenna/model/parabolic-antenna-model.cc 

OBJS += \
./ns-3/src/antenna/model/angles.o \
./ns-3/src/antenna/model/antenna-model.o \
./ns-3/src/antenna/model/cosine-antenna-model.o \
./ns-3/src/antenna/model/isotropic-antenna-model.o \
./ns-3/src/antenna/model/parabolic-antenna-model.o 

CC_DEPS += \
./ns-3/src/antenna/model/angles.d \
./ns-3/src/antenna/model/antenna-model.d \
./ns-3/src/antenna/model/cosine-antenna-model.d \
./ns-3/src/antenna/model/isotropic-antenna-model.d \
./ns-3/src/antenna/model/parabolic-antenna-model.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/antenna/model/%.o: ../ns-3/src/antenna/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


