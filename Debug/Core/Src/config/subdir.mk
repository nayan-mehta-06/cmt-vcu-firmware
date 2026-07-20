################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/config/amk_data_structs.c \
../Core/Src/config/vehicle_data_structs.c 

OBJS += \
./Core/Src/config/amk_data_structs.o \
./Core/Src/config/vehicle_data_structs.o 

C_DEPS += \
./Core/Src/config/amk_data_structs.d \
./Core/Src/config/vehicle_data_structs.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/config/%.o Core/Src/config/%.su Core/Src/config/%.cyclo: ../Core/Src/config/%.c Core/Src/config/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-config

clean-Core-2f-Src-2f-config:
	-$(RM) ./Core/Src/config/amk_data_structs.cyclo ./Core/Src/config/amk_data_structs.d ./Core/Src/config/amk_data_structs.o ./Core/Src/config/amk_data_structs.su ./Core/Src/config/vehicle_data_structs.cyclo ./Core/Src/config/vehicle_data_structs.d ./Core/Src/config/vehicle_data_structs.o ./Core/Src/config/vehicle_data_structs.su

.PHONY: clean-Core-2f-Src-2f-config

