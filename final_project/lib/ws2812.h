/**
 * Patterns for WS2812 and helper functions
 * from PICO example code using PIO
 * 
*/

#ifndef _WS2812_H_
#define _WS2812_H_
#include "pico/stdlib.h"


void pattern_greys(uint len, uint t);
void pattern_random(uint len, uint t);
void pattern_sparkle(uint len, uint t);
void pattern_snakes(uint len, uint t);
void cycle_through_patterns(uint num_pixels);
#endif //_WS2812_H_