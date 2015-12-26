################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ns-3/build/.conf_check_48955ac54d2136a572805353a7b9b62d/test.cpp 

OBJS += \
./ns-3/build/.conf_check_48955ac54d2136a572805353a7b9b62d/test.o 

CPP_DEPS += \
./ns-3/build/.conf_check_48955ac54d2136a572805353a7b9b62d/test.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/build/.conf_check_48955ac54d2136a572805353a7b9b62d/%.o: ../ns-3/build/.conf_check_48955ac54d2136a572805353a7b9b62d/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


