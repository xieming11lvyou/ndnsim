################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/plugins/tracers-broken/ccnx-trace-helper.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/weights-path-stretch-tag.cc 

OBJS += \
./ns-3/src/ndnSIM/plugins/tracers-broken/ccnx-trace-helper.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/weights-path-stretch-tag.o 

CC_DEPS += \
./ns-3/src/ndnSIM/plugins/tracers-broken/ccnx-trace-helper.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/weights-path-stretch-tag.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/plugins/tracers-broken/%.o: ../ns-3/src/ndnSIM/plugins/tracers-broken/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


