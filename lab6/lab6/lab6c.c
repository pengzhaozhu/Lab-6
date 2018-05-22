/* Lab 6 Part C
   Name: Pengzhao Zhu
   Section#: 112D
   TA Name: Chris Crary
   Description: This program initializes the DAC system and generates a waveform with a constant voltage of 1 V.
				I will then measure the signal with the DAD oscilloscope at the output.
   
 */



#include <avr/io.h>
#include <avr/interrupt.h>


void CLK_32MHZ(void);
void DAC(void);


int main(void) {
	CLK_32MHZ();
	DAC();               //initialize DAC
	
	//VDAC=(CHDATA/0xFFF) x VREF
	PORTA_DIRSET=0x04;   //set PA2 as DAC0 output
	
	DACA_CH0DATA=1638;   //DAC output value according to the formula
	
	while(1);
	
	return 0;
}

void DAC(void) {
	DACA_CTRLA= DAC_ENABLE_bm | DAC_CH0EN_bm ;        //enable DAC, enable channel 0 output
	DACA_CTRLB=DAC_CHSEL_SINGLE_gc;   //single-channel operation on channel 0
	DACA_CTRLC=DAC_REFSEL_AREFB_gc;  //AREF on PORTB as reference
	
	
	/*
	
	DACA_CTRLA= 0b00000101;        //enable DAC, enable channel 0 output
	DACA_CTRLB= 0x00;				//single-channel operation on channel 0
	DACA_CTRLC= 0b00011000;			//AREF on PORTB as reference
	*/
}


void CLK_32MHZ(void)
{
	
	OSC_CTRL=0x02;     //select the 32Mhz osciliator
	while ( ((OSC_STATUS) & 0x02) != 0x02 );   //check if 32Mhz oscillator is stable
	//if not stable. keep looping
	
	CPU_CCP= 0xD8;                       //write IOREG to CPU_CCP to enable change
	CLK_CTRL= 0x01;						//select the 32Mhz oscillator
	CPU_CCP= 0xD8;						//write IOREG to CPU_CCP to enable change
	CLK_PSCTRL= 0x00;					//0x00 for the prescaler
	
}
