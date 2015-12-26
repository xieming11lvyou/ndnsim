################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/content-type-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/depth-first-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/name-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/no-argu-depth-first-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/non-negative-integer-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/string-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/timestamp-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/uint32t-blob-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/void-depth-first-visitor.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/void-no-argu-depth-first-visitor.cc 

OBJS += \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/content-type-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/depth-first-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/name-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/no-argu-depth-first-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/non-negative-integer-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/string-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/timestamp-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/uint32t-blob-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/void-depth-first-visitor.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/void-no-argu-depth-first-visitor.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/content-type-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/depth-first-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/name-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/no-argu-depth-first-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/non-negative-integer-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/string-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/timestamp-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/uint32t-blob-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/void-depth-first-visitor.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/void-no-argu-depth-first-visitor.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/%.o: ../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/visitors/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


