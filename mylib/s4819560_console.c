/**
***************************************************************
* @file mylib/ s4819560_console.c
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

#include "board.h"
#include "processor_hal.h"
#include "s4819560_console.h"
#include "s4819560_mfs_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "debug_log.h"

#define CONSOLE_WIDTH 30 // Width of the console display

QueueHandle_t consoleQueue; // External reference to the queue
EventGroupHandle_t commandEventGroup;
XyzPayload_t xyzPayload;
RotPayload_t rotPayload;
ZoomPayload_t zoomPayload;
static int cursor_position = 0;

char recvchar;


/**
 * @brief Convert ASCII character to hexadecimal value.
 * 
 * @param value2 ASCII character to be converted.
 * @return int Hexadecimal value.
 */
int s4819560_lib_console_ascii2hex(char value2)
{
    int hex_value = 0;
    if (value2 >= '0' && value2 <= '9')
    {
        hex_value += value2 - '0';
    }
    else if (value2 >= 'A' && value2 <= 'F')
    {
        hex_value += (value2 - 'A' + 10);
    }
    else if (value2 >= 'a' && value2 <= 'f')
    {
        hex_value += (value2 - 'a' + 10);
    }
    else
    {
        return 0;
    }
    return hex_value;
}

/**
 * @brief Task to display console input.
 */
void s4819560_lib_console_tsk_display(void)
{
    char display_str[2];

    for (;;)
    {
        if (xQueueReceive(consoleQueue, &display_str, 10))
        {
            debug_putc(display_str);
            cursor_position += 2;

            if (cursor_position >= CONSOLE_WIDTH)
            {
                cursor_position = cursor_position % CONSOLE_WIDTH;
                debug_putc("\r");
            }
        }
    }
}
void processInput(char recvchar){
         if (recvchar == 'q') {
            xyzPayload.x += 2;
            xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'w') {
             xyzPayload.x -= 2;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'e') {
             xyzPayload.y += 2;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'r') {
             xyzPayload.y -= 2;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 't') {
             xyzPayload.z += 2;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'y') {
             xyzPayload.z -= 2;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'a') {
             xyzPayload.x += 10;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 's') {
             xyzPayload.x -= 10;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'd') {
             xyzPayload.y += 10;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'f') {
             xyzPayload.y -= 10;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'g') {
             xyzPayload.z += 10;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'h') {
             xyzPayload.z -= 10;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'z') {
             xyzPayload.x += 50;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'x') {
             xyzPayload.x -= 50;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'c') {
             xyzPayload.y += 50;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'v') {
             xyzPayload.y -= 50;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'b') {
             xyzPayload.z += 50;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == 'n') {
             xyzPayload.z -= 50;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }
         else if (recvchar == '1') {
            zoomPayload.zoomFactor += 1;
            xEventGroupSetBits(commandEventGroup, ZOOM_EVENT);
         }
         else if (recvchar == '2') {
            zoomPayload.zoomFactor -= 1;
            xEventGroupSetBits(commandEventGroup, ZOOM_EVENT);
         }
         else if (recvchar == '3') {
            rotPayload.angle += 10;
            xEventGroupSetBits(commandEventGroup, ROTATE_EVENT);
         }
         else if (recvchar == '4') {
            rotPayload.angle -= 10;
            xEventGroupSetBits(commandEventGroup, ROTATE_EVENT);
         }
         else if (recvchar == '5') {
             xyzPayload.x = 0;
             xyzPayload.y = 0;
             xEventGroupSetBits(commandEventGroup, MOVE_EVENT);
         }

}
void s4819560_lib_console_tsk_processInput(void) {
   
    for (;;) {
        commandEventGroup = xEventGroupCreate();
        recvchar = BRD_debuguart_getc(); 
        if (recvchar !=0)
        {processInput(recvchar);}
    }
}


