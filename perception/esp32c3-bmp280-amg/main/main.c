/* HTTP GET Example using plain POSIX sockets

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include <bmp280.h>
#include "../config.h"

// HTTP constants that aren't configurable in menuconfig
#define WEB_PATH "/measurement"
#define WEB_PATH_DEV "/device"

static const char *TAG = "temp_collector";

// POST message for device measurements
static char *BODY = "id=%s&t=%0.2f&h=%0.2f&p=%0.2f";
static char *REQUEST_POST = "POST "WEB_PATH" HTTP/1.0\r\n"
    "Host: "API_IP_PORT"\r\n"
    "User-Agent: "USER_AGENT"\r\n"
    "Content-Type: application/x-www-form-urlencoded\r\n"
    "Content-Length: %d\r\n"
    "\r\n"
    "%s";

// POST message for the device registration data
static char *BODY_DEV = "id=%s&n=%s&k=%s";
static char *REQUEST_POST_DEV = "POST "WEB_PATH_DEV" HTTP/1.0\r\n"
    "Host: "API_IP_PORT"\r\n"
    "User-Agent: "USER_AGENT"\r\n"
    "Content-Type: application/x-www-form-urlencoded\r\n"
    "Content-Length: %d\r\n"
    "\r\n"
    "%s";

// Main task that takes readings from BME280 sensor and pushes them to the server
static void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET, 
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;

    // Measurements
    char body[64];
    char recv_buf[64];
    char send_buf[256];

    // Device
    char body_dev[64];
    char recv_buf_dev[64];
    char send_buf_dev[256];
    bool send_key = true;

    // Initialize the BME280 sensor
    bmp280_params_t params;
    bmp280_init_default_params(&params);
    bmp280_t dev;
    memset(&dev, 0, sizeof(bmp280_t));

    ESP_ERROR_CHECK(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_0, 0, SDA_GPIO, SCL_GPIO));
    ESP_ERROR_CHECK_WITHOUT_ABORT(bmp280_init(&dev, &params));

    bool bme280p = dev.id == BME280_CHIP_ID;
    ESP_LOGI(TAG, "BMP280: found %s\n", bme280p ? "BME280" : "BMP280");

    float pressure, temperature, humidity;

    // MAC address as device key
    uint8_t mac[6];
    esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);
    char mac_str[12];
    sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    ESP_LOGI(TAG,"MAC address: %s\n", mac_str);

    // Execute in loop
    while(1) {
        if (bmp280_read_float(&dev, &temperature, &pressure, &humidity) != ESP_OK) {
            ESP_LOGI(TAG, "Temperature/pressure reading failed\n");

        } else {
            ESP_LOGI(TAG, "Temperature: %.2f C", temperature);
            ESP_LOGI(TAG, "Pressure: %.2f Pa", pressure);
            if (bme280p) {
                ESP_LOGI(TAG,"Humidity: %.2f\n", humidity);
		        sprintf(body, BODY, DEVICE_ID, temperature , humidity, pressure);
                sprintf(send_buf, REQUEST_POST, (int)strlen(body),body );
	        } else {
                sprintf(send_buf, REQUEST_POST, DEVICE_ID, temperature, 0, pressure);
            }
	        ESP_LOGI(TAG,"sending: \n%s\n",send_buf);
        }    

        int err = getaddrinfo(API_IP, API_PORT, &hints, &res);

        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        /* Code to print the resolved IP.
           Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);
        
        // Send device registration data
        if (send_key) { 
            sprintf(body_dev, BODY_DEV, DEVICE_ID, USER_AGENT, mac_str);
            sprintf(send_buf_dev, REQUEST_POST_DEV, (int)strlen(body_dev),body_dev );

	        ESP_LOGI(TAG,"sending: \n%s\n",send_buf_dev);
            
            if (write(s, send_buf_dev, strlen(send_buf_dev)) < 0) {
                ESP_LOGE(TAG, "... socket send failed");
                close(s);
                vTaskDelay(4000 / portTICK_PERIOD_MS);
                continue;
            }
            ESP_LOGI(TAG, "... socket send success");

            struct timeval receiving_timeout_dev;
            receiving_timeout_dev.tv_sec = 5;
            receiving_timeout_dev.tv_usec = 0;
            if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout_dev,
                    sizeof(receiving_timeout_dev)) < 0) {
                ESP_LOGE(TAG, "... failed to set socket receiving timeout");
                close(s);
                vTaskDelay(4000 / portTICK_PERIOD_MS);
                continue;
            }
            ESP_LOGI(TAG, "... set socket receiving timeout success");

            // Read HTTP response
            do {
                bzero(recv_buf_dev, sizeof(recv_buf_dev));
                r = read(s, recv_buf_dev, sizeof(recv_buf_dev)-1);
                for(int i = 0; i < r; i++) {
                    putchar(recv_buf_dev[i]);
                }
            } while(r > 0);
            
            // Disable device registration
            send_key = false;

            ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        // Send measurements
        if (write(s, send_buf, strlen(send_buf)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        // Read HTTP response
        do {
            bzero(recv_buf, sizeof(recv_buf));
            r = read(s, recv_buf, sizeof(recv_buf)-1);
            for(int i = 0; i < r; i++) {
                putchar(recv_buf[i]);
            }
        } while(r > 0);

        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
        close(s);

        for(int countdown = 10; countdown >= 0; countdown--) {
            ESP_LOGI(TAG, "%d... ", countdown);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Starting again!");
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(i2cdev_init());

    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(&http_get_task, "http_get_task", 4096, NULL, 5, NULL);
}
