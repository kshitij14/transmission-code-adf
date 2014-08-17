/*
 * bb.c
 *
 *  Created on: Jun 10, 2014
 *      Author: kshitij
 */

/* ALL REGISTER VALUES FOR 10 MHZ */

#include "bb.h"
//#include "spi.h"

void bitbang_initialise()
{
    BITBANG_CE_PxDIR |= BITBANG_CE_BIT;
    BITBANG_SLE_PxDIR |= BITBANG_SLE_BIT;
    BITBANG_SCLK_PxDIR |= BITBANG_SCLK_BIT;
    BITBANG_SDATA_PxDIR |= BITBANG_SDATA_BIT;
    BITBANG_SREAD_PxDIR &= ~BITBANG_SREAD_BIT;
    BITBANG_SREAD_PxREN |= BITBANG_SREAD_BIT;                   //PULL UP/DOWN ENABLED*

    BITBANG_CE_PxOUT |= BITBANG_CE_BIT;
    __delay_cycles(100000);            //wasn't included before
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
    BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;
    BITBANG_SDATA_PxOUT &= ~ BITBANG_SDATA_BIT;

}

void bitbang_deinitialise()
{
	BITBANG_CE_PxOUT &= ~BITBANG_CE_BIT;
	BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
	BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;
	BITBANG_SDATA_PxOUT &= ~BITBANG_SDATA_BIT;
}

void bitbang_write(unsigned char data)

{
  unsigned long i;
   for(i=0; i<8; i++)
{

	   data=data<<1;                //should be done in the end technically, note

	   if(data & 0x80) 		//data is the 8 bit register
 { BITBANG_SDATA_PxOUT |= BITBANG_SDATA_BIT; }
    else
 { BITBANG_SDATA_PxOUT &= ~BITBANG_SDATA_BIT; }

    BITBANG_SCLK_PxOUT |= BITBANG_SCLK_BIT;             //try clearing the clock again here itself.
   __delay_cycles(100);
    BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;
    __delay_cycles(100);

}
}

long bitbang_read(unsigned char bitsize)
{

  BITBANG_SDATA_PxOUT &= ~BITBANG_SDATA_BIT;
  BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;
  BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
  __delay_cycles(10);


  /* Clock in first bit and discard */
	BITBANG_SCLK_PxOUT |= BITBANG_SCLK_BIT;
        __delay_cycles(50);
        BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;
        __delay_cycles(50);


	unsigned long i, value = 0;


        for(i = 0; i < bitsize; i++)
	{
          value = value<<1;
		//BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;                //try low to high transition too
		       __delay_cycles(50);

                BITBANG_SCLK_PxOUT |= BITBANG_SCLK_BIT;
                       __delay_cycles(50);

		if((BITBANG_SREAD_PxIN & BITBANG_SREAD_BIT)>0)
                { value |= 1; }

                BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;

                __delay_cycles(100);


               // value = value | ((BITBANG_SREAD_PxIN & BITBANG_SREAD_BIT) << (bitsize - i));
	}
        BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;
                                // throw away last bit
        BITBANG_SCLK_PxOUT |= BITBANG_SCLK_BIT;                 //before or after pulling sle low?
        BITBANG_SCLK_PxOUT &= ~BITBANG_SCLK_BIT;
	BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
        return (value);
}

void send_char(unsigned char ch)
{
	unsigned char e;

	for(e=0; e<8; e++)
  {
	while(TXRXCLK_PxIN & TXRXCLK_BIT);
		ch = ch<<1;                              //should be done in the end technically, note
		__delay_cycles(1);
		if (ch & 0x80)
		{ TXRXDATA_PxOUT |= TXRXDATA_BIT; }
		else
		{ TXRXDATA_PxOUT &= ~TXRXDATA_BIT; }


	while(!(TXRXCLK_PxIN & TXRXCLK_BIT));

	}
}

void regwrite0()
{
	 bitbang_write(0x00);			//bitbang_write(0x00);
	 bitbang_write(0xE9);			//bitbang_write(0xF4);
	 bitbang_write(0x2C);			//bitbang_write(0x74);
	 bitbang_write(0x50); 			//bitbang_write(0xA0);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);
}

void regwrite1()
{
	 bitbang_write(0x02);			//bitbang_write(0x02);
	 bitbang_write(0x1F);			//bitbang_write(0x1F);
	 bitbang_write(0xD0);			//bitbang_write(0xD0);
	 bitbang_write(0x11);			//bitbang_write(0x21);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}

void regwrite2()
{
    bitbang_write(0x00);
    bitbang_write(0x81);
    bitbang_write(0xC0);
    bitbang_write(0x82);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}

void regwrite3()
{
	bitbang_write(0x35);				//bitbang_write(0x36);
    bitbang_write(0x90);				//bitbang_write(0xF8);
    bitbang_write(0x81);				//bitbang_write(0x8D);
    bitbang_write(0x13);				//bitbang_write(0xE3);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}
void regwrite4()
{
    bitbang_write(0x00);
    bitbang_write(0x58);			// bitbang_write(0x59);
    bitbang_write(0x02);			// bitbang_write(0x26);
    bitbang_write(0x14);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}
void regwrite5()
{
    bitbang_write(0x00);
    bitbang_write(0x00);
    bitbang_write(0x19);
    bitbang_write(0x15);
        									//bitbang_write(0x2F);
    										//bitbang_write(0xB5); 	11.059
   BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}
void regwrite6()
{
	    bitbang_write(0x0A);
	    bitbang_write(0x05);
	    bitbang_write(0x68);
	    bitbang_write(0x06);
	    										//   bitbang_write(0x0A);
	    										//   bitbang_write(0x0A);
	    										//   bitbang_write(0x4F);    VALUES FOR 11.059Mhz
	    										//   bitbang_write(0x46);
   BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}

void regwrite7()
{
    bitbang_write(0x00);
    bitbang_write(0x00);
    bitbang_write(0x01);
    bitbang_write(0xC7);                        //vary according to readback required
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}

void regwrite9()
{
    bitbang_write(0x00);
    bitbang_write(0x02);
    bitbang_write(0x31);
    bitbang_write(0xE9);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}

void regwrite10()
{
    bitbang_write(0x32);
    bitbang_write(0x96);
    bitbang_write(0x35);
    bitbang_write(0x4A);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}
void regwrite11()
{
    bitbang_write(0x12);
    bitbang_write(0x34);
    bitbang_write(0x56);
    bitbang_write(0x3B);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}
void regwrite12()
{
    bitbang_write(0x00);
    bitbang_write(0x00);
    bitbang_write(0x01);
    bitbang_write(0x4C);
    BITBANG_SLE_PxOUT |= BITBANG_SLE_BIT;
                   __delay_cycles(10);
    BITBANG_SLE_PxOUT &= ~BITBANG_SLE_BIT;
                   __delay_cycles(10);

}




