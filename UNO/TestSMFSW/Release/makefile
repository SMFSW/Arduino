################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include arduino/core/subdir.mk
-include Libraries/MsTimer2/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \

ELF += \

EEP += \

FLASH_IMAGE += \


# All Target
all: TestSMFSW

# Tool invocations
TestSMFSW: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/bin/avr-size" -A "G:/code/Arduino/TestSMFSW/Release/TestSMFSW.elf"
	@echo 'Finished building target: $@'
	@echo ' '

G:/code/Arduino/TestSMFSW/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/bin/avr-ar" rcs  "G:/code/Arduino/TestSMFSW/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

TestSMFSW.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/bin/avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega328p  -o "G:/code/Arduino/TestSMFSW/Release/TestSMFSW.elf"   $(LINK_OBJ) $(AR)  "G:/code/Arduino/TestSMFSW/Release/arduino.ar" "-LG:/code/Arduino/TestSMFSW/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

TestSMFSW.eep: $(ELF)
	@echo 'Create eeprom image'
	"/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "G:/code/Arduino/TestSMFSW/Release/TestSMFSW.elf" "G:/code/Arduino/TestSMFSW/Release/TestSMFSW.eep"
	@echo 'Finished building: $@'
	@echo ' '

TestSMFSW.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"/bin/avr-objcopy" -O ihex -R .eeprom  "G:/code/Arduino/TestSMFSW/Release/TestSMFSW.elf" "G:/code/Arduino/TestSMFSW/Release/TestSMFSW.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(C_DEPS)$(AR)$(EEP)$(FLASH_IMAGE)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS) TestSMFSW
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
