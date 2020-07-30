################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../app/rtos/uC-CPU/MC9S12/Codewarrior/cpu_a.s 

OBJS += \
./app/rtos/uC-CPU/MC9S12/Codewarrior/cpu_a.o 

S_DEPS += \
./app/rtos/uC-CPU/MC9S12/Codewarrior/cpu_a.d 


# Each subdirectory must supply rules for building sources it contributes
app/rtos/uC-CPU/MC9S12/Codewarrior/cpu_a.o: ../app/rtos/uC-CPU/MC9S12/Codewarrior/cpu_a.s
	@echo 'Building file: $<'
	@echo 'Invoking: IAR Assembler for ARM'
	iasmarm "$<" -o  "$@" --cpu Cortex-M4 --fpu VFPv4_sp -s+ -r -L -N -p80 -t8 -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\hardware\kit\common\halconfig" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\hardware\kit\common\drivers" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\emlib\inc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\emdrv\sleep\src" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\emlib\src" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\hardware\kit\common\bsp" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\CMSIS\Include" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\app\bluetooth\common\util" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\service\sleeptimer\src" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\halconfig\inc\hal-config" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\Device\SiliconLabs\EFR32BG13P\Source\IAR" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\emdrv\sleep\inc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\bootloader\api" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\radio\rail_lib\common" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\emdrv\common\inc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\emdrv\uartdrv\inc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\radio\rail_lib\protocol\ble" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\service\sleeptimer\inc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\common\inc" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\service\sleeptimer\config" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\iar" -I"C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\bootloader" '-DHAL_CONFIG=1' '-DEFR32BG13P632F512GM48=1' -E100 -S
	@echo 'Finished building: $<'
	@echo ' '


