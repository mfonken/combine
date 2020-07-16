################################################################################
# Automatically-generated file. Do not edit!
################################################################################

export WINEDEBUG=-all

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include protocol/bluetooth/lib/EFR32BG13P/IAR/subdir.mk
-include platform/service/sleeptimer/src/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/IAR/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include app/system/sysiocontroller/subdir.mk
-include app/system/interfaces/subdir.mk
-include app/system/core/subdir.mk
-include app/rtos/uCOS-III/TLS/IAR/subdir.mk
-include app/rtos/uCOS-III/TLS/CCES/subdir.mk
-include app/rtos/uCOS-III/Source/subdir.mk
-include app/rtos/uCOS-III/Ports/Template/subdir.mk
-include app/rtos/uCOS-III/Ports/ARM-Cortex-M/ARMv7-M/IAR/subdir.mk
-include app/rtos/uCOS-III/Ports/ARM-Cortex-M/ARMv7-M/subdir.mk
-include app/rtos/uCOS-III/Cfg/Template/subdir.mk
-include app/rtos/uC-LIB-master/Ports/AVR32/UC3/GNU/subdir.mk
-include app/rtos/uC-LIB-master/Ports/AVR32/AP7000/IAR/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM/RealView/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM/IAR/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM/GNU/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM-Cortex-M4/RealView/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM-Cortex-M4/IAR/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM-Cortex-M4/GNU/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM-Cortex-M3/RealView/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM-Cortex-M3/IAR/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM-Cortex-M3/GNU/subdir.mk
-include app/rtos/uC-LIB-master/Ports/ARM-Cortex-M1/RealView/subdir.mk
-include app/rtos/uC-LIB-master/subdir.mk
-include app/rtos/uC-CPU-master/Template/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv7-M/IAR/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv7-M/GNU/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv7-M/CCS/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv7-M/ARM/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv7-M/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv6-M/IAR/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv6-M/GNU/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv6-M/ARM/subdir.mk
-include app/rtos/uC-CPU-master/ARM-Cortex-M/ARMv6-M/subdir.mk
-include app/rtos/uC-CPU-master/subdir.mk
-include app/peripherals/sensors/rhoclient/subdir.mk
-include app/peripherals/sensors/motionsensor/subdir.mk
-include app/peripherals/sensors/batterymonitor/subdir.mk
-include app/peripherals/drivers/hapticcontroller/subdir.mk
-include app/bluetooth/common/util/subdir.mk
-include app/application/tau/kinetic/qmath/subdir.mk
-include app/application/tau/kinetic/filters/subdir.mk
-include app/application/tau/kinetic/subdir.mk
-include app/application/tau/subdir.mk
-include app/application/subdir.mk
-include app/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(MSA_UPPER_DEPS)),)
-include $(MSA_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S79_DEPS)),)
-include $(S79_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(MSA_DEPS)),)
-include $(MSA_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S79_UPPER_DEPS)),)
-include $(S79_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: C12_master.out

# Tool invocations
C12_master.out: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: IAR Linker for ARM'
	ilinkarm $(OBJS) $(USER_OBJS) -o "C12_master.out" --config "C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\efr32bg13p632f512gm48.icf" --no_wrap_diagnostics "C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\protocol\bluetooth\lib\EFR32BG13P\IAR\libpsstore.a" "C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg13_iar_release.a" "C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\protocol\bluetooth\lib\EFR32BG13P\IAR\binapploader.o" "C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\protocol\bluetooth\lib\EFR32BG13P\IAR\libmbedtls.a" "C:\Users\Matthew Fonken\Desktop\marbl\Techincal\combine\master\C12_master\protocol\bluetooth\lib\EFR32BG13P\IAR\libbluetooth.a" --entry __iar_program_start --no_range_reservations --silent --keep __iar_program_start  
	ielftool --srec --verbose "C12_master.out" "C12_master.s37"  
	ielftool --ihex --verbose "C12_master.out" "C12_master.hex"  
	ielftool --bin --verbose "C12_master.out" "C12_master.bin"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(MSA_UPPER_DEPS)$(EXECUTABLES)$(OBJS)$(ASM_DEPS)$(S79_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(MSA_DEPS)$(C_DEPS)$(S79_UPPER_DEPS) C12_master.out
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
