/**
***************************************************************
* @file mylib/ s4819560_mfs_trimpot.c
* @author s48195609
* @date 09/03/2024
* @brief MFS Trimpot
* REFERENCE: templates/mylib, examples/analog
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s4819560_reg_mfs_trimpot_init(void) - initialise push button
* s4819560_reg_mfs_trimpot_get(void) - returns raw adc value
* s4819560_reg_mfs_trimpot_calculate_turns(int adc_value) - calculates number of trimpot turns
***************************************************************
*/

#include "s4819560_mfs_trimpot.h"
#include "board.h"
#include "processor_hal.h"

#define MAX_ADC_VALUE 1638
#define MAX_TURNS 10

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;

// Define GPIO pins and ADC initialization function
void s4819560_reg_mfs_trimpot_init(void) {

    // Enable the GPIO Clock
    __GPIOA_CLK_ENABLE();

    // Initialize PA3 as an Analog input.
    GPIOA->MODER |= (0x03 << (3 * 2));            // Set bits for Analog input mode
    GPIOA->OSPEEDR &= ~(0x03<<(3 * 2));            
    GPIOA->OSPEEDR |= 0x02<<(3 * 2);              
    GPIOA->PUPDR &= ~(0x03 << (3 * 2));           // Clear bits for no push/pull

    __ADC1_CLK_ENABLE();						//Enable ADC1 clock

	AdcHandle.Instance = (ADC_TypeDef *)(ADC1_BASE);						
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;
 
	HAL_ADC_Init(&AdcHandle);		//Initialise ADC

	// Configure ADC Channel
	AdcChanConfig.Channel = ADC_CHANNEL_3;					//PA3 has Analog Channel 3 connected
	AdcChanConfig.Rank         = 1;
	AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig.Offset       = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		//Initialise ADC Channel

    HAL_ADC_Start(&AdcHandle); // Start ADC conversion

	while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);      // Wait for ADC conversion to finish
}

// Read the current Trimpot input value (raw ADC value)
int s4819560_reg_mfs_trimpot_get(void) {
    // Read ADC value from Trimpot
   unsigned int adc_value;
   adc_value = AdcHandle.Instance->DR;	
   return adc_value;
}

int s4819560_reg_mfs_trimpot_calculate_turns(int adc_value) {
    // Calculate the number of turns
    int turns = (int)(((double)adc_value / MAX_ADC_VALUE) * MAX_TURNS);
    return turns%11;
}
