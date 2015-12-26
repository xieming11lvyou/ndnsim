################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mesh/model/mesh-information-element-vector.cc \
../ns-3/src/mesh/model/mesh-l2-routing-protocol.cc \
../ns-3/src/mesh/model/mesh-point-device.cc \
../ns-3/src/mesh/model/mesh-wifi-beacon.cc \
../ns-3/src/mesh/model/mesh-wifi-interface-mac.cc 

OBJS += \
./ns-3/src/mesh/model/mesh-information-element-vector.o \
./ns-3/src/mesh/model/mesh-l2-routing-protocol.o \
./ns-3/src/mesh/model/mesh-point-device.o \
./ns-3/src/mesh/model/mesh-wifi-beacon.o \
./ns-3/src/mesh/model/mesh-wifi-interface-mac.o 

CC_DEPS += \
./ns-3/src/mesh/model/mesh-information-element-vector.d \
./ns-3/src/mesh/model/mesh-l2-routing-protocol.d \
./ns-3/src/mesh/model/mesh-point-device.d \
./ns-3/src/mesh/model/mesh-wifi-beacon.d \
./ns-3/src/mesh/model/mesh-wifi-interface-mac.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mesh/model/%.o: ../ns-3/src/mesh/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


