/*
 * ADS1299.h
 *
 *  Created on: 7 Dec 2021
 *      Author: Liam
 */

#ifndef INC_ADS1299_H_
#define INC_ADS1299_H_

#define ADS1_CS_PORT GPIOA
#define ADS1_CS_PIN GPIO_PIN_4
#define ADS1_DRDY_PORT GPIOC
#define ADS1_DRDY_PIN GPIO_PIN_4
#define ADS1_START_PORT GPIOC
#define ADS1_START_PIN GPIO_PIN_5
#define ADS1_RESET_PORT GPIOB
#define ADS1_RESET_PIN GPIO_PIN_0
#define ADS1_PWDN_PORT GPIOB
#define ADS1_PWDN_PIN GPIO_PIN_1

void IndicateADSError(void);
uint8_t InitADS(uint8_t cs, uint8_t rate, uint8_t mode, uint8_t*gains);
uint8_t SetCheckReg(uint8_t command, uint8_t value, uint8_t errcode);
void ReadReg(uint8_t regAddr, uint8_t numReg, uint8_t cs);
void WriteReg(uint8_t regAddr,uint8_t numReg,uint8_t cs,uint8_t value);
void StartRead(uint8_t regAdcsdr);
void StopRead(uint8_t cs);
void Delay_us (uint32_t us);

#endif /* INC_ADS1299_H_ */
