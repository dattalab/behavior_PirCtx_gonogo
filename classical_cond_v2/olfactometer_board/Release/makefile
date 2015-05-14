################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include arduino/core/subdir.mk
-include Libraries/SPI/subdir.mk
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
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
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
arduino.ar \

ELF += \
olfactometer_board.elf \

EEP += \
olfactometer_board.eep \

FLASH_IMAGE += \
olfactometer_board.hex \


# All Target
all: olfactometer_board

# Tool invocations
olfactometer_board: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-size" -A "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/olfactometer_board.elf"
	@echo 'Finished building target: $@'
	@echo ' '

arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-ar" rcs  "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

olfactometer_board.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega32u4  -o "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/olfactometer_board.elf"   $(LINK_OBJ) $(AR)  "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/arduino.ar" "-L/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

olfactometer_board.eep: $(ELF)
	@echo 'Create eeprom image'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/olfactometer_board.elf" "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/olfactometer_board.eep"
	@echo 'Finished building: $@'
	@echo ' '

olfactometer_board.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-objcopy" -O ihex -R .eeprom  "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/olfactometer_board.elf" "/Users/ludoviccacheux/Documents/Github/behavior/classical_cond_v2/olfactometer_board/Release/olfactometer_board.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(EEP)$(FLASH_IMAGE)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS) olfactometer_board
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
