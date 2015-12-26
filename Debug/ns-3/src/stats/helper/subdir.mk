################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/stats/helper/file-helper.cc \
../ns-3/src/stats/helper/gnuplot-helper.cc 

OBJS += \
./ns-3/src/stats/helper/file-helper.o \
./ns-3/src/stats/helper/gnuplot-helper.o 

CC_DEPS += \
./ns-3/src/stats/helper/file-helper.d \
./ns-3/src/stats/helper/gnuplot-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/stats/helper/%.o: ../ns-3/src/stats/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


