################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/test/ns3tcp/ns3tcp-cwnd-test-suite.cc \
../ns-3/src/test/ns3tcp/ns3tcp-interop-test-suite.cc \
../ns-3/src/test/ns3tcp/ns3tcp-loss-test-suite.cc \
../ns-3/src/test/ns3tcp/ns3tcp-no-delay-test-suite.cc \
../ns-3/src/test/ns3tcp/ns3tcp-socket-test-suite.cc \
../ns-3/src/test/ns3tcp/ns3tcp-socket-writer.cc \
../ns-3/src/test/ns3tcp/ns3tcp-state-test-suite.cc \
../ns-3/src/test/ns3tcp/nsctcp-loss-test-suite.cc 

OBJS += \
./ns-3/src/test/ns3tcp/ns3tcp-cwnd-test-suite.o \
./ns-3/src/test/ns3tcp/ns3tcp-interop-test-suite.o \
./ns-3/src/test/ns3tcp/ns3tcp-loss-test-suite.o \
./ns-3/src/test/ns3tcp/ns3tcp-no-delay-test-suite.o \
./ns-3/src/test/ns3tcp/ns3tcp-socket-test-suite.o \
./ns-3/src/test/ns3tcp/ns3tcp-socket-writer.o \
./ns-3/src/test/ns3tcp/ns3tcp-state-test-suite.o \
./ns-3/src/test/ns3tcp/nsctcp-loss-test-suite.o 

CC_DEPS += \
./ns-3/src/test/ns3tcp/ns3tcp-cwnd-test-suite.d \
./ns-3/src/test/ns3tcp/ns3tcp-interop-test-suite.d \
./ns-3/src/test/ns3tcp/ns3tcp-loss-test-suite.d \
./ns-3/src/test/ns3tcp/ns3tcp-no-delay-test-suite.d \
./ns-3/src/test/ns3tcp/ns3tcp-socket-test-suite.d \
./ns-3/src/test/ns3tcp/ns3tcp-socket-writer.d \
./ns-3/src/test/ns3tcp/ns3tcp-state-test-suite.d \
./ns-3/src/test/ns3tcp/nsctcp-loss-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/test/ns3tcp/%.o: ../ns-3/src/test/ns3tcp/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


