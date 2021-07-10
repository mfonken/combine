################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/master.c 

OBJS += \
./Core/Src/App/master.o 

C_DEPS += \
./Core/Src/App/master.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/master.o: ../Core/Src/App/master.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G431xx -D__OV9712__ -D__STM32__ '-DSYSTEM_CONFIG_HEADER="system_config.h"' '-DAPPLICATION_STATES_HEADER="state.h"' '-DGLOBAL_LOG_LEVEL=TEST' -D__RHO__ -c -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Core/Src -I../Core/Src/App -I../Core/Src/Rho/core -I../Core/Src/Rho/types -I../Core/Src/Rho/psm -I../Core/Src/UniSM -I../Core/Src/UniSM/OV9712 -I../Core/Src/UniSM/platforms -I../Core/Src/UniLog -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/App/master.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

