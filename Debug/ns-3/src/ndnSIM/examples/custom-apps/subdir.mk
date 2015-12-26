################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/examples/custom-apps/custom-app.cc \
../ns-3/src/ndnSIM/examples/custom-apps/dumb-requester.cc \
../ns-3/src/ndnSIM/examples/custom-apps/hijacker.cc \
../ns-3/src/ndnSIM/examples/custom-apps/ndn-api-app.cc 

OBJS += \
./ns-3/src/ndnSIM/examples/custom-apps/custom-app.o \
./ns-3/src/ndnSIM/examples/custom-apps/dumb-requester.o \
./ns-3/src/ndnSIM/examples/custom-apps/hijacker.o \
./ns-3/src/ndnSIM/examples/custom-apps/ndn-api-app.o 

CC_DEPS += \
./ns-3/src/ndnSIM/examples/custom-apps/custom-app.d \
./ns-3/src/ndnSIM/examples/custom-apps/dumb-requester.d \
./ns-3/src/ndnSIM/examples/custom-apps/hijacker.d \
./ns-3/src/ndnSIM/examples/custom-apps/ndn-api-app.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/examples/custom-apps/%.o: ../ns-3/src/ndnSIM/examples/custom-apps/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


