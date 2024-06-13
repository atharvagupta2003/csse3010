#ifndef s4819560_RCMCONT_H
#define s4819560_RCMCONT_H

#define idle_state 0
#define rotate_state 1
#define zoom_state 2
#define move_state 3

#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"

void s4819560_rcmcont_task();

extern QueueHandle_t commandQueue;
extern QueueHandle_t xyzpayloadQueue;
extern QueueHandle_t rotpayloadQueue;
extern QueueHandle_t zoompayloadQueue;
#endif