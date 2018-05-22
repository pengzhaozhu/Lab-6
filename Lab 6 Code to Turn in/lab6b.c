/* Lab 6 Part B
   Name: Pengzhao Zhu
   Section#: 112D
   TA Name: Chris Crary
   Description: This program creates a voltmeter than will measure the drop across the CDS cell every 100 ms. It will then output
				to Putty.
 */



#include <avr/io.h>
#include <avr/interrupt.h>


void CLK_32MHZ(void);

void ADC(void);
void USART_INIT(void);
void TIMER_INIT(void);
void OUT_CHAR(uint8_t data);

#define BSELHIGH (((4)*((32000000/(16*57600))-1))>>8)   //bscale of -2
#define BSEL ((4)*((32000000/(16*57600))-1))			//bscale of -2


#define timer_100 (32000000*.1)/1024	

int16_t adc;
uint8_t sign;  //for +, -, or neither


float voltage;
float voltage2;
float voltage3;
int int1;
int int2;
int int3;
int int1_send;
int int2_send;
int int3_send;
uint8_t hex1_send;
uint8_t hex2_send;

uint8_t adc_send;



int main(void)
{
	CLK_32MHZ();
	ADC();
	USART_INIT();
	TIMER_INIT();
   
      //8 bit adc. y=(adc/51)+(1/102).   .5 V= adc of 
	  //y=(1/819)x + (1/1638) for 12 bit
 
   
		while(1) {       //uncomment for full part b code
		
		while((TCC0_INTFLAGS & 0x01) != 0x01);   //wait for interrup flag of 100 ms for TCC0
		
		TCC0_CNT=0x00;    //reset TCC0_CNT to 0
		TCC0_INTFLAGS=0x01;   //clears the interrupt flag
		
		
		while((ADCA_CH0_INTFLAGS & 0x01)!= 0x01);   //wait for adc conversion to be completed
		adc=ADCA_CH0_RES;      //take adc value
		ADCA_CH0_INTFLAGS=0x01;    //clear adc interrupt flag
		
		if (adc < 0) {
			sign='-';
		} else if (adc > 0) {
			sign='+';
		} else if (adc==0) {
			sign=' ';
		}
		
		OUT_CHAR(sign);  //transmit positive or negative sign
		
		
		
	    voltage = ( (((float)adc)/51)+(1/102));      //get floating point voltage value
		
		if (voltage<0) {
			voltage=voltage*(-1);                 //so voltage value will always be positive when i am doing math later
		}
		
		int1 = (int) voltage;                        //transmit the tenth place
		int1_send = int1+48;                               //from number to ascii according to the ascii table
		OUT_CHAR(int1_send);
		
		OUT_CHAR('.');
		
		voltage2=10*(((float)voltage)-int1);         //transmit the first decimal place
		int2= (int) voltage2;						
		int2_send= int2+48;								//from number to ascii according to the ascii table
		OUT_CHAR(int2_send);
		
		voltage3=10*(((float)voltage2)-int2);		//transmit the second decimal place
		int3= (int) voltage3;			
		int3_send=int3+48;								//from number to ascii according to the ascii table
		OUT_CHAR(int3_send);
		
		OUT_CHAR(' ');
		OUT_CHAR('V');
		OUT_CHAR(' ');
		OUT_CHAR('(');
		OUT_CHAR('0');
		OUT_CHAR('x');
		
		adc_send= adc>>4;                              //take the upper byte of the 8 bit
		adc_send=adc_send & 0x0F;                   
		if ( adc_send >= 10) {                         //if it is a character, add 55 (ascii table)
			hex1_send=adc_send+55;
		} else if (adc_send < 10) {                    //if it is a number, add 48 (ascii table)
			hex1_send=adc_send +48;
		}
		OUT_CHAR(hex1_send);
		
		adc_send= adc;                                 //take the lower byte of the 8 bit
		adc_send=adc_send & 0x0F;
		if ( adc_send >= 10) {                         //if it is a character, add 55 (ascii table)
			hex2_send=adc_send+55;
			} else if (adc_send < 10) {				//if it is a number, add 48 (ascii table)
			hex2_send=adc_send +48;
		}
		OUT_CHAR(hex2_send);
	
		OUT_CHAR(')');
		OUT_CHAR(' ');
		OUT_CHAR(' ');
		OUT_CHAR(' ');
		
		TCC0_CNT=0x00;    //reset TCC0_CNT to 0

		}                              //uncomment for full part B code
		
    return 0;
}




void ADC(void) {
	
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
	ADCA_PRESCALER=ADC_PRESCALER_DIV512_gc;                //512 prescaler or adc clock
	ADCA_CTRLB=ADC_CONMODE_bm | ADC_RESOLUTION_8BIT_gc | ADC_FREERUN_bm;     //signed mode, 12 bit resolution, free run
	PORTA_DIRCLR= 0b01000010; //PA1 as positive input, PA6 as negative input. used later for cds cell
	ADCA_CH0_CTRL=ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_DIFFWGAIN_gc;
	ADCA_CH0_MUXCTRL=0b00001010; //muxcontrol for PA1 as positive, PA6 as negative
	
	ADCA_CTRLA=ADC_ENABLE_bm|ADC_CH0START_bm;
	
	}
	
void USART_INIT(void)
{
	PORTD_DIRSET=0x08;   //set transmitter as output
	PORTD_DIRCLR=0X04;	 //set receiver as input
	
	USARTD0_CTRLB=0x18;  //enable receiver and transmitter
	USARTD0_CTRLC= 0X33; //USART asynchronous, 8 data bit, odd parity, 1 stop bit
	
	USARTD0_BAUDCTRLA= (uint8_t) BSEL;    //load lowest 8 bits of BSEL
	USARTD0_BAUDCTRLB= (((uint8_t) BSELHIGH) | 0xE0); //load BSCALE and upper 4 bits of BSEL. bitwise OR them
	
	PORTD_OUTSET= 0x08;   //set transit pin idle
}

void TIMER_INIT(void) {
	
	TCC0_CNT=0x0000;   //set CNT to zero
	TCC0_PER=(uint16_t) timer_100;    //timer per value to 100 ms
	TCC0_CTRLA=0b00000111; //timer prescaler of 1024
	
}


void OUT_CHAR(uint8_t data) {
	
	while( ((USARTD0_STATUS) & 0x20) != 0x20);			//keep looping if DREIF flag is not set
	
	USARTD0_DATA= (uint8_t) data;
	
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
