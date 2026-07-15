#include "pumps.h"
#include "zones.h"

extern "C"
{
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
}

#include <stdio.h>

static gpio_num_t pumpPins[NUM_ZONES] =
{
    GPIO_NUM_18,
    GPIO_NUM_19,
    GPIO_NUM_21,
    GPIO_NUM_22,
    GPIO_NUM_23,
    GPIO_NUM_25
};

static constexpr uint32_t PUMP_RUNTIME_MS = 10000;


void initPumps()
{
    for(int i=0;i<NUM_ZONES;i++)
    {
        gpio_reset_pin(pumpPins[i]);

        gpio_set_direction(
            pumpPins[i],
            GPIO_MODE_OUTPUT
        );

        gpio_set_level(
            pumpPins[i],
            0
        );
    }
}



void runPump(int zone)
{

    if(zone < 0 || zone >= NUM_ZONES)
        return;

    printf("\n");
    printf("================================\n");
    printf("Running Pump %d\n", zone + 1);
    printf("================================\n");

    zones[zone].pumpRunning = true;

    gpio_set_level(
        pumpPins[zone],
        1
    );

    vTaskDelay(
        pdMS_TO_TICKS(PUMP_RUNTIME_MS)
    );

    gpio_set_level(
        pumpPins[zone],
        0
    );

    zones[zone].pumpRunning = false;

}