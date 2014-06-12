/**
author : c2
date: 9th june 2014
**/


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
    
    regwrite1();
        __delay_cycles(10000);
    regwrite3();
    regwrite0();
        __delay_cycles(100);
    regwrite2();
        __delay_cycles(100);
    /*now in tx mode*/
    
     __delay_cycles(100);                  //to wait for latency bits
     regwrite7();                               //readback register
    
   readback = bitbang_read(16);                //even though one bit is being thrown out, 18 bits for safty
    
   TXRXDATA_PxDIR |= TXRXDATA_BIT;              //TXRXDATA IS OUTPUT (writing to it)
   TXRXCLK_PxDIR &= ~TXRXCLK_BIT;               //CLK IS INPUT (BIT 1)
  
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
    


/* 
assign a gpio pin to txrxdata/clk and adf is the master, msp the slave.. 
now, when we toggle the clock which is provided by adf(oops, can't do this)
so, when we notice that the adfclock(txrxclk) is say high, then we know that data needs to be extracted/sent from txrxdata(verify)
so, you could create an ISR. 
basically, check for whenever the pin goes high. when it doesn, set up an ISR to send in a 0 or one on the respective gpio pin connected to respective
peripheral(txrxdata/clk).

*) check for signals on oscillooscope after writing to all tx registers and check for the txrxclk 


doubt 1) will the adf clock keep on counting, or will it wait for you to toggle data.
doubt 2) since the ISR will start whenever the pin on the clock goes high, and this could be erratic, when do you exactly start transmission and 
          when do you set up the ISR
lets say this is sorted by first writing to all registers(tx) and then, checking for the toggle clock, and then whenever clock is toggled send in 1/0
then you come out of your ISR, where do you go back to? if you are checking for the toggled clock in a while loop, it'll check for the clock again,
except, if the clock is continuous, and is being going up/down whenever you are in your ISR, it... shouldn't technically matter during transmission
because you can just send data on every high clock.. and ignore some of the high clocks.. but will this cause the adf to send noise/zeros on the 
unwritten high clocks?
also, when receiving data.. if you receive data on every high clock, does your clock keep ticking even when your ISR is running, effectively causing
loss of data when you are busy capturing older data.

doubt 3) what if the ISR is initiated on the rising edge of the ADF clock as required, but by the time you send a bit, it's already the lower clock edge
        effectively, not really sending any data.
doubt 4) while sending data on the higher clock edge how many bits can be sent? 

