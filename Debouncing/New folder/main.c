#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //Turn off lowpower of FR5994

    //takes 382ns to debounce
    TA0CTL = TASSEL_2 + MC_0 + ID_3; //SMCLK, halted, divided by 8, 1MHz/8=125kHz
    TA0CCTL0 = CCIE; //enable interrupt
    TA0CCR0 = 125; //Max TimerA reaches before coming down, so 1kHz  signal

    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1; //enable interrupt on bit 3
    P1IES |= BIT1;
    P1IFG &= ~BIT1; //clear flag

    P1DIR |= BIT0; //Set P1.0 to output
    P1OUT &= ~BIT0; //set p1.0 to off

    __enable_interrupt();


    while(1){}

    return 0;
}

#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    P1IE |= BIT1;
    TA0CTL = TASSEL_2 + MC_0 +ID_3 + TACLR;
    P1IFG &= ~BIT1;
}
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) //interrupt for button
{
    P1OUT ^=BIT0;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;
    P1IE &= ~BIT1;
    P1IFG &= ~BIT1;
    P1IES ^= BIT1; //Toggle interrupt edge
}
