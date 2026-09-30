#include "main.h"
#include "stm32h7xx_it.h"
#include "ADS1299.h"

uint8_t sampleRates[6] = {0xd6,0xd5,0xd4,0xd3,0xd2,0xd1};
/* USER CODE BEGIN PV */
// ADS1299 Register Address Values
//Read Only ID Register
uint8_t ID = 0x00;
//Global Settings Across Channels
uint8_t CONFIG1 = 0x01;
uint8_t CONFIG2 = 0x02;
uint8_t CONFIG3 = 0x03;
uint8_t LOFF 	= 0x04;
//Channel-Specific Settings
uint8_t CHSETS[8] = {0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C};
uint8_t BIAS_SENSP	= 0x0D;
uint8_t BIAS_SENSN	= 0x0E;
uint8_t LOFF_SENSP	= 0x0F;
uint8_t LOFF_SENSN 	= 0x10;
uint8_t LOFF_FLIP	= 0x11;
//Lead-Off Status Registers (Read-Only Registers)
uint8_t LOFF_STATP	= 0x12;
uint8_t LOFF_STATN	= 0x13;
//GPIO and OTHER Registers
//uint8_t GPIO	= 0x14;
uint8_t MISC1	= 0x15;
uint8_t MISC2	= 0x16;
uint8_t CONFIG4	= 0x17;
//The following commands are from pg40 of the data-sheet. Section 9.5.3 SPI Command Definitions
//System Commands
uint8_t WAKEUP 	= 0x02; //Wake-up from standby mode;
uint8_t STANDBY = 0x04; //Enter standby mode;
//uint8_t RESET 	= 0x06; //Reset the device
uint8_t START	= 0x08; //Start and restart (synchronise) conversions
uint8_t STOP 	= 0x0A; //Stop conversion
//Data Read Commands
uint8_t RDATAC	= 0x10; //Enable Read Data Continuous mode. This mode is the default mode at power-up.
uint8_t SDATAC	= 0x11; //Stop Read Data Continuously mode
uint8_t RDATA	= 0x12; //Read data by command; supports multiple read back.
//Register Read Commands
uint8_t RREG	= 0x20; //Read n nnnn registers starting at address r rrrr	#First Byte: 001r rrrr (2xh) #Second Byte: 000n nnnn
uint8_t WREG	= 0x40; //Write n nnnn registers starting at address r rrrr	#First Byte: 010r rrrr (4xh) #Second Byte: 000n nnnn

// Preserve the study error-indicator pulse sequence on PE1.
void IndicateADSError(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
}

uint8_t ADS_CS(uint8_t cs, uint8_t state){
    switch(cs)
    {
        case '1':
        	HAL_GPIO_WritePin(ADS1_CS_PORT, ADS1_CS_PIN, state);
            break;

        case '2':
        	HAL_GPIO_WritePin(GPIOE, ADS2_CS_Pin, state);
            break;

        case '3':
        	HAL_GPIO_WritePin(GPIOE, ADS3_CS_Pin, state);
            break;

        case '4':
        	HAL_GPIO_WritePin(GPIOE, ADS4_CS_Pin, state);
            break;

        default:
            break;
    }
}
#pragma GCC push_options
#pragma GCC optimize ("O0")
uint8_t InitADS(uint8_t cs, uint8_t mode, uint8_t rate, uint8_t*gains){

	//ALL IO PINS (EXP. RESET) MUST BE LOW ON BOOT!
	//CS LOW
	ADS_CS(1,0);
	ADS_CS(2,0);
	ADS_CS(3,0);
	ADS_CS(4,0);
	//HAL_GPIO_WritePin(ADS1_CS_PORT, ADS1_CS_PIN, 0);
	//PWDN LOW
	HAL_GPIO_WritePin(GPIOB, ADS1_PWDN_Pin, 0); //0

	//START LOW
	HAL_GPIO_WritePin(ADS1_START_PORT, ADS1_START_PIN, 0);
	//

	//RST HIGH
	HAL_GPIO_WritePin(GPIOB, ADS1_RESET_Pin, 1);
	//POWER ON WAIT
	HAL_Delay(150); //150
	//RST PULSE
	HAL_GPIO_WritePin(GPIOB, ADS1_RESET_Pin, 0);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, ADS1_RESET_Pin, 1);
	//CS HIGH
	ADS_CS(1,1);
	ADS_CS(2,1);
	ADS_CS(3,1);
	ADS_CS(4,1);

	//PWDN HIGH
	HAL_GPIO_WritePin(GPIOB, ADS1_PWDN_Pin, 1);

	HAL_Delay(500);

	StopRead(cs);
	uint8_t offset = ((1+cs) * 8);
	uint8_t errcode = 0;
	ReadReg(ID,1,cs); //Check Chip ID
	if ((RXADS_Buffer[0] & 0x0E) != 0x0E){IndicateADSError();errcode = 1;}
	if (errcode != 0) {return errcode;}
	errcode = SetCheckReg(CONFIG1,sampleRates[rate], 5); // set config 1 (errcode incremented by 4 to make room for other ADS's)
	if (errcode != 0) {return errcode;}
	errcode = SetCheckReg(CONFIG2,0xD0, 9); // set config 2 (errcode incremented by 4 to make room for other ADS's)
	if (errcode != 0) {return errcode;}
	if (mode == 0){ // normal mode
		for (uint8_t i = 0;i<8;i++){
			uint8_t combo = (uint8_t) ((gains[i+offset]<<4) + 0x08); // add gain with mode
			errcode = SetCheckReg(CHSETS[i],combo, 13);
			if (errcode != 0) {return errcode;}
		}
	}
	else if(mode == 1){ // test mode
		for (uint8_t i = 0;i<8;i++){
			uint8_t combo = (uint8_t) ((gains[i+offset]<<4) + 0x05); // add gain with mode
			errcode = SetCheckReg(CHSETS[i],combo, 13);
			if (errcode != 0) {return errcode;}
		}
	}
	if(cs == 3){
		errcode = SetCheckReg(CONFIG3,0xE0, 17); //E0 for no RLD   EC
		if (errcode != 0) {return errcode;}
	}
	else{
		errcode = SetCheckReg(CONFIG3,0xEC, 17); //EC
		if (errcode != 0) {return errcode;}
	}

	return 0; // return 0 for no errors
}
#pragma GCC pop_options
uint8_t SetCheckReg(uint8_t command, uint8_t value, uint8_t errcode){
	HAL_Delay(1);
	WriteReg(command,1,0,value); // Write the requested register value.
	HAL_Delay(1);
	ReadReg(command, 1, 0);
	if ((RXADS_Buffer[0] & value) != value){
		IndicateADSError();
		return errcode;
	}
	else{
		return 0; // Register check passed.
	}
}

void ReadReg(uint8_t regAddr, uint8_t numReg, uint8_t cs){
	//Delay_us(1);
	ADS_CS(cs,0);
	TXADS_Buffer[0]  = RREG + regAddr;
	TXADS_Buffer[1]  = 0;
	HAL_SPI_Transmit(&hspi1, TXADS_Buffer, 2,HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, RXADS_Buffer, 1,HAL_MAX_DELAY);
	ADS_CS(cs,1);
	//Delay_us(1);
}

void WriteReg(uint8_t regAddr,uint8_t numReg,uint8_t cs,uint8_t value){
	//Delay_us(1);
	ADS_CS(cs,0);
	TXADS_Buffer[0] = WREG + regAddr;
	TXADS_Buffer[1] = 0;
	TXADS_Buffer[2] = value;
	HAL_SPI_Transmit(&hspi1, TXADS_Buffer, 3, HAL_MAX_DELAY);
	ADS_CS(cs,1);
	//Delay_us(1);
}

void StartRead(uint8_t cs){ // needs extra CS adding on new board
	HAL_GPIO_WritePin(ADS1_START_PORT, ADS1_START_PIN, GPIO_PIN_SET);
	ADS_CS(cs,0);
	HAL_SPI_Transmit(&hspi1, &RDATAC, 1, HAL_MAX_DELAY);
	ADS_CS(cs,1);
}

void StopRead(uint8_t cs){ // needs extra CS adding on new board
	HAL_GPIO_WritePin(ADS1_START_PORT, ADS1_START_PIN, GPIO_PIN_RESET);
	ADS_CS(cs,0);
	//HAL_GPIO_WritePin(GPIOE, ADS2_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &SDATAC, 1, HAL_MAX_DELAY);
	ADS_CS(cs,1);
	//HAL_GPIO_WritePin(GPIOE, ADS2_CS_Pin, GPIO_PIN_SET);
}

void Delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}
