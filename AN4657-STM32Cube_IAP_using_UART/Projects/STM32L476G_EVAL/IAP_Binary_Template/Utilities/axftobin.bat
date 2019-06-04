@echo off
echo AXF to BIN file generation ...

"C:\Program Files\Keil\ARM\ARMCC\bin\fromelf.exe" --bin --output ..\MDK-ARM\STM32L476G_EVAL\STM32L476G_EVAL_SysTick.bin ..\MDK-ARM\STM32L476G_EVAL\STM32L476G_EVAL.axf 

echo AXF to BIN file generation completed.

