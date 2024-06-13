/**
***************************************************************
* @file mylib/ s4819560_rgb.c
* @author s48195609
* @date 08/03/2024
* @brief RGB LED
* REFERENCE: templates/mylib, examples/pwm
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
s4819560_reg_rgb_init() - initialise RGB led
s4819560_reg_rgb_brightness_write(int level) - set brightness of led
s4819560_reg_rgb_brightness_read() - read brightness of led
s4819560_reg_rgb_colour_set(unsigned char rgb_mask) - set colour of led
s4819560_reg_rgb_colour_get() - get colour of led
***************************************************************
*/


#include "board.h"
#include "processor_hal.h"
#include "s4819560_rgb.h"

#define PWM_FREQUENCY  1000000
#define PWM_PULSE_WIDTH_SECONDS  0.05                
#define PWM_PULSE_WIDTH_TICKS    1000  
#define PWM_PERCENT2TICKS_DUTYCYCLE(value) (uint16_t)(value*PWM_PULSE_WIDTH_TICKS/100) 
#define PWM_DUTYCYCLE_CHANGE(value) TIM1->CCR1=(uint16_t)value 

void s4819560_reg_rgb_init(){

    // Enable the GPIO Clock for Port E and F - output
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();

    //Initialise Pins as outputs
    // Port E pin 11 - red CN12 pin 56
    GPIOE->MODER &= ~(0x03 << (11 * 2));  //clear bits
    GPIOE->MODER |= (0x01 << (11 * 2));   //Set for push pull
    GPIOE->OSPEEDR &= ~(0x03<<(11 * 2));
	GPIOE->OSPEEDR |= (0x02<<(11 * 2));  // Set for Fast speed
    GPIOE->OTYPER &= ~(0x01 << 11);       //Clear Bit for Push/Pull Output
    GPIOE->PUPDR &= ~(0x03 << (11 * 2));   //Clear Bits
    GPIOE->PUPDR |= ((0x01) << (11 * 2));  //Set for Pull down output

    // PORT F PIN 14 - Green CN12 pin 50
    GPIOF->MODER &= ~(0x03 << (14 * 2));  
    GPIOF->MODER |= (0x01 << (14 * 2));
    GPIOF->OSPEEDR &= ~(0x03<<(14 * 2));
	GPIOF->OSPEEDR |= (0x02<<(14 * 2));  
    GPIOF->OTYPER &= ~(0x01 << 14);       
    GPIOF->PUPDR &= ~(0x03 << (14 * 2));   
    GPIOF->PUPDR |= ((0x01) << (14 * 2));

    //PORT F PIN 15 - Blue CN12 pin 60
    GPIOF->MODER &= ~(0x03 << (15 * 2));  
    GPIOF->MODER |= (0x01 << (15 * 2));
    GPIOF->OSPEEDR &= ~(0x03<<(15 * 2));
	GPIOF->OSPEEDR |= (0x02<<(15 * 2));  
    GPIOF->OTYPER &= ~(0x01 << 15);       
    GPIOF->PUPDR &= ~(0x03 << (15 * 2));   
    GPIOF->PUPDR |= ((0x01) << (15 * 2));

    // Brightness control GPIO(PWM) - PORT E PIN 9
    GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << 2);		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (9 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (9 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (9 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << ((9-8) * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << ((9-8) * 4));	//Set Alternate Function for pin (lower ARF register) 

	__TIM1_CLK_ENABLE();      // Timer 1 clock enable

    //Set the clock prescaler to 100kHz (COUNT FREQUENCYß)
    //SystemCoreClock is the system clock frequency 
    TIM1->PSC = ((SystemCoreClock/2) / PWM_FREQUENCY) - 1;

    // Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
    TIM1->CR1 &= ~TIM_CR1_DIR; 

    TIM1->ARR = PWM_PULSE_WIDTH_TICKS; 
    TIM1->CCR1 = PWM_PERCENT2TICKS_DUTYCYCLE(50);   //INITIALISATION VALUE - equivalent to 2.5ms p/w

    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M); 	// Clear OC1M (Channel 1) 
	TIM1->CCMR1 |= (0x6 << 4); 		// Enable PWM Mode 1, upcounting, on Channel 1
	TIM1->CCMR1 |= (TIM_CCMR1_OC1PE);  // Enable output preload bit for channel 1

 	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC1E; 	// Set CC2E Bit
	TIM1->CCER &= ~TIM_CCER_CC1NE; 	// Clear CC2NE Bit for active high output
	
	/* Set Main Output Enable (MOE) bit
	Set Off-State Selection for Run mode (OSSR) bit
    Set Off-State Selection for Idle mode (OSSI) bit */
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter   
}

void s4819560_reg_rgb_brightness_write(int level){

    int dutyCycle = level / 2;
    PWM_DUTYCYCLE_CHANGE(PWM_PERCENT2TICKS_DUTYCYCLE(dutyCycle));
}

int s4819560_reg_rgb_brightness_read(){

    int regValue = TIM1->CCR1; // access register where PWM duty cycle determined
    int brightness = regValue / 5; //scales accordingly to established duty cycles

    return brightness;
}

void s4819560_reg_rgb_colour_set(unsigned char rgb_mask){

    // rgb_mask bits: 0bRGB (R->Bit2, G->Bit1, B->Bit0)
    // Set Blue
    if (rgb_mask & 0x01) { 
        GPIOF->ODR |= (0x01 << 15);  
    } else {
        GPIOF->ODR &= ~(0x01 << 15);
    }
    
    // Set Green
    if (rgb_mask & 0x02) { 
        GPIOF->ODR |= (0x01 << 14);
    } else {
        GPIOF->ODR &= ~(0x01 << 14);
    }
    
    // Set Red
    if (rgb_mask & 0x04) { 
        GPIOE->ODR |= (0x01 << 11); 
    } else {
        GPIOE->ODR &= ~(0x01 << 11); 
    }
}

void s4819560_reg_rgb_colour_get(){}