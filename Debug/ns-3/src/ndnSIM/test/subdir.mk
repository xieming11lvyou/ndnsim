################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/ndnSIM/test/ndnSIM-api.cc \
../ns-3/src/ndnSIM/test/ndnSIM-fib-entry.cc \
../ns-3/src/ndnSIM/test/ndnSIM-pit.cc \
../ns-3/src/ndnSIM/test/ndnSIM-serialization.cc \
../ns-3/src/ndnSIM/test/ndnSIM-tests.cc \
../ns-3/src/ndnSIM/test/ndnSIM-trie.cc 

OBJS += \
./ns-3/src/ndnSIM/test/ndnSIM-api.o \
./ns-3/src/ndnSIM/test/ndnSIM-fib-entry.o \
./ns-3/src/ndnSIM/test/ndnSIM-pit.o \
./ns-3/src/ndnSIM/test/ndnSIM-serialization.o \
./ns-3/src/ndnSIM/test/ndnSIM-tests.o \
./ns-3/src/ndnSIM/test/ndnSIM-trie.o 

CC_DEPS += \
./ns-3/src/ndnSIM/test/ndnSIM-api.d \
./ns-3/src/ndnSIM/test/ndnSIM-fib-entry.d \
./ns-3/src/ndnSIM/test/ndnSIM-pit.d \
./ns-3/src/ndnSIM/test/ndnSIM-serialization.d \
./ns-3/src/ndnSIM/test/ndnSIM-tests.d \
./ns-3/src/ndnSIM/test/ndnSIM-trie.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/ndnSIM/test/%.o: ../ns-3/src/ndnSIM/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


