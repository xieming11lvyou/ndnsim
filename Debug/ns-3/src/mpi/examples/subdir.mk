################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mpi/examples/nms-p2p-nix-distributed.cc \
../ns-3/src/mpi/examples/simple-distributed-empty-node.cc \
../ns-3/src/mpi/examples/simple-distributed.cc \
../ns-3/src/mpi/examples/third-distributed.cc 

OBJS += \
./ns-3/src/mpi/examples/nms-p2p-nix-distributed.o \
./ns-3/src/mpi/examples/simple-distributed-empty-node.o \
./ns-3/src/mpi/examples/simple-distributed.o \
./ns-3/src/mpi/examples/third-distributed.o 

CC_DEPS += \
./ns-3/src/mpi/examples/nms-p2p-nix-distributed.d \
./ns-3/src/mpi/examples/simple-distributed-empty-node.d \
./ns-3/src/mpi/examples/simple-distributed.d \
./ns-3/src/mpi/examples/third-distributed.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mpi/examples/%.o: ../ns-3/src/mpi/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


