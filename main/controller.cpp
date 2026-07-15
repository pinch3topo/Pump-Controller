#include "controller.h"
#include "zones.h"
#include "pumps.h"

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
}

#include <stdio.h>

static constexpr uint16_t MOISTURE_LIMIT = 30;

void controllerTask(void *parameter)
{
    while (true)
    {
        for (int i = 0; i < NUM_ZONES; i++)
        {
            // Ignore offline zones
            if (!zones[i].online)
                continue;

            // Pump already running
            if (zones[i].pumpRunning)
                continue;

            // Check moisture
            if (zones[i].moisture < MOISTURE_LIMIT)
            {
                printf(
                    "Zone %d is dry (%d%%). Starting pump.\n",
                    zones[i].sensorID,
                    zones[i].moisture
                );

                enqueuePump(i);
            }
        }

        // Check every 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
