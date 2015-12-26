################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mesh/model/flame/flame-header.cc \
../ns-3/src/mesh/model/flame/flame-protocol-mac.cc \
../ns-3/src/mesh/model/flame/flame-protocol.cc \
../ns-3/src/mesh/model/flame/flame-rtable.cc 

OBJS += \
./ns-3/src/mesh/model/flame/flame-header.o \
./ns-3/src/mesh/model/flame/flame-protocol-mac.o \
./ns-3/src/mesh/model/flame/flame-protocol.o \
./ns-3/src/mesh/model/flame/flame-rtable.o 

CC_DEPS += \
./ns-3/src/mesh/model/flame/flame-header.d \
./ns-3/src/mesh/model/flame/flame-protocol-mac.d \
./ns-3/src/mesh/model/flame/flame-protocol.d \
./ns-3/src/mesh/model/flame/flame-rtable.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mesh/model/flame/%.o: ../ns-3/src/mesh/model/flame/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


