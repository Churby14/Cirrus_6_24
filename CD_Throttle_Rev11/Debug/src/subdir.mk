################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/accel.c \
../src/adc.c \
../src/analog.c \
../src/can.c \
../src/crc16.c \
../src/crc8.c \
../src/default.c \
../src/dig.c \
../src/dma.c \
../src/error_handler.c \
../src/errorcondition.c \
../src/execution.c \
../src/freqin.c \
../src/freqout.c \
../src/fuzzy.c \
../src/global.c \
../src/gpio.c \
../src/hardware.c \
../src/i2c.c \
../src/indexes.c \
../src/initsystem.c \
../src/interrupts.c \
../src/logevent.c \
../src/main.c \
../src/math64.c \
../src/mcp3421.c \
../src/pwm.c \
../src/reflash.c \
../src/reflash_page.c \
../src/serial.c \
../src/testinput.c \
../src/throttle.c \
../src/ticks.c \
../src/timer.c \
../src/usart.c \
../src/verify.c \
../src/watchdog.c 

OBJS += \
./src/accel.o \
./src/adc.o \
./src/analog.o \
./src/can.o \
./src/crc16.o \
./src/crc8.o \
./src/default.o \
./src/dig.o \
./src/dma.o \
./src/error_handler.o \
./src/errorcondition.o \
./src/execution.o \
./src/freqin.o \
./src/freqout.o \
./src/fuzzy.o \
./src/global.o \
./src/gpio.o \
./src/hardware.o \
./src/i2c.o \
./src/indexes.o \
./src/initsystem.o \
./src/interrupts.o \
./src/logevent.o \
./src/main.o \
./src/math64.o \
./src/mcp3421.o \
./src/pwm.o \
./src/reflash.o \
./src/reflash_page.o \
./src/serial.o \
./src/testinput.o \
./src/throttle.o \
./src/ticks.o \
./src/timer.o \
./src/usart.o \
./src/verify.o \
./src/watchdog.o 

C_DEPS += \
./src/accel.d \
./src/adc.d \
./src/analog.d \
./src/can.d \
./src/crc16.d \
./src/crc8.d \
./src/default.d \
./src/dig.d \
./src/dma.d \
./src/error_handler.d \
./src/errorcondition.d \
./src/execution.d \
./src/freqin.d \
./src/freqout.d \
./src/fuzzy.d \
./src/global.d \
./src/gpio.d \
./src/hardware.d \
./src/i2c.d \
./src/indexes.d \
./src/initsystem.d \
./src/interrupts.d \
./src/logevent.d \
./src/main.d \
./src/math64.d \
./src/mcp3421.d \
./src/pwm.d \
./src/reflash.d \
./src/reflash_page.d \
./src/serial.d \
./src/testinput.d \
./src/throttle.d \
./src/ticks.d \
./src/timer.d \
./src/usart.d \
./src/verify.d \
./src/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
src/accel.o: ../src/accel.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/accel.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/adc.o: ../src/adc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/adc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/analog.o: ../src/analog.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/analog.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/can.o: ../src/can.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/can.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/crc16.o: ../src/crc16.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/crc16.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/crc8.o: ../src/crc8.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/crc8.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/default.o: ../src/default.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/default.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/dig.o: ../src/dig.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/dig.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/dma.o: ../src/dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/dma.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/error_handler.o: ../src/error_handler.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/error_handler.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/errorcondition.o: ../src/errorcondition.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/errorcondition.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/execution.o: ../src/execution.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/execution.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/freqin.o: ../src/freqin.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/freqin.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/freqout.o: ../src/freqout.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/freqout.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/fuzzy.o: ../src/fuzzy.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/fuzzy.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/global.o: ../src/global.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/global.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/gpio.o: ../src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/gpio.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/hardware.o: ../src/hardware.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/hardware.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/i2c.o: ../src/i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/i2c.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/indexes.o: ../src/indexes.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/indexes.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/initsystem.o: ../src/initsystem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/initsystem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/interrupts.o: ../src/interrupts.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/interrupts.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/logevent.o: ../src/logevent.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/logevent.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/main.o: ../src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/math64.o: ../src/math64.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/math64.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/mcp3421.o: ../src/mcp3421.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/mcp3421.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/pwm.o: ../src/pwm.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/pwm.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/reflash.o: ../src/reflash.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/reflash.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/reflash_page.o: ../src/reflash_page.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/reflash_page.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/serial.o: ../src/serial.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/serial.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/testinput.o: ../src/testinput.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/testinput.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/throttle.o: ../src/throttle.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/throttle.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/ticks.o: ../src/ticks.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/ticks.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/timer.o: ../src/timer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/timer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/usart.o: ../src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/usart.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/verify.o: ../src/verify.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/verify.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/watchdog.o: ../src/watchdog.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/watchdog.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

