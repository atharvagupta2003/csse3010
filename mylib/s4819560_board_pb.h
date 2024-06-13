#include "FreeRTOS.h"
#include "semphr.h"
#ifndef S4819560_BOARD_PB_H
#define S4819560_BOARD_PB_H

extern SemaphoreHandle_t bd_semaphore;

void s4819560_reg_board_pb_init(int pb_select);
void s4819560_reg_board_pb_isr(void);
int s4819560_reg_board_pb_press_get(int pb_select);
void s4819560_reg_board_pb_press_reset(int pb_select);
void s4819560_reg_board_pb_semaphore_init(void);
#endif