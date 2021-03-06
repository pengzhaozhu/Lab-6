/* Lab 6 Part D
   Name: Pengzhao Zhu
   Section#: 112D
   TA Name: Chris Crary
   Description: This program generates a 1760 Hz sine waveform using a look-up data of 256 data points.
				I will then measure the signal at the output using the DAD oscilloscope. 
   
 */



#include <avr/io.h>
#include <avr/interrupt.h>


void CLK_32MHZ(void);
void DAC(void);
void ADC(void);
void TIMER_INIT(void);

#define timer_freq ((32000000)*(1/450560))

//#define timer_freq ((32000000)*.1)/1024
//double decimal (1/901120);
//double timer=((32000000)*decimal);

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
	
	//  int arr[100]={1,2,3,4,5};
	//int size = sizeof(arr)/sizeof(arr[0]);
	// to find number of elements in an array
	
	
	PORTA_DIRSET=0x04;   //set PA2 as DAC0 output
	
	while(1) {
		
	for (int i=0; i< 256;i++) {      //go through the 512 samples
		while((TCC0_INTFLAGS & 0x01) != 0x01);   //wait for interrupt flag of sample rate to be set
		TCC0_INTFLAGS=0x01;   //clears the interrupt flag
		
		DACA_CH0DATA=Table[i];   //DAC output value according to the formula
		
		TCC0_CNT=0x00;    //reset TCC0_CNT to 0
		}
	
	}
	
	
	return 0;
}

void DAC(void) {
	DACA_CTRLA= DAC_ENABLE_bm | DAC_CH0EN_bm ;        //enable DAC, enable channel 0 output
	DACA_CTRLB=DAC_CHSEL_SINGLE_gc;   //single-channel operation on channel 0
	DACA_CTRLC=DAC_REFSEL_AREFB_gc;  //AREF on PORTB as reference
	
}

void TIMER_INIT(void) {
	
	TCC0_CNT=0x0000;   //set CNT to zero
	TCC0_PER=54;    //timer per value to output 1760 Hz sine wave
	TCC0_CTRLA=TC_CLKSEL_DIV1_gc; //timer prescaler of 1
	//TCC0_CTRLA=TC_CLKSEL_DIV1024_gc;
	
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
