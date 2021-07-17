################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Rho/types/kalman.c \
../Core/Src/Rho/types/matvec.c \
../Core/Src/Rho/types/pid.c \
../Core/Src/Rho/types/statistics.c 

OBJS += \
./Core/Src/Rho/types/kalman.o \
./Core/Src/Rho/types/matvec.o \
./Core/Src/Rho/types/pid.o \
./Core/Src/Rho/types/statistics.o 

C_DEPS += \
./Core/Src/Rho/types/kalman.d \
./Core/Src/Rho/types/matvec.d \
./Core/Src/Rho/types/pid.d \
./Core/Src/Rho/types/statistics.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Rho/types/kalman.o: ../Core/Src/Rho/types/kalman.c Core/Src/Rho/types/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DGLOBAL_LOG_LEVEL=TEST' -D__OV9712__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/types/kalman.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Rho/types/matvec.o: ../Core/Src/Rho/types/matvec.c Core/Src/Rho/types/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DGLOBAL_LOG_LEVEL=TEST' -D__OV9712__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/types/matvec.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Rho/types/pid.o: ../Core/Src/Rho/types/pid.c Core/Src/Rho/types/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DGLOBAL_LOG_LEVEL=TEST' -D__OV9712__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/types/pid.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Rho/types/statistics.o: ../Core/Src/Rho/types/statistics.c Core/Src/Rho/types/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ '-DGLOBAL_LOG_LEVEL=TEST' -D__OV9712__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/types/statistics.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

