#include "gpio_lib.h"

// Functie om een pin hoog of laag te zetten
void pinSet(uint16_t port, uint16_t bit, bool val) {
    val ? (*(&P1OUT + port) |= bit) : (*(&P1OUT + port) &= ~bit);
}

// Functie om een pin te togglen
void pinToggle(uint16_t port, uint16_t bit) {
    *(&P1OUT + port) ^= bit;
}

// Functie om de waarde van een pin te lezen
bool pinGet(uint16_t port, uint16_t bit) {
    return (*(&P1IN + port) & bit) != 0;
}

// Configureer pin als input met pull-up/down en interrupts
void pinConfigInput(uint16_t port, uint16_t bit, bool pullResistor, bool pullUP, bool IES, bool IE) {
    // Zet direction naar input
    *(&P1DIR + port) &= ~bit;

    // Configureer pull resistor
    if (pullResistor) {
        *(&P1REN + port) |= bit;
        pullUP ? (*(&P1OUT + port) |= bit) : (*(&P1OUT + port) &= ~bit);
    } else {
        *(&P1REN + port) &= ~bit;
    }

    // Configureer interrupt edge select
    IES ? (*(&P1IES + port) |= bit) : (*(&P1IES + port) &= ~bit);

    // Enable interrupt
    IE ? (*(&P1IE + port) |= bit) : (*(&P1IE + port) &= ~bit);
}

// Zet pin richting (input/output) en initiële waarde
void pinSetDir(uint16_t port, uint16_t bit, uint16_t val) {
    val ? (*(&P1DIR + port) |= bit) : (*(&P1DIR + port) &= ~bit);
}

// Configureer pin functionaliteit (digital/primary/secondary/tertiary)
void pinConfigFunction(uint16_t port, uint16_t bit, purposeFunction pf) {
    switch (pf) {
        case DIGITAL:
            *(&P1SEL0 + port) &= ~bit;
            *(&P1SEL1 + port) &= ~bit;
            break;
        case PRIMARY:
            *(&P1SEL0 + port) |= bit;
            *(&P1SEL1 + port) &= ~bit;
            break;
        case SECONDARY:
            *(&P1SEL0 + port) &= ~bit;
            *(&P1SEL1 + port) |= bit;
            break;
        case TERTIARY:
            *(&P1SEL0 + port) |= bit;
            *(&P1SEL1 + port) |= bit;
            break;
    }
}
