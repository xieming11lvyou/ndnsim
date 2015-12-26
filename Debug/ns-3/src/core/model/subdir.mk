################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ns-3/src/core/model/cairo-wideint.c 

CC_SRCS += \
../ns-3/src/core/model/attribute-construction-list.cc \
../ns-3/src/core/model/attribute.cc \
../ns-3/src/core/model/boolean.cc \
../ns-3/src/core/model/breakpoint.cc \
../ns-3/src/core/model/calendar-scheduler.cc \
../ns-3/src/core/model/callback.cc \
../ns-3/src/core/model/command-line.cc \
../ns-3/src/core/model/config.cc \
../ns-3/src/core/model/default-simulator-impl.cc \
../ns-3/src/core/model/double.cc \
../ns-3/src/core/model/enum.cc \
../ns-3/src/core/model/event-id.cc \
../ns-3/src/core/model/event-impl.cc \
../ns-3/src/core/model/fatal-impl.cc \
../ns-3/src/core/model/global-value.cc \
../ns-3/src/core/model/hash-fnv.cc \
../ns-3/src/core/model/hash-function.cc \
../ns-3/src/core/model/hash-murmur3.cc \
../ns-3/src/core/model/hash.cc \
../ns-3/src/core/model/heap-scheduler.cc \
../ns-3/src/core/model/int64x64-128.cc \
../ns-3/src/core/model/int64x64-cairo.cc \
../ns-3/src/core/model/int64x64.cc \
../ns-3/src/core/model/integer.cc \
../ns-3/src/core/model/list-scheduler.cc \
../ns-3/src/core/model/log.cc \
../ns-3/src/core/model/make-event.cc \
../ns-3/src/core/model/map-scheduler.cc \
../ns-3/src/core/model/names.cc \
../ns-3/src/core/model/object-base.cc \
../ns-3/src/core/model/object-factory.cc \
../ns-3/src/core/model/object-ptr-container.cc \
../ns-3/src/core/model/object.cc \
../ns-3/src/core/model/pointer.cc \
../ns-3/src/core/model/random-variable-stream.cc \
../ns-3/src/core/model/random-variable.cc \
../ns-3/src/core/model/realtime-simulator-impl.cc \
../ns-3/src/core/model/ref-count-base.cc \
../ns-3/src/core/model/rng-seed-manager.cc \
../ns-3/src/core/model/rng-stream.cc \
../ns-3/src/core/model/scheduler.cc \
../ns-3/src/core/model/simulator-impl.cc \
../ns-3/src/core/model/simulator.cc \
../ns-3/src/core/model/string.cc \
../ns-3/src/core/model/synchronizer.cc \
../ns-3/src/core/model/system-path.cc \
../ns-3/src/core/model/system-thread.cc \
../ns-3/src/core/model/test.cc \
../ns-3/src/core/model/time.cc \
../ns-3/src/core/model/timer.cc \
../ns-3/src/core/model/trace-source-accessor.cc \
../ns-3/src/core/model/type-id.cc \
../ns-3/src/core/model/type-name.cc \
../ns-3/src/core/model/uinteger.cc \
../ns-3/src/core/model/unix-fd-reader.cc \
../ns-3/src/core/model/unix-system-condition.cc \
../ns-3/src/core/model/unix-system-mutex.cc \
../ns-3/src/core/model/unix-system-wall-clock-ms.cc \
../ns-3/src/core/model/vector.cc \
../ns-3/src/core/model/wall-clock-synchronizer.cc \
../ns-3/src/core/model/watchdog.cc \
../ns-3/src/core/model/win32-system-wall-clock-ms.cc 

OBJS += \
./ns-3/src/core/model/attribute-construction-list.o \
./ns-3/src/core/model/attribute.o \
./ns-3/src/core/model/boolean.o \
./ns-3/src/core/model/breakpoint.o \
./ns-3/src/core/model/cairo-wideint.o \
./ns-3/src/core/model/calendar-scheduler.o \
./ns-3/src/core/model/callback.o \
./ns-3/src/core/model/command-line.o \
./ns-3/src/core/model/config.o \
./ns-3/src/core/model/default-simulator-impl.o \
./ns-3/src/core/model/double.o \
./ns-3/src/core/model/enum.o \
./ns-3/src/core/model/event-id.o \
./ns-3/src/core/model/event-impl.o \
./ns-3/src/core/model/fatal-impl.o \
./ns-3/src/core/model/global-value.o \
./ns-3/src/core/model/hash-fnv.o \
./ns-3/src/core/model/hash-function.o \
./ns-3/src/core/model/hash-murmur3.o \
./ns-3/src/core/model/hash.o \
./ns-3/src/core/model/heap-scheduler.o \
./ns-3/src/core/model/int64x64-128.o \
./ns-3/src/core/model/int64x64-cairo.o \
./ns-3/src/core/model/int64x64.o \
./ns-3/src/core/model/integer.o \
./ns-3/src/core/model/list-scheduler.o \
./ns-3/src/core/model/log.o \
./ns-3/src/core/model/make-event.o \
./ns-3/src/core/model/map-scheduler.o \
./ns-3/src/core/model/names.o \
./ns-3/src/core/model/object-base.o \
./ns-3/src/core/model/object-factory.o \
./ns-3/src/core/model/object-ptr-container.o \
./ns-3/src/core/model/object.o \
./ns-3/src/core/model/pointer.o \
./ns-3/src/core/model/random-variable-stream.o \
./ns-3/src/core/model/random-variable.o \
./ns-3/src/core/model/realtime-simulator-impl.o \
./ns-3/src/core/model/ref-count-base.o \
./ns-3/src/core/model/rng-seed-manager.o \
./ns-3/src/core/model/rng-stream.o \
./ns-3/src/core/model/scheduler.o \
./ns-3/src/core/model/simulator-impl.o \
./ns-3/src/core/model/simulator.o \
./ns-3/src/core/model/string.o \
./ns-3/src/core/model/synchronizer.o \
./ns-3/src/core/model/system-path.o \
./ns-3/src/core/model/system-thread.o \
./ns-3/src/core/model/test.o \
./ns-3/src/core/model/time.o \
./ns-3/src/core/model/timer.o \
./ns-3/src/core/model/trace-source-accessor.o \
./ns-3/src/core/model/type-id.o \
./ns-3/src/core/model/type-name.o \
./ns-3/src/core/model/uinteger.o \
./ns-3/src/core/model/unix-fd-reader.o \
./ns-3/src/core/model/unix-system-condition.o \
./ns-3/src/core/model/unix-system-mutex.o \
./ns-3/src/core/model/unix-system-wall-clock-ms.o \
./ns-3/src/core/model/vector.o \
./ns-3/src/core/model/wall-clock-synchronizer.o \
./ns-3/src/core/model/watchdog.o \
./ns-3/src/core/model/win32-system-wall-clock-ms.o 

C_DEPS += \
./ns-3/src/core/model/cairo-wideint.d 

CC_DEPS += \
./ns-3/src/core/model/attribute-construction-list.d \
./ns-3/src/core/model/attribute.d \
./ns-3/src/core/model/boolean.d \
./ns-3/src/core/model/breakpoint.d \
./ns-3/src/core/model/calendar-scheduler.d \
./ns-3/src/core/model/callback.d \
./ns-3/src/core/model/command-line.d \
./ns-3/src/core/model/config.d \
./ns-3/src/core/model/default-simulator-impl.d \
./ns-3/src/core/model/double.d \
./ns-3/src/core/model/enum.d \
./ns-3/src/core/model/event-id.d \
./ns-3/src/core/model/event-impl.d \
./ns-3/src/core/model/fatal-impl.d \
./ns-3/src/core/model/global-value.d \
./ns-3/src/core/model/hash-fnv.d \
./ns-3/src/core/model/hash-function.d \
./ns-3/src/core/model/hash-murmur3.d \
./ns-3/src/core/model/hash.d \
./ns-3/src/core/model/heap-scheduler.d \
./ns-3/src/core/model/int64x64-128.d \
./ns-3/src/core/model/int64x64-cairo.d \
./ns-3/src/core/model/int64x64.d \
./ns-3/src/core/model/integer.d \
./ns-3/src/core/model/list-scheduler.d \
./ns-3/src/core/model/log.d \
./ns-3/src/core/model/make-event.d \
./ns-3/src/core/model/map-scheduler.d \
./ns-3/src/core/model/names.d \
./ns-3/src/core/model/object-base.d \
./ns-3/src/core/model/object-factory.d \
./ns-3/src/core/model/object-ptr-container.d \
./ns-3/src/core/model/object.d \
./ns-3/src/core/model/pointer.d \
./ns-3/src/core/model/random-variable-stream.d \
./ns-3/src/core/model/random-variable.d \
./ns-3/src/core/model/realtime-simulator-impl.d \
./ns-3/src/core/model/ref-count-base.d \
./ns-3/src/core/model/rng-seed-manager.d \
./ns-3/src/core/model/rng-stream.d \
./ns-3/src/core/model/scheduler.d \
./ns-3/src/core/model/simulator-impl.d \
./ns-3/src/core/model/simulator.d \
./ns-3/src/core/model/string.d \
./ns-3/src/core/model/synchronizer.d \
./ns-3/src/core/model/system-path.d \
./ns-3/src/core/model/system-thread.d \
./ns-3/src/core/model/test.d \
./ns-3/src/core/model/time.d \
./ns-3/src/core/model/timer.d \
./ns-3/src/core/model/trace-source-accessor.d \
./ns-3/src/core/model/type-id.d \
./ns-3/src/core/model/type-name.d \
./ns-3/src/core/model/uinteger.d \
./ns-3/src/core/model/unix-fd-reader.d \
./ns-3/src/core/model/unix-system-condition.d \
./ns-3/src/core/model/unix-system-mutex.d \
./ns-3/src/core/model/unix-system-wall-clock-ms.d \
./ns-3/src/core/model/vector.d \
./ns-3/src/core/model/wall-clock-synchronizer.d \
./ns-3/src/core/model/watchdog.d \
./ns-3/src/core/model/win32-system-wall-clock-ms.d 


# Each subdirectory must supply rules for building sources it contributes
ns-3/src/core/model/%.o: ../ns-3/src/core/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ns-3/src/core/model/%.o: ../ns-3/src/core/model/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


