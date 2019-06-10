################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_audio.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_eeprom.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_glass_lcd.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_idd.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_io.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_lcd.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_nor.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_qspi.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_sd.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_sram.c \
../Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_ts.c 

OBJS += \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_audio.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_eeprom.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_glass_lcd.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_idd.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_io.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_lcd.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_nor.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_qspi.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_sd.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_sram.o \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_ts.o 

C_DEPS += \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_audio.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_eeprom.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_glass_lcd.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_idd.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_io.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_lcd.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_nor.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_qspi.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_sd.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_sram.d \
./Drivers/BSP/STM32L476G_EVAL/stm32l476g_eval_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32L476G_EVAL/%.o: ../Drivers/BSP/STM32L476G_EVAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -I"C:/Git/SFBL_Tuan/DEMO1/Inc" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/STM32L4xx_HAL_Driver/Inc" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/CMSIS/Include" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/BSP/STM32L476G_EVAL" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/STM32L4xx_HAL_Driver/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


