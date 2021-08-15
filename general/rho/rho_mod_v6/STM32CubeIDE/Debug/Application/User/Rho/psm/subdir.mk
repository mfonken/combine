################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/Rho/psm/fsm.c \
../Application/User/Rho/psm/gmm.c \
../Application/User/Rho/psm/hmm.c \
../Application/User/Rho/psm/psm.c 

OBJS += \
./Application/User/Rho/psm/fsm.o \
./Application/User/Rho/psm/gmm.o \
./Application/User/Rho/psm/hmm.o \
./Application/User/Rho/psm/psm.o 

C_DEPS += \
./Application/User/Rho/psm/fsm.d \
./Application/User/Rho/psm/gmm.d \
./Application/User/Rho/psm/hmm.d \
./Application/User/Rho/psm/psm.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Rho/psm/%.o: ../Application/User/Rho/psm/%.c Application/User/Rho/psm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -D__STM32__ -DGLOBAL_LOG_LEVEL=TEST -D__RHO__ -D__OV9712__ -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/App" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniSM" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniLog" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/Rho" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

