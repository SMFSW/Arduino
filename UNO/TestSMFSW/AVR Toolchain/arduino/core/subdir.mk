################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/WInterrupts.c \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/hooks.c \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring.c \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_analog.c \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_digital.c \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_pulse.c \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_shift.c 

CPP_SRCS += \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/CDC.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HID.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial0.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial1.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial2.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial3.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/IPAddress.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/Print.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/Stream.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/Tone.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/USBCore.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/WMath.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/WString.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/abi.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/main.cpp \
C:/prog/Arduino/hardware/arduino/avr/cores/arduino/new.cpp 

C_DEPS += \
./arduino/core/WInterrupts.d \
./arduino/core/hooks.d \
./arduino/core/wiring.d \
./arduino/core/wiring_analog.d \
./arduino/core/wiring_digital.d \
./arduino/core/wiring_pulse.d \
./arduino/core/wiring_shift.d 

CPP_DEPS += \
./arduino/core/CDC.d \
./arduino/core/HID.d \
./arduino/core/HardwareSerial.d \
./arduino/core/HardwareSerial0.d \
./arduino/core/HardwareSerial1.d \
./arduino/core/HardwareSerial2.d \
./arduino/core/HardwareSerial3.d \
./arduino/core/IPAddress.d \
./arduino/core/Print.d \
./arduino/core/Stream.d \
./arduino/core/Tone.d \
./arduino/core/USBCore.d \
./arduino/core/WMath.d \
./arduino/core/WString.d \
./arduino/core/abi.d \
./arduino/core/main.d \
./arduino/core/new.d 


# Each subdirectory must supply rules for building sources it contributes
arduino/core/CDC.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/CDC.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/HID.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HID.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/HardwareSerial.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/HardwareSerial0.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial0.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/HardwareSerial1.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial1.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/HardwareSerial2.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial2.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/HardwareSerial3.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial3.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/IPAddress.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/IPAddress.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/Print.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/Print.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/Stream.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/Stream.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/Tone.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/Tone.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/USBCore.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/USBCore.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/WInterrupts.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/WInterrupts.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/WMath.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/WMath.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/WString.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/WString.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/abi.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/abi.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/hooks.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/hooks.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/main.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/new.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/new.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/wiring.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/wiring_analog.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_analog.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/wiring_digital.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_digital.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/wiring_pulse.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_pulse.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/core/wiring_shift.o: C:/prog/Arduino/hardware/arduino/avr/cores/arduino/wiring_shift.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\prog\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\prog\Arduino\hardware\arduino\avr\variants\standard" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=${AVRTARGETMCU} -DF_CPU=${AVRTARGETFCPU}UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


