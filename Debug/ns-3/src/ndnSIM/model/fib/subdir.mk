################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/fib/ndn-fib-entry.cc \
../ns-3/src/ndnSIM/model/fib/ndn-fib-impl.cc \
../ns-3/src/ndnSIM/model/fib/ndn-fib.cc 

OBJS += \
./ns-3/src/ndnSIM/model/fib/ndn-fib-entry.o \
./ns-3/src/ndnSIM/model/fib/ndn-fib-impl.o \
./ns-3/src/ndnSIM/model/fib/ndn-fib.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/fib/ndn-fib-entry.d \
./ns-3/src/ndnSIM/model/fib/ndn-fib-impl.d \
./ns-3/src/ndnSIM/model/fib/ndn-fib.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/fib/%.o: ../ns-3/src/ndnSIM/model/fib/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


