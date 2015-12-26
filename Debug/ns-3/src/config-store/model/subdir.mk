################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/config-store/model/attribute-default-iterator.cc \
../ns-3/src/config-store/model/attribute-iterator.cc \
../ns-3/src/config-store/model/config-store.cc \
../ns-3/src/config-store/model/display-functions.cc \
../ns-3/src/config-store/model/file-config.cc \
../ns-3/src/config-store/model/gtk-config-store.cc \
../ns-3/src/config-store/model/model-node-creator.cc \
../ns-3/src/config-store/model/model-typeid-creator.cc \
../ns-3/src/config-store/model/raw-text-config.cc \
../ns-3/src/config-store/model/xml-config.cc 

OBJS += \
./ns-3/src/config-store/model/attribute-default-iterator.o \
./ns-3/src/config-store/model/attribute-iterator.o \
./ns-3/src/config-store/model/config-store.o \
./ns-3/src/config-store/model/display-functions.o \
./ns-3/src/config-store/model/file-config.o \
./ns-3/src/config-store/model/gtk-config-store.o \
./ns-3/src/config-store/model/model-node-creator.o \
./ns-3/src/config-store/model/model-typeid-creator.o \
./ns-3/src/config-store/model/raw-text-config.o \
./ns-3/src/config-store/model/xml-config.o 

CC_DEPS += \
./ns-3/src/config-store/model/attribute-default-iterator.d \
./ns-3/src/config-store/model/attribute-iterator.d \
./ns-3/src/config-store/model/config-store.d \
./ns-3/src/config-store/model/display-functions.d \
./ns-3/src/config-store/model/file-config.d \
./ns-3/src/config-store/model/gtk-config-store.d \
./ns-3/src/config-store/model/model-node-creator.d \
./ns-3/src/config-store/model/model-typeid-creator.d \
./ns-3/src/config-store/model/raw-text-config.d \
./ns-3/src/config-store/model/xml-config.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/config-store/model/%.o: ../ns-3/src/config-store/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


