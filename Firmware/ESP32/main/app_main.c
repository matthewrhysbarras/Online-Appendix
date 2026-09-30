/* SPI Slave example, receiver (uses SPI Slave driver to communicate with sender)

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/igmp.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "soc/rtc_periph.h"

#include "esp_log.h"
#include "esp_spi_flash.h"

#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "sdkconfig.h"
#include "tusb_console.h"


#include "main.h"
#include "wifi.h"
#include "spi.h"


static const char *TAG = "example";

EventGroupHandle_t event_group_dude;

/* Study SPI-to-TCP bridge. Command transactions use the GPIO8 handshake;
   streaming receives 2048-byte packets and adds a counter at bytes 1984-1987.
   The PC receiver, not this firmware, publishes to LSL. */

//Variables
uint8_t		StreamingWiFi = 0;
uint8_t		SettingUp = 0;
uint8_t		sampleCounter = 0;
uint32_t	totalSamples = 0;
uint8_t sendtosocket = 0;


int commandcount = 0;
uint8_t commandstring[42];



void CreateWifiAP(void){
	wifi_ap(EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS); // Access-point settings are defined in wifi.h.
	event_group_dude = xEventGroupCreate();
	xTaskCreate(&monitor_task, "monitor_task", 2048, NULL, 5, NULL);
	xTaskCreate(&tcp_server_task, "tcp_server", 4096,  (void*)AF_INET, 5, NULL);
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}


void check_command(uint8_t* commandstring)
{
	sendtosocket = 0;
	if (SettingUp == 0){
		printf("\n%s\n", (char*)commandstring);
		if (strcmp((char*)commandstring, "Setup_Device") == 0)
		{
			sending = 1;
			RESET_SPI();
			printf("Setting up\n");
			memset(&sendbuf, 0, 44);
			sendbuf[0] =  0x55;
			SPI_SLAVE_TRANSMIT(1);
			SettingUp = 1;

		}
		else if (strcmp((char*)commandstring, "Start_Stream") == 0){
			printf("Start stream\n");
			memset(&sendbuf, 0, 44);
			sendbuf[0] =  0xAA;
			sending = 1;
			RESET_SPI();
			SPI_SLAVE_TRANSMIT(1);
			sampleCounter = 0;
			StreamingWiFi = 1;

			sending = 0;


		}
		else if (strcmp((char*)commandstring, "STOP!")==0){
			StreamingWiFi = 0;
			//memset(&recvbuf, 0, 2048);
			printf("Stop stream\n");
			sending = 1;
			RESET_SPI();
			memset(&sendbuf, 0, 44);
			sendbuf[0] =  0x42;
			SPI_SLAVE_TRANSMIT(1);
			spi_slave_free(RCV_HOST);

		}
		else{
			StreamingWiFi = 0;
		}
	}
	else
	{
		memset(&sendbuf, 0, 44);
		for(int i = 0; i < 42; i++){
			sendbuf[i] = commandstring[i];
			printf("Command string: 0x%02hhX\n",sendbuf[i]);
		}
		sending = 1;
		//vTaskDelay(10 / portTICK_PERIOD_MS);
		SPI_SLAVE_TRANSMIT(44);
		sending = 0;
		printf("%d\n",sizeof(commandstring));
		vTaskDelay(1 / portTICK_RATE_MS);
		SettingUp = 0;
	}
	clear_command_string();
}

// command processing functions
void clear_command_string(void){
	for (int i = 0; i<42; i++){
		commandstring[i] = 0x00;
	}
	commandcount = 0;
}

//Main application
void app_main(void)
{
	sendtosocket = 0;
    /* Setting TinyUSB up */
    ESP_LOGI(TAG, "USB initialization");
    tinyusb_config_t tusb_cfg = { 0 }; // the configuration uses default values
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    tinyusb_config_cdcacm_t amc_cfg = { 0 }; // the configuration uses default values
    ESP_ERROR_CHECK(tusb_cdc_acm_init(&amc_cfg));
    esp_tusb_init_console(TINYUSB_CDC_ACM_0); // log to usb

    SettingUp = 0;

    ESP_LOGI(TAG, "USB initialization DONE");
    SPI_SETUP();
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Allow STM32 to boot first
    CreateWifiAP();

    gpio_set_direction(16, GPIO_MODE_OUTPUT);
    gpio_set_direction(6, GPIO_MODE_OUTPUT);

    while(1) {
    	if(StreamingWiFi == 0)
    	{
			gpio_set_level(16, 0);
			vTaskDelay(100 / portTICK_PERIOD_MS);
			gpio_set_level(16, 1);
			vTaskDelay(100 / portTICK_PERIOD_MS);
    	}
    	else
    	{
				// Preserve blocking reception and forwarding in the main loop.
				gpio_set_level(6, 1);
				SPI_SLAVE_RECV(2048);
				gpio_set_level(6, 0);
				totalSamples++;
				recvbuf[1984]=(totalSamples >> 24) & 0xFF;
				recvbuf[1985]=(totalSamples >> 16) & 0xFF;
				recvbuf[1986]=(totalSamples >> 8) & 0xFF;
				recvbuf[1987]=(totalSamples) & 0xFF;
				sendtosocket = 1;

				send_to_socket((char*)recvbuf, sock, 2048);
    	}
    }
}
