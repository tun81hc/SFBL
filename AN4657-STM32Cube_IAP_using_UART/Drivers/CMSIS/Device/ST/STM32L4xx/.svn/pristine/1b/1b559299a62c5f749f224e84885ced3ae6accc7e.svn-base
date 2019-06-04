@color 0B
@echo off

REM Generate all CMSIS files 
REM Active perl must be installed

set scriptPATH=%CD%\..\..\..\..\..\..\__INTERNAL__Validation_Integration\Integration_Tool\_CmsisDeviceGenerator\

if not exist "%scriptPATH%"  (
	echo Input directory does not exist!
	pause
	exit
)

cd %scriptPATH%

DeviceGeneration.pl  --target STM32L4xx -diff

pause
:EOF
