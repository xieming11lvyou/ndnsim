################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/fw/best-route.cc \
../ns-3/src/ndnSIM/model/fw/flooding.cc \
../ns-3/src/ndnSIM/model/fw/green-yellow-red.cc \
../ns-3/src/ndnSIM/model/fw/nacks.cc \
../ns-3/src/ndnSIM/model/fw/ndn-forwarding-strategy.cc \
../ns-3/src/ndnSIM/model/fw/per-fib-limits.cc \
../ns-3/src/ndnSIM/model/fw/per-out-face-limits.cc \
../ns-3/src/ndnSIM/model/fw/smart-flooding.cc 

OBJS += \
./ns-3/src/ndnSIM/model/fw/best-route.o \
./ns-3/src/ndnSIM/model/fw/flooding.o \
./ns-3/src/ndnSIM/model/fw/green-yellow-red.o \
./ns-3/src/ndnSIM/model/fw/nacks.o \
./ns-3/src/ndnSIM/model/fw/ndn-forwarding-strategy.o \
./ns-3/src/ndnSIM/model/fw/per-fib-limits.o \
./ns-3/src/ndnSIM/model/fw/per-out-face-limits.o \
./ns-3/src/ndnSIM/model/fw/smart-flooding.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/fw/best-route.d \
./ns-3/src/ndnSIM/model/fw/flooding.d \
./ns-3/src/ndnSIM/model/fw/green-yellow-red.d \
./ns-3/src/ndnSIM/model/fw/nacks.d \
./ns-3/src/ndnSIM/model/fw/ndn-forwarding-strategy.d \
./ns-3/src/ndnSIM/model/fw/per-fib-limits.d \
./ns-3/src/ndnSIM/model/fw/per-out-face-limits.d \
./ns-3/src/ndnSIM/model/fw/smart-flooding.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/fw/%.o: ../ns-3/src/ndnSIM/model/fw/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


