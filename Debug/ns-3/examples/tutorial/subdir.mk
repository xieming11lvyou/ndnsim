################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/tutorial/fifth.cc \
../ns-3/examples/tutorial/first.cc \
../ns-3/examples/tutorial/fourth.cc \
../ns-3/examples/tutorial/hello-simulator.cc \
../ns-3/examples/tutorial/second.cc \
../ns-3/examples/tutorial/seventh.cc \
../ns-3/examples/tutorial/sixth.cc \
../ns-3/examples/tutorial/third.cc 

OBJS += \
./ns-3/examples/tutorial/fifth.o \
./ns-3/examples/tutorial/first.o \
./ns-3/examples/tutorial/fourth.o \
./ns-3/examples/tutorial/hello-simulator.o \
./ns-3/examples/tutorial/second.o \
./ns-3/examples/tutorial/seventh.o \
./ns-3/examples/tutorial/sixth.o \
./ns-3/examples/tutorial/third.o 

CC_DEPS += \
./ns-3/examples/tutorial/fifth.d \
./ns-3/examples/tutorial/first.d \
./ns-3/examples/tutorial/fourth.d \
./ns-3/examples/tutorial/hello-simulator.d \
./ns-3/examples/tutorial/second.d \
./ns-3/examples/tutorial/seventh.d \
./ns-3/examples/tutorial/sixth.d \
./ns-3/examples/tutorial/third.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/tutorial/%.o: ../ns-3/examples/tutorial/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


