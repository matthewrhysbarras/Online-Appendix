
#define GPIO_MOSI 35
#define GPIO_MISO 37
#define GPIO_SCLK 36
#define GPIO_CS 34
#define ReadDataSTM32 8

#define RCV_HOST    SPI2_HOST

#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include <string.h>

void my_post_trans_cb(spi_slave_transaction_t *trans);
void my_post_setup_cb(spi_slave_transaction_t *trans);

void SPI_SETUP(void);
void SPI_SLAVE_TRANSMIT(uint32_t length);
void SPI_SLAVE_RECV(uint32_t length);
void RESET_SPI(void);

spi_slave_transaction_t t;
extern DMA_ATTR uint8_t recvbuf[2048];
extern DMA_ATTR uint8_t sendbuf[2048];
extern uint8_t sending;
