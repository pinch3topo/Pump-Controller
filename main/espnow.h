#ifndef ESPNOW_H
#define ESPNOW_H

#include <stdint.h>

typedef struct
{
    uint8_t sensor_id;
    uint16_t moisture;

} SensorData;


// Initialize WiFi + ESP-NOW
void initESPNow();

#endif