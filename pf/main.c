#include "s4819560_console.h"
#include "s4819560_rcmcont.h"
#include "s4819560_mfs_led.h"
#include "s4819560_txradio.h"
#include "s4819560_board_pb.h"
#include "board.h"
#include "processor_hal.h"
#include "s4819560_mfs_pb.h"
#include "s4819560_sysmon.h"
#include "event_groups.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "debug_log.h"

void hardware_init();
#define TASK1_PRIORITY (tskIDLE_PRIORITY + 1)
#define TASK2_PRIORITY (tskIDLE_PRIORITY + 2)
#define TASK3_PRIORITY (tskIDLE_PRIORITY + 3)

#define TASK1_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK2_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK3_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)

int main(void){
    hardware_init();
    HAL_Init();
    BRD_debuguart_init();
    s4819560_reg_mfs_led_init();
    commandEventGroup = xEventGroupCreate();
        
    // Create tasks
    xTaskCreate((void *)&s4819560_lib_console_tsk_processInput, (const signed char *)"T1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
    xTaskCreate((void *)&s4819560_rcmcont_task, (const signed char *)"T2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, NULL);
    xTaskCreate((void *)&s4819560_tsk_txradio, (const signed char *)"T3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, NULL);

    // Start the scheduler
    vTaskStartScheduler();
 
    return 0;
} 

void hardware_init(){
    taskENTER_CRITICAL();
    s4819560_txradio_init();
    taskEXIT_CRITICAL();
}
