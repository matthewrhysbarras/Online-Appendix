/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern TIM_HandleTypeDef htim1;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern uint32_t buf_size;
extern uint8_t RXADS_Buffer[27];
extern uint8_t RXADS_Buffer_2[128];
extern uint8_t TXADS_Buffer[27];
extern uint8_t RX_Buffer[255];
extern uint8_t TX_Buffer[255];
extern uint8_t stat1;
extern uint8_t receiveSetup;
extern uint8_t setupBytes[42];
extern uint8_t waitingSetup;
extern uint8_t TX_Buffer1[128]; //994 //103
extern uint8_t TX_Buffer2[128];
extern uint8_t TX_TEST[994];
extern uint8_t streaming;
extern uint32_t rolloverCount;
extern uint8_t doInit;
extern uint8_t doTransfer;
extern uint8_t ADS_DATA_RDY;
extern uint8_t sampleCounter;
extern uint32_t totalSamples;
extern uint8_t sendData;
extern uint8_t highlowbool;
extern uint8_t TxTLC_Buffer_0[24];
extern uint8_t TxTLC_Buffer_1[24];
extern uint8_t ledCurrentState;
extern uint8_t totalNumLEDs;
extern uint8_t RxESP_Buffer[2048];
extern uint8_t TxESP_Buffer[2048];
extern uint8_t STM32_State;




/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//void MX_SPI3_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_B_Pin GPIO_PIN_2
#define LED_B_GPIO_Port GPIOE
#define LED_G_Pin GPIO_PIN_3
#define LED_G_GPIO_Port GPIOE
#define LED_R_Pin GPIO_PIN_4
#define LED_R_GPIO_Port GPIOE
#define ADS1_CS_Pin GPIO_PIN_4
#define ADS1_CS_GPIO_Port GPIOA
#define ADS1_DRDY_Pin GPIO_PIN_4
#define ADS1_DRDY_GPIO_Port GPIOC
#define ADS1_DRDY_EXTI_IRQn EXTI4_IRQn
#define ADS1_START_Pin GPIO_PIN_5
#define ADS1_START_GPIO_Port GPIOC
#define ADS1_RESET_Pin GPIO_PIN_0
#define ADS1_RESET_GPIO_Port GPIOB
#define ADS1_PWDN_Pin GPIO_PIN_1
#define ADS1_PWDN_GPIO_Port GPIOB
#define ADS4_CS_Pin GPIO_PIN_10
#define ADS4_CS_GPIO_Port GPIOE
#define ADS3_CS_Pin GPIO_PIN_11
#define ADS3_CS_GPIO_Port GPIOE
#define ADS2_CS_Pin GPIO_PIN_12
#define ADS2_CS_GPIO_Port GPIOE
#define ESP32_DRDY_Pin GPIO_PIN_8
#define ESP32_DRDY_GPIO_Port GPIOD
#define ESP32_DRDY_EXTI_IRQn EXTI9_5_IRQn
#define TLC5940_BLANK_Pin GPIO_PIN_6
#define TLC5940_BLANK_GPIO_Port GPIOC
#define TLC5940_GSCLK_Pin GPIO_PIN_7
#define TLC5940_GSCLK_GPIO_Port GPIOC
#define TLC5940_VPRG_Pin GPIO_PIN_8
#define TLC5940_VPRG_GPIO_Port GPIOC
#define TLC5940_XLAT_Pin GPIO_PIN_9
#define TLC5940_XLAT_GPIO_Port GPIOC
#define TLC5940_XERR_Pin GPIO_PIN_10
#define TLC5940_XERR_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
