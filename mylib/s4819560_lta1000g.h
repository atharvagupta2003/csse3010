/**
***************************************************************
* @file mylib/ s4819560_lta1000g.h
* @author s48195609
* @date 27/02/2024
* @brief LED Light Bar
* REFERENCE: templates/mylib
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s48195609_reg_lta1000g_init() - intialise LED bar
* lta1000g_seg_set(int segment, unsigned char value) - set LED bar value
* s48195609_reg_lta1000g_write(unsigned short value) - Write the LED Bar segments high or low
* s48195609_reg_lta1000g_invert(unsigned short value, unsigned short mask) - Inverts the LED Bar segments
***************************************************************
*/

#ifndef s4819560_LTA000G_H
#define s4819560_LTA000G_H

void s4819560_reg_lta1000g_init();
void lta1000g_seg_set(int segment, unsigned char value);
void s4819560_reg_lta1000g_write(unsigned short value);
void s4819560_reg_lta1000g_invert(unsigned short value, unsigned short mask);

#endif
