################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

BUILD_ARTIFACT_NAME := CD_Throttle_Rev11
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CD_Throttle_Rev11.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CD_Throttle_Rev11.list \

OBJCOPY_BIN += \
CD_Throttle_Rev11.bin \


# All Target
all: main-build

# Main-build Target
main-build: CD_Throttle_Rev11.elf secondary-outputs

# Tool invocations
CD_Throttle_Rev11.elf: $(OBJS) $(USER_OBJS) C:\Users\Charley\STM32CubeIDE\workspace_1.5.1\CD_Throttle_Rev11\STM32F072CBUX_FLASH.ld
	arm-none-eabi-gcc -o "CD_Throttle_Rev11.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\Charley\STM32CubeIDE\workspace_1.5.1\CD_Throttle_Rev11\STM32F072CBUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CD_Throttle_Rev11.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CD_Throttle_Rev11.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CD_Throttle_Rev11.list"
	@echo 'Finished building: $@'
	@echo ' '

CD_Throttle_Rev11.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "CD_Throttle_Rev11.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
