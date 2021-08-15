################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Matthew\ Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/main.c \
C:/Users/Matthew\ Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/stm32g4xx_hal_msp.c \
C:/Users/Matthew\ Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/stm32g4xx_it.c \
../Application/User/Core/syscalls.c \
../Application/User/Core/sysmem.c 

OBJS += \
./Application/User/Core/main.o \
./Application/User/Core/stm32g4xx_hal_msp.o \
./Application/User/Core/stm32g4xx_it.o \
./Application/User/Core/syscalls.o \
./Application/User/Core/sysmem.o 

C_DEPS += \
./Application/User/Core/main.d \
./Application/User/Core/stm32g4xx_hal_msp.d \
./Application/User/Core/stm32g4xx_it.d \
./Application/User/Core/syscalls.d \
./Application/User/Core/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Core/main.o: C:/Users/Matthew\ Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/main.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -DGLOBAL_LOG_LEVEL=TEST -D__RHO__ -D__OV9712__ -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/App" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniSM" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniLog" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/Rho" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/Core/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/stm32g4xx_hal_msp.o: C:/Users/Matthew\ Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/stm32g4xx_hal_msp.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -DGLOBAL_LOG_LEVEL=TEST -D__RHO__ -D__OV9712__ -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/App" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniSM" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniLog" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/Rho" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/Core/stm32g4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/stm32g4xx_it.o: C:/Users/Matthew\ Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/stm32g4xx_it.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -DGLOBAL_LOG_LEVEL=TEST -D__RHO__ -D__OV9712__ -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/App" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniSM" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniLog" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/Rho" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/Core/stm32g4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/Core/%.o: ../Application/User/Core/%.c Application/User/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -DGLOBAL_LOG_LEVEL=TEST -D__RHO__ -D__OV9712__ -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/App" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniSM" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniLog" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/Rho" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

