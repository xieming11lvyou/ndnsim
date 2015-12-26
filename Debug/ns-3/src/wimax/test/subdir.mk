################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/wimax/test/mac-messages-test.cc \
../ns-3/src/wimax/test/phy-test.cc \
../ns-3/src/wimax/test/qos-test.cc \
../ns-3/src/wimax/test/ss-mac-test.cc \
../ns-3/src/wimax/test/wimax-fragmentation-test.cc \
../ns-3/src/wimax/test/wimax-service-flow-test.cc \
../ns-3/src/wimax/test/wimax-tlv-test.cc 

OBJS += \
./ns-3/src/wimax/test/mac-messages-test.o \
./ns-3/src/wimax/test/phy-test.o \
./ns-3/src/wimax/test/qos-test.o \
./ns-3/src/wimax/test/ss-mac-test.o \
./ns-3/src/wimax/test/wimax-fragmentation-test.o \
./ns-3/src/wimax/test/wimax-service-flow-test.o \
./ns-3/src/wimax/test/wimax-tlv-test.o 

CC_DEPS += \
./ns-3/src/wimax/test/mac-messages-test.d \
./ns-3/src/wimax/test/phy-test.d \
./ns-3/src/wimax/test/qos-test.d \
./ns-3/src/wimax/test/ss-mac-test.d \
./ns-3/src/wimax/test/wimax-fragmentation-test.d \
./ns-3/src/wimax/test/wimax-service-flow-test.d \
./ns-3/src/wimax/test/wimax-tlv-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/wimax/test/%.o: ../ns-3/src/wimax/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


