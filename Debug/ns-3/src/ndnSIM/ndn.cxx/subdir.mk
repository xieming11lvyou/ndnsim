################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/ndn.cxx/exclude.cc \
../ns-3/src/ndnSIM/ndn.cxx/name-component.cc \
../ns-3/src/ndnSIM/ndn.cxx/name.cc \
../ns-3/src/ndnSIM/ndn.cxx/ndn-api-face.cc 

OBJS += \
./ns-3/src/ndnSIM/ndn.cxx/exclude.o \
./ns-3/src/ndnSIM/ndn.cxx/name-component.o \
./ns-3/src/ndnSIM/ndn.cxx/name.o \
./ns-3/src/ndnSIM/ndn.cxx/ndn-api-face.o 

CC_DEPS += \
./ns-3/src/ndnSIM/ndn.cxx/exclude.d \
./ns-3/src/ndnSIM/ndn.cxx/name-component.d \
./ns-3/src/ndnSIM/ndn.cxx/name.d \
./ns-3/src/ndnSIM/ndn.cxx/ndn-api-face.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/ndn.cxx/%.o: ../ns-3/src/ndnSIM/ndn.cxx/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


