################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/visualizer/model/dummy-file-for-static-builds.cc \
../ns-3/src/visualizer/model/pyviz.cc \
../ns-3/src/visualizer/model/visual-simulator-impl.cc 

OBJS += \
./ns-3/src/visualizer/model/dummy-file-for-static-builds.o \
./ns-3/src/visualizer/model/pyviz.o \
./ns-3/src/visualizer/model/visual-simulator-impl.o 

CC_DEPS += \
./ns-3/src/visualizer/model/dummy-file-for-static-builds.d \
./ns-3/src/visualizer/model/pyviz.d \
./ns-3/src/visualizer/model/visual-simulator-impl.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/visualizer/model/%.o: ../ns-3/src/visualizer/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


