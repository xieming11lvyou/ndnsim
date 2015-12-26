################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/internet/examples/codel-vs-droptail-asymmetric.cc \
../ns-3/src/internet/examples/codel-vs-droptail-basic-test.cc \
../ns-3/src/internet/examples/main-simple.cc 

OBJS += \
./ns-3/src/internet/examples/codel-vs-droptail-asymmetric.o \
./ns-3/src/internet/examples/codel-vs-droptail-basic-test.o \
./ns-3/src/internet/examples/main-simple.o 

CC_DEPS += \
./ns-3/src/internet/examples/codel-vs-droptail-asymmetric.d \
./ns-3/src/internet/examples/codel-vs-droptail-basic-test.d \
./ns-3/src/internet/examples/main-simple.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/internet/examples/%.o: ../ns-3/src/internet/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


