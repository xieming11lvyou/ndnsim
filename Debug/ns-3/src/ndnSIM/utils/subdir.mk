################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/utils/batches.cc \
../ns-3/src/ndnSIM/utils/ndn-fw-hop-count-tag.cc \
../ns-3/src/ndnSIM/utils/ndn-limits-rate.cc \
../ns-3/src/ndnSIM/utils/ndn-limits-window.cc \
../ns-3/src/ndnSIM/utils/ndn-limits.cc \
../ns-3/src/ndnSIM/utils/ndn-local-info-tag.cc \
../ns-3/src/ndnSIM/utils/ndn-rtt-estimator.cc \
../ns-3/src/ndnSIM/utils/ndn-rtt-mean-deviation.cc 

OBJS += \
./ns-3/src/ndnSIM/utils/batches.o \
./ns-3/src/ndnSIM/utils/ndn-fw-hop-count-tag.o \
./ns-3/src/ndnSIM/utils/ndn-limits-rate.o \
./ns-3/src/ndnSIM/utils/ndn-limits-window.o \
./ns-3/src/ndnSIM/utils/ndn-limits.o \
./ns-3/src/ndnSIM/utils/ndn-local-info-tag.o \
./ns-3/src/ndnSIM/utils/ndn-rtt-estimator.o \
./ns-3/src/ndnSIM/utils/ndn-rtt-mean-deviation.o 

CC_DEPS += \
./ns-3/src/ndnSIM/utils/batches.d \
./ns-3/src/ndnSIM/utils/ndn-fw-hop-count-tag.d \
./ns-3/src/ndnSIM/utils/ndn-limits-rate.d \
./ns-3/src/ndnSIM/utils/ndn-limits-window.d \
./ns-3/src/ndnSIM/utils/ndn-limits.d \
./ns-3/src/ndnSIM/utils/ndn-local-info-tag.d \
./ns-3/src/ndnSIM/utils/ndn-rtt-estimator.d \
./ns-3/src/ndnSIM/utils/ndn-rtt-mean-deviation.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/utils/%.o: ../ns-3/src/ndnSIM/utils/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


