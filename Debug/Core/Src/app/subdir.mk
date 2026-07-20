################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/app/RTD.c \
../Core/Src/app/calibration.c \
../Core/Src/app/cmt_utils.c \
../Core/Src/app/inverter_control.c \
../Core/Src/app/process_adc.c \
../Core/Src/app/process_can_msgs.c \
../Core/Src/app/process_pedals_data.c \
../Core/Src/app/state_control.c \
../Core/Src/app/vehicle.control.c 

OBJS += \
./Core/Src/app/RTD.o \
./Core/Src/app/calibration.o \
./Core/Src/app/cmt_utils.o \
./Core/Src/app/inverter_control.o \
./Core/Src/app/process_adc.o \
./Core/Src/app/process_can_msgs.o \
./Core/Src/app/process_pedals_data.o \
./Core/Src/app/state_control.o \
./Core/Src/app/vehicle.control.o 

C_DEPS += \
./Core/Src/app/RTD.d \
./Core/Src/app/calibration.d \
./Core/Src/app/cmt_utils.d \
./Core/Src/app/inverter_control.d \
./Core/Src/app/process_adc.d \
./Core/Src/app/process_can_msgs.d \
./Core/Src/app/process_pedals_data.d \
./Core/Src/app/state_control.d \
./Core/Src/app/vehicle.control.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/app/%.o Core/Src/app/%.su Core/Src/app/%.cyclo: ../Core/Src/app/%.c Core/Src/app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-app

clean-Core-2f-Src-2f-app:
	-$(RM) ./Core/Src/app/RTD.cyclo ./Core/Src/app/RTD.d ./Core/Src/app/RTD.o ./Core/Src/app/RTD.su ./Core/Src/app/calibration.cyclo ./Core/Src/app/calibration.d ./Core/Src/app/calibration.o ./Core/Src/app/calibration.su ./Core/Src/app/cmt_utils.cyclo ./Core/Src/app/cmt_utils.d ./Core/Src/app/cmt_utils.o ./Core/Src/app/cmt_utils.su ./Core/Src/app/inverter_control.cyclo ./Core/Src/app/inverter_control.d ./Core/Src/app/inverter_control.o ./Core/Src/app/inverter_control.su ./Core/Src/app/process_adc.cyclo ./Core/Src/app/process_adc.d ./Core/Src/app/process_adc.o ./Core/Src/app/process_adc.su ./Core/Src/app/process_can_msgs.cyclo ./Core/Src/app/process_can_msgs.d ./Core/Src/app/process_can_msgs.o ./Core/Src/app/process_can_msgs.su ./Core/Src/app/process_pedals_data.cyclo ./Core/Src/app/process_pedals_data.d ./Core/Src/app/process_pedals_data.o ./Core/Src/app/process_pedals_data.su ./Core/Src/app/state_control.cyclo ./Core/Src/app/state_control.d ./Core/Src/app/state_control.o ./Core/Src/app/state_control.su ./Core/Src/app/vehicle.control.cyclo ./Core/Src/app/vehicle.control.d ./Core/Src/app/vehicle.control.o ./Core/Src/app/vehicle.control.su

.PHONY: clean-Core-2f-Src-2f-app

