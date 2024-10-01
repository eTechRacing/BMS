################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LlibreriaESTEIB/LTC6811_2.c 

OBJS += \
./LlibreriaESTEIB/LTC6811_2.o 

C_DEPS += \
./LlibreriaESTEIB/LTC6811_2.d 


# Each subdirectory must supply rules for building sources it contributes
LlibreriaESTEIB/%.o LlibreriaESTEIB/%.su LlibreriaESTEIB/%.cyclo: ../LlibreriaESTEIB/%.c LlibreriaESTEIB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F777xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LlibreriaESTEIB

clean-LlibreriaESTEIB:
	-$(RM) ./LlibreriaESTEIB/LTC6811_2.cyclo ./LlibreriaESTEIB/LTC6811_2.d ./LlibreriaESTEIB/LTC6811_2.o ./LlibreriaESTEIB/LTC6811_2.su

.PHONY: clean-LlibreriaESTEIB

