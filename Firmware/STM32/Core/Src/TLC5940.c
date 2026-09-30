#include "main.h"
#include "TLC5940.h"


void updateLED(uint8_t* data)
{
HAL_SPI_Transmit(&hspi2, data, 24, 1); // Blocking transfer of 24 bytes with the historical 1 ms HAL timeout.
HAL_GPIO_WritePin(TLC5940_BLANK_GPIO_Port, TLC5940_BLANK_Pin, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC, TLC5940_XLAT_Pin, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC, TLC5940_XLAT_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(TLC5940_BLANK_GPIO_Port, TLC5940_BLANK_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOC, TLC5940_GSCLK_Pin, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC, TLC5940_GSCLK_Pin, GPIO_PIN_RESET);
}
