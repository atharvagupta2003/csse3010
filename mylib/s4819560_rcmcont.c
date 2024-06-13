#include "s4819560_rcmcont.h"
#include "board.h"
#include "processor_hal.h"
#include "nrf24l01plus.h"
#include "s4819560_mfs_led.h"
#include "s4819560_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "debug_log.h"


#define MOVE_EVENT    (1 << 0)
#define ROTATE_EVENT  (1 << 1)
#define ZOOM_EVENT    (1 << 2)
#define EVENTS    MOVE_EVENT|ROTATE_EVENT|ZOOM_EVENT

CommandType_t command;
QueueHandle_t commandQueue;
QueueHandle_t xyzpayloadQueue;
QueueHandle_t rotpayloadQueue;
QueueHandle_t zoompayloadQueue;

void s4819560_fsm_rcmcont(CommandType_t current_state){
    
    switch (current_state)
    {
    case IDLE:
        debug_log("Idle State!\n\r");
        command = IDLE;
        break;
    case MOVE:
    xQueueSend(xyzpayloadQueue, ( void * ) &xyzPayload, 5);
        command = MOVE;
        debug_log("Move State!\n\r");
        debug_log("%d %d %d\n\r",xyzPayload.x,xyzPayload.y,xyzPayload.z);
        break;
    case ROTATE:
       xQueueSend(rotpayloadQueue, ( void * ) &rotPayload, 0);
       command = ROTATE;
        debug_log("Rotate State!\n\r");
        debug_log("%d\n\r",rotPayload.angle);
        break;
    case ZOOM:
        xQueueSend(zoompayloadQueue, ( void * ) &zoomPayload, 0);
        command = ZOOM;
        debug_log("Zoom State!\n\r");
        debug_log("%d\n\r",zoomPayload.zoomFactor);
        break;    
    }
    
    
}

void s4819560_rcmcont_task() {

    BRD_debuguart_init();
    s4819560_reg_mfs_led_init();

    for (;;) {
       EventBits_t bits = xEventGroupWaitBits(commandEventGroup, EVENTS, pdTRUE, pdFALSE, 10);
       xQueueSend(commandQueue, ( void * )  &command, 0);
       if (bits&MOVE_EVENT)
       {
        s4819560_fsm_rcmcont(MOVE);
        // s4819560_REG_MFS_LED_D2_ON();
        // s4819560_REG_MFS_LED_D1_OFF();
        // s4819560_REG_MFS_LED_D3_OFF();
       }else if (bits&ROTATE_EVENT)
       {
        
        // s4819560_REG_MFS_LED_D1_ON();
        // s4819560_REG_MFS_LED_D3_OFF();
        // s4819560_REG_MFS_LED_D2_OFF();
        s4819560_fsm_rcmcont(ROTATE);
       }else if (bits&ZOOM_EVENT)
       {
        
        // s4819560_REG_MFS_LED_D3_ON();
        // s4819560_REG_MFS_LED_D2_OFF();
        // s4819560_REG_MFS_LED_D1_OFF();
        s4819560_fsm_rcmcont(ZOOM);
       }
        
    }
}

