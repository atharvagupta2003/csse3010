#include "board.h"
#include "processor_hal.h"
#include "nrf24l01plus.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

#define MYRADIOCHAN 51
uint8_t myradiotxaddr[5] = {0x30,0x10,0x00,0x00,0x51};

// #define MYRADIOCHAN 52
// uint8_t myradiotxaddr[5] = {0x30,0x10,0x00,0x00,0x52};
// #define MYRADIOCHAN 56
// uint8_t myradiotxaddr[5] = {0x30,0x10,0x00,0x00,0x56};
// #define MYRADIOCHAN 58
// uint8_t myradiotxaddr[5] = {0x30,0x10,0x00,0x00,0x58};