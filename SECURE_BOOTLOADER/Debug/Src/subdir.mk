################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/KeyMng.c \
../Src/SHE.c \
../Src/aes.c \
../Src/cmac.c \
../Src/common.c \
../Src/flash_if.c \
../Src/main.c \
../Src/menu.c \
../Src/stm32l4xx_hal_msp.c \
../Src/stm32l4xx_hal_timebase_tim.c \
../Src/stm32l4xx_it.c \
../Src/syscalls.c \
../Src/system_stm32l4xx.c \
../Src/ymodem.c 

OBJS += \
./Src/KeyMng.o \
./Src/SHE.o \
./Src/aes.o \
./Src/cmac.o \
./Src/common.o \
./Src/flash_if.o \
./Src/main.o \
./Src/menu.o \
./Src/stm32l4xx_hal_msp.o \
./Src/stm32l4xx_hal_timebase_tim.o \
./Src/stm32l4xx_it.o \
./Src/syscalls.o \
./Src/system_stm32l4xx.o \
./Src/ymodem.o 

C_DEPS += \
./Src/KeyMng.d \
./Src/SHE.d \
./Src/aes.d \
./Src/cmac.d \
./Src/common.d \
./Src/flash_if.d \
./Src/main.d \
./Src/menu.d \
./Src/stm32l4xx_hal_msp.d \
./Src/stm32l4xx_hal_timebase_tim.d \
./Src/stm32l4xx_it.d \
./Src/syscalls.d \
./Src/system_stm32l4xx.d \
./Src/ymodem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -I"C:/Git/SFBL_Tuan/SECURE_BOOTLOADER/Inc" -I"C:/Git/SFBL_Tuan/SECURE_BOOTLOADER/Drivers/STM32L4xx_HAL_Driver/Inc" -I"C:/Git/SFBL_Tuan/SECURE_BOOTLOADER/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"C:/Git/SFBL_Tuan/SECURE_BOOTLOADER/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Git/SFBL_Tuan/SECURE_BOOTLOADER/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

