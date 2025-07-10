################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

C_DEPS += \
./Debug/debug.d 

OBJS += \
./Debug/debug.o 


EXPANDS += \
./Debug/debug.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Debug" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Core" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/User" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Peripheral/inc" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

