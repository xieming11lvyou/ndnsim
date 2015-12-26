################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/utils/tracers/ipv4-app-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ipv4-l3-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ipv4-rate-l3-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ipv4-seqs-app-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/l2-rate-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/l2-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ndn-app-delay-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ndn-cs-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ndn-l3-aggregate-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ndn-l3-rate-tracer.cc \
../ns-3/src/ndnSIM/utils/tracers/ndn-l3-tracer.cc 

OBJS += \
./ns-3/src/ndnSIM/utils/tracers/ipv4-app-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ipv4-l3-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ipv4-rate-l3-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ipv4-seqs-app-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/l2-rate-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/l2-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ndn-app-delay-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ndn-cs-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ndn-l3-aggregate-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ndn-l3-rate-tracer.o \
./ns-3/src/ndnSIM/utils/tracers/ndn-l3-tracer.o 

CC_DEPS += \
./ns-3/src/ndnSIM/utils/tracers/ipv4-app-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ipv4-l3-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ipv4-rate-l3-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ipv4-seqs-app-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/l2-rate-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/l2-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ndn-app-delay-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ndn-cs-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ndn-l3-aggregate-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ndn-l3-rate-tracer.d \
./ns-3/src/ndnSIM/utils/tracers/ndn-l3-tracer.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/utils/tracers/%.o: ../ns-3/src/ndnSIM/utils/tracers/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


