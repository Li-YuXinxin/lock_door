################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 


EXPANDS += \
./User/ch32v30x_it.c.234r.expand \
./User/main.c.234r.expand \
./User/system_ch32v30x.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Debug" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Core" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/User" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Peripheral/inc" -I"e:/单片机/CH32V307/智能门锁/intelligent_door_lock/Driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

