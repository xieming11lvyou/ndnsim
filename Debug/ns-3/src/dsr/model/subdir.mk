################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/dsr/model/dsr-errorbuff.cc \
../ns-3/src/dsr/model/dsr-fs-header.cc \
../ns-3/src/dsr/model/dsr-gratuitous-reply-table.cc \
../ns-3/src/dsr/model/dsr-maintain-buff.cc \
../ns-3/src/dsr/model/dsr-network-queue.cc \
../ns-3/src/dsr/model/dsr-option-header.cc \
../ns-3/src/dsr/model/dsr-options.cc \
../ns-3/src/dsr/model/dsr-passive-buff.cc \
../ns-3/src/dsr/model/dsr-rcache.cc \
../ns-3/src/dsr/model/dsr-routing.cc \
../ns-3/src/dsr/model/dsr-rreq-table.cc \
../ns-3/src/dsr/model/dsr-rsendbuff.cc 

OBJS += \
./ns-3/src/dsr/model/dsr-errorbuff.o \
./ns-3/src/dsr/model/dsr-fs-header.o \
./ns-3/src/dsr/model/dsr-gratuitous-reply-table.o \
./ns-3/src/dsr/model/dsr-maintain-buff.o \
./ns-3/src/dsr/model/dsr-network-queue.o \
./ns-3/src/dsr/model/dsr-option-header.o \
./ns-3/src/dsr/model/dsr-options.o \
./ns-3/src/dsr/model/dsr-passive-buff.o \
./ns-3/src/dsr/model/dsr-rcache.o \
./ns-3/src/dsr/model/dsr-routing.o \
./ns-3/src/dsr/model/dsr-rreq-table.o \
./ns-3/src/dsr/model/dsr-rsendbuff.o 

CC_DEPS += \
./ns-3/src/dsr/model/dsr-errorbuff.d \
./ns-3/src/dsr/model/dsr-fs-header.d \
./ns-3/src/dsr/model/dsr-gratuitous-reply-table.d \
./ns-3/src/dsr/model/dsr-maintain-buff.d \
./ns-3/src/dsr/model/dsr-network-queue.d \
./ns-3/src/dsr/model/dsr-option-header.d \
./ns-3/src/dsr/model/dsr-options.d \
./ns-3/src/dsr/model/dsr-passive-buff.d \
./ns-3/src/dsr/model/dsr-rcache.d \
./ns-3/src/dsr/model/dsr-routing.d \
./ns-3/src/dsr/model/dsr-rreq-table.d \
./ns-3/src/dsr/model/dsr-rsendbuff.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/dsr/model/%.o: ../ns-3/src/dsr/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


