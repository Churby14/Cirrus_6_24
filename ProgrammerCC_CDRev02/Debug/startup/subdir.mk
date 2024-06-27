################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

S_DEPS += \
./startup/startup_stm32.d 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/startup_stm32.o: ../startup/startup_stm32.s
	arm-none-eabi-gcc -mcpu=cortex-m0 -g3 -c -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/inc" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/core" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/CMSIS/device" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc/Legacy" -I"C:/Users/Charley/STM32CubeIDE/workspace_1.5.1/ProgrammerCC_CDRev02/HAL_Driver/Inc" -x assembler-with-cpp -MMD -MP -MF"startup/startup_stm32.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

