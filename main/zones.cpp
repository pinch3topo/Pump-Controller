#include "zones.h"

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

Zone zones[NUM_ZONES];



void initZones()
{
    for(int i = 0; i < NUM_ZONES; i++)
    {
        zones[i].sensorID = i + 1;

        zones[i].moisture = 0;

        zones[i].online = false;

        zones[i].pumpRunning = false;

        zones[i].lastUpdate = 0;
    }
}



void updateZone(uint8_t sensorID, uint16_t moisture)
{
    if(sensorID < 1 || sensorID > NUM_ZONES)
        return;

    int index = sensorID - 1;

    zones[index].moisture = moisture;

    zones[index].online = true;

    zones[index].lastUpdate =
    xTaskGetTickCount() * portTICK_PERIOD_MS;
}



void printZones()
{

    printf("\n");

    printf("=============================================\n");

    printf(" Zone | Moisture | Pump | Status\n");

    printf("---------------------------------------------\n");

    for(int i = 0; i < NUM_ZONES; i++)
    {

        printf("  %d    |",
               zones[i].sensorID);


        if(zones[i].online)
        {
            printf("   %3d%%   |",
                   zones[i].moisture);
        }
        else
        {
            printf("   ---   |");
        }


        printf("  %s  |",
               zones[i].pumpRunning ? "ON " : "OFF");


        printf(" %s\n",
               zones[i].online ? "ONLINE" : "OFFLINE");

    }

    printf("=============================================\n");

}