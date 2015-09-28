################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/init.cc \
../src/main.cc \
../src/monitor.cc \
../src/sess_read_data.cc \
../src/sess_write_data.cc \
../src/utils.cc 

CPP_SRCS += \
../src/CMDtable.cpp \
../src/F4_RTC.cpp \
../src/input_port.cpp \
../src/led.cpp \
../src/logger.cpp \
../src/stm32cpu.cpp \
../src/usb_device.cpp 

CC_DEPS += \
./src/init.d \
./src/main.d \
./src/monitor.d \
./src/sess_read_data.d \
./src/sess_write_data.d \
./src/utils.d 

OBJS += \
./src/CMDtable.o \
./src/F4_RTC.o \
./src/init.o \
./src/input_port.o \
./src/led.o \
./src/logger.o \
./src/main.o \
./src/monitor.o \
./src/sess_read_data.o \
./src/sess_write_data.o \
./src/stm32cpu.o \
./src/usb_device.o \
./src/utils.o 

CPP_DEPS += \
./src/CMDtable.d \
./src/F4_RTC.d \
./src/input_port.d \
./src/led.d \
./src/logger.d \
./src/stm32cpu.d \
./src/usb_device.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-eabi-g++ -I../../ecos/ecos_install/include -I../../caboodle/include -I../../stm_usb/include -O0 -g3 -Wall $(ECOS_GLOBAL_CFLAGS) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-eabi-g++ -I../../ecos/ecos_install/include -I../../caboodle/include -I../../stm_usb/include -O0 -g3 -Wall $(ECOS_GLOBAL_CFLAGS) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


