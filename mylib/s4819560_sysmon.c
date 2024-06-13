/**
***************************************************************
* @file mylib/ s4819560_sysmon.c
* @author s48195609
* @date 13/04/2024
* @brief system monitor
* REFERENCE: templates/mylib, sourcelib/os/fr_sysmon
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
s4819560_sysmon_init() - initialise system monitor
***************************************************************
*/
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "s4819560_sysmon.h"

/**
 * @brief Initializes the system monitoring module.
 */
void s4819560_sysmon_init()
{
    // Enable GPIO clock for channel 0
    __GPIOD_CLK_ENABLE();

    // Configure GPIO for channel 0
    SYSMON_CHAN0_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));  // Clear bits
    SYSMON_CHAN0_GPIO->MODER |= (0x01 << (SYSMON_CHAN0_PIN * 2));   // Set for push-pull

    SYSMON_CHAN0_GPIO->OSPEEDR &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));
    SYSMON_CHAN0_GPIO->OSPEEDR |= 0x02 << (SYSMON_CHAN0_PIN * 2);  // Set for Fast speed

    SYSMON_CHAN0_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN0_PIN);       // Clear Bit for Push/Pull output

    SYSMON_CHAN0_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));   // Clear Bits
    SYSMON_CHAN0_GPIO->PUPDR |= (0x01 << (SYSMON_CHAN0_PIN * 2));    // Set for Pull down output

    // Enable GPIO clock for channel 1
    __GPIOG_CLK_ENABLE();

    // Configure GPIO for channel 1
    SYSMON_CHAN1_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));  // Clear bits
    SYSMON_CHAN1_GPIO->MODER |= (0x01 << (SYSMON_CHAN1_PIN * 2));   // Set for push-pull

    SYSMON_CHAN1_GPIO->OSPEEDR &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));
    SYSMON_CHAN1_GPIO->OSPEEDR |= 0x02 << (SYSMON_CHAN1_PIN * 2);  // Set for Fast speed

    SYSMON_CHAN1_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN1_PIN);       // Clear Bit for Push/Pull output

    SYSMON_CHAN1_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));   // Clear Bits
    SYSMON_CHAN1_GPIO->PUPDR |= (0x01 << (SYSMON_CHAN1_PIN * 2));    // Set for Pull down output

    // Enable GPIO clock for channel 2
    __GPIOG_CLK_ENABLE();

    // Configure GPIO for channel 2
    SYSMON_CHAN2_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));  // Clear bits
    SYSMON_CHAN2_GPIO->MODER |= (0x01 << (SYSMON_CHAN2_PIN * 2));   // Set for push-pull

    SYSMON_CHAN2_GPIO->OSPEEDR &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));
    SYSMON_CHAN2_GPIO->OSPEEDR |= 0x02 << (SYSMON_CHAN2_PIN * 2);  // Set for Fast speed

    SYSMON_CHAN2_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN2_PIN);       // Clear Bit for Push/Pull output

    SYSMON_CHAN2_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));   // Clear Bits
    SYSMON_CHAN2_GPIO->PUPDR |= (0x01 << (SYSMON_CHAN2_PIN * 2));    // Set for Pull down output
}
