################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g431k6tx.s 

OBJS += \
./Core/Startup/startup_stm32g431k6tx.o 

S_DEPS += \
./Core/Startup/startup_stm32g431k6tx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32g431k6tx.o: ../Core/Startup/startup_stm32g431k6tx.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 '-DGLOBAL_LOG_LEVEL=TEST' '-DAPPLICATION_STATE_HEADER="states.h"' '-DAPPLICATION_HEADER="app.h"' -c -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/Core/Src/Rho/cpp_wrapper" -I../Core/Src/Core -I../Core/Src/Rho/core -I../Core/Src/Rho/types -I../Core/Src/Core/platform -I../Core/Src/Core/OV9712 -I../Core/Src/Rho/psm -I../Core/Src -I../Core/Src/UniLog -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32g431k6tx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

