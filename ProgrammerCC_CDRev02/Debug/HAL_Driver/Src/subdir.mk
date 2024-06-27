################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL_Driver/Src/stm32f0xx_hal.c \
../HAL_Driver/Src/stm32f0xx_hal_can.c \
../HAL_Driver/Src/stm32f0xx_hal_cortex.c \
../HAL_Driver/Src/stm32f0xx_hal_crc.c \
../HAL_Driver/Src/stm32f0xx_hal_crc_ex.c \
../HAL_Driver/Src/stm32f0xx_hal_dma.c \
../HAL_Driver/Src/stm32f0xx_hal_flash.c \
../HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
../HAL_Driver/Src/stm32f0xx_hal_gpio.c \
../HAL_Driver/Src/stm32f0xx_hal_i2c.c \
../HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
../HAL_Driver/Src/stm32f0xx_hal_iwdg.c \
../HAL_Driver/Src/stm32f0xx_hal_pwr.c \
../HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c \
../HAL_Driver/Src/stm32f0xx_hal_rcc.c \
../HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
../HAL_Driver/Src/stm32f0xx_hal_tim.c \
../HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
../HAL_Driver/Src/stm32f0xx_hal_uart.c \
../HAL_Driver/Src/stm32f0xx_hal_uart_ex.c 

C_DEPS += \
./HAL_Driver/Src/stm32f0xx_hal.d \
./HAL_Driver/Src/stm32f0xx_hal_can.d \
./HAL_Driver/Src/stm32f0xx_hal_cortex.d \
./HAL_Driver/Src/stm32f0xx_hal_crc.d \
./HAL_Driver/Src/stm32f0xx_hal_crc_ex.d \
./HAL_Driver/Src/stm32f0xx_hal_dma.d \
./HAL_Driver/Src/stm32f0xx_hal_flash.d \
./HAL_Driver/Src/stm32f0xx_hal_flash_ex.d \
./HAL_Driver/Src/stm32f0xx_hal_gpio.d \
./HAL_Driver/Src/stm32f0xx_hal_i2c.d \
./HAL_Driver/Src/stm32f0xx_hal_i2c_ex.d \
./HAL_Driver/Src/stm32f0xx_hal_iwdg.d \
./HAL_Driver/Src/stm32f0xx_hal_pwr.d \
./HAL_Driver/Src/stm32f0xx_hal_pwr_ex.d \
./HAL_Driver/Src/stm32f0xx_hal_rcc.d \
./HAL_Driver/Src/stm32f0xx_hal_rcc_ex.d \
./HAL_Driver/Src/stm32f0xx_hal_tim.d \
./HAL_Driver/Src/stm32f0xx_hal_tim_ex.d \
./HAL_Driver/Src/stm32f0xx_hal_uart.d \
./HAL_Driver/Src/stm32f0xx_hal_uart_ex.d 

OBJS += \
./HAL_Driver/Src/stm32f0xx_hal.o \
./HAL_Driver/Src/stm32f0xx_hal_can.o \
./HAL_Driver/Src/stm32f0xx_hal_cortex.o \
./HAL_Driver/Src/stm32f0xx_hal_crc.o \
./HAL_Driver/Src/stm32f0xx_hal_crc_ex.o \
./HAL_Driver/Src/stm32f0xx_hal_dma.o \
./HAL_Driver/Src/stm32f0xx_hal_flash.o \
./HAL_Driver/Src/stm32f0xx_hal_flash_ex.o \
./HAL_Driver/Src/stm32f0xx_hal_gpio.o \
./HAL_Driver/Src/stm32f0xx_hal_i2c.o \
./HAL_Driver/Src/stm32f0xx_hal_i2c_ex.o \
./HAL_Driver/Src/stm32f0xx_hal_iwdg.o \
./HAL_Driver/Src/stm32f0xx_hal_pwr.o \
./HAL_Driver/Src/stm32f0xx_hal_pwr_ex.o \
./HAL_Driver/Src/stm32f0xx_hal_rcc.o \
./HAL_Driver/Src/stm32f0xx_hal_rcc_ex.o \
./HAL_Driver/Src/stm32f0xx_hal_tim.o \
./HAL_Driver/Src/stm32f0xx_hal_tim_ex.o \
./HAL_Driver/Src/stm32f0xx_hal_uart.o \
./HAL_Driver/Src/stm32f0xx_hal_uart_ex.o 


# Each subdirectory must supply rules for building sources it contributes
HAL_Driver/Src/stm32f0xx_hal.o: ../HAL_Driver/Src/stm32f0xx_hal.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_can.o: ../HAL_Driver/Src/stm32f0xx_hal_can.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_can.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_cortex.o: ../HAL_Driver/Src/stm32f0xx_hal_cortex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_cortex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_crc.o: ../HAL_Driver/Src/stm32f0xx_hal_crc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_crc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_crc_ex.o: ../HAL_Driver/Src/stm32f0xx_hal_crc_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_crc_ex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_dma.o: ../HAL_Driver/Src/stm32f0xx_hal_dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_dma.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_flash.o: ../HAL_Driver/Src/stm32f0xx_hal_flash.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_flash.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_flash_ex.o: ../HAL_Driver/Src/stm32f0xx_hal_flash_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_flash_ex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_gpio.o: ../HAL_Driver/Src/stm32f0xx_hal_gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_gpio.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_i2c.o: ../HAL_Driver/Src/stm32f0xx_hal_i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_i2c.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_i2c_ex.o: ../HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_i2c_ex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_iwdg.o: ../HAL_Driver/Src/stm32f0xx_hal_iwdg.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_iwdg.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_pwr.o: ../HAL_Driver/Src/stm32f0xx_hal_pwr.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_pwr.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_pwr_ex.o: ../HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_pwr_ex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_rcc.o: ../HAL_Driver/Src/stm32f0xx_hal_rcc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_rcc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_rcc_ex.o: ../HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_rcc_ex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_tim.o: ../HAL_Driver/Src/stm32f0xx_hal_tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_tim.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_tim_ex.o: ../HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_tim_ex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_uart.o: ../HAL_Driver/Src/stm32f0xx_hal_uart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_uart.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HAL_Driver/Src/stm32f0xx_hal_uart_ex.o: ../HAL_Driver/Src/stm32f0xx_hal_uart_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -c -I../inc -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HAL_Driver/Src/stm32f0xx_hal_uart_ex.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

