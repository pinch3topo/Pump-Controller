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

static int queue[NUM_ZONES];

static int head = 0;
static int tail = 0;
static int count = 0;

void initPumps()
{
    for(int i=0;i<NUM_ZONES;i++)
    {
        gpio_reset_pin(pumpPins[i]);
        gpio_set_direction(pumpPins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(pumpPins[i],0);
    }
}

void enqueuePump(int zone)
{
    if(zone < 0 || zone >= NUM_ZONES)
        return;

    // Already watering
    if(zones[zone].pumpRunning)
        return;

    // Already waiting in queue
    for(int i=0;i<count;i++)
    {
        int index=(head+i)%NUM_ZONES;

        if(queue[index]==zone)
            return;
    }

    if(count>=NUM_ZONES)
        return;

    queue[tail]=zone;
    tail=(tail+1)%NUM_ZONES;
    count++;

    printf("Queued Pump %d\n",zone+1);
}

void pumpTask(void *parameter)
{
    while(true)
    {
        if(count==0)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        int zone=queue[head];

        head=(head+1)%NUM_ZONES;
        count--;

        zones[zone].pumpRunning=true;

        printf("\n=============================\n");
        printf("Running Pump %d\n",zone+1);
        printf("=============================\n");

        gpio_set_level(pumpPins[zone],1);

        vTaskDelay(pdMS_TO_TICKS(PUMP_RUNTIME_MS));

        gpio_set_level(pumpPins[zone],0);

        zones[zone].pumpRunning=false;

        printf("Pump %d Finished\n",zone+1);
    }
}
