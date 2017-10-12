#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; //Turn off lowpower of FR5994
	
	//takes 382ns to debounce
	TA0CTL = TASSEL_2 + MC_0 + ID_3; //SMCLK, halted, divided by 8, 1MHz/8=125kHz
    TA0CCTL0 = CCIE; //enable interrupt
    TA0CCR0 = 125; //Max TimerA reaches before coming down, so 1kHz  signal

    P5DIR &= ~BIT5;
    P5REN |= BIT5;
    P5OUT |= BIT5;
    P5IE |= BIT5; //enable interrupt on bit 3
    P5IES |= BIT5;
    P5IFG &= ~BIT5; //clear flag

    P1DIR |= BIT0; //Set P1.0 to output
    P1OUT &= ~BIT0; //set p1.0 to off

    __enable_interrupt();


    while(1){}

	return 0;
}

#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    P5IE |= BIT5;
    TA0CTL = TASSEL_2 + MC_0 +ID_3 + TACLR;
    P5IFG &= ~BIT5;
}
#pragma vector = PORT5_VECTOR
__interrupt void Port_5(void) //interrupt for button
{
    P1OUT ^=BIT0;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;
    P5IE &= ~BIT5;
    P5IFG &= ~BIT5;
    P5IES ^= BIT5; //Toggle interrupt edge
}
