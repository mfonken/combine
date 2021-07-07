################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/UniSM/platform.c \
../Core/Src/UniSM/printers.c \
../Core/Src/UniSM/system.c 

OBJS += \
./Core/Src/UniSM/platform.o \
./Core/Src/UniSM/printers.o \
./Core/Src/UniSM/system.o 

C_DEPS += \
./Core/Src/UniSM/platform.d \
./Core/Src/UniSM/printers.d \
./Core/Src/UniSM/system.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/UniSM/platform.o: ../Core/Src/UniSM/platform.c Core/Src/UniSM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DTHIS_ID="C13_PROTO"' '-DGLOBAL_LOG_LEVEL=TEST' '-DAPPLICATION_STATE_HEADER="states.h"' '-DAPPLICATION_HEADER="app.h"' '-DSYSTEM_CONFIG_HEADER="system_config.h"' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/Rho/ -I../Core/Src/Rho/core -I../Core/Src/Rho/types -I../Core/Src/Rho/psm -I../Core/Src -I../Core/Src/UniSM -I../Core/Src/UniSM/OV9712 -I../Core/Src/UniSM/platforms -I../Core/Src/UniLog -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/UniSM/platform.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/UniSM/printers.o: ../Core/Src/UniSM/printers.c Core/Src/UniSM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DTHIS_ID="C13_PROTO"' '-DGLOBAL_LOG_LEVEL=TEST' '-DAPPLICATION_STATE_HEADER="states.h"' '-DAPPLICATION_HEADER="app.h"' '-DSYSTEM_CONFIG_HEADER="system_config.h"' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/Rho/ -I../Core/Src/Rho/core -I../Core/Src/Rho/types -I../Core/Src/Rho/psm -I../Core/Src -I../Core/Src/UniSM -I../Core/Src/UniSM/OV9712 -I../Core/Src/UniSM/platforms -I../Core/Src/UniLog -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/UniSM/printers.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/UniSM/system.o: ../Core/Src/UniSM/system.c Core/Src/UniSM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DTHIS_ID="C13_PROTO"' '-DGLOBAL_LOG_LEVEL=TEST' '-DAPPLICATION_STATE_HEADER="states.h"' '-DAPPLICATION_HEADER="app.h"' '-DSYSTEM_CONFIG_HEADER="system_config.h"' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/Rho/ -I../Core/Src/Rho/core -I../Core/Src/Rho/types -I../Core/Src/Rho/psm -I../Core/Src -I../Core/Src/UniSM -I../Core/Src/UniSM/OV9712 -I../Core/Src/UniSM/platforms -I../Core/Src/UniLog -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/UniSM/system.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

