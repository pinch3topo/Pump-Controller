#ifndef ZONES_H
#define ZONES_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_ZONES 6

typedef struct
{
    uint8_t sensorID;

    uint16_t moisture;

    bool online;

    bool pumpRunning;

    uint32_t lastUpdate;

} Zone;


// Array containing all zones
extern Zone zones[NUM_ZONES];


// Initialize all zones
void initZones();


// Update a zone when new data arrives
void updateZone(uint8_t sensorID, uint16_t moisture);


// Print dashboard
void printZones();

#endif