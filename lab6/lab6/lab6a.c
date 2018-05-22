
#include <avr/io.h>
#include <avr/interrupt.h>
void CLK_32MHZ(void);

void ADC(void);
volatile int16_t adc;

int main(void)
{
	CLK_32MHZ();
	ADC();
   
   //int16_t adc;    //8 bit adc. y=(adc/51)+.009804.   .5 V= adc of 
 
   
		while(1) {
		while((ADCA_CH0_INTFLAGS & 0x01)!= 0x01);
		adc=ADCA_CH0_RES;
		ADCA_CH0_INTFLAGS=0x01;
		}
		
    return 0;
}

void ADC(void) {
	
	/*
	PORTA_DIRCLR=0b01000010; //PA1 as positive input, PA6 as negative input. used later for cds cell
	ADCA_CTRLA=0x01; //enable ADC
	ADCA_CTRLB= 0b00010100; //signed mode, free running, and 8 bit right adjusted
	
	ADCA_REFCTRL=0b00110000; //arefb are the voltage reference of 2.5
	ADCA_PRESCALER=0b00000000; //adc prescaler of 512
	ADCA_CH0_CTRL=0b00000011; //start channel 0 conversion, 1x gain, differential input signal with gain
	ADCA_CH0_MUXCTRL=0b00001010; //muxcontrol for PA1 as positive, PA6 as negative
	
	*/
	
	
	/*
	PORTA_DIRCLR=0b01000010; //PA1 as positive input, PA6 as negative input. used later for cds cell
	ADCA_CTRLA=0x01; //enable ADC
	ADCA_CTRLB= 0b00011100; //signed mode, free running, and 8 bit right adjusted
	
	ADCA_REFCTRL=0b00110000; //arefb are the voltage reference of 2.5
	ADCA_PRESCALER=0b00000111; //adc prescaler of 512
	ADCA_CH0_CTRL=0b10000011; //start channel 0 conversion, 1x gain, differential input signal with gain
	ADCA_CH0_MUXCTRL=0b00001010; //muxcontrol for PA1 as positive, PA6 as negative
	*/
	
	
	
	
	ADCA_REFCTRL=ADC_REFSEL_AREFB_gc;      //adc reference as PORTB aref. start scanning on channel 0
	ADCA_PRESCALER=ADC_PRESCALER_DIV128_gc;                //128 prescaler or adc clock
	ADCA_CTRLB=ADC_CONMODE_bm | ADC_RESOLUTION_8BIT_gc | ADC_FREERUN_bm;     //signed mode, 8 bit resolution, free run
	PORTA_DIRCLR= 0b00000001; //PA0 as positive input
	ADCA_CH0_CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA_CH0_MUXCTRL=0b00000000; //muxcontrol for PA0
	
	ADCA_CTRLA=ADC_ENABLE_bm|ADC_CH0START_bm;
	
	
	
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