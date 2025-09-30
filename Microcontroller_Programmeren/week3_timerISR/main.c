/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#include "driverlib.h"
#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog
    PM5CTL0 &= ~LOCKLPM5;       // GPIO vrijgeven

    // --- LED configuratie ---
    P1DIR |= BIT0;              // P1.0 = output
    P1OUT &= ~BIT0;             // LED uit

    // --- ACLK configureren ---
    CSCTL0_H = CSKEY_H;         // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;         // Zet LFXT aan
    do {
        CSCTL5 &= ~LFXTOFFG;    // Clear oscillator fault flags
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);  // Blijf wachten tot stabiel
    CSCTL0_H = 0;               // Lock CS registers weer

    // --- Timer configureren ---
    TA1CTL = TASSEL__ACLK | MC__CONTINUOUS | TACLR;  // Timer A1, ACLK, continuous mode
    TA1CTL |= TAIE;             // Overflow interrupt enable
    TA1CCTL2 = CCIE;            // CCR2 interrupt enable
    TA1CCR2 = 16384;            // Compare waarde (0.5s bij 32kHz)

    // --- Global interrupt aan + LPM0 ---
    __bis_SR_register(LPM0_bits | GIE);
}

// === ISR’s ===
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void) {
    // CCR0 (nu niet gebruikt)
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void) {
    switch(__even_in_range(TA1IV, 0x0E)) {
        case 0x04:                  // CCR2 interrupt
            P1OUT ^= BIT0;          // Toggle LED
            TA1CCR2 += 16384;       // Volgende trigger na 0.5s
            break;
        case 0x08:                  // Overflow interrupt
            P1OUT ^= BIT0;          // Toggle LED
            break;
        default: break;
    }
}
