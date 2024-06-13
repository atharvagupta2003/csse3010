/**
***************************************************************
* @file mylib/ s4819560_mfs_trimpot.h
* @author s48195609
* @date 09/03/2024
* @brief MFS Trimpot
* REFERENCE: templates/mylib
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s4819560_reg_mfs_trimpot_init(void) - initialise push button
* s4819560_reg_mfs_trimpot_get(void) - returns raw adc value
* s4819560_reg_mfs_trimpot_calculate_turns(int adc_value) - calculates number of trimpot turns
***************************************************************
*/


#ifndef s4819560_MFS_TRIMPOT_H
#define s4819560_MFS_TRIMPOT_H

// Configuration Parameters
#define s4819560_REG_MFS_TRIMPOT_ZERO_CAL_OFFSET

// Function Prototypes
void s4819560_reg_mfs_trimpot_init(void);
int s4819560_reg_mfs_trimpot_get(void);
int s4819560_reg_mfs_trimpot_calculate_turns(int adc_value);

#endif