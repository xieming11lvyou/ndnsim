################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/ndn-app-face.cc \
../ns-3/src/ndnSIM/model/ndn-data.cc \
../ns-3/src/ndnSIM/model/ndn-face.cc \
../ns-3/src/ndnSIM/model/ndn-global-router.cc \
../ns-3/src/ndnSIM/model/ndn-interest.cc \
../ns-3/src/ndnSIM/model/ndn-l3-protocol.cc \
../ns-3/src/ndnSIM/model/ndn-net-device-face.cc 

OBJS += \
./ns-3/src/ndnSIM/model/ndn-app-face.o \
./ns-3/src/ndnSIM/model/ndn-data.o \
./ns-3/src/ndnSIM/model/ndn-face.o \
./ns-3/src/ndnSIM/model/ndn-global-router.o \
./ns-3/src/ndnSIM/model/ndn-interest.o \
./ns-3/src/ndnSIM/model/ndn-l3-protocol.o \
./ns-3/src/ndnSIM/model/ndn-net-device-face.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/ndn-app-face.d \
./ns-3/src/ndnSIM/model/ndn-data.d \
./ns-3/src/ndnSIM/model/ndn-face.d \
./ns-3/src/ndnSIM/model/ndn-global-router.d \
./ns-3/src/ndnSIM/model/ndn-interest.d \
./ns-3/src/ndnSIM/model/ndn-l3-protocol.d \
./ns-3/src/ndnSIM/model/ndn-net-device-face.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/%.o: ../ns-3/src/ndnSIM/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


