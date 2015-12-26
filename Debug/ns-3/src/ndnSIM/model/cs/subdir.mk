################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/cs/content-store-impl.cc \
../ns-3/src/ndnSIM/model/cs/content-store-nocache.cc \
../ns-3/src/ndnSIM/model/cs/content-store-with-freshness.cc \
../ns-3/src/ndnSIM/model/cs/content-store-with-probability.cc \
../ns-3/src/ndnSIM/model/cs/content-store-with-stats.cc \
../ns-3/src/ndnSIM/model/cs/ndn-content-store.cc 

OBJS += \
./ns-3/src/ndnSIM/model/cs/content-store-impl.o \
./ns-3/src/ndnSIM/model/cs/content-store-nocache.o \
./ns-3/src/ndnSIM/model/cs/content-store-with-freshness.o \
./ns-3/src/ndnSIM/model/cs/content-store-with-probability.o \
./ns-3/src/ndnSIM/model/cs/content-store-with-stats.o \
./ns-3/src/ndnSIM/model/cs/ndn-content-store.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/cs/content-store-impl.d \
./ns-3/src/ndnSIM/model/cs/content-store-nocache.d \
./ns-3/src/ndnSIM/model/cs/content-store-with-freshness.d \
./ns-3/src/ndnSIM/model/cs/content-store-with-probability.d \
./ns-3/src/ndnSIM/model/cs/content-store-with-stats.d \
./ns-3/src/ndnSIM/model/cs/ndn-content-store.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/cs/%.o: ../ns-3/src/ndnSIM/model/cs/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


