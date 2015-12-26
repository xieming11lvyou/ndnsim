################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/plugins/mobility/highway-position-allocator.cc \
../ns-3/src/ndnSIM/plugins/mobility/spring-mobility-helper.cc \
../ns-3/src/ndnSIM/plugins/mobility/spring-mobility-model.cc 

OBJS += \
./ns-3/src/ndnSIM/plugins/mobility/highway-position-allocator.o \
./ns-3/src/ndnSIM/plugins/mobility/spring-mobility-helper.o \
./ns-3/src/ndnSIM/plugins/mobility/spring-mobility-model.o 

CC_DEPS += \
./ns-3/src/ndnSIM/plugins/mobility/highway-position-allocator.d \
./ns-3/src/ndnSIM/plugins/mobility/spring-mobility-helper.d \
./ns-3/src/ndnSIM/plugins/mobility/spring-mobility-model.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/plugins/mobility/%.o: ../ns-3/src/ndnSIM/plugins/mobility/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


