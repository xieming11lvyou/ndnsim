################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/helper/ndn-app-helper.cc \
../ns-3/src/ndnSIM/helper/ndn-face-container.cc \
../ns-3/src/ndnSIM/helper/ndn-global-routing-helper.cc \
../ns-3/src/ndnSIM/helper/ndn-header-helper.cc \
../ns-3/src/ndnSIM/helper/ndn-link-control-helper.cc \
../ns-3/src/ndnSIM/helper/ndn-stack-helper.cc 

OBJS += \
./ns-3/src/ndnSIM/helper/ndn-app-helper.o \
./ns-3/src/ndnSIM/helper/ndn-face-container.o \
./ns-3/src/ndnSIM/helper/ndn-global-routing-helper.o \
./ns-3/src/ndnSIM/helper/ndn-header-helper.o \
./ns-3/src/ndnSIM/helper/ndn-link-control-helper.o \
./ns-3/src/ndnSIM/helper/ndn-stack-helper.o 

CC_DEPS += \
./ns-3/src/ndnSIM/helper/ndn-app-helper.d \
./ns-3/src/ndnSIM/helper/ndn-face-container.d \
./ns-3/src/ndnSIM/helper/ndn-global-routing-helper.d \
./ns-3/src/ndnSIM/helper/ndn-header-helper.d \
./ns-3/src/ndnSIM/helper/ndn-link-control-helper.d \
./ns-3/src/ndnSIM/helper/ndn-stack-helper.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/helper/%.o: ../ns-3/src/ndnSIM/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


