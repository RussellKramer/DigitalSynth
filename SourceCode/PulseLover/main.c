#include "p18f2550.h"

#include <adc.h> 

	

	
unsigned int abs(unsigned int a, unsigned int b){
	if(b>a)
	   return b-a;
	else
	   return a-b;
}	
	
	
	
unsigned char loadADC(){
	unsigned short value;
	SetChanADC (ADC_CH0);
	   Delay10TCYx(20);
	   
       ConvertADC ();	// Start an A/D conversion				
       while(BusyADC()){	// Wait for the conversion to complete
       //maybe some code for while were waiting (could turn an led on maybe)
        }
        Delay10TCYx(20);
        
        value = ReadADC()/10.24f;
       
       return value;
}
	
	
	
 /* RED
     ---------------
          RB4
     ---------------
|    |              | RB5|
| RB1|              |    |
|    |              |    |
     ---------------
         RB0 
     ---------------
|    |              |    |
|RC0 |              |RA4 |
|    |              |    |
     ---------------
         RA5                   ------
     ---------------          | RA1  |
                               ------
*/
//turning on ra3 turns off ra5?

 /* GREEN
     ---------------
           RC6
     ---------------
|    |              |RC7 |
| RA2  |              |    |
|    |              |    |
TOP, SECOND FROM LEFT  TOP FAR LEFT
     ---------------
          RA3
     ---------------
|     |              |      |
|  RB2|              |  RC2 |
|     |              |      |
     ---------------
         RB3
     ---------------         ----
                           | RC1 |
                            -----
                           */ 


//assume input 0-99
void SetNumbers(unsigned char input, unsigned char ShowDot){
	
	unsigned char PA, PB, PC;
	unsigned char dright = input%10;
	unsigned char dleft = input/10;
		
	if(input > 99)
		input  = 99;

	


		
		
	switch(dright){
		case 0:
			PA = 0b00110000;
			PB = 0b00110010;
			PC = 0b00000001;
		break;
		
		case 1:
			PA = 0b00010000;
			PB = 0b00100000;
			PC = 0b00000000;
		break;
		
		case 2:
			PA = 0b00100000;
			PB = 0b00110001;
			PC = 0b00000001;
		break;
		
		case 3:
			PA = 0b00110000;
			PB = 0b00110001;
			PC = 0b00000000;
		break;
		
		case 4:
			PA = 0b00010000;
			PB = 0b00100011;
			PC = 0b00000000;
		break;
		
		case 5:
			PA = 0b00110000;
			PB = 0b00010011;
			PC = 0b00000000;
		break;
		
		case 6:
			PA = 0b00110000;
			PB = 0b00010011;
			PC = 0b00000001;
		break;
		
		case 7:
			PA = 0b00010000;
			PB = 0b00110000;
			PC = 0b00000000;
		break;
		
		case 8:
			PA = 0b00110000;
			PB = 0b00110011;
			PC = 0b00000001;
		break;
		
		case 9:
			PA = 0b00110000;
			PB = 0b00110011;
			PC = 0b00000000;
		break;
	}
		
	switch(dleft){ //green
		case 0:
			PA |= 0b00000000;
			PB |= 0b00000000;
			PC |= 0b00000000;
		break;
		case 1:
			PA |= 0b00000000;
			PB |= 0b00000000;
			PC |= 0b10000100;
		break;
		case 2:
			PA |= 0b00001000;
			PB |= 0b00001100;
			PC |= 0b11000000;
		break;
		case 3:
			PA |= 0b00001000;
			PB |= 0b00001000;
			PC |= 0b11000100;
		break;
		case 4:
			PA |= 0b00001100; //	PORTA = 0b00110000;
			PB |= 0b00000000;
			PC |= 0b10000100;
		break;
		case 5:
			PA |= 0b00001100;
			PB |= 0b00001000;
			PC |= 0b01000100;
		break;
		case 6:
			PA |= 0b00001100;
			PB |= 0b00001100;
			PC |= 0b01000100;
		break;
		case 7:
			PA |= 0b00000000;
			PB |= 0b00000000;
			PC |= 0b11000100;
		break;
		case 8:
			PA |= 0b00001100;
			PB |= 0b00001100;
			PC |= 0b11000100;
		break;
		case 9:
			PA |= 0b00001100;
			PB |= 0b00001000;
			PC |= 0b11000100;
		break;
	}
	if(ShowDot){
		PA |= 0b00000010;	
	}	
	PORTA = PA;
	PORTB = PB;
	PORTC = PC;
	
	
}
	
void main(void){
    unsigned char  ADCval,count=0,output=0;
	TRISC = 0;
	TRISB = 0b00000000;
    TRISA = 0b00000001;


	LATC = 0;
	LATB = 0;
	LATA = 0;
	
	
	PORTC = 0;
	PORTB = 0;
	PORTA = 0;

SSPCON1 = 0;
UCON = UCON & 0B11110111;
UCFG = UCFG | 0B00001000;

	   
    OpenADC( ADC_FOSC_8 & // A/D clock source set to 32Tosc
  ADC_RIGHT_JUST& // write the Digital result(10bits) from right, into ADRESH:ADRESL(16bits).
  ADC_20_TAD, // A/D acquisition time: 20TAD (for 10bit conversion at least12TAD)
  ADC_CH0 & //choose input channel (11), AN11
  ADC_INT_OFF& //ADC interrupt off
  ADC_VREFPLUS_VDD& // choose the supply voltage VDD as reference voltage, V+
  ADC_VREFMINUS_VSS, // choose the supply voltage VSS as reference voltage, V-
   0b00001110 // an0 input, all others digital
);




	while(1){
		ADCval++;// = loadADC();
		  Delay10TCYx(200);
		count ++;
		if(count >= 102){
			count = ADCval;
			output = !output;	
		}	
		
		SetNumbers(ADCval,output);
		
	}
}