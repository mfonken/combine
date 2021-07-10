################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Rho/psm/fsm.c \
../Core/Src/Rho/psm/gmm.c \
../Core/Src/Rho/psm/hmm.c \
../Core/Src/Rho/psm/psm.c 

OBJS += \
./Core/Src/Rho/psm/fsm.o \
./Core/Src/Rho/psm/gmm.o \
./Core/Src/Rho/psm/hmm.o \
./Core/Src/Rho/psm/psm.o 

C_DEPS += \
./Core/Src/Rho/psm/fsm.d \
./Core/Src/Rho/psm/gmm.d \
./Core/Src/Rho/psm/hmm.d \
./Core/Src/Rho/psm/psm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Rho/psm/fsm.o: ../Core/Src/Rho/psm/fsm.c Core/Src/Rho/psm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -D__OV9712__ '-DGLOBAL_LOG_LEVEL=TEST' -D__RHO__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/psm/fsm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Rho/psm/gmm.o: ../Core/Src/Rho/psm/gmm.c Core/Src/Rho/psm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -D__OV9712__ '-DGLOBAL_LOG_LEVEL=TEST' -D__RHO__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/psm/gmm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Rho/psm/hmm.o: ../Core/Src/Rho/psm/hmm.c Core/Src/Rho/psm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -D__OV9712__ '-DGLOBAL_LOG_LEVEL=TEST' -D__RHO__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/psm/hmm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Rho/psm/psm.o: ../Core/Src/Rho/psm/psm.c Core/Src/Rho/psm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -D__OV9712__ '-DGLOBAL_LOG_LEVEL=TEST' -D__RHO__ -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -fstack-usage -MMD -MP -MF"Core/Src/Rho/psm/psm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

