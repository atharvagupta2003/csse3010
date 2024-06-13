/**
******************************************************************************
* @file    s1/main.c
* @author  s48195609
* @date    28/02/2024
* @brief   
* REFERENCE: examples/getting-started/blink, examples/gpio/interrupt, examples/gpio/gpio
******************************************************************************
*/

#include "board.h"
#include "processor_hal.h" 
#include "s4819560_lta1000g.h"
#include "s4819560_mfs_pb.h"

int a = 0, b = 1, c; 
int counter_value = 0; // Initialize counter value
volatile  uint32_t  bits_inverted = 0; // Flag to track if upper five bits are inverted

void hardware_init(void){

__GPIOD_CLK_ENABLE();
// Configure pin PD10 on GPIO port D
// Clear bits in MODER register to configure PD10 as digital output
GPIOD->MODER &= ~(0x03 << (10 * 2));
// Set PD10 for push-pull output
GPIOD->MODER |= (0x01 << (10 * 2));
// Set PD10 for fast speed
GPIOD->OSPEEDR &= ~(0x03 << (10 * 2));
GPIOD->OSPEEDR |= (0x02 << (10 * 2));
// Clear bit for push-pull output
GPIOD->OTYPER &= ~(0x01 << 10);

s4819560_reg_lta1000g_init();
s4819560_reg_mfs_pb_init(1);
}

// Function to generate the next Fibonacci number
int Fibonacci() {

    c = a + b;
    a = b;
    b = c;
    if (c>610)
    {a=0, b=1;}
    return c;
}

int main() {

    // Initialize LED bar
    hardware_init();
    
    while (1) {
    GPIOD->ODR ^= (1UL << 10); // Heart Beat Signal

    if (s4819560_reg_mfs_pb_press_get(1) > 0) {

    // Invert upper five bits of the counter value
    bits_inverted = !bits_inverted;

    // Display counter value on LED bar with upper five bits inverted if needed
    if (bits_inverted) {

        s4819560_reg_lta1000g_write(counter_value ^= 0x1F);
        counter_value ^= 0x1F;  // Invert upper five bits
        }
    }
    else{ 
        s4819560_reg_lta1000g_write(counter_value);}
    
    // Delay
    for (volatile uint32_t i = 0; i < 1000000; i++);
    // counter value using Fibonacci sequence
    counter_value = Fibonacci();
    }

    return 0;
};