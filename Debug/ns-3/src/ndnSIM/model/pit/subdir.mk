################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/pit/ndn-pit-entry-incoming-face.cc \
../ns-3/src/ndnSIM/model/pit/ndn-pit-entry-outgoing-face.cc \
../ns-3/src/ndnSIM/model/pit/ndn-pit-entry.cc \
../ns-3/src/ndnSIM/model/pit/ndn-pit-impl.cc \
../ns-3/src/ndnSIM/model/pit/ndn-pit.cc 

OBJS += \
./ns-3/src/ndnSIM/model/pit/ndn-pit-entry-incoming-face.o \
./ns-3/src/ndnSIM/model/pit/ndn-pit-entry-outgoing-face.o \
./ns-3/src/ndnSIM/model/pit/ndn-pit-entry.o \
./ns-3/src/ndnSIM/model/pit/ndn-pit-impl.o \
./ns-3/src/ndnSIM/model/pit/ndn-pit.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/pit/ndn-pit-entry-incoming-face.d \
./ns-3/src/ndnSIM/model/pit/ndn-pit-entry-outgoing-face.d \
./ns-3/src/ndnSIM/model/pit/ndn-pit-entry.d \
./ns-3/src/ndnSIM/model/pit/ndn-pit-impl.d \
./ns-3/src/ndnSIM/model/pit/ndn-pit.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/pit/%.o: ../ns-3/src/ndnSIM/model/pit/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


