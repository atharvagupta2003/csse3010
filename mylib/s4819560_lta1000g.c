/**   
******************************************************************************   
* @file mylib/ s4819560_lta1000g.c
* @author s48195609
* @date 27/02/2024
* @brief LED Light Bar
* REFERENCE: examples/getting started/blink, examples/gpio/gpio, templates/mylib
*
******************************************************************************   
*     EXTERNAL FUNCTIONS
******************************************************************************
* s4819560_reg_lta1000g_init() - intialise LED bar
* lta1000g_seg_set(int segment, unsigned char value) - set LED bar value
* s4819560_reg_lta1000g_write(unsigned short value) - Write the LED Bar segments high or low
* s4819560_reg_lta1000g_invert(unsigned short value, unsigned short mask) - Inverts the LED Bar segments
******************************************************************************   
*/


#include "board.h"
#include "processor_hal.h"
#include "s4819560_lta1000g.h"

void s4819560_reg_lta1000g_init(){

    // Enable the GPIO E, F and G Clocks
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();

    //Initialise Pins as outputs
    GPIOE->MODER &= ~((0x03 << (6 * 2)) | (0x03 << (0 * 2)));  // Clear the bits for Pins 6 and 0 in Port E.
    GPIOE->MODER |= ((0x01 << (6 * 2)) | (0x01 << (0 * 2)));  // Set pins to output
    
    GPIOF->MODER &= ~((0x03 << (11 * 2)));  // Clear the bits for Pins 11 in Port F.
    GPIOF->MODER |= ((0x01 << (11 * 2)));  // Set pin to output

    GPIOG->MODER &= ~((0x03 << (15 * 2)) | (0x03 << (10 * 2)) | (0x03 << (8 * 2)) | (0x03 << (13 * 2)) | (0x03 << (5 * 2)) | (0x03 << (11 * 2)) | (0x03 << (6 * 2)));   // Clear the bits for Pins 15, 10, 8, 13, 5, 11 and 6 in Port G.
    GPIOG->MODER |= ((0x01 << (15 * 2)) | (0x01 << (10 * 2)) | (0x01 << (8 * 2)) | (0x01 << (13 * 2)) | (0x01 << (5 * 2)) | (0x01 << (11 * 2)) | (0x01 << (6 * 2)));   // Set pins to output

    //Clear Bits in OTYPE register for Push/Pull Output
    GPIOE->OTYPER &= ~((0x01 << 6) | (0x01 << 0)); 
    GPIOF->OTYPER &= ~((0x01 << 11));
    GPIOG->OTYPER &= ~((0x01 << 15) | (0x01 << 10) | (0x01 << 8) | (0x01 << 13) | (0x01 << 5) | (0x01 << 11) | (0x01 << 6)); 

}

void lta1000g_seg_set(int segment, unsigned char value) {

        if (segment == 0)
        {
            if (value) {

                GPIOE->ODR |= (0x01 << 6);   // Set bit
            } else {

                GPIOE->ODR &= ~(0x01 << 6);   // Clear bit
            }
            
        }
        else if(segment ==1)
        {
            if (value) {

                GPIOG->ODR |= (0x01 << 15);   // Set bit
            } else {

                GPIOG->ODR &= ~(0x01 << 15);   // Clear bit
            }
            
        }
        else if(segment ==2)
        {
            if (value) {

                GPIOG->ODR |= (0x01 << 10);   // Set bit
            } else {

                GPIOG->ODR &= ~(0x01 << 10);   // Clear bit
            }
            
        }
        else if(segment ==3)
        {
            if (value) {

                GPIOG->ODR |= (0x01 << 13);   // Set bit
            } else {
                
                GPIOG->ODR &= ~(0x01 << 13);   // Clear bit
            }
            
        }
        else if(segment ==4)
        {
            if (value) {

                GPIOG->ODR |= (0x01 << 11);   // Set bit
            } else {

                GPIOG->ODR &= ~(0x01 << 11);   // Clear bit
            }
            
        }
        else if(segment ==5)
        {
            if (value) {

                GPIOF->ODR |= (0x01 << 11);   // Set bit
            } else {

                GPIOF->ODR &= ~(0x01 << 11);   // Clear bit
            }
            
        }
        else if(segment ==6)
        {
            if (value) {

                GPIOE->ODR |= (0x01 << 0);   // Set bit
            } else {

                GPIOE->ODR &= ~(0x01 << 0);   // Clear bit
            }
            
        }
        else if(segment ==7)
        {
            if (value) {

                GPIOG->ODR |= (0x01 << 8);   // Set bit
            } else {

                GPIOG->ODR &= ~(0x01 << 8);   // Clear bit
            }
            
        }
        else if(segment ==8)
        {
            if (value) {

                GPIOG->ODR |= (0x01 << 5);   // Set bit
            } else {

                GPIOG->ODR &= ~(0x01 << 5);   // Clear bit
            }
            
        }
        else if(segment ==9)
        {
            if (value) {

                GPIOG->ODR |= (0x01 << 6);   // Set bit
            } else {

                GPIOG->ODR &= ~(0x01 << 6);   // Clear bit
            }
            
        }    
    }

void s4819560_reg_lta1000g_write(unsigned short value) {

    // Iterate through each digit in value to see if segment is high or low.
    for (int seg = 0; seg < 10; seg++) {
        if (((value & (0x0001 << seg)) == (0x0001 << seg))) {

            lta1000g_seg_set(seg, 1);
        } else {

            lta1000g_seg_set(seg, 0);
        }
    }
}

void s4819560_reg_lta1000g_invert(unsigned short value, unsigned short mask) {

    // Write the LED Bar segments high or low based on the value and mask
    // Iterate over each bit of mask
    for (int i = 0; i < 10; i++) {
        if (mask & (1 << i)) {

            // Invert value bit if corresponding mask bit is set
            if (value & (1 << i)) {

                lta1000g_seg_set(i, 0); // Set LED segment to low
            } else {

                lta1000g_seg_set(i, 1); // Set LED segment to high
            }
        }
    }
}
