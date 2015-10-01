################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
G:/code/Arduino/libraries/MsTimer2/MsTimer2.cpp 

CPP_DEPS += \
./Libraries/MsTimer2/MsTimer2.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/MsTimer2/MsTimer2.o: G:/code/Arduino/libraries/MsTimer2/MsTimer2.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


