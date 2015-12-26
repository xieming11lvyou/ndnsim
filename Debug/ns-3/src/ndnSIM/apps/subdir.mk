################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/apps/callback-based-app.cc \
../ns-3/src/ndnSIM/apps/ndn-app.cc \
../ns-3/src/ndnSIM/apps/ndn-consumer-batches.cc \
../ns-3/src/ndnSIM/apps/ndn-consumer-cbr.cc \
../ns-3/src/ndnSIM/apps/ndn-consumer-window.cc \
../ns-3/src/ndnSIM/apps/ndn-consumer-zipf-mandelbrot.cc \
../ns-3/src/ndnSIM/apps/ndn-consumer.cc \
../ns-3/src/ndnSIM/apps/ndn-producer.cc 

OBJS += \
./ns-3/src/ndnSIM/apps/callback-based-app.o \
./ns-3/src/ndnSIM/apps/ndn-app.o \
./ns-3/src/ndnSIM/apps/ndn-consumer-batches.o \
./ns-3/src/ndnSIM/apps/ndn-consumer-cbr.o \
./ns-3/src/ndnSIM/apps/ndn-consumer-window.o \
./ns-3/src/ndnSIM/apps/ndn-consumer-zipf-mandelbrot.o \
./ns-3/src/ndnSIM/apps/ndn-consumer.o \
./ns-3/src/ndnSIM/apps/ndn-producer.o 

CC_DEPS += \
./ns-3/src/ndnSIM/apps/callback-based-app.d \
./ns-3/src/ndnSIM/apps/ndn-app.d \
./ns-3/src/ndnSIM/apps/ndn-consumer-batches.d \
./ns-3/src/ndnSIM/apps/ndn-consumer-cbr.d \
./ns-3/src/ndnSIM/apps/ndn-consumer-window.d \
./ns-3/src/ndnSIM/apps/ndn-consumer-zipf-mandelbrot.d \
./ns-3/src/ndnSIM/apps/ndn-consumer.d \
./ns-3/src/ndnSIM/apps/ndn-producer.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/apps/%.o: ../ns-3/src/ndnSIM/apps/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


