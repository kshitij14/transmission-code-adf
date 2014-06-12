#include <msp430.h> 

/*
 * main.c
 */
/***
author : kshitij
date : 9th july 2014
subject: different readbacks
***/


#include "bb.h"
//#include "spi.h"

unsigned long readback;                //*change1

int main()
{

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    bitbang_initialise();

    regwrite4();
    regwrite5();
    		__delay_cycles(1000);
    regwrite6();
    		__delay_cycles(1000);
    regwrite9();
    regwrite10();
    regwrite11();
    regwrite12();
        __delay_cycles(10000);
    regwrite1();
    	__delay_cycles(10000);
    regwrite3();
        __delay_cycles(1000);
    regwrite0();
        __delay_cycles(1000);
    regwrite2();
        __delay_cycles(1000);
    /*now in tx mode*/

     __delay_cycles(1000);                  //to wait for latency bits
     regwrite7();                               //readback register

   readback = bitbang_read(16);                //even though one bit is being thrown out, 18 bits for safty

   TXRXDATA_PxDIR |= TXRXDATA_BIT;              //TXRXDATA IS OUTPUT (writing to it)
   TXRXCLK_PxDIR &= ~TXRXCLK_BIT;               //CLK IS INPUT (BIT 1)
   unsigned char buf[3];

   while(1)
   {
	buf[0]=0xFA;
	buf[1]=0xAA;
	buf[2]=0xAE;
	send_char(buf[0]);
	send_char(buf[1]);
	send_char(buf[2]);

	buf[0]=0xFF;
	send_char(buf[0]);
	__delay_cycles(1000000);

   }
   return 0;

}

   /*
    					``**** if i ever do use interupt for transmission ****

   P2IE |= TXRXCLK_BIT;                                 //interupt set on txrxclk
   P2IES &= ~TXRXCLK_BIT;                         //LOW TO HIGH EDGE SELECTED
   _BIS_SR(GIE);                                // Enter w/interrupt

   while(1);


	return 0;
}

unsigned long i=0;
#pragma vector=PORT2_VECTOR
__interrupt void hehe(void)
{
     while(!(TXRXCLK_PxOUT & TXRXCLK_BIT));
      i = i++ ;
       if((i%2)==0)
  TXRXDATA_PxOUT |= TXRXDATA_BIT;
        else
  TXRXDATA_PxOUT &= ~TXRXDATA_BIT;

  P2IFG &= ~TXRXCLK_BIT;                //CLEAR THE INTERUPT FLAG
}
*/

