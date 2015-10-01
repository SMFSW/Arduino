################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
G:/code/Arduino/libraries/MsTimer2/MsTimer2.cpp 

LINK_OBJ += \
./Libraries/MsTimer2/MsTimer2.cpp.o 

CPP_DEPS += \
./Libraries/MsTimer2/MsTimer2.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/MsTimer2/MsTimer2.cpp.o: G:/code/Arduino/libraries/MsTimer2/MsTimer2.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=165-r2 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


