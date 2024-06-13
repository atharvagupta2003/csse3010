/**
***************************************************************
* @file mylib/ s4819560_hamming.c
* @author s48195609
* @date 06/04/24
* @brief hamming encoding decoding and parity error check
* REFERENCE: templates/mylib
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* uint16_t s4819560_lib_hamming_byte_encode(uint8_t value)
* uint8_t s4819560_lib_hamming_byte_decode(uint8_t value)
* uint8_t hamming_hbyte_encode(uint8_t value)
* int s4819560_lib_hamming_parity_error(uint8_t value)
***************************************************************
*/

#ifndef s4819560_HAMMING_H
#define s4819560_HAMMING_H

uint16_t s4819560_lib_hamming_byte_encode(uint8_t value);
uint8_t s4819560_lib_hamming_byte_decode(uint8_t value);
uint8_t hamming_hbyte_encode(uint8_t value);
int s4819560_lib_hamming_parity_error(uint8_t value);

#endif
