################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/sixlowpan/test/error-channel-sixlow.cc \
../ns-3/src/sixlowpan/test/sixlowpan-fragmentation-test.cc \
../ns-3/src/sixlowpan/test/sixlowpan-hc1-test.cc \
../ns-3/src/sixlowpan/test/sixlowpan-iphc-test.cc 

OBJS += \
./ns-3/src/sixlowpan/test/error-channel-sixlow.o \
./ns-3/src/sixlowpan/test/sixlowpan-fragmentation-test.o \
./ns-3/src/sixlowpan/test/sixlowpan-hc1-test.o \
./ns-3/src/sixlowpan/test/sixlowpan-iphc-test.o 

CC_DEPS += \
./ns-3/src/sixlowpan/test/error-channel-sixlow.d \
./ns-3/src/sixlowpan/test/sixlowpan-fragmentation-test.d \
./ns-3/src/sixlowpan/test/sixlowpan-hc1-test.d \
./ns-3/src/sixlowpan/test/sixlowpan-iphc-test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/sixlowpan/test/%.o: ../ns-3/src/sixlowpan/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


