################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../pybindgen/tests/c-hello/hello.c 

OBJS += \
./pybindgen/tests/c-hello/hello.o 

C_DEPS += \
./pybindgen/tests/c-hello/hello.d 


# Each subdirectory must supply rules for building sources it contributes
pybindgen/tests/c-hello/%.o: ../pybindgen/tests/c-hello/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


