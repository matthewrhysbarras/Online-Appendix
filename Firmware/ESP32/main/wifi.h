/* primary_header.h */
#ifndef WIFI_H
#define WIFI_H

#include "esp_wifi.h"

uint8_t send_to_socket(char* buffer, const int sock, int len);
void tcp_server_task(void *pvParameters);
void wifi_sta(char *name, char *password);
void wifi_ap(char *name, char *password);
esp_err_t wifi_STA_event_handler(void *ctx, system_event_t *event);
esp_err_t wifi_AP_event_handler(void *ctx, system_event_t *event);
void monitor_task(void *pvParameter);
void station_list_task(void *pvParameter);

// Replace both publication placeholders before use; keep local credentials private.
#define EXAMPLE_ESP_WIFI_SSID      "CONFIGURE_AP_SSID"
#define EXAMPLE_ESP_WIFI_PASS      "CONFIGURE_AP_PASSWORD"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       2

#define CONFIG_EXAMPLE_IPV4
#define PORT 80
int sock;
char wifi_rx_buffer[128]; // buffer for TCP socket
#define TCPIP_SERVER
// Historical TCP keepalive settings.
#define KEEPALIVE_IDLE              5
#define KEEPALIVE_INTERVAL          5
#define KEEPALIVE_COUNT             3

#endif /* WIFI_H */
