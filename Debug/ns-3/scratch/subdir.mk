################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/scratch/distance-test.cc \
../ns-3/scratch/final.cc \
../ns-3/scratch/first.cc \
../ns-3/scratch/ndn-another-test.cc \
../ns-3/scratch/ndn-origin-test.cc \
../ns-3/scratch/ndn-wifi-test.cc \
../ns-3/scratch/random-array.cc \
../ns-3/scratch/scratch-simulator.cc 

OBJS += \
./ns-3/scratch/distance-test.o \
./ns-3/scratch/final.o \
./ns-3/scratch/first.o \
./ns-3/scratch/ndn-another-test.o \
./ns-3/scratch/ndn-origin-test.o \
./ns-3/scratch/ndn-wifi-test.o \
./ns-3/scratch/random-array.o \
./ns-3/scratch/scratch-simulator.o 

CC_DEPS += \
./ns-3/scratch/distance-test.d \
./ns-3/scratch/final.d \
./ns-3/scratch/first.d \
./ns-3/scratch/ndn-another-test.d \
./ns-3/scratch/ndn-origin-test.d \
./ns-3/scratch/ndn-wifi-test.d \
./ns-3/scratch/random-array.d \
./ns-3/scratch/scratch-simulator.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/scratch/%.o: ../ns-3/scratch/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


