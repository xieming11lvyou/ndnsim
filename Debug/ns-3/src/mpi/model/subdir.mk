################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mpi/model/distributed-simulator-impl.cc \
../ns-3/src/mpi/model/granted-time-window-mpi-interface.cc \
../ns-3/src/mpi/model/mpi-interface.cc \
../ns-3/src/mpi/model/mpi-receiver.cc \
../ns-3/src/mpi/model/null-message-mpi-interface.cc \
../ns-3/src/mpi/model/null-message-simulator-impl.cc \
../ns-3/src/mpi/model/remote-channel-bundle-manager.cc \
../ns-3/src/mpi/model/remote-channel-bundle.cc 

OBJS += \
./ns-3/src/mpi/model/distributed-simulator-impl.o \
./ns-3/src/mpi/model/granted-time-window-mpi-interface.o \
./ns-3/src/mpi/model/mpi-interface.o \
./ns-3/src/mpi/model/mpi-receiver.o \
./ns-3/src/mpi/model/null-message-mpi-interface.o \
./ns-3/src/mpi/model/null-message-simulator-impl.o \
./ns-3/src/mpi/model/remote-channel-bundle-manager.o \
./ns-3/src/mpi/model/remote-channel-bundle.o 

CC_DEPS += \
./ns-3/src/mpi/model/distributed-simulator-impl.d \
./ns-3/src/mpi/model/granted-time-window-mpi-interface.d \
./ns-3/src/mpi/model/mpi-interface.d \
./ns-3/src/mpi/model/mpi-receiver.d \
./ns-3/src/mpi/model/null-message-mpi-interface.d \
./ns-3/src/mpi/model/null-message-simulator-impl.d \
./ns-3/src/mpi/model/remote-channel-bundle-manager.d \
./ns-3/src/mpi/model/remote-channel-bundle.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mpi/model/%.o: ../ns-3/src/mpi/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


