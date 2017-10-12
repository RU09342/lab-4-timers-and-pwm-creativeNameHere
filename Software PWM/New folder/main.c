#include <msp430.h> 


/**
 * main.c
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //Turn off lowpower of FR5994

    TA0CTL = TASSEL_2 + MC_3 + ID_0; //SMCLK, UPDOWN, divided by 1, 1MHz
    TA0CCTL1 = CCIE; //enable interrupt
    TA0CCR0 = 1000; //Max TimerA reaches before coming down, so 1kHz  signal
    TA0CCR1 = 500;

    //takes 382ns to debounce
    TA1CTL = TASSEL_2 + MC_0 + ID_3; //SMCLK, halted, divided by 8, 1MHz/8=125kHz
    TA1CCTL0 = CCIE; //enable interrupt
    TA1CCR0 = 125; //Max TimerA reaches before coming down, so 1kHz  signal

    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1; //enable interrupt on bit 3
    P1IES |= BIT1;
    P1IFG &= ~BIT1; //clear flag

    P1DIR |= BIT0; //Set P1.0 to output
    P1OUT &= ~(BIT0); //set p1.0 to off

    P9DIR |=BIT7;
    P9OUT &= ~BIT7;
    __enable_interrupt();


    while(1){}

    return 0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    P1OUT ^= BIT0;
}
#pragma vector = PORT5_VECTOR
__interrupt void Port_5(void) //interrupt for button
{
    P9OUT ^=BIT7;
    TA1CTL = TASSEL_2 + MC_1 + ID_3;
    P1IE &= ~BIT1;
    P1IFG &= ~BIT1;
    P1IES ^= BIT1; //Toggle interrupt edge

    if(TA1CTL == TASSEL_2 + MC_0 +ID_3){
        TA0CCR1 = (TA0CCR1 + 100);
        if(TA0CCR1 > 1000){
            TA0CCR1 = 0;
        }
    }
}
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A1(void)
{
    P1IE |= BIT1;
    TA1CTL = TASSEL_2 + MC_0 +ID_3 + TACLR;
    P1IFG &= ~BIT1;
}
