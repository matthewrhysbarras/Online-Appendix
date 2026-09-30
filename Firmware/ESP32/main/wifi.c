// Wifi related libraries
#include "esp_wifi.h"
#include "esp_system.h"
//#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sys.h"
// Socket related libraries
#include <sys/param.h>
#include "esp_netif.h"
//#include "protocol_examples_common.h"
#include "lwip/sockets.h"
#include <lwip/netdb.h>
#include "wifi.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
//#include "usb.h"
#include "esp_log.h"
#include "main.h"

uint8_t send_to_socket(char* buffer, const int sock, int len);
void wifi_sta(char *name, char *password);
void wifi_ap(char *name, char *password);
esp_err_t wifi_STA_event_handler(void *ctx, system_event_t *event);
esp_err_t wifi_AP_event_handler(void *ctx, system_event_t *event);
void monitor_task(void *pvParameter);
void station_list_task(void *pvParameter);
void tcp_server_task(void *pvParameters);
const char *TAG = "TAG!";
const int STA_CONNECTED_BIT = BIT0;
const int STA_DISCONNECTED_BIT = BIT1;

void shutdown_socket(int socknum){
	shutdown(socknum, 0);
	close(socknum);
}

// AP event handler
esp_err_t wifi_AP_event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_AP_START:
		printf("Access point started\n");
		break;
	case SYSTEM_EVENT_AP_STACONNECTED:
		xEventGroupSetBits(event_group_dude, STA_CONNECTED_BIT);
		break;
	case SYSTEM_EVENT_AP_STADISCONNECTED:
		xEventGroupSetBits(event_group_dude, STA_DISCONNECTED_BIT);
		break;
	default:
        break;
    }
	return ESP_OK;
}

esp_err_t wifi_STA_event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

// create wifi AP function
void wifi_ap(char *name, char *passwordgiven)
{
	//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
	  ESP_ERROR_CHECK(nvs_flash_erase());
	  ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_ap();
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_AP_event_handler, NULL, NULL));
	wifi_config_t wifi_config = {
		.ap = {
			.ssid = EXAMPLE_ESP_WIFI_SSID,
			.ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
			.channel = EXAMPLE_ESP_WIFI_CHANNEL,
			.password = EXAMPLE_ESP_WIFI_PASS,
			.max_connection = EXAMPLE_MAX_STA_CONN,
			.beacon_interval = 100,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK
		},
	};
	if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());
	// Publication safety: do not print network credentials.
}
// create wifi sta function
void wifi_sta(char *name, char *passwordgiven)
{
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(wifi_STA_event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
            .sta = {
                .ssid = "CONFIGURE_STA_SSID",
                .password = "CONFIGURE_STA_PASSWORD",
                .bssid_set = false
            }
        };
    // Station mode is retained for reference; app_main starts access-point mode.
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}

// socket functions
#ifdef TCPIP_SERVER
static void do_retransmit(const int sock)
{

    int len;
    do {
        len = recv(sock, wifi_rx_buffer, sizeof(wifi_rx_buffer) - 1, 0);
        printf("len is %d, data is %s",len, wifi_rx_buffer);
        if (len < 0) {
            ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
        } else if (len == 0) {
            ESP_LOGW(TAG, "Connection closed");
        } else {
        	wifi_rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            check_command((uint8_t*)wifi_rx_buffer);
        }
    } while (len > 0);
}

// send() can return less bytes than supplied length.
// Continue sending until all bytes are accepted or send reports an error.
uint8_t send_to_socket(char* buffer, const int sock, int len){
	int to_write = len;
	while (to_write > 0) {
		int written = send(sock, buffer + (len - to_write), to_write, 0);
		if (written < 0) {
			ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
			return 255;
		}
		to_write -= written;
	}
	return 0;
}

void tcp_server_task(void *pvParameters)
{
	printf("starting tcp server\n");
	char addr_str[128];
	int addr_family = (int)pvParameters;
	int ip_protocol = 0;
	int keepAlive = 1;
	int keepIdle = KEEPALIVE_IDLE;
	int keepInterval = KEEPALIVE_INTERVAL;
	int keepCount = KEEPALIVE_COUNT;
	struct sockaddr_storage dest_addr;
	if (addr_family == AF_INET) {
		struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
		dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
		dest_addr_ip4->sin_family = AF_INET;
		dest_addr_ip4->sin_port = htons(PORT);
		ip_protocol = IPPROTO_IP;
	}
	int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
	if (listen_sock < 0) {
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		vTaskDelete(NULL);
		return;
	}
	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	ESP_LOGI(TAG, "Socket created");
	int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	if (err != 0) {
		ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
		ESP_LOGE(TAG, "IPPROTO: %d", addr_family);
		goto CLEAN_UP;
	}
	ESP_LOGI(TAG, "Socket bound, port %d", PORT);
	err = listen(listen_sock, 1);
	if (err != 0) {
		ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
		goto CLEAN_UP;
	}
	while (1) {
		ESP_LOGI(TAG, "Socket listening");
		struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
		socklen_t addr_len = sizeof(source_addr);
		sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
		if (sock < 0) {
			ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
			break;
		}
		// Set tcp keepalive option
		setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
		setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
		setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
		setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
		setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, 2, sizeof(int));	//MB: SOCKET TIMEOUT
		// Historical socket error handling is retained without correction.


		// Convert ip address to string
		if (source_addr.ss_family == PF_INET) {
			inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
		}
		ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);
		do_retransmit(sock);
		//shutdown(sock, 0);
		//close(sock);
	}
CLEAN_UP:
	close(listen_sock);
	vTaskDelete(NULL);
}
#endif

// print the list of connected stations
void printStationList()
{
	printf(" Connected stations:\n");
	printf("--------------------------------------------------\n");
	wifi_sta_list_t wifi_sta_list;
	tcpip_adapter_sta_list_t adapter_sta_list;
	memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
	memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
	ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list(&wifi_sta_list));
	ESP_ERROR_CHECK(tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list));
	for(int i = 0; i < adapter_sta_list.num; i++) {
		tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
         printf("%d - mac: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x - IP: %s\n", i + 1,
				station.mac[0], station.mac[1], station.mac[2],
				station.mac[3], station.mac[4], station.mac[5],
				ip4addr_ntoa(&(station.ip)));
	}
	printf("\n");
}

// Monitor task, receive Wifi AP events
void monitor_task(void *pvParameter)
{
	while(1) {
		EventBits_t staBits = xEventGroupWaitBits(event_group_dude, STA_CONNECTED_BIT | STA_DISCONNECTED_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
		if((staBits & STA_CONNECTED_BIT) != 0)
		{
			printf("New station connected\n\n");
		}
		else printf("A station disconnected\n\n");
	}
}

// Station list task, print station list every 10 seconds
void station_list_task(void *pvParameter)
{
	while(1) {
		printStationList();
		vTaskDelay(10000 / portTICK_RATE_MS);
	}
}
