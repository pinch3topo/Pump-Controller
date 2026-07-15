#include "espnow.h"
#include "zones.h"

#include <string.h>
#include <stdio.h>

extern "C"
{
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"
}

static const char *TAG = "ESPNOW";


//--------------------------------------------------
// Called whenever an ESP-NOW packet is received
//--------------------------------------------------
static void onDataReceive(
    const esp_now_recv_info_t *info,
    const uint8_t *data,
    int len
)
{
    // Make sure we received the expected packet size
    if (len != sizeof(SensorData))
    {
        printf("\nERROR: Invalid packet size (%d bytes)\n", len);
        return;
    }

    SensorData packet;

    memcpy(
        &packet,
        data,
        sizeof(packet)
    );

    printf("\n=========================================\n");
    printf("ESP-NOW Packet Received\n");

    printf(
        "MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
        info->src_addr[0],
        info->src_addr[1],
        info->src_addr[2],
        info->src_addr[3],
        info->src_addr[4],
        info->src_addr[5]
    );

    printf("Sensor ID : %d\n", packet.sensor_id);
    printf("Moisture  : %d%%\n", packet.moisture);
    printf("=========================================\n");

    // Update the zone database
    updateZone(
        packet.sensor_id,
        packet.moisture
    );
}


//--------------------------------------------------
// Initialize ESP-NOW
//--------------------------------------------------
void initESPNow()
{
    // Initialize NVS
    ESP_ERROR_CHECK(
        nvs_flash_init()
    );

    // Initialize network stack
    ESP_ERROR_CHECK(
        esp_netif_init()
    );

    // Create default event loop
    ESP_ERROR_CHECK(
        esp_event_loop_create_default()
    );

    // Configure WiFi
    wifi_init_config_t cfg =
        WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(
        esp_wifi_init(&cfg)
    );

    ESP_ERROR_CHECK(
        esp_wifi_set_mode(WIFI_MODE_STA)
    );

    ESP_ERROR_CHECK(
        esp_wifi_start()
    );

    // Initialize ESP-NOW
    ESP_ERROR_CHECK(
        esp_now_init()
    );

    // Register callback
    ESP_ERROR_CHECK(
        esp_now_register_recv_cb(
            onDataReceive
        )
    );

    ESP_LOGI(
        TAG,
        "ESP-NOW Receiver Ready"
    );
}