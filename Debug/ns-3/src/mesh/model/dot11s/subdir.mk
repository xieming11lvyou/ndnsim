################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/mesh/model/dot11s/airtime-metric.cc \
../ns-3/src/mesh/model/dot11s/dot11s-mac-header.cc \
../ns-3/src/mesh/model/dot11s/hwmp-protocol-mac.cc \
../ns-3/src/mesh/model/dot11s/hwmp-protocol.cc \
../ns-3/src/mesh/model/dot11s/hwmp-rtable.cc \
../ns-3/src/mesh/model/dot11s/hwmp-tag.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-beacon-timing.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-configuration.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-id.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-metric-report.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-peer-management.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-peering-protocol.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-perr.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-prep.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-preq.cc \
../ns-3/src/mesh/model/dot11s/ie-dot11s-rann.cc \
../ns-3/src/mesh/model/dot11s/peer-link-frame.cc \
../ns-3/src/mesh/model/dot11s/peer-link.cc \
../ns-3/src/mesh/model/dot11s/peer-management-protocol-mac.cc \
../ns-3/src/mesh/model/dot11s/peer-management-protocol.cc 

OBJS += \
./ns-3/src/mesh/model/dot11s/airtime-metric.o \
./ns-3/src/mesh/model/dot11s/dot11s-mac-header.o \
./ns-3/src/mesh/model/dot11s/hwmp-protocol-mac.o \
./ns-3/src/mesh/model/dot11s/hwmp-protocol.o \
./ns-3/src/mesh/model/dot11s/hwmp-rtable.o \
./ns-3/src/mesh/model/dot11s/hwmp-tag.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-beacon-timing.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-configuration.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-id.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-metric-report.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-peer-management.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-peering-protocol.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-perr.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-prep.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-preq.o \
./ns-3/src/mesh/model/dot11s/ie-dot11s-rann.o \
./ns-3/src/mesh/model/dot11s/peer-link-frame.o \
./ns-3/src/mesh/model/dot11s/peer-link.o \
./ns-3/src/mesh/model/dot11s/peer-management-protocol-mac.o \
./ns-3/src/mesh/model/dot11s/peer-management-protocol.o 

CC_DEPS += \
./ns-3/src/mesh/model/dot11s/airtime-metric.d \
./ns-3/src/mesh/model/dot11s/dot11s-mac-header.d \
./ns-3/src/mesh/model/dot11s/hwmp-protocol-mac.d \
./ns-3/src/mesh/model/dot11s/hwmp-protocol.d \
./ns-3/src/mesh/model/dot11s/hwmp-rtable.d \
./ns-3/src/mesh/model/dot11s/hwmp-tag.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-beacon-timing.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-configuration.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-id.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-metric-report.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-peer-management.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-peering-protocol.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-perr.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-prep.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-preq.d \
./ns-3/src/mesh/model/dot11s/ie-dot11s-rann.d \
./ns-3/src/mesh/model/dot11s/peer-link-frame.d \
./ns-3/src/mesh/model/dot11s/peer-link.d \
./ns-3/src/mesh/model/dot11s/peer-management-protocol-mac.d \
./ns-3/src/mesh/model/dot11s/peer-management-protocol.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/mesh/model/dot11s/%.o: ../ns-3/src/mesh/model/dot11s/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


