################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/plugins/ip-faces/ndn-ip-face-stack.cc \
../ns-3/src/ndnSIM/plugins/ip-faces/ndn-ip-faces-helper.cc \
../ns-3/src/ndnSIM/plugins/ip-faces/ndn-tcp-face.cc \
../ns-3/src/ndnSIM/plugins/ip-faces/ndn-udp-face.cc 

OBJS += \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-ip-face-stack.o \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-ip-faces-helper.o \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-tcp-face.o \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-udp-face.o 

CC_DEPS += \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-ip-face-stack.d \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-ip-faces-helper.d \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-tcp-face.d \
./ns-3/src/ndnSIM/plugins/ip-faces/ndn-udp-face.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/plugins/ip-faces/%.o: ../ns-3/src/ndnSIM/plugins/ip-faces/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


