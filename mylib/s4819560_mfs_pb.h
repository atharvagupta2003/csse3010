/**   
******************************************************************************   
* @file mylib/ s4819560_mfs_pb.h
* @author s48195609
* @date 28/02/2024
* @brief MFS PUSH BUTTON
* REFERENCE: examples/getting-started/blink, examples/gpio/interrupt, templates/mylib
*
******************************************************************************   
*     EXTERNAL FUNCTIONS
******************************************************************************
* s4819560_reg_mfs_pb_init(int pb_select) - intialise MFS push button
* s4819560_reg_mfs_pb_isr(int pb_select) - MFS interrupt service routine
* s4819560_reg_mfs_pb_press_get(int pb_select) - Returns the value of pushbutton press counter
* s4819560_reg_mfs_pb_press_reset(int pb_select) - Reset the value of pushbutton event counter to 0
* s4819560_reg_mfs_pb_semaphore_init(void) - initiates semaphore for pushbutton
******************************************************************************   
*/
#include "FreeRTOS.h"
#include "semphr.h"
#ifndef S4819560_MFS_PB_H
#define S4819560_MFS_PB_H
#define S4819560_REG_MFA_PB_S1 (1 << 0)

extern SemaphoreHandle_t pb_semaphore;

void s4819560_reg_mfs_pb_init(int pb_select);
void s4819560_reg_mfs_pb_isr(int pb_select);
int s4819560_reg_mfs_pb_press_get(int pb_select);
void s4819560_reg_mfs_pb_press_reset(int pb_select);
void s4819560_reg_mfs_pb_semaphore_init(void);
#endif
