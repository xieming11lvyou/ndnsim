################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/attr.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/blob.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/block.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/dattr.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/dtag.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/ext.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/tag.cc \
../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/udata.cc 

OBJS += \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/attr.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/blob.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/block.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/dattr.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/dtag.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/ext.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/tag.o \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/udata.o 

CC_DEPS += \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/attr.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/blob.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/block.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/dattr.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/dtag.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/ext.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/tag.d \
./ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/udata.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/%.o: ../ns-3/src/ndnSIM/model/wire/ccnb/ccnb-parser/syntax-tree/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


