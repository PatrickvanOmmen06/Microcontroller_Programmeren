//gpio_lib.h  
#include <msp430.h> 
#include <stdint.h> 
#include <stdbool.h>

#ifndef INCLUDE_GPIO_LIB_H_ 
#define INCLUDE_GPIO_LIB_H_ 

typedef enum {
    DIGITAL = 0,
    PRIMARY,
    SECONDARY,
    TERTIARY
} purposeFunction;

void pinSet(uint16_t port, uint16_t bit, bool val);
void pinToggle(uint16_t port, uint16_t bit);
bool pinGet(uint16_t port, uint16_t bit);
void pinConfigInput(uint16_t port, uint16_t bit, bool pullResistor, bool pullUP, bool IES, bool IE);
void pinSetDir(uint16_t port, uint16_t bit, uint16_t val);
void pinConfigFunction(uint16_t port, uint16_t bit, purposeFunction pf);

#endif/*INCLUDE_GPIO_LIB_H_*/ 
