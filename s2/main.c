/**
******************************************************************************
* @file    s2/main.c
* @author  s48195609
* @date    28/02/2024
* @brief   Control the colour and brightness of an RGB LED, using the MFS pushbutton and trimpot on lta1000 led bar.
* REFERENCE: 
******************************************************************************
*/

#include "s4819560_mfs_pb.h"
#include "s4819560_mfs_trimpot.h"
#include "s4819560_rgb.h"
#include "s4819560_lta1000g.h"

#define MAX_DUTY_CYCLE 100

int rgbColorIndex = 7; 
int brightnessLevel = 0; 

// Color sequence table
const unsigned char colorSequence[8] = {0x03, 0x05, 0x06, 0x04, 0x02, 0x01, 0x00, 0x07};

void updateRGBColor();
void updateBrightnessLevel();
void hardware_init(void);

int main() {
    hardware_init();
    while (1) {

        // Read the MFS pushbutton press counter
        int pressCounter = s4819560_reg_mfs_pb_press_get(1);
        int adc_value = s4819560_reg_mfs_trimpot_get();
        int turns = s4819560_reg_mfs_trimpot_calculate_turns(adc_value);

        // If pushbutton is pressed, update the RGB color
        if (pressCounter > 0) {
            updateRGBColor();
            s4819560_reg_mfs_pb_press_reset(1); // Reset pushbutton press counter
        }

        // Read the trimpot turns count and update brightness level if a complete turn is detected    
        updateBrightnessLevel(turns);

        // Display brightness level on the LTA1000G LED Bar in binary
        s4819560_reg_lta1000g_write(turns);
    }

    return 0;
}

// Update RGB color based on the color sequence table
void updateRGBColor() {
    rgbColorIndex = (rgbColorIndex + 1) % 8;
    s4819560_reg_rgb_colour_set(colorSequence[rgbColorIndex]);
}

// Update brightness level based on trimpot turns
void updateBrightnessLevel(int turn_counter) {
    
    brightnessLevel = turn_counter * 10;
    if (brightnessLevel > MAX_DUTY_CYCLE) {
        brightnessLevel = 0;
    }
    s4819560_reg_rgb_brightness_write(brightnessLevel);
}

void hardware_init(void){
    // Initialize RGB LED, MFS Trimpot, MFS Pushbutton, and LTA1000G LED Bar
    s4819560_reg_rgb_init();
    s4819560_reg_mfs_trimpot_init();
    s4819560_reg_mfs_pb_init(1);
    s4819560_reg_lta1000g_init();
    s4819560_reg_rgb_colour_set(0x07);
}
