################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-aggregate-app-tracer.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-aggregate-l3-tracer.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-app-tracer.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-consumer-window-tracer.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-l3-tracer.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-path-weight-tracer.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-rate-l3-tracer.cc \
../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-seqs-app-tracer.cc 

OBJS += \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-aggregate-app-tracer.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-aggregate-l3-tracer.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-app-tracer.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-consumer-window-tracer.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-l3-tracer.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-path-weight-tracer.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-rate-l3-tracer.o \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-seqs-app-tracer.o 

CC_DEPS += \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-aggregate-app-tracer.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-aggregate-l3-tracer.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-app-tracer.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-consumer-window-tracer.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-l3-tracer.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-path-weight-tracer.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-rate-l3-tracer.d \
./ns-3/src/ndnSIM/plugins/tracers-broken/tracers/ccnx-seqs-app-tracer.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/plugins/tracers-broken/tracers/%.o: ../ns-3/src/ndnSIM/plugins/tracers-broken/tracers/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


