#include "s4819560_board_pb.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "debug_log.h"


// Define global array variable to hold current MFS pressed count
int MfbdPressCounter[3] = {0};
static uint32_t previous_time = 0;
SemaphoreHandle_t bd_semaphore;


// EXTI0_IRQHandler function prototype
void EXTI15_10_IRQHandler(void);

// pb_callback function prototype
void pb_callback(uint16_t GPIO_Pin);

void s4819560_reg_board_pb_init(int pb_select){
    taskENTER_CRITICAL();
     // Initialise onboard pushbutton (PC13)
     __GPIOC_CLK_ENABLE();

    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (13 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (13 * 2));			//Clear bits for input mode

    // Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 13)
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_PC;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->RTSR |= EXTI_RTSR_TR13;	//enable rising dedge
	EXTI->FTSR &= ~EXTI_FTSR_TR13;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM13;		//Enable external interrupt

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    taskEXIT_CRITICAL();
}
void s4819560_reg_board_pb_isr(void){
    // Debounce the pushbutton press
    uint32_t current_time = HAL_GetTick();
    if ((current_time - previous_time) > 10) {
        EXTI15_10_IRQHandler();
    }
    previous_time = current_time;

}

void EXTI15_10_IRQHandler(void){

    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) { // Check if switch signal is high
            EXTI->PR |= EXTI_PR_PR13;  //Clear interrupt flag.
            MfbdPressCounter[0]++; // Increment press counter
            pb_callback(13);
        }

}

void pb_callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken;

	if (GPIO_Pin == 13) {
		
		// Is it time for another Task() to run?
		xHigherPriorityTaskWoken = pdFALSE;

		if (bd_semaphore != NULL) {	// Check if semaphore exists 
			xSemaphoreGiveFromISR( bd_semaphore, &xHigherPriorityTaskWoken );		// Give PB Semaphore from ISR
		}

		// Perform context switching, if required.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	}	
}


int s4819560_reg_board_pb_press_get(int pb_select){
    return MfbdPressCounter[0];
}
void s4819560_reg_board_pb_press_reset(int pb_select){
    MfbdPressCounter[0] = 0;
}
void s4819560_reg_board_pb_semaphore_init(void){
    bd_semaphore = xSemaphoreCreateBinary();
}