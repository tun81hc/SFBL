################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.c 

OBJS += \
./Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.o 

C_DEPS += \
./Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/mfxstm32l152/%.o: ../Drivers/BSP/Components/mfxstm32l152/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -I"C:/Git/SFBL_Tuan/DEMO1/Inc" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/STM32L4xx_HAL_Driver/Inc" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/CMSIS/Include" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/BSP/STM32L476G_EVAL" -I"C:/Git/SFBL_Tuan/DEMO1/Drivers/STM32L4xx_HAL_Driver/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

