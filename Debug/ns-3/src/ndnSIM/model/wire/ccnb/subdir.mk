################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb-data.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb-interest.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb.cc 

OBJS += \
./ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb-data.o \
./ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb-interest.o \
./ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb-data.d \
./ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb-interest.d \
./ns-3/src/ndnSIM/model/wire/ccnb/wire-ccnb.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/wire/ccnb/%.o: ../ns-3/src/ndnSIM/model/wire/ccnb/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


