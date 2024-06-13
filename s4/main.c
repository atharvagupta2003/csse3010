/**
 * ******************************************************************************
 * @file    s4/main.c
 * @author  s48195609
 * @date    13/04/2024
 * @brief   Pass semaphore to task 3 through isr to control task 2
 * REFERENCE: sourcelib/examples/os
******************************************************************************
*/

#include "board.h"
#include "processor_hal.h"
#include "s4819560_mfs_pb.h"
#include "s4819560_sysmon.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4819560_console.h"

#define DEBUG
#ifdef DEBUG
#include "debug_log.h"
#endif

#define QUEUE_LENGTH 15
#define ITEM_SIZE sizeof(char)

QueueHandle_t consoleQueue;

void hardware_init();
void Task1_Task(void);
void Task2_Task(void);
void Task3_Task(void); 

// Task Priorities (Idle Priority is the lowest priority)
#define TASK1_PRIORITY (tskIDLE_PRIORITY + 2)
#define TASK2_PRIORITY (tskIDLE_PRIORITY + 2)
#define TASK3_PRIORITY (tskIDLE_PRIORITY + 1)

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define TASK1_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define TASK2_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define TASK3_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

TaskHandle_t task2_handle = NULL;
TaskHandle_t task1_handle = NULL;
TaskHandle_t task3_handle = NULL;

/*
 * Starts all the other tasks, then starts the scheduler. Must not have a Cyclic Executive.
 */
int main(void)
{
    HAL_Init(); // Only HAL_Init() must be called before task creation.
    hardware_init();
    #ifdef DEBUG
    BRD_debuguart_init();
    #endif
    consoleQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);

    // Start the task to display console input on LED.
     xTaskCreate((void *)&s4819560_lib_console_tsk_display, (const signed char *)"ConsoleDisplay", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, NULL);
       xTaskCreate((void *)&Task3_Task, (const signed char *)"T3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, &task3_handle);
      xTaskCreate((void *)&Task1_Task, (const signed char *)"T1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, &task1_handle);

      xTaskCreate((void *)&Task2_Task, (const signed char *)"T2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, &task2_handle);

    // Start the scheduler.gf

    vTaskStartScheduler();

    return 0;
}

/*
 * Task 1.
 */
void Task1_Task(void)
{
    hardware_init();

    S4819560_REG_SYSMON_CHAN0_CLR(); // Clear LA Channel 0

    for (;;)
    {

        S4819560_REG_SYSMON_CHAN0_SET(); // Set LA Channel 0
        vTaskDelay(3);                    // Extra Delay for 3ms
        S4819560_REG_SYSMON_CHAN0_CLR(); // Clear LA Channel 0
    }
}

/*
 * Task 2.
 */
void Task2_Task(void)
{
    hardware_init();

    S4819560_REG_SYSMON_CHAN1_CLR(); // Clear LA Channel 1

    for (;;)
    {

        S4819560_REG_SYSMON_CHAN1_SET(); // Set LA Channel 1
        char display_str[2] = ">>";
        xQueueSendToFront(consoleQueue, &display_str, 10);
        vTaskDelay(3); // Extra Delay for 3ms
        S4819560_REG_SYSMON_CHAN1_CLR(); // Clear LA Channel 1
    }
    char display_str_move[] = "  ";
    xQueueSend(consoleQueue, &display_str_move, portMAX_DELAY);
}

/*
 * Task3.
 */
void Task3_Task(void)
{
    hardware_init();

    S4819560_REG_SYSMON_CHAN2_CLR(); // Clear LA Channel 2

    for (;;)
    {
        S4819560_REG_SYSMON_CHAN2_SET(); // Set LA Channel 2

        if (xSemaphoreTake(pb_semaphore, 10) == pdTRUE)
        {
            if (s4819560_reg_mfs_pb_press_get(1) > 0)
            {
                if (task2_handle == NULL)
                {
                    // Create Task2 if it doesn't exist
                    xTaskCreate((void *)&Task2_Task, (const signed char *)"T2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, &task2_handle);
                }
                else
                {
                    // Delete Task2 if it exists
                    vTaskDelete(task2_handle);
                    task2_handle = NULL;
                }
                s4819560_reg_mfs_pb_press_reset(1);
            }
            xSemaphoreGive(pb_semaphore);
        }
        vTaskDelay(3);
        S4819560_REG_SYSMON_CHAN2_CLR(); // Clear LA Channel 2
    }
}

void hardware_init()
{
    taskENTER_CRITICAL();
    BRD_debuguart_init();
    s4819560_sysmon_init();
    s4819560_reg_mfs_pb_init(1);
    s4819560_reg_mfs_pb_semaphore_init();
    taskEXIT_CRITICAL();
}
