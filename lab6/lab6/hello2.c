/* Lab 6 Part F
   Name: Pengzhao Zhu
   Section#: 112D
   TA Name: Chris Crary
   Description: This program creates keyboard piano synthesizer with notes in the 6th octave.
				It has both a sine and sawtooth mode.
 */


#include <avr/io.h>
#include <avr/interrupt.h>


void CLK_32MHZ(void);
void DAC(void);
void ADC(void);
void TIMER_INIT(void);
void USARTD0_init(void);
uint8_t IN_CHAR(void);
void OUT_CHAR(uint8_t data);

#define BSELHIGH (((4)*((32000000/(16*57600))-1))>>8)   //bscale of -2
#define BSEL ((4)*((32000000/(16*57600))-1))			//bscale of -2

#define timer_freq ((32000000)*(1/450560))

//#define timer_freq ((32000000)*.1)/1024
//double decimal (1/901120);
//double timer=((32000000)*decimal);

uint8_t input;
int change=2;
uint32_t count=0;
uint32_t delay=0;
uint32_t hello=2;


uint8_t hi;

const uint16_t Table[]= {
	2048,2098,2148,2198,2248,2298,2348,2398,
	2447,2496,2545,2594,2642,2690,2737,2784,
	2831,2877,2923,2968,3013,3057,3100,3143,
	3185,3226,3267,3307,3346,3385,3423,3459,
	3495,3530,3565,3598,3630,3662,3692,3722,
	3750,3777,3804,3829,3853,3876,3898,3919,
	3939,3958,3975,3992,4007,4021,4034,4045,
	4056,4065,4073,4080,4085,4089,4093,4094,
	4095,4094,4093,4089,4085,4080,4073,4065,
	4056,4045,4034,4021,4007,3992,3975,3958,
	3939,3919,3898,3876,3853,3829,3804,3777,
	3750,3722,3692,3662,3630,3598,3565,3530,
	3495,3459,3423,3385,3346,3307,3267,3226,
	3185,3143,3100,3057,3013,2968,2923,2877,
	2831,2784,2737,2690,2642,2594,2545,2496,
	2447,2398,2348,2298,2248,2198,2148,2098,
	2048,1997,1947,1897,1847,1797,1747,1697,
	1648,1599,1550,1501,1453,1405,1358,1311,
	1264,1218,1172,1127,1082,1038,995,952,
	910,869,828,788,749,710,672,636,
	600,565,530,497,465,433,403,373,
	345,318,291,266,242,219,197,176,
	156,137,120,103,88,74,61,50,
	39,30,22,15,10,6,2,1,
	0,1,2,6,10,15,22,30,
	39,50,61,74,88,103,120,137,
	156,176,197,219,242,266,291,318,
	345,373,403,433,465,497,530,565,
	600,636,672,710,749,788,828,869,
	910,952,995,1038,1082,1127,1172,1218,
	1264,1311,1358,1405,1453,1501,1550,1599,
	1648,1697,1747,1797,1847,1897,1947,1997,
};

int main(void) {
	
	//output frequency=sample rate(Hz)/ size of table
	//how fast you need to sample 512 to get (1/1760) when you finished the whole table
	//(1/1760)=512(1/x).   x is the number in Hz
	
	//sample rate(Hz)=output frequency x No. samples
	
	
	CLK_32MHZ();
	TIMER_INIT();
	DAC();
	USARTD0_init();
	
	PORTA_DIRSET=PIN3_bm; //set PA3 as DAC1 output
	PORTC_DIRSET=PIN7_bm; //set POWER DOWN pin as output
	PORTC_OUTSET=PIN7_bm; //set POWER DOWN pin always high to prevent shutdown
	
	while(1) {
		hi=input;
		
		CHECK:;
		input=IN_CHAR();
		OUT_CHAR(input);
		
		if ((input != 'S') && (input != 'W') && (input != '3') && (input != 'E') && (input != '4') && (input != 'R')
		&& (input != 'T') && (input !='6') && (input !='Y') && (input != '7') && (input != 'U') && (input != '8') && (input != 'I')) {
			goto CHECK;
			
		}
		
		if (input=='S') {
			change=change *(-1);       //2 means sine, -2 means sawtooth
			goto CHECK;
		}
		
		if ((input=='W') && (change==2)) {
			TCC0_PER=103;
			} else if ((input=='W') && (change==-2)) {
			TCC0_PER=112;
		}
		
		if ((input=='3') && (change==2)) {
			TCC0_PER=95;
			} else if ((input=='3') && (change==-2)) {
			TCC0_PER=103;
		}
		
		if ((input=='E') && (change==2)) {
			TCC0_PER=91;
			} else if ((input=='E') && (change==-2)) {
			TCC0_PER=97;
		}
		
		if ((input=='4') && (change==2)) {
			TCC0_PER=85;
			} else if ((input=='4') && (change==-2)) {
			TCC0_PER=90;
		}
		
		if ((input=='R') && (change==2)) {
			TCC0_PER=77;
			} else if ((input=='R') && (change==-2)) {
			TCC0_PER=85;
		}
		
		if ((input=='T') && (change==2)) {
			TCC0_PER=72;
			} else if ((input=='T') && (change==-2)) {
			TCC0_PER=79;
		}
		
		if ((input=='6') && (change==2)) {
			TCC0_PER=69;
			} else if ((input=='6') && (change==-2)) {
			TCC0_PER=75;
		}
		
		if ((input=='Y') && (change==2)) {
			TCC0_PER=61;
			} else if ((input=='Y') && (change==-2)) {
			TCC0_PER=71;
		}
		
		if ((input=='7') && (change==2)) {
			TCC0_PER=57;
			} else if ((input=='7') && (change==-2)) {
			TCC0_PER=66;
		}
		
		if ((input=='U') && (change==2)) {
			TCC0_PER=54;
			} else if ((input=='U') && (change==-2)) {
			TCC0_PER=62;
		}
		
		if ((input=='8') && (change==2)) {
			TCC0_PER=50;
			} else if ((input=='8') && (change==-2)) {
			TCC0_PER=58;
		}
		
		if ((input=='I') && (change==2)) {
			TCC0_PER=46;
			} else if ((input=='I') && (change==-2)) {
			TCC0_PER=54;
		}
		
		TCC0_CNT=0x00;
		
		
			
			if (input==hi) {
				count++;
			}
			if (input!=hi) {
				count=0;
				hello=2;
			}
			
			if (count>=1) {
				delay=0;
				
			}
			if (count < 1) {
				delay=2;
			}
			
		if ((change==2) && (hello==2) ) {
			for(int i=0; i< 150;i++){
				for (int i=0; i< 256;i++) {      //go through the 512 samples
					while((TCC0_INTFLAGS & 0x01) != 0x01);   //wait for interrupt flag of sample rate to be set
					TCC0_INTFLAGS=0x01;   //clears the interrupt flag
					
					DACA_CH1DATA=Table[i];   //DAC output value according to the formula
					
					TCC0_CNT=0x00;    //reset TCC0_CNT to 0
				}
				i++;
				
			}
		}
		
			
			
		
		if ((change==2) && (delay==2) ) {
			for(int i=0; i< 1000;i++){
				for (int i=0; i< 256;i++) {      //go through the 512 samples
					while((TCC0_INTFLAGS & 0x01) != 0x01);   //wait for interrupt flag of sample rate to be set
					TCC0_INTFLAGS=0x01;   //clears the interrupt flag
					
					DACA_CH1DATA=Table[i];   //DAC output value according to the formula
					
					TCC0_CNT=0x00;    //reset TCC0_CNT to 0
				}
				i++;
				
			}
		}
	
		
		if((change==-2) && (delay==2) ) {
			for(int i=0; i< 1000;i++){
				for (int i=0; i< 256;i++) {      //go through the 512 samples
					while((TCC0_INTFLAGS & 0x01) != 0x01);   //wait for interrupt flag of sample rate to be set
					TCC0_INTFLAGS=0x01;   //clears the interrupt flag
					
					float sawtooth=i*(273/17);
					DACA_CH1DATA=(int) sawtooth;   //DAC output value according to the formula
					
					TCC0_CNT=0x00;    //reset TCC0_CNT to 0
					
					
				}
				i++;
			}
		}
		
		
		
		if ((change==2)  && (delay==0) ) {
			for(int i=0; i< 150;i++){
				for (int i=0; i< 256;i++) {      //go through the 512 samples
					while((TCC0_INTFLAGS & 0x01) != 0x01);   //wait for interrupt flag of sample rate to be set
					TCC0_INTFLAGS=0x01;   //clears the interrupt flag
					
					DACA_CH1DATA=Table[i];   //DAC output value according to the formula
					
					TCC0_CNT=0x00;    //reset TCC0_CNT to 0
				}
				i++;
				
			}
		}
		
		
		
		
		if((change==-2) && (delay==0) ) {
			for(int i=0; i< 150;i++){
				for (int i=0; i< 256;i++) {      //go through the 512 samples
					while((TCC0_INTFLAGS & 0x01) != 0x01);   //wait for interrupt flag of sample rate to be set
					TCC0_INTFLAGS=0x01;   //clears the interrupt flag
					
					float sawtooth=i*(273/17);
					DACA_CH1DATA=(int) sawtooth;   //DAC output value according to the formula
					
					TCC0_CNT=0x00;    //reset TCC0_CNT to 0
					
					
				}
				i++;
			}
		}
		
	}
	
	
	return 0;
}

void DAC(void) {
	DACA_CTRLA= DAC_ENABLE_bm | DAC_CH1EN_bm ;        //enable DAC, enable channel 1 output
	DACA_CTRLB=DAC_CHSEL_SINGLE1_gc;   //single-channel operation on channel 1
	DACA_CTRLC=DAC_REFSEL_AREFB_gc;  //AREF on PORTB as reference
	
}

void TIMER_INIT(void) {
	
	TCC0_CNT=0x0000;   //set CNT to zero
	TCC0_PER=0;    //timer per value to output 1760 Hz sine wave
	TCC0_CTRLA=TC_CLKSEL_DIV1_gc; //timer prescaler of 1
	//TCC0_CTRLA=TC_CLKSEL_DIV1024_gc;
	
}

void USARTD0_init(void)
{
	PORTD_DIRSET=0x08;   //set transmitter as output
	PORTD_DIRCLR=0X04;	 //set receiver as input
	
	USARTD0_CTRLB=0x18;  //enable receiver and transmitter
	USARTD0_CTRLC= 0X33; //USART asynchronous, 8 data bit, odd parity, 1 stop bit
	
	USARTD0_BAUDCTRLA= (uint8_t) BSEL;    //load lowest 8 bits of BSEL
	USARTD0_BAUDCTRLB= (((uint8_t) BSELHIGH) | 0xE0); //load BSCALE and upper 4 bits of BSEL. bitwise OR them
	
	PORTD_OUTSET= 0x08;   //set transit pin idle
	
	
}


uint8_t IN_CHAR(void) {
	
	while( (USARTD0_STATUS & 0x80) != 0x80);			//keep looping if DREIF flag is not set
	
	return USARTD0_DATA;
	
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
