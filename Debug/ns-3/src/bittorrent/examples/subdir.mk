################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/bittorrent/examples/bittorrent-no-story.cc \
../ns-3/src/bittorrent/examples/vodsim-no-realtime.cc \
../ns-3/src/bittorrent/examples/vodsim.cc 

OBJS += \
./ns-3/src/bittorrent/examples/bittorrent-no-story.o \
./ns-3/src/bittorrent/examples/vodsim-no-realtime.o \
./ns-3/src/bittorrent/examples/vodsim.o 

CC_DEPS += \
./ns-3/src/bittorrent/examples/bittorrent-no-story.d \
./ns-3/src/bittorrent/examples/vodsim-no-realtime.d \
./ns-3/src/bittorrent/examples/vodsim.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/bittorrent/examples/%.o: ../ns-3/src/bittorrent/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


