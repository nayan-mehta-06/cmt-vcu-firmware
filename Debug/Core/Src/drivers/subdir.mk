################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/drivers/adc_driver.c \
../Core/Src/drivers/can_driver.c \
../Core/Src/drivers/cmt_M95512.c \
../Core/Src/drivers/usart_driver.c 

OBJS += \
./Core/Src/drivers/adc_driver.o \
./Core/Src/drivers/can_driver.o \
./Core/Src/drivers/cmt_M95512.o \
./Core/Src/drivers/usart_driver.o 

C_DEPS += \
./Core/Src/drivers/adc_driver.d \
./Core/Src/drivers/can_driver.d \
./Core/Src/drivers/cmt_M95512.d \
./Core/Src/drivers/usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/drivers/%.o Core/Src/drivers/%.su Core/Src/drivers/%.cyclo: ../Core/Src/drivers/%.c Core/Src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-drivers

clean-Core-2f-Src-2f-drivers:
	-$(RM) ./Core/Src/drivers/adc_driver.cyclo ./Core/Src/drivers/adc_driver.d ./Core/Src/drivers/adc_driver.o ./Core/Src/drivers/adc_driver.su ./Core/Src/drivers/can_driver.cyclo ./Core/Src/drivers/can_driver.d ./Core/Src/drivers/can_driver.o ./Core/Src/drivers/can_driver.su ./Core/Src/drivers/cmt_M95512.cyclo ./Core/Src/drivers/cmt_M95512.d ./Core/Src/drivers/cmt_M95512.o ./Core/Src/drivers/cmt_M95512.su ./Core/Src/drivers/usart_driver.cyclo ./Core/Src/drivers/usart_driver.d ./Core/Src/drivers/usart_driver.o ./Core/Src/drivers/usart_driver.su

.PHONY: clean-Core-2f-Src-2f-drivers

