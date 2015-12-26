################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/tcp/star.cc \
../ns-3/examples/tcp/tcp-bulk-send.cc \
../ns-3/examples/tcp/tcp-large-transfer.cc \
../ns-3/examples/tcp/tcp-nsc-comparison.cc \
../ns-3/examples/tcp/tcp-nsc-lfn.cc \
../ns-3/examples/tcp/tcp-nsc-zoo.cc \
../ns-3/examples/tcp/tcp-star-server.cc \
../ns-3/examples/tcp/tcp-variants-comparison.cc 

OBJS += \
./ns-3/examples/tcp/star.o \
./ns-3/examples/tcp/tcp-bulk-send.o \
./ns-3/examples/tcp/tcp-large-transfer.o \
./ns-3/examples/tcp/tcp-nsc-comparison.o \
./ns-3/examples/tcp/tcp-nsc-lfn.o \
./ns-3/examples/tcp/tcp-nsc-zoo.o \
./ns-3/examples/tcp/tcp-star-server.o \
./ns-3/examples/tcp/tcp-variants-comparison.o 

CC_DEPS += \
./ns-3/examples/tcp/star.d \
./ns-3/examples/tcp/tcp-bulk-send.d \
./ns-3/examples/tcp/tcp-large-transfer.d \
./ns-3/examples/tcp/tcp-nsc-comparison.d \
./ns-3/examples/tcp/tcp-nsc-lfn.d \
./ns-3/examples/tcp/tcp-nsc-zoo.d \
./ns-3/examples/tcp/tcp-star-server.d \
./ns-3/examples/tcp/tcp-variants-comparison.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/tcp/%.o: ../ns-3/examples/tcp/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


