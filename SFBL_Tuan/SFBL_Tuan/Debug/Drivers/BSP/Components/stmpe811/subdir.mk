################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/stmpe811/stmpe811.c 

OBJS += \
./Drivers/BSP/Components/stmpe811/stmpe811.o 

C_DEPS += \
./Drivers/BSP/Components/stmpe811/stmpe811.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/stmpe811/%.o: ../Drivers/BSP/Components/stmpe811/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -I"C:/Git/SFBL_Tuan/SFBL_Tuan/Inc" -I"C:/Git/SFBL_Tuan/SFBL_Tuan/Drivers/STM32L4xx_HAL_Driver/Inc" -I"C:/Git/SFBL_Tuan/SFBL_Tuan/Drivers/STM32L4xx_HAL_Driver/Inc" -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I"C:/Git/SFBL_Tuan/SFBL_Tuan/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Git/SFBL_Tuan/SFBL_Tuan/Drivers/CMSIS/Include" -I"C:/Git/SFBL_Tuan/SFBL_Tuan/Drivers/BSP/STM32L476G_EVAL"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


