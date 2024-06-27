################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/crc16.c \
../src/dig.c \
../src/error_handler.c \
../src/fpgacomm.c \
../src/freqin.c \
../src/global.c \
../src/i2c.c \
../src/initsystem.c \
../src/math64.c \
../src/mysprintf.c \
../src/nau7802.c \
../src/reflash.c \
../src/reflash_page.c \
../src/serial.c \
../src/stm32f0xx_hal_msp.c \
../src/stm32f0xx_it.c \
../src/subsystems.c \
../src/system_clock_config.c \
../src/system_stm32f0xx.c \
../src/ticks.c 

CPP_SRCS += \
../src/main.cpp 

C_DEPS += \
./src/crc16.d \
./src/dig.d \
./src/error_handler.d \
./src/fpgacomm.d \
./src/freqin.d \
./src/global.d \
./src/i2c.d \
./src/initsystem.d \
./src/math64.d \
./src/mysprintf.d \
./src/nau7802.d \
./src/reflash.d \
./src/reflash_page.d \
./src/serial.d \
./src/stm32f0xx_hal_msp.d \
./src/stm32f0xx_it.d \
./src/subsystems.d \
./src/system_clock_config.d \
./src/system_stm32f0xx.d \
./src/ticks.d 

OBJS += \
./src/crc16.o \
./src/dig.o \
./src/error_handler.o \
./src/fpgacomm.o \
./src/freqin.o \
./src/global.o \
./src/i2c.o \
./src/initsystem.o \
./src/main.o \
./src/math64.o \
./src/mysprintf.o \
./src/nau7802.o \
./src/reflash.o \
./src/reflash_page.o \
./src/serial.o \
./src/stm32f0xx_hal_msp.o \
./src/stm32f0xx_it.o \
./src/subsystems.o \
./src/system_clock_config.o \
./src/system_stm32f0xx.o \
./src/ticks.o 

CPP_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/crc16.o: ../src/crc16.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/crc16.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/dig.o: ../src/dig.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/dig.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/error_handler.o: ../src/error_handler.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/error_handler.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/fpgacomm.o: ../src/fpgacomm.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/fpgacomm.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/freqin.o: ../src/freqin.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/freqin.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/global.o: ../src/global.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/global.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/i2c.o: ../src/i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/i2c.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/initsystem.o: ../src/initsystem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/initsystem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/main.o: ../src/main.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/math64.o: ../src/math64.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/math64.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/mysprintf.o: ../src/mysprintf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/mysprintf.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/nau7802.o: ../src/nau7802.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/nau7802.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/reflash.o: ../src/reflash.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/reflash.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/reflash_page.o: ../src/reflash_page.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/reflash_page.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/serial.o: ../src/serial.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/serial.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/stm32f0xx_hal_msp.o: ../src/stm32f0xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/stm32f0xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/stm32f0xx_it.o: ../src/stm32f0xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/stm32f0xx_it.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/subsystems.o: ../src/subsystems.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/subsystems.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/system_clock_config.o: ../src/system_clock_config.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/system_clock_config.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/system_stm32f0xx.o: ../src/system_stm32f0xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/system_stm32f0xx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/ticks.o: ../src/ticks.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/ticks.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

