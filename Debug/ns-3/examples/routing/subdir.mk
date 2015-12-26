################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/examples/routing/dynamic-global-routing.cc \
../ns-3/examples/routing/global-injection-slash32.cc \
../ns-3/examples/routing/global-routing-slash32.cc \
../ns-3/examples/routing/manet-routing-compare.cc \
../ns-3/examples/routing/mixed-global-routing.cc \
../ns-3/examples/routing/ripng-simple-network.cc \
../ns-3/examples/routing/simple-alternate-routing.cc \
../ns-3/examples/routing/simple-global-routing.cc \
../ns-3/examples/routing/simple-routing-ping6.cc \
../ns-3/examples/routing/static-routing-slash32.cc 

OBJS += \
./ns-3/examples/routing/dynamic-global-routing.o \
./ns-3/examples/routing/global-injection-slash32.o \
./ns-3/examples/routing/global-routing-slash32.o \
./ns-3/examples/routing/manet-routing-compare.o \
./ns-3/examples/routing/mixed-global-routing.o \
./ns-3/examples/routing/ripng-simple-network.o \
./ns-3/examples/routing/simple-alternate-routing.o \
./ns-3/examples/routing/simple-global-routing.o \
./ns-3/examples/routing/simple-routing-ping6.o \
./ns-3/examples/routing/static-routing-slash32.o 

CC_DEPS += \
./ns-3/examples/routing/dynamic-global-routing.d \
./ns-3/examples/routing/global-injection-slash32.d \
./ns-3/examples/routing/global-routing-slash32.d \
./ns-3/examples/routing/manet-routing-compare.d \
./ns-3/examples/routing/mixed-global-routing.d \
./ns-3/examples/routing/ripng-simple-network.d \
./ns-3/examples/routing/simple-alternate-routing.d \
./ns-3/examples/routing/simple-global-routing.d \
./ns-3/examples/routing/simple-routing-ping6.d \
./ns-3/examples/routing/static-routing-slash32.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/examples/routing/%.o: ../ns-3/examples/routing/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


