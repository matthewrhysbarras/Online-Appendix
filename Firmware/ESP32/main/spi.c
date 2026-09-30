#include "spi.h"

DMA_ATTR uint8_t recvbuf[2048];
DMA_ATTR uint8_t sendbuf[2048];

//spi_slave_transaction_t t;
spi_slave_transaction_t t2;
uint8_t sending = 1;

//Called after a transaction is queued and ready for pickup by master. We use this to set the handshake line high.
void my_post_setup_cb(spi_slave_transaction_t *trans) {
    //WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1<<ReadDataSTM32));
	if (sending){
		gpio_set_level(ReadDataSTM32, 1);
	}
}

//Called after transaction is sent/received. We use this to set the handshake line low.
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    //WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1<<ReadDataSTM32));
	if (sending){
		gpio_set_level(ReadDataSTM32, 0);

	}
}

void SPI_SETUP(void){

	memset(&t, 0, sizeof(t));
    gpio_config_t io_conf =
    {
    	.intr_type = GPIO_INTR_DISABLE,
		.mode = GPIO_MODE_OUTPUT,
		.pin_bit_mask=(1<<ReadDataSTM32),
    };
    gpio_config(&io_conf);



    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=0,
        .spics_io_num=GPIO_CS,
        .queue_size=1,
        .flags=0,
        .post_setup_cb=my_post_setup_cb,
        .post_trans_cb=my_post_trans_cb
    };

    //Initialize SPI slave interface
    spi_slave_initialize(RCV_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);

}

void SPI_SLAVE_TRANSMIT(uint32_t length){

    //Set up a transaction of length bytes to send/receive
    t.length=length*8;
    t.tx_buffer=sendbuf;
    t.rx_buffer=recvbuf;
    spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);

}

void SPI_SLAVE_RECV(uint32_t length){
    //Set up a transaction of length bytes to send/receive
    t.length=length*8;
    t.tx_buffer=sendbuf;
    t.rx_buffer=recvbuf;
    spi_slave_transmit(RCV_HOST, &t, 10);
    //spi_slave_queue_trans(RCV_HOST, &t, portMAX_DELAY);

}

void RESET_SPI(void){
	gpio_set_level(ReadDataSTM32, 0);
	spi_slave_free(RCV_HOST);
	SPI_SETUP();
}

