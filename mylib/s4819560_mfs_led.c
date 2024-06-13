/**
***************************************************************
* @file mylib/ s4819560_mfs_led.c
* @author s48195609
* @date 06/04/24
* @brief mfs led
* REFERENCE: templates/mylib
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* void s4819560_reg_mfs_led_init(void)
* void s4819560_REG_MFS_LED_D1_ON(void)
* void s4819560_REG_MFS_LED_D1_OFF(void)
* void s4819560_REG_MFS_LED_D1_ON(void)
* void s4819560_REG_MFS_LED_D1_OFF(void)
* void s4819560_REG_MFS_LED_D1_ON(void)
* void s4819560_REG_MFS_LED_D1_OFF(void)
* void s4819560_REG_MFS_LED_D1_ON(void)
* void s4819560_REG_MFS_LED_D1_OFF(void)
***************************************************************
*/

#include "s4819560_mfs_led.h"
#include "board.h" 
#include "processor_hal.h"
#include "debug_log.h"

void s4819560_reg_mfs_led_init(void) {
    
    __GPIOA_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    GPIOA->MODER &= ~((0x03 << (5 * 2)) | (0x03 << (6 * 2)) | (0x03 << (7 * 2)) );  
    GPIOA->MODER |= ((0x01 << (5 * 2)) | (0x01 << (6 * 2)) | (0x01 << (7 * 2)) );  
    GPIOA->OTYPER &= ~((0x01 << 5) | (0x01 << 6) | (0x01 << 7) ); 
    GPIOD->MODER &= ~(0x03 << (14 * 2));  
    GPIOD->MODER |= ((0x01 << (14 * 2)) );  
    GPIOD->OTYPER &= ~((0x01 << 14));

    s4819560_REG_MFS_LED_D1_OFF();
    s4819560_REG_MFS_LED_D2_OFF();
    s4819560_REG_MFS_LED_D3_OFF();
    s4819560_REG_MFS_LED_D4_OFF();
  
}
void s4819560_REG_MFS_LED_D1_ON(void) {GPIOA->ODR &= ~(0x01 << 5);}

void s4819560_REG_MFS_LED_D1_OFF(void) {GPIOA->ODR |= (0x01 << 5);}

void s4819560_REG_MFS_LED_D2_ON(void) {GPIOA->ODR &= ~(0x01 << 6);}

void s4819560_REG_MFS_LED_D2_OFF(void) {GPIOA->ODR |= (0x01 << 6);}

void s4819560_REG_MFS_LED_D3_ON(void) {GPIOA->ODR &= ~(0x01 << 7);}

void s4819560_REG_MFS_LED_D3_OFF(void) {GPIOA->ODR |= (0x01 << 7);}

void s4819560_REG_MFS_LED_D4_ON(void) {GPIOD->ODR &= ~(0x01 << 14);}

void s4819560_REG_MFS_LED_D4_OFF(void) {GPIOD->ODR |= (0x01 << 14);}

void s4819560_REG_MFS_LED_D1_TOGGLE(void){
    s4819560_REG_MFS_LED_D1_ON();
    s4819560_REG_MFS_LED_D1_OFF();
    s4819560_REG_MFS_LED_D1_ON();
    s4819560_REG_MFS_LED_D1_OFF();
    s4819560_REG_MFS_LED_D1_ON();
    s4819560_REG_MFS_LED_D1_OFF();   
    s4819560_REG_MFS_LED_D1_ON();
    s4819560_REG_MFS_LED_D1_OFF();
}