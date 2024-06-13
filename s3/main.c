/**
******************************************************************************
* @file    s3/main.c
* @author  s48195609
* @date    06/04/2024
* @brief   Encode and decode console inputs using fsm processing and display results on LED bar.
* REFERENCE: getting started/hamming, getting started/fsm, getting started/console, timer/interrupt
******************************************************************************
*/

#include "board.h"
#include "processor_hal.h"
#include "s4819560_hamming.h"
#include "s4819560_console.h"
#include "s4819560_lta1000g.h"
#include "s4819560_mfs_pb.h"
#include "s4819560_mfs_led.h"
#define DEBUG
#ifdef DEBUG
#include "debug_log.h"
#endif


#define S0  0  //IDLE State
#define S1  1  //Encode State
#define S2  2  //Decode State
int pb_get;
int flag2;
int flag;
int currentState = S0;
int fsm_processing(int current_state, char input1, char input2);
void state_processing(int current_state, char in1, char in2);
uint8_t calculate_syndrome(uint8_t value);
void hardware_init(){
    HAL_Init();	
    s4819560_reg_lta1000g_init();
    s4819560_reg_mfs_pb_init(1);
    s4819560_reg_mfs_led_init();
}
void main(void){
    hardware_init();
    #ifdef DEBUG
 	BRD_debuguart_init();
    #endif
    int inputs = 0;
    uint32_t prev_tick = 0;
    char recvChar1, recvChar2;
    flag = 0;
    flag2 = 0;
    while (1) {
        if ((HAL_GetTick() - prev_tick) >= 50) {
            prev_tick = HAL_GetTick();
            pb_get = s4819560_reg_mfs_pb_press_get(1);
            if (inputs == 0) {
                recvChar1 = BRD_debuguart_getc();
                if (recvChar1 != '\0') {
                    inputs = 1;
                    #ifdef DEBUG
                    debug_log("recvChar1 %c\n\r", recvChar1);
                    #endif
                }
            }

            if (inputs == 1) {
                recvChar2 = BRD_debuguart_getc();
                if (recvChar2 != '\0') {
                    inputs = 0;
                    #ifdef DEBUG
                    debug_log("recvChar2 %c\n\r", recvChar2);
                    #endif 
                    flag = 0;
                }
            }
        }
        if ((recvChar1 != '\0') && (recvChar2 != '\0')) {
            currentState = fsm_processing(currentState, recvChar1, recvChar2);
            if (flag2 == 0)
            {
                debug_log("current state = %c\n\r", currentState + '0');
                flag2 = 1;
            }
        } 
        if (((currentState == S1) || (currentState == S2)) && (inputs == 0)) {
            state_processing(currentState, recvChar1, recvChar2);
        }        
        
    }
}

int fsm_processing(int current_state, char input1, char input2)
{
    int next_state = S0;
    switch (current_state)
    {
    case S0:
        if (input1 == '+'&&input2 == '+')
        {
            next_state = S0;
            debug_log("idle State!\n\r");
        }
        else if (input1 == '#'&&input2 == '#')
        {
            next_state = S1;
            debug_log("Encode State!\n\r");
        }
        else if (input1 == '$'&& input2 == '$')
        {
            next_state = S2;
            debug_log("Decode State!\n\r");
        }
        else{
            next_state = current_state;
        }
        break;
    case S1:
        if (input1 == '&'&&input2 == '&')
        {
            next_state = S1;
            debug_log("Encode State!\n\r");
        }
        else if (input1 == '*'&&input2 == '*')
        {
            next_state = S2;
            debug_log("Decode State!\n\r");
        }
        else if (input1 == '!'&& input2 == '!')
        {
            next_state = S0;
            debug_log("Idle State!\n\r");
        }
        else{
            next_state = current_state;
        }        
        break;
    case S2:
        if (input1 == '?'&&input2 == '?')
        {
            next_state = S2;
            debug_log("Decode State!\n\r");
        }
        else if (input1 == '%'&&input2 == '%')
        {
            next_state = S0;
            debug_log("Idle State!\n\r");
        }
        else if (input1 == '@'&& input2 == '@')
        {
            next_state = S1;
            debug_log("Encode State!\n\r");
        }
        else{
            next_state = current_state;
        }        
        break;
    }
    return next_state;
}

void state_processing(int current_state, char in1, char in2) {
        uint16_t Word = 0;
        uint8_t codedWord = 0;
        uint8_t codedWord2 = 0;
        uint8_t inputWord1 = 0;
        uint8_t inputWord2 = 0;
        int inputs = 0;

    if (((in1 >= '0' && in1 <= '9') || (in1 >= 'A' && in1 <= 'F') || (in1 >= 'a' && in1 <= 'f')) &&
        ((in2 >= '0' && in2 <= '9') || (in2 >= 'A' && in2 <= 'F') || (in2 >= 'a' && in2 <= 'f'))) {
        inputWord1 = s4819560_lib_console_ascii2hex(in1);
        inputWord2 = s4819560_lib_console_ascii2hex(in2);
        inputs = 2;
    }
        if (inputs == 2) {
            s4819560_REG_MFS_LED_D4_ON();
            HAL_Delay(500);
            s4819560_REG_MFS_LED_D4_OFF();
            HAL_Delay(500);
            s4819560_REG_MFS_LED_D4_ON();
            HAL_Delay(500);
            s4819560_REG_MFS_LED_D4_OFF();
            HAL_Delay(500);
            s4819560_REG_MFS_LED_D4_ON();
            HAL_Delay(500);
            s4819560_REG_MFS_LED_D4_OFF();
            HAL_Delay(500);
            inputs = 0;
        }
    if ((in1 != '#' || in2 != '#')&& current_state == S1)
    {
        codedWord = hamming_hbyte_encode(inputWord1);
        codedWord2 = hamming_hbyte_encode(inputWord2);
        if (pb_get%2 == 0)
        {
            s4819560_reg_lta1000g_write(codedWord);
            s4819560_REG_MFS_LED_D3_ON();
        }else{
            s4819560_reg_lta1000g_write(codedWord2);
            s4819560_REG_MFS_LED_D3_OFF();
        }HAL_Delay(1000);
        
        if (flag == 0){

            debug_log("CodedWord 0x%x%x\n\r", codedWord, codedWord2);
            flag = 1;
        }
    }else if ((in1 != '*' || in2 != '*') && current_state == S2) 
    {
        inputWord1 = inputWord1 << 4;
        inputWord1 |= inputWord2 << 0;
        uint8_t decodedWord1 = s4819560_lib_hamming_byte_decode(inputWord1);
        s4819560_reg_lta1000g_write(decodedWord1);
        HAL_Delay(1000);
        if (flag == 0)
        {debug_log("decoded word 0x%d\n\r", decodedWord1);
        flag = 1;}
        uint8_t syndrome = calculate_syndrome(decodedWord1);
        // Set the MFS D2 LED for 2-bit error
        if (s4819560_lib_hamming_parity_error(decodedWord1) == 0 && syndrome != 0)
        {s4819560_REG_MFS_LED_D2_ON();
        }else
        {s4819560_REG_MFS_LED_D2_OFF();}
        if (s4819560_lib_hamming_parity_error(decodedWord1)) {
        s4819560_REG_MFS_LED_D1_ON();
        } else {
        s4819560_REG_MFS_LED_D1_OFF();
        }
    }
}

uint8_t calculate_syndrome(uint8_t value){
        uint8_t p0 = !!(value & 0x01);
    
        // Hamming bits
        uint8_t h0 = !!(value & 0x02);
        uint8_t h1 = !!(value & 0x04);
        uint8_t h2 = !!(value & 0x08);
    
        // Data bits
        uint8_t d0 = !!(value & 0x10);
        uint8_t d1 = !!(value & 0x20);
        uint8_t d2 = !!(value & 0x40);
        uint8_t d3 = !!(value & 0x80);

        // Calculate syndrome
        uint8_t s0 = h0 ^ d1 ^ d2 ^ d3;
        uint8_t s1 = h1 ^ d0 ^ d2 ^ d3;
        uint8_t s2 = h2 ^ d0 ^ d1 ^ d3;

        uint8_t syndrome = (s0 << 0) + (s1 << 1) + (s2 << 2);
        return syndrome;
}

