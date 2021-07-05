################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Core/platforms/stm32_interface.c 

OBJS += \
./Core/Src/Core/platforms/stm32_interface.o 

C_DEPS += \
./Core/Src/Core/platforms/stm32_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Core/platforms/stm32_interface.o: ../Core/Src/Core/platforms/stm32_interface.c Core/Src/Core/platforms/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DTHIS_ID="C13_PROTO"' '-DGLOBAL_LOG_LEVEL=TEST' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/Core -I../Core/Src/Core/OV9712 -I../Core/Src/Rho/ -I../Core/Src/Rho/core -I../Core/Src/Rho/types -I../Core/Src/Rho/psm -I../Core/Src -I../Core/Src/Core/platforms -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/Core/platforms/stm32_interface.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

