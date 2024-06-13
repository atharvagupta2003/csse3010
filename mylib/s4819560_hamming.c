/**
***************************************************************
* @file mylib/ s4819560_hamming.h
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

#include "board.h"
#include "processor_hal.h"
#include "s4819560_hamming.h"

uint8_t hamming_hbyte_encode(uint8_t value);
int s4819560_lib_hamming_parity_error(uint8_t value);

uint16_t s4819560_lib_hamming_byte_encode(uint8_t value){
    // Least significant byte
    uint8_t lsb = value & 0x0F;
    
    // Most significant byte
    uint8_t msb = (value & 0xF0) >> 4;

    // Least significant byte (encoded)
    uint8_t lsb_encoded = hamming_hbyte_encode(lsb);
    
    // Most significant byte (encoded)
    uint8_t msb_encoded = hamming_hbyte_encode(msb);
   
    // Combine 
    uint16_t output = lsb_encoded | (msb_encoded << 8);
    
    return output;
}

uint8_t hamming_hbyte_encode(uint8_t value){
    uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;

	/* extract bits */
	d0 = !!(value & 0x1);
	d1 = !!(value & 0x2);
	d2 = !!(value & 0x4);
	d3 = !!(value & 0x8);

	/* calculate hamming parity bits */
	h0 = d1 ^ d2 ^ d3;
	h1 = d0 ^ d2 ^ d3;
	h2 = d0 ^ d1 ^ d3;

	/* generate out byte without parity bit P0 */
	out = (h0 << 1) | (h1 << 2) | (h2 << 3) |
		(d0 << 4) | (d1 << 5) | (d2 << 6) | (d3 << 7);

	/* calculate even parity bit */
	for (z = 1; z<8; z++)
		p0 = p0 ^ !!(out & (1 << z));

	out |= p0;

	return(out);
}

uint8_t s4819560_lib_hamming_byte_decode(uint8_t value){
        // End parity bit
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

        // XOR all the bits together, do not correct if XORing equals 0
    if (s4819560_lib_hamming_parity_error(value) == 0 && syndrome != 0) {

        // 2-bit error, can't correct
        return (value >> 4);        
    }
    if (syndrome == 0) {
        
        // No errors occured
        return (value >> 4);
    } else {

        // Else, one-bit correctable error has occured. Correcting here
        if (s0 && !s1 && !s2) {

            // Column 0 error [1 0 0]            
            h0 = !h0;
        } else if (!s0 && s1 && !s2) {
        
            // Column 1 error [0 1 0]
            h1 = !h1;
        } else if (!s0 && !s1 && s2) {

            // Column 2 error [0 0 1]
            h2 = !h2;
        } else if (!s0 && s1 && s2) {
            
            // Column 3 error [0 1 1]
            d0 = !d0;
        } else if (s0 && !s1 && s2) {
            
            // Column 4 error [1 0 1]
            d1 = !d1;
        } else if (s0 && s1 && !s2) {
            
            // Column 5 error [1 1 0]
            d2 = !d2;
        } else if (s0 && s1 && s2) {
            
            // Column 6 error [1 1 1]
            d3 = !d3;
        } else {

            // Shouldn't reach here, something went wrong
            return (value >> 4);
        }
        // Compose data bits back 
        uint8_t output = (d0 << 0) + (d1 << 1) + (d2 << 2);
        return output;
    }
}

int s4819560_lib_hamming_parity_error(uint8_t value){
        // End parity bit
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

    uint8_t calculatedParity = (h0 ^ h1 ^ h2 ^ d0 ^ d1 ^ d2 ^ d3);
    if (calculatedParity == p0) {
        
        return 0;
    } else {
        
        return 1;
    }
}