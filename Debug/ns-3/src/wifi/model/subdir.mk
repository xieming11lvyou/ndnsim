################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ns-3/src/wifi/model/aarf-wifi-manager.cc \
../ns-3/src/wifi/model/aarfcd-wifi-manager.cc \
../ns-3/src/wifi/model/adhoc-wifi-mac.cc \
../ns-3/src/wifi/model/amrr-wifi-manager.cc \
../ns-3/src/wifi/model/amsdu-subframe-header.cc \
../ns-3/src/wifi/model/ap-wifi-mac.cc \
../ns-3/src/wifi/model/arf-wifi-manager.cc \
../ns-3/src/wifi/model/block-ack-agreement.cc \
../ns-3/src/wifi/model/block-ack-cache.cc \
../ns-3/src/wifi/model/block-ack-manager.cc \
../ns-3/src/wifi/model/capability-information.cc \
../ns-3/src/wifi/model/cara-wifi-manager.cc \
../ns-3/src/wifi/model/constant-rate-wifi-manager.cc \
../ns-3/src/wifi/model/ctrl-headers.cc \
../ns-3/src/wifi/model/dca-txop.cc \
../ns-3/src/wifi/model/dcf-manager.cc \
../ns-3/src/wifi/model/dcf.cc \
../ns-3/src/wifi/model/dsss-error-rate-model.cc \
../ns-3/src/wifi/model/edca-txop-n.cc \
../ns-3/src/wifi/model/error-rate-model.cc \
../ns-3/src/wifi/model/ht-capabilities.cc \
../ns-3/src/wifi/model/ideal-wifi-manager.cc \
../ns-3/src/wifi/model/interference-helper.cc \
../ns-3/src/wifi/model/mac-low.cc \
../ns-3/src/wifi/model/mac-rx-middle.cc \
../ns-3/src/wifi/model/mac-tx-middle.cc \
../ns-3/src/wifi/model/mgt-headers.cc \
../ns-3/src/wifi/model/minstrel-wifi-manager.cc \
../ns-3/src/wifi/model/msdu-aggregator.cc \
../ns-3/src/wifi/model/msdu-standard-aggregator.cc \
../ns-3/src/wifi/model/nist-error-rate-model.cc \
../ns-3/src/wifi/model/onoe-wifi-manager.cc \
../ns-3/src/wifi/model/originator-block-ack-agreement.cc \
../ns-3/src/wifi/model/qos-blocked-destinations.cc \
../ns-3/src/wifi/model/qos-tag.cc \
../ns-3/src/wifi/model/qos-utils.cc \
../ns-3/src/wifi/model/random-stream.cc \
../ns-3/src/wifi/model/regular-wifi-mac.cc \
../ns-3/src/wifi/model/rraa-wifi-manager.cc \
../ns-3/src/wifi/model/snr-tag.cc \
../ns-3/src/wifi/model/ssid.cc \
../ns-3/src/wifi/model/sta-wifi-mac.cc \
../ns-3/src/wifi/model/status-code.cc \
../ns-3/src/wifi/model/supported-rates.cc \
../ns-3/src/wifi/model/wifi-channel.cc \
../ns-3/src/wifi/model/wifi-information-element-vector.cc \
../ns-3/src/wifi/model/wifi-information-element.cc \
../ns-3/src/wifi/model/wifi-mac-header.cc \
../ns-3/src/wifi/model/wifi-mac-queue.cc \
../ns-3/src/wifi/model/wifi-mac-trailer.cc \
../ns-3/src/wifi/model/wifi-mac.cc \
../ns-3/src/wifi/model/wifi-mode.cc \
../ns-3/src/wifi/model/wifi-net-device.cc \
../ns-3/src/wifi/model/wifi-phy-state-helper.cc \
../ns-3/src/wifi/model/wifi-phy.cc \
../ns-3/src/wifi/model/wifi-remote-station-manager.cc \
../ns-3/src/wifi/model/wifi-tx-vector.cc \
../ns-3/src/wifi/model/yans-error-rate-model.cc \
../ns-3/src/wifi/model/yans-wifi-channel.cc \
../ns-3/src/wifi/model/yans-wifi-phy.cc 

OBJS += \
./ns-3/src/wifi/model/aarf-wifi-manager.o \
./ns-3/src/wifi/model/aarfcd-wifi-manager.o \
./ns-3/src/wifi/model/adhoc-wifi-mac.o \
./ns-3/src/wifi/model/amrr-wifi-manager.o \
./ns-3/src/wifi/model/amsdu-subframe-header.o \
./ns-3/src/wifi/model/ap-wifi-mac.o \
./ns-3/src/wifi/model/arf-wifi-manager.o \
./ns-3/src/wifi/model/block-ack-agreement.o \
./ns-3/src/wifi/model/block-ack-cache.o \
./ns-3/src/wifi/model/block-ack-manager.o \
./ns-3/src/wifi/model/capability-information.o \
./ns-3/src/wifi/model/cara-wifi-manager.o \
./ns-3/src/wifi/model/constant-rate-wifi-manager.o \
./ns-3/src/wifi/model/ctrl-headers.o \
./ns-3/src/wifi/model/dca-txop.o \
./ns-3/src/wifi/model/dcf-manager.o \
./ns-3/src/wifi/model/dcf.o \
./ns-3/src/wifi/model/dsss-error-rate-model.o \
./ns-3/src/wifi/model/edca-txop-n.o \
./ns-3/src/wifi/model/error-rate-model.o \
./ns-3/src/wifi/model/ht-capabilities.o \
./ns-3/src/wifi/model/ideal-wifi-manager.o \
./ns-3/src/wifi/model/interference-helper.o \
./ns-3/src/wifi/model/mac-low.o \
./ns-3/src/wifi/model/mac-rx-middle.o \
./ns-3/src/wifi/model/mac-tx-middle.o \
./ns-3/src/wifi/model/mgt-headers.o \
./ns-3/src/wifi/model/minstrel-wifi-manager.o \
./ns-3/src/wifi/model/msdu-aggregator.o \
./ns-3/src/wifi/model/msdu-standard-aggregator.o \
./ns-3/src/wifi/model/nist-error-rate-model.o \
./ns-3/src/wifi/model/onoe-wifi-manager.o \
./ns-3/src/wifi/model/originator-block-ack-agreement.o \
./ns-3/src/wifi/model/qos-blocked-destinations.o \
./ns-3/src/wifi/model/qos-tag.o \
./ns-3/src/wifi/model/qos-utils.o \
./ns-3/src/wifi/model/random-stream.o \
./ns-3/src/wifi/model/regular-wifi-mac.o \
./ns-3/src/wifi/model/rraa-wifi-manager.o \
./ns-3/src/wifi/model/snr-tag.o \
./ns-3/src/wifi/model/ssid.o \
./ns-3/src/wifi/model/sta-wifi-mac.o \
./ns-3/src/wifi/model/status-code.o \
./ns-3/src/wifi/model/supported-rates.o \
./ns-3/src/wifi/model/wifi-channel.o \
./ns-3/src/wifi/model/wifi-information-element-vector.o \
./ns-3/src/wifi/model/wifi-information-element.o \
./ns-3/src/wifi/model/wifi-mac-header.o \
./ns-3/src/wifi/model/wifi-mac-queue.o \
./ns-3/src/wifi/model/wifi-mac-trailer.o \
./ns-3/src/wifi/model/wifi-mac.o \
./ns-3/src/wifi/model/wifi-mode.o \
./ns-3/src/wifi/model/wifi-net-device.o \
./ns-3/src/wifi/model/wifi-phy-state-helper.o \
./ns-3/src/wifi/model/wifi-phy.o \
./ns-3/src/wifi/model/wifi-remote-station-manager.o \
./ns-3/src/wifi/model/wifi-tx-vector.o \
./ns-3/src/wifi/model/yans-error-rate-model.o \
./ns-3/src/wifi/model/yans-wifi-channel.o \
./ns-3/src/wifi/model/yans-wifi-phy.o 

CC_DEPS += \
./ns-3/src/wifi/model/aarf-wifi-manager.d \
./ns-3/src/wifi/model/aarfcd-wifi-manager.d \
./ns-3/src/wifi/model/adhoc-wifi-mac.d \
./ns-3/src/wifi/model/amrr-wifi-manager.d \
./ns-3/src/wifi/model/amsdu-subframe-header.d \
./ns-3/src/wifi/model/ap-wifi-mac.d \
./ns-3/src/wifi/model/arf-wifi-manager.d \
./ns-3/src/wifi/model/block-ack-agreement.d \
./ns-3/src/wifi/model/block-ack-cache.d \
./ns-3/src/wifi/model/block-ack-manager.d \
./ns-3/src/wifi/model/capability-information.d \
./ns-3/src/wifi/model/cara-wifi-manager.d \
./ns-3/src/wifi/model/constant-rate-wifi-manager.d \
./ns-3/src/wifi/model/ctrl-headers.d \
./ns-3/src/wifi/model/dca-txop.d \
./ns-3/src/wifi/model/dcf-manager.d \
./ns-3/src/wifi/model/dcf.d \
./ns-3/src/wifi/model/dsss-error-rate-model.d \
./ns-3/src/wifi/model/edca-txop-n.d \
./ns-3/src/wifi/model/error-rate-model.d \
./ns-3/src/wifi/model/ht-capabilities.d \
./ns-3/src/wifi/model/ideal-wifi-manager.d \
./ns-3/src/wifi/model/interference-helper.d \
./ns-3/src/wifi/model/mac-low.d \
./ns-3/src/wifi/model/mac-rx-middle.d \
./ns-3/src/wifi/model/mac-tx-middle.d \
./ns-3/src/wifi/model/mgt-headers.d \
./ns-3/src/wifi/model/minstrel-wifi-manager.d \
./ns-3/src/wifi/model/msdu-aggregator.d \
./ns-3/src/wifi/model/msdu-standard-aggregator.d \
./ns-3/src/wifi/model/nist-error-rate-model.d \
./ns-3/src/wifi/model/onoe-wifi-manager.d \
./ns-3/src/wifi/model/originator-block-ack-agreement.d \
./ns-3/src/wifi/model/qos-blocked-destinations.d \
./ns-3/src/wifi/model/qos-tag.d \
./ns-3/src/wifi/model/qos-utils.d \
./ns-3/src/wifi/model/random-stream.d \
./ns-3/src/wifi/model/regular-wifi-mac.d \
./ns-3/src/wifi/model/rraa-wifi-manager.d \
./ns-3/src/wifi/model/snr-tag.d \
./ns-3/src/wifi/model/ssid.d \
./ns-3/src/wifi/model/sta-wifi-mac.d \
./ns-3/src/wifi/model/status-code.d \
./ns-3/src/wifi/model/supported-rates.d \
./ns-3/src/wifi/model/wifi-channel.d \
./ns-3/src/wifi/model/wifi-information-element-vector.d \
./ns-3/src/wifi/model/wifi-information-element.d \
./ns-3/src/wifi/model/wifi-mac-header.d \
./ns-3/src/wifi/model/wifi-mac-queue.d \
./ns-3/src/wifi/model/wifi-mac-trailer.d \
./ns-3/src/wifi/model/wifi-mac.d \
./ns-3/src/wifi/model/wifi-mode.d \
./ns-3/src/wifi/model/wifi-net-device.d \
./ns-3/src/wifi/model/wifi-phy-state-helper.d \
./ns-3/src/wifi/model/wifi-phy.d \
./ns-3/src/wifi/model/wifi-remote-station-manager.d \
./ns-3/src/wifi/model/wifi-tx-vector.d \
./ns-3/src/wifi/model/yans-error-rate-model.d \
./ns-3/src/wifi/model/yans-wifi-channel.d \
./ns-3/src/wifi/model/yans-wifi-phy.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/wifi/model/%.o: ../ns-3/src/wifi/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


