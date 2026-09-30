/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ADS1299.h"
#include "TLC5940.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t buf_size = 994;
uint8_t RX_Buffer[255] = {0};
uint8_t TX_Buffer[255] = {0};
uint8_t RXADS_Buffer[27] = {0}; //27
uint8_t TXADS_Buffer[27] = {0};
uint8_t TX_Buffer1[128]; //994 103
uint8_t TX_Buffer2[128];
uint8_t TX_TEST[994];
uint8_t waitingSetup = 1;
uint8_t streaming = 0;
uint32_t rolloverCount = 50;
uint8_t doInit=0;
uint8_t doTransfer = 0;
uint8_t ADS_DATA_RDY = 0;
uint8_t sampleCounter = 0;
uint32_t totalSamples = 0;
uint8_t sendData = 0;
uint8_t highlowbool = 0;
uint8_t TxTLC_Buffer_0[24]; //128
uint8_t TxTLC_Buffer_1[24];
uint8_t TxTLC_Buffer_BLANK[24];

uint8_t TxTLC_Buffer[12][24];


uint8_t ledCurrentState = 0;
uint8_t totalNumLEDs = 6;
uint8_t RxESP_Buffer[2048];
uint8_t TxESP_Buffer[2048];
uint8_t STM32_State = 0;
uint8_t ESP32_Command = 0;
uint8_t ADS_Buffer[27];
uint8_t gainarray[32] = {0};

uint32_t fnirsCounter = 0;
uint32_t fNIRS_LED_ID = 666;



// Historical retry state: increments of ten also mark completion.
uint8_t adsInitAttemptState = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM16_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI3_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
static void MX_SPI1_Init_Fast(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	RxESP_Buffer[0] = 1;
	RxESP_Buffer[37] = 1;
	STM32_State = 3;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM16_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_SPI3_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */


  HAL_TIM_Base_Start(&htim1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  for (int var1 = 0; var1 < 12; ++var1) {
	  for (int var2 = 0; var2 < 24; ++var2) {
		  TxTLC_Buffer[var1][var2] = 0;
	  }
  }

  TxTLC_Buffer[0][23] = 0xFF;
  TxTLC_Buffer[0][22] = 0x0F;

  TxTLC_Buffer[1][22] = 0xF0;
  TxTLC_Buffer[1][21] = 0xFF;

  TxTLC_Buffer[2][20] = 0xFF;
  TxTLC_Buffer[2][19] = 0x0F;

  TxTLC_Buffer[3][19] = 0xF0;
  TxTLC_Buffer[3][18] = 0xFF;

  TxTLC_Buffer[4][17] = 0xFF;
  TxTLC_Buffer[4][16] = 0x0F;

  TxTLC_Buffer[5][16] = 0xF0;
  TxTLC_Buffer[5][15] = 0xFF;

  TxTLC_Buffer[4][17] = 0xFF;
  TxTLC_Buffer[4][16] = 0x0F;

  TxTLC_Buffer[5][16] = 0xF0;
  TxTLC_Buffer[5][15] = 0xFF;

  TxTLC_Buffer[6][14] = 0xFF;
  TxTLC_Buffer[6][13] = 0x0F;

  TxTLC_Buffer[7][13] = 0xF0;
  TxTLC_Buffer[7][12] = 0xFF;

  TxTLC_Buffer[6][14] = 0xFF;
  TxTLC_Buffer[6][13] = 0x0F;

  TxTLC_Buffer[7][13] = 0xF0;
  TxTLC_Buffer[7][12] = 0xFF;

  TxTLC_Buffer[8][11] = 0xFF;
  TxTLC_Buffer[8][10] = 0x0F;

  TxTLC_Buffer[9][10] = 0xF0;
  TxTLC_Buffer[9][9] =  0xFF;

  TxTLC_Buffer[10][8] = 0xFF;
  TxTLC_Buffer[10][7] = 0x0F;

  TxTLC_Buffer[11][7] = 0xF0;
  TxTLC_Buffer[11][6] =  0xFF;

for (int var = 0; var < 24; ++var) {
	TxTLC_Buffer_BLANK[var] = 0;
}

  //HAL_SPI_TransmitReceive (&hspi3, TxESP_Buffer, RxESP_Buffer, 128, HAL_MAX_DELAY);

	while (1)
	{
		switch(STM32_State)
		{
			case 0: //BOOT
				HAL_GPIO_WritePin(GPIOE, LED_B_Pin,1);
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOE, LED_B_Pin,0);
				HAL_Delay(200);
				break;

			case 1: //Reading from ESP32
				//HAL_Delay(500);
				//\while(HAL_GPIO_ReadPin(GPIOD ,GPIO_PIN_8) == 1){
					HAL_SPI_TransmitReceive_DMA(&hspi3, TxESP_Buffer, RxESP_Buffer, 1);
					while(HAL_DMA_GetState(&hdma_spi3_tx) == 2){}
				//}
				//ESP32_Command = RxESP_Buffer[0];

				switch(RxESP_Buffer[0])
				{
					case 0xAA:
						HAL_SPI_DeInit(&hspi1);
						MX_SPI1_Init_Fast();
						HAL_NVIC_EnableIRQ(EXTI4_IRQn);
						for (int ADS_SEL = 1; ADS_SEL < 5; ++ADS_SEL) {
							StartRead(ADS_SEL);
						}
						STM32_State = 2;
						break;

					case 0x55:
						HAL_Delay(500); //Must be greater than 100ms to 'beat' delay in Python plotter
						HAL_SPI_TransmitReceive_DMA(&hspi3, TxESP_Buffer, RxESP_Buffer, 44);
						while(HAL_DMA_GetState(&hdma_spi3_tx) == 2){}
						STM32_State = 3;
						break;

					case 0x42:
						STM32_State = 5;
						break;
					//case 0x00:
					//	STM32_State = 0;
					//default:
					//	STM32_State = 0;
				}


				break;

			case 2: //IDLE
				break;

			case 3: // SET UP ADS1299
				//HAL_Delay(500);
				HAL_SPI_DeInit(&hspi1);
				MX_SPI1_Init();

				for (uint8_t i = 0; i<32;i++){
					gainarray[i] = RxESP_Buffer[i+5];
				}

				  HAL_GPIO_WritePin(GPIOE, LED_G_Pin,1);
				  HAL_Delay(100);
				  HAL_GPIO_WritePin(GPIOE, LED_G_Pin,0);

				  adsInitAttemptState = 0;
				  while(adsInitAttemptState < 3){
				for (int ADS_SEL = 1; ADS_SEL < 5; ++ADS_SEL) {

					int CHECKME = InitADS(ADS_SEL, RxESP_Buffer[0], RxESP_Buffer[37], gainarray);
					//int CHECKME = InitADS(ADS_SEL,1,1,gainarray);
					if(CHECKME != 0){
						  while (adsInitAttemptState == 2)
						  {
								HAL_GPIO_WritePin(GPIOE, LED_R_Pin,1);
								HAL_Delay(200);
								HAL_GPIO_WritePin(GPIOE, LED_R_Pin,0);
								HAL_Delay(200);
								STM32_State = 0;
								adsInitAttemptState += 10;
						  }
						  adsInitAttemptState ++;
					}
					else{adsInitAttemptState += 10;}
					HAL_GPIO_WritePin(GPIOE, LED_R_Pin,0);

				}
				  }
				  STM32_State = 0;
				break;

			case 4: // ADS1299 data ready: service the five-phase optical slot.
				// Phase 2 tags one record; 666 (0x029A) marks unselected records.
				switch(fnirsCounter)
				{
					case 0:
						updateLED(TxTLC_Buffer[ledCurrentState]);
						break;

					case 1:
						break;

					case 2:
						fNIRS_LED_ID = ledCurrentState;
						ledCurrentState++;
						if(ledCurrentState == 12) {ledCurrentState = 0;} //if(ledCurrentState == 12) {ledCurrentState = 0;}
						break;

					case 3:
						fNIRS_LED_ID = 666;
						updateLED(TxTLC_Buffer_BLANK);
						break;

					case 4:
						break;

					default:
						updateLED(TxTLC_Buffer_BLANK);
				}

				// Descending 27-byte reads overlap status words: the final record
				// holds a three-byte state followed by 32 three-byte channel words.
				HAL_GPIO_WritePin(ADS1_CS_PORT, ADS1_CS_PIN, GPIO_PIN_RESET);
				HAL_SPI_TransmitReceive_DMA(&hspi1, TXADS_Buffer, &TxESP_Buffer[72+(sampleCounter*99)], 27); //RXADS_Buffer
				while(HAL_DMA_GetState(&hdma_spi1_tx) == 2){}
				HAL_GPIO_WritePin(ADS1_CS_PORT, ADS1_CS_PIN, GPIO_PIN_SET);

				HAL_GPIO_WritePin(GPIOE, ADS2_CS_Pin, GPIO_PIN_RESET);
				HAL_SPI_TransmitReceive_DMA(&hspi1, TXADS_Buffer, &TxESP_Buffer[48+(sampleCounter*99)], 27); //RXADS_Buffer
				while(HAL_DMA_GetState(&hdma_spi1_tx) == 2){}
				HAL_GPIO_WritePin(GPIOE, ADS2_CS_Pin, GPIO_PIN_SET);

				HAL_GPIO_WritePin(GPIOE, ADS4_CS_Pin, GPIO_PIN_RESET);
				HAL_SPI_TransmitReceive_DMA(&hspi1, TXADS_Buffer, &TxESP_Buffer[24+(sampleCounter*99)], 27); //RXADS_Buffer
				while(HAL_DMA_GetState(&hdma_spi1_tx) == 2){}
				HAL_GPIO_WritePin(GPIOE, ADS4_CS_Pin, GPIO_PIN_SET);

				HAL_GPIO_WritePin(GPIOE, ADS3_CS_Pin, GPIO_PIN_RESET);
				HAL_SPI_TransmitReceive_DMA(&hspi1, TXADS_Buffer, &TxESP_Buffer[sampleCounter*99], 27); //RXADS_Buffer
				while(HAL_DMA_GetState(&hdma_spi1_tx) == 2){}
				HAL_GPIO_WritePin(GPIOE, ADS3_CS_Pin, GPIO_PIN_SET);

				TxESP_Buffer[sampleCounter*99]=(fNIRS_LED_ID >> 16) & 0xFF;
				TxESP_Buffer[1+sampleCounter*99]=(fNIRS_LED_ID >> 8) & 0xFF;
				TxESP_Buffer[2+sampleCounter*99]=(fNIRS_LED_ID) & 0xFF;

				if(sampleCounter == 19){
					totalSamples++;
					TxESP_Buffer[1980]=(totalSamples >> 24) & 0xFF;
					TxESP_Buffer[1981]=(totalSamples >> 16) & 0xFF;
					TxESP_Buffer[1982]=(totalSamples >> 8) & 0xFF;
					TxESP_Buffer[1983]=(totalSamples) & 0xFF;

					HAL_SPI_TransmitReceive_DMA(&hspi3, TxESP_Buffer, RxESP_Buffer, 2048);
					while(HAL_DMA_GetState(&hdma_spi3_tx) == 2){}


					sampleCounter = 0;
				}
				else{
					sampleCounter++;
				}

				fnirsCounter++;

				if(fnirsCounter == 5){ //19
					fnirsCounter = 0;
				}

				STM32_State = 2;

				break;

			case 5: //STOP ADS READ
				// RESET COUNTER FOR BUFFER AND LEDS etc
				// Stop continuous conversion reads on all four converters.
				fnirsCounter = 0;
				totalSamples = 0;
				for (int var = 0; var < 2; ++var) {
					for (int ADS_SEL = 1; ADS_SEL < 5; ++ADS_SEL) {
						StopRead(ADS_SEL);
					}
				}

				STM32_State = 0;
				break;
			}




	}


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_CSI;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_CSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 12;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 0x0;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 80-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 200-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 16000-1;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED_B_Pin|LED_G_Pin|LED_R_Pin|ADS4_CS_Pin
                          |ADS3_CS_Pin|ADS2_CS_Pin|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS1_CS_GPIO_Port, ADS1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ADS1_START_Pin|TLC5940_GSCLK_Pin|TLC5940_VPRG_Pin|TLC5940_XLAT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ADS1_RESET_Pin|ADS1_PWDN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TLC5940_BLANK_GPIO_Port, TLC5940_BLANK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED_B_Pin LED_G_Pin LED_R_Pin ADS4_CS_Pin
                           ADS3_CS_Pin ADS2_CS_Pin PE0 PE1 */
  GPIO_InitStruct.Pin = LED_B_Pin|LED_G_Pin|LED_R_Pin|ADS4_CS_Pin
                          |ADS3_CS_Pin|ADS2_CS_Pin|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS1_CS_Pin */
  GPIO_InitStruct.Pin = ADS1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS1_DRDY_Pin */
  GPIO_InitStruct.Pin = ADS1_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADS1_DRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS1_START_Pin */
  GPIO_InitStruct.Pin = ADS1_START_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS1_START_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ADS1_RESET_Pin ADS1_PWDN_Pin */
  GPIO_InitStruct.Pin = ADS1_RESET_Pin|ADS1_PWDN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP32_DRDY_Pin */
  GPIO_InitStruct.Pin = ESP32_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ESP32_DRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : TLC5940_BLANK_Pin TLC5940_GSCLK_Pin TLC5940_VPRG_Pin TLC5940_XLAT_Pin */
  GPIO_InitStruct.Pin = TLC5940_BLANK_Pin|TLC5940_GSCLK_Pin|TLC5940_VPRG_Pin|TLC5940_XLAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : TLC5940_XERR_Pin */
  GPIO_InitStruct.Pin = TLC5940_XERR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TLC5940_XERR_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len) {
    CDC_Transmit_FS(*ptr, len);
    return len;
}



// FAST SPI1 BEGIN -- SPI_BAUDRATEPRESCALER_4
static void MX_SPI1_Init_Fast(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4; //8
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}
// FAST SPI1 END


// Callback: timer has rolled over
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	//StreamData = lowarray;
  // Check which version of the timer triggered this callback and toggle LED
  //if (htim == &htim16 )
  //{
  //  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  //}
}
//void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi)
//{
	//HAL_Delay(10);
	//HAL_SPI_Transmit_DMA(&hspi3, lowarray, 99);
	//InitADS();
	//HAL_SPI_Receive_IT(&hspi3, RX_Buffer, 1);
    //HAL_UART_Transmit_IT(&huart1, RX_Buffer, BUFFER_SIZE);
//}
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
	//HAL_Delay(10);
	//HAL_SPI_Transmit_DMA(&hspi3, lowarray, 99);

    //HAL_UART_Transmit_IT(&huart1, RX_Buffer, BUFFER_SIZE);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
