################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/wimax/model/bandwidth-manager.cc \
../ns-3/src/wimax/model/bs-link-manager.cc \
../ns-3/src/wimax/model/bs-net-device.cc \
../ns-3/src/wimax/model/bs-scheduler-rtps.cc \
../ns-3/src/wimax/model/bs-scheduler-simple.cc \
../ns-3/src/wimax/model/bs-scheduler.cc \
../ns-3/src/wimax/model/bs-service-flow-manager.cc \
../ns-3/src/wimax/model/bs-uplink-scheduler-mbqos.cc \
../ns-3/src/wimax/model/bs-uplink-scheduler-rtps.cc \
../ns-3/src/wimax/model/bs-uplink-scheduler-simple.cc \
../ns-3/src/wimax/model/bs-uplink-scheduler.cc \
../ns-3/src/wimax/model/burst-profile-manager.cc \
../ns-3/src/wimax/model/cid-factory.cc \
../ns-3/src/wimax/model/cid.cc \
../ns-3/src/wimax/model/connection-manager.cc \
../ns-3/src/wimax/model/crc8.cc \
../ns-3/src/wimax/model/cs-parameters.cc \
../ns-3/src/wimax/model/dl-mac-messages.cc \
../ns-3/src/wimax/model/ipcs-classifier-record.cc \
../ns-3/src/wimax/model/ipcs-classifier.cc \
../ns-3/src/wimax/model/mac-messages.cc \
../ns-3/src/wimax/model/ofdm-downlink-frame-prefix.cc \
../ns-3/src/wimax/model/send-params.cc \
../ns-3/src/wimax/model/service-flow-manager.cc \
../ns-3/src/wimax/model/service-flow-record.cc \
../ns-3/src/wimax/model/service-flow.cc \
../ns-3/src/wimax/model/simple-ofdm-send-param.cc \
../ns-3/src/wimax/model/simple-ofdm-wimax-channel.cc \
../ns-3/src/wimax/model/simple-ofdm-wimax-phy.cc \
../ns-3/src/wimax/model/snr-to-block-error-rate-manager.cc \
../ns-3/src/wimax/model/snr-to-block-error-rate-record.cc \
../ns-3/src/wimax/model/ss-link-manager.cc \
../ns-3/src/wimax/model/ss-manager.cc \
../ns-3/src/wimax/model/ss-net-device.cc \
../ns-3/src/wimax/model/ss-record.cc \
../ns-3/src/wimax/model/ss-scheduler.cc \
../ns-3/src/wimax/model/ss-service-flow-manager.cc \
../ns-3/src/wimax/model/ul-job.cc \
../ns-3/src/wimax/model/ul-mac-messages.cc \
../ns-3/src/wimax/model/wimax-channel.cc \
../ns-3/src/wimax/model/wimax-connection.cc \
../ns-3/src/wimax/model/wimax-mac-header.cc \
../ns-3/src/wimax/model/wimax-mac-queue.cc \
../ns-3/src/wimax/model/wimax-mac-to-mac-header.cc \
../ns-3/src/wimax/model/wimax-net-device.cc \
../ns-3/src/wimax/model/wimax-phy.cc \
../ns-3/src/wimax/model/wimax-tlv.cc 

OBJS += \
./ns-3/src/wimax/model/bandwidth-manager.o \
./ns-3/src/wimax/model/bs-link-manager.o \
./ns-3/src/wimax/model/bs-net-device.o \
./ns-3/src/wimax/model/bs-scheduler-rtps.o \
./ns-3/src/wimax/model/bs-scheduler-simple.o \
./ns-3/src/wimax/model/bs-scheduler.o \
./ns-3/src/wimax/model/bs-service-flow-manager.o \
./ns-3/src/wimax/model/bs-uplink-scheduler-mbqos.o \
./ns-3/src/wimax/model/bs-uplink-scheduler-rtps.o \
./ns-3/src/wimax/model/bs-uplink-scheduler-simple.o \
./ns-3/src/wimax/model/bs-uplink-scheduler.o \
./ns-3/src/wimax/model/burst-profile-manager.o \
./ns-3/src/wimax/model/cid-factory.o \
./ns-3/src/wimax/model/cid.o \
./ns-3/src/wimax/model/connection-manager.o \
./ns-3/src/wimax/model/crc8.o \
./ns-3/src/wimax/model/cs-parameters.o \
./ns-3/src/wimax/model/dl-mac-messages.o \
./ns-3/src/wimax/model/ipcs-classifier-record.o \
./ns-3/src/wimax/model/ipcs-classifier.o \
./ns-3/src/wimax/model/mac-messages.o \
./ns-3/src/wimax/model/ofdm-downlink-frame-prefix.o \
./ns-3/src/wimax/model/send-params.o \
./ns-3/src/wimax/model/service-flow-manager.o \
./ns-3/src/wimax/model/service-flow-record.o \
./ns-3/src/wimax/model/service-flow.o \
./ns-3/src/wimax/model/simple-ofdm-send-param.o \
./ns-3/src/wimax/model/simple-ofdm-wimax-channel.o \
./ns-3/src/wimax/model/simple-ofdm-wimax-phy.o \
./ns-3/src/wimax/model/snr-to-block-error-rate-manager.o \
./ns-3/src/wimax/model/snr-to-block-error-rate-record.o \
./ns-3/src/wimax/model/ss-link-manager.o \
./ns-3/src/wimax/model/ss-manager.o \
./ns-3/src/wimax/model/ss-net-device.o \
./ns-3/src/wimax/model/ss-record.o \
./ns-3/src/wimax/model/ss-scheduler.o \
./ns-3/src/wimax/model/ss-service-flow-manager.o \
./ns-3/src/wimax/model/ul-job.o \
./ns-3/src/wimax/model/ul-mac-messages.o \
./ns-3/src/wimax/model/wimax-channel.o \
./ns-3/src/wimax/model/wimax-connection.o \
./ns-3/src/wimax/model/wimax-mac-header.o \
./ns-3/src/wimax/model/wimax-mac-queue.o \
./ns-3/src/wimax/model/wimax-mac-to-mac-header.o \
./ns-3/src/wimax/model/wimax-net-device.o \
./ns-3/src/wimax/model/wimax-phy.o \
./ns-3/src/wimax/model/wimax-tlv.o 

CC_DEPS += \
./ns-3/src/wimax/model/bandwidth-manager.d \
./ns-3/src/wimax/model/bs-link-manager.d \
./ns-3/src/wimax/model/bs-net-device.d \
./ns-3/src/wimax/model/bs-scheduler-rtps.d \
./ns-3/src/wimax/model/bs-scheduler-simple.d \
./ns-3/src/wimax/model/bs-scheduler.d \
./ns-3/src/wimax/model/bs-service-flow-manager.d \
./ns-3/src/wimax/model/bs-uplink-scheduler-mbqos.d \
./ns-3/src/wimax/model/bs-uplink-scheduler-rtps.d \
./ns-3/src/wimax/model/bs-uplink-scheduler-simple.d \
./ns-3/src/wimax/model/bs-uplink-scheduler.d \
./ns-3/src/wimax/model/burst-profile-manager.d \
./ns-3/src/wimax/model/cid-factory.d \
./ns-3/src/wimax/model/cid.d \
./ns-3/src/wimax/model/connection-manager.d \
./ns-3/src/wimax/model/crc8.d \
./ns-3/src/wimax/model/cs-parameters.d \
./ns-3/src/wimax/model/dl-mac-messages.d \
./ns-3/src/wimax/model/ipcs-classifier-record.d \
./ns-3/src/wimax/model/ipcs-classifier.d \
./ns-3/src/wimax/model/mac-messages.d \
./ns-3/src/wimax/model/ofdm-downlink-frame-prefix.d \
./ns-3/src/wimax/model/send-params.d \
./ns-3/src/wimax/model/service-flow-manager.d \
./ns-3/src/wimax/model/service-flow-record.d \
./ns-3/src/wimax/model/service-flow.d \
./ns-3/src/wimax/model/simple-ofdm-send-param.d \
./ns-3/src/wimax/model/simple-ofdm-wimax-channel.d \
./ns-3/src/wimax/model/simple-ofdm-wimax-phy.d \
./ns-3/src/wimax/model/snr-to-block-error-rate-manager.d \
./ns-3/src/wimax/model/snr-to-block-error-rate-record.d \
./ns-3/src/wimax/model/ss-link-manager.d \
./ns-3/src/wimax/model/ss-manager.d \
./ns-3/src/wimax/model/ss-net-device.d \
./ns-3/src/wimax/model/ss-record.d \
./ns-3/src/wimax/model/ss-scheduler.d \
./ns-3/src/wimax/model/ss-service-flow-manager.d \
./ns-3/src/wimax/model/ul-job.d \
./ns-3/src/wimax/model/ul-mac-messages.d \
./ns-3/src/wimax/model/wimax-channel.d \
./ns-3/src/wimax/model/wimax-connection.d \
./ns-3/src/wimax/model/wimax-mac-header.d \
./ns-3/src/wimax/model/wimax-mac-queue.d \
./ns-3/src/wimax/model/wimax-mac-to-mac-header.d \
./ns-3/src/wimax/model/wimax-net-device.d \
./ns-3/src/wimax/model/wimax-phy.d \
./ns-3/src/wimax/model/wimax-tlv.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/wimax/model/%.o: ../ns-3/src/wimax/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


