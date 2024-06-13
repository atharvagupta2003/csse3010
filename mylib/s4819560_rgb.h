/**
***************************************************************
* @file mylib/ s4819560_rgb.h
* @author s48195609
* @date 08/03/2024
* @brief RGB LED
* REFERENCE: templates/mylib
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

#ifndef s4819560_RGB_H
#define s4819560_RGB_H

#define S4819560_REG_RGB_BLACK() s4819560_reg_rgb_colour_set(0x07)
#define S4819560_REG_RGB_WHITE() s4819560_reg_rgb_colour_set(0x00)
#define S4819560_REG_RGB_RED() s4819560_reg_rgb_colour_set(0x03)
#define S4819560_REG_RGB_GREEN() s4819560_reg_rgb_colour_set(0x05)
#define S4819560_REG_RGB_BLUE() s4819560_reg_rgb_colour_set(0x06)
#define S4819560_REG_RGB_YELLOW() s4819560_reg_rgb_colour_set(0x01)
#define S4819560_REG_RGB_CYAN() s4819560_reg_rgb_colour_set(0x04)
#define S4819560_REG_RGB_MAGENTA() s4819560_reg_rgb_colour_set(0x02)

void s4819560_reg_rgb_init();
void s4819560_reg_rgb_brightness_write(int level);
int s4819560_reg_rgb_brightness_read();
void s4819560_reg_rgb_colour_set(unsigned char rgb_mask);
void s4819560_reg_rgb_colour_get();

#endif