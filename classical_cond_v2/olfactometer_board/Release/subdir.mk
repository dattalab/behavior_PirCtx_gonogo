################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
../olfactometer_board_m.ino 

CPP_SRCS += \
../.ino.cpp \
../MFC_control.cpp \
../icspSlaveFunctions.cpp \
../valves_functions.cpp 

LINK_OBJ += \
./.ino.cpp.o \
./MFC_control.cpp.o \
./icspSlaveFunctions.cpp.o \
./valves_functions.cpp.o 

INO_DEPS += \
./olfactometer_board_m.ino.d 

CPP_DEPS += \
./.ino.cpp.d \
./MFC_control.cpp.d \
./icspSlaveFunctions.cpp.d \
./valves_functions.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ../.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=161 -DARDUINO_AVR_MICRO -DARDUINO_ARCH_AVR  -DUSB_VID=0x2341 -DUSB_PID=0x8037 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Micro"'   -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/cores/arduino" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/variants/micro" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/libraries/SPI" -I"/Users/ludoviccacheux/Documents/Arduino/libraries/SPI_anything" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

MFC_control.cpp.o: ../MFC_control.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=161 -DARDUINO_AVR_MICRO -DARDUINO_ARCH_AVR  -DUSB_VID=0x2341 -DUSB_PID=0x8037 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Micro"'   -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/cores/arduino" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/variants/micro" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/libraries/SPI" -I"/Users/ludoviccacheux/Documents/Arduino/libraries/SPI_anything" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

icspSlaveFunctions.cpp.o: ../icspSlaveFunctions.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=161 -DARDUINO_AVR_MICRO -DARDUINO_ARCH_AVR  -DUSB_VID=0x2341 -DUSB_PID=0x8037 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Micro"'   -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/cores/arduino" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/variants/micro" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/libraries/SPI" -I"/Users/ludoviccacheux/Documents/Arduino/libraries/SPI_anything" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

olfactometer_board_m.o: ../olfactometer_board_m.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=161 -DARDUINO_AVR_MICRO -DARDUINO_ARCH_AVR  -DUSB_VID=0x2341 -DUSB_PID=0x8037 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Micro"'   -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/cores/arduino" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/variants/micro" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/libraries/SPI" -I"/Users/ludoviccacheux/Documents/Arduino/libraries/SPI_anything" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

valves_functions.cpp.o: ../valves_functions.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Applications/Arduino1.6.1.app/Contents/Java/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=161 -DARDUINO_AVR_MICRO -DARDUINO_ARCH_AVR  -DUSB_VID=0x2341 -DUSB_PID=0x8037 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Micro"'   -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/cores/arduino" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/variants/micro" -I"/Applications/Arduino1.6.1.app/Contents/Java/hardware/arduino/avr/libraries/SPI" -I"/Users/ludoviccacheux/Documents/Arduino/libraries/SPI_anything" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


