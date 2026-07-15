#include "zones.h"
#include "espnow.h"
#include "controller.h"
#include "pumps.h"

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
}

static constexpr uint32_t DASHBOARD_PERIOD_MS = 30000;

extern "C"
void app_main()
{
    printf("Initializing Zones...\n");
    initZones();

    printf("Initializing Pumps...\n");
    initPumps();

    printf("Initializing ESP-NOW...\n");
    initESPNow();

    printf("Creating Controller Task...\n");

    BaseType_t result =
        xTaskCreate(
            controllerTask,
            "Controller",
            4096,
            NULL,
            5,
            NULL
        );

    if(result != pdPASS)
    {
        printf("ERROR: Failed to create Controller Task\n");
    }

    printf("Creating Pump Task...\n");

    result =
        xTaskCreate(
            pumpTask,
            "PumpTask",
            4096,
            NULL,
            5,
            NULL
        );

    if(result != pdPASS)
    {
        printf("ERROR: Failed to create Pump Task\n");
    }

    while(true)
    {
        printZones();

        vTaskDelay(
            pdMS_TO_TICKS(DASHBOARD_PERIOD_MS)
        );
    }
}
