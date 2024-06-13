/**
***************************************************************
* @file mylib/ s4819560_console.h
* @author s48195609
* @date 13/04/2024
* @brief system monitor
* REFERENCE: templates/mylib
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
s4819560_lib_console_ascii2hex(char value2) - converts ascii to hex
s4819560_lib_console_tsk_display(void) - display task for console
***************************************************************
*/
#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#ifndef s4819560_CONSOLE_H
#define s4819560_CONSOLE_H

    
typedef struct {
    int x;
    int y;
    int z;
} XyzPayload_t;

typedef struct {
    int angle;
} RotPayload_t;

typedef struct {
    int zoomFactor;
} ZoomPayload_t;

typedef enum {
    IDLE,
    MOVE,
    ROTATE,
    ZOOM
} CommandType_t;

extern QueueHandle_t consoleQueue; // External reference to the queue
extern EventGroupHandle_t commandEventGroup;

extern XyzPayload_t xyzPayload;
extern RotPayload_t rotPayload;
extern ZoomPayload_t zoomPayload;
extern CommandType_t command;

#define MOVE_EVENT    (1 << 0)
#define ROTATE_EVENT  (1 << 1)
#define ZOOM_EVENT    (1 << 2)

int s4819560_lib_console_ascii2hex(char value2);
void s4819560_lib_console_tsk_display(void);
void s4819560_lib_console_tsk_processInput(void);
#endif