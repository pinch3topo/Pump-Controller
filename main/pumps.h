#ifndef PUMPS_H
#define PUMPS_H

void initPumps();

void enqueuePump(int zone);

void pumpTask(void *parameter);

#endif
