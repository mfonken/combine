################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/User/Startup/startup_stm32g431kbux.s 

OBJS += \
./Application/User/Startup/startup_stm32g431kbux.o 

S_DEPS += \
./Application/User/Startup/startup_stm32g431kbux.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Startup/%.o: ../Application/User/Startup/%.s Application/User/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -D__STM32__ -DGLOBAL_LOG_LEVEL=TEST -D__OV9712__ -c -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/App" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniSM" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/Rho" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/UniLog" -I"C:/Users/Matthew Fonken/Desktop/marbl/Techincal/combine/general/rho/rho_mod_v6/STM32CubeIDE/Application/User/Rho/cpp_wrapper" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

