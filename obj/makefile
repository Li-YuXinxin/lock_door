################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Driver/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
intelligent_door_lock.hex \

SECONDARY_LIST += \
intelligent_door_lock.lst \

SECONDARY_SIZE += \
intelligent_door_lock.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: intelligent_door_lock.elf secondary-outputs

# Tool invocations
intelligent_door_lock.elf: $(OBJS) $(USER_OBJS)
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -T "e:/��Ƭ��/CH32V307/��������/intelligent_door_lock/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage -Wl,-Map,"intelligent_door_lock.map" --specs=nano.specs --specs=nosys.specs -o "intelligent_door_lock.elf" $(OBJS)  $(LIBS)
intelligent_door_lock.hex: intelligent_door_lock.elf
	@	riscv-none-embed-objcopy -O ihex "intelligent_door_lock.elf" "intelligent_door_lock.hex"
intelligent_door_lock.lst: intelligent_door_lock.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "intelligent_door_lock.elf" > "intelligent_door_lock.lst"
intelligent_door_lock.siz: intelligent_door_lock.elf
	riscv-none-embed-size --format=berkeley "intelligent_door_lock.elf"

# Other Targets
clean:
	-$(RM) $(OBJS) $(EXPANDS) $(CALLGRAPH_DOT) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(S_UPPER_DEPS)$(ASM_DEPS)$(ASM_UPPER_DEPS)$(C_DEPS) intelligent_door_lock.elf

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets