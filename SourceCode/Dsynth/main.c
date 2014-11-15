#include "p18f4520.h"

//#pragma config WDT=OFF, LVP=OFF, DEBUG=OFF, MCLRE = ON
 
//Internal oscillator, port function on RA6, EC used by USB 
//#pragma config FOSC = INTOSCIO_EC //1MHz oscillation


#include <adc.h> 

#define INDICATOR PORTCbits.RC1

#define  DUTY(data)               \
	        CCPR1L  = data >> 2; \
	        CCP1CON =   ((data & 0b00000011)<<4) | 0b00001100;
	        
//10 bit value
//void SetDutyCycle(unsigned char input){
	//two lsbs in CCPXcon

//	CCP2CON = CCP2CON & 0b11001111;//clear bits
//	CCP1CON = 0b00001100 | ((input & 0b00000011) << 4);
	//0b00001100 puts it into PWM Mode
	
//	CCPR1L = input;
	// 8 msbs go in CCP1L 
	
//}	



unsigned int safesub(unsigned int a, unsigned int b){
if(b>a)
  return 0;
return a-b;	
}

unsigned char SafeByte(unsigned int input){
   if(input > 255)
      return 255;
      return input;	
	
}
	
	


unsigned int loadADCchannel(unsigned char channel){
	   SetChanADC (channel);
   	   Delay10TCYx(60);
       ConvertADC ();	// Start an A/D conversion				
       while(BusyADC()){	// Wait for the conversion to complete
       //maybe some code for while were waiting (could turn an led on maybe)
        }
        Delay10TCYx(60);
        return  ReadADC();
}
	


	



	




	
	

		


rom unsigned char coswave[400] = {
35, //0
35, //1
35, //2
35, //3
35, //4
35, //5
35, //6
35, //7
35, //8
35, //9
35, //10
35, //11
35, //12
35, //13
34, //14
34, //15
34, //16
34, //17
34, //18
34, //19
33, //20
33, //21
33, //22
33, //23
32, //24
32, //25
32, //26
32, //27
31, //28
31, //29
31, //30
31, //31
30, //32
30, //33
30, //34
29, //35
29, //36
29, //37
28, //38
28, //39
28, //40
27, //41
27, //42
26, //43
26, //44
26, //45
25, //46
25, //47
24, //48
24, //49
24, //50
23, //51
23, //52
22, //53
22, //54
21, //55
21, //56
21, //57
20, //58
20, //59
19, //60
19, //61
18, //62
18, //63
18, //64
17, //65
17, //66
16, //67
16, //68
15, //69
15, //70
14, //71
14, //72
14, //73
13, //74
13, //75
12, //76
12, //77
11, //78
11, //79
11, //80
10, //81
10, //82
9, //83
9, //84
9, //85
8, //86
8, //87
7, //88
7, //89
7, //90
6, //91
6, //92
6, //93
5, //94
5, //95
5, //96
4, //97
4, //98
4, //99
4, //100
3, //101
3, //102
3, //103
3, //104
2, //105
2, //106
2, //107
2, //108
1, //109
1, //110
1, //111
1, //112
1, //113
1, //114
0, //115
0, //116
0, //117
0, //118
0, //119
0, //120
0, //121
0, //122
0, //123
0, //124
0, //125
0, //126
0, //127
0, //128
0, //129
0, //130
0, //131
0, //132
0, //133
0, //134
0, //135
0, //136
0, //137
0, //138
0, //139
0, //140
0, //141
1, //142
1, //143
1, //144
1, //145
1, //146
1, //147
2, //148
2, //149
2, //150
2, //151
3, //152
3, //153
3, //154
3, //155
4, //156
4, //157
4, //158
4, //159
5, //160
5, //161
5, //162
6, //163
6, //164
6, //165
7, //166
7, //167
7, //168
8, //169
8, //170
9, //171
9, //172
9, //173
10, //174
10, //175
11, //176
11, //177
11, //178
12, //179
12, //180
13, //181
13, //182
14, //183
14, //184
14, //185
15, //186
15, //187
16, //188
16, //189
17, //190
17, //191
17, //192
18, //193
18, //194
19, //195
19, //196
20, //197
20, //198
21, //199
21, //200
21, //201
22, //202
22, //203
23, //204
23, //205
24, //206
24, //207
24, //208
25, //209
25, //210
26, //211
26, //212
26, //213
27, //214
27, //215
28, //216
28, //217
28, //218
29, //219
29, //220
29, //221
30, //222
30, //223
30, //224
31, //225
31, //226
31, //227
31, //228
32, //229
32, //230
32, //231
32, //232
33, //233
33, //234
33, //235
33, //236
34, //237
34, //238
34, //239
34, //240
34, //241
34, //242
35, //243
35, //244
35, //245
35, //246
35, //247
35, //248
35, //249
35, //250
35, //251
35, //252
35, //253
35, //254
35, //255
35, //256
};


#define LEFT1 PortDbits.RD5 //controlls  0-3
#define LEFT2 PortDbits.RD6 //controlls 5-8
#define LEFT3 PORTDbits.RD7
#define LEFT4 PORTDbits.RD0
#define LEFT5 PORTDbits.RD1

#define RIGHT1 PORTDbits.RD4
#define RIGHT2 PORTCbits.RC4
#define RIGHT3 PORTDbits.RD3
#define RIGHT4 PORTDbits.RD2


//led color values
#define RED 1
#define YELLOW 2
#define NONE 0
unsigned char SetSwitchLED(unsigned char index, unsigned char value){
	TRISD = 0b00000000;
	TRISC = 0b00000000;
	
	switch(index>>2){ //Set the left side to 0
		case 0: //0-3
			PORTD = 0b11000011; //LEFT1 PortDbits.RD5
		break;
		case 1: //4-7
			PORTD = 0b10100011; // LEFT2 PortDbits.RD6 
		break;
		case 2: //8-11
			PORTD = 0b01100011; //LEFT3 PORTDbits.RD7
		break;
		case 3: //12-15
			PORTD = 0b11100010; //PORTDbits.RD0
		break;
		case 4: //16-19
			PORTD = 0b11100001; //LEFT5 PORTDbits.RD1
		break;
	}
	
	//off
	//	PORTC = PORTC & 0b11101111;
	//	PORTD = PORTD | 0b00000000;
	
	//ALL ON
	PORTC = PORTC | 0b00010000;
	PORTD = PORTD | 0b00001100;
	
		//1 ON
//	PORTC = PORTC | 0b00010000;
//	PORTD = PORTD | 0b00011100;
	
	
	/*
	switch(index & 0b00000011){//set thwe right side to 1
		case 0: //RIGHT1 PORTDbits.RD4
				PORTC = PORTC & 0b11101111;
			
			PORTD = PORTD | 0b00010000;
		break;
		case 1: //RIGHT2 PORTCbits.RC4
			PORTC = PORTC | 0b00010000;
		
			//PORTD = PORTD | 0b00011100;
		break;
		case 2: //RIGHT3 PORTDbits.RD3
			PORTC = PORTC & 0b11101111;
			PORTD = PORTD | 0b00001000;
		break;
		case 3: //RIGHT4 PORTDbits.RD2
			PORTC = PORTC & 0b11101111;
			PORTD = PORTD | 0b00000100;
		
		break;
		
	}	
	
	*/
}	

//ask if a button is Closed
unsigned char GetSwitchValue(unsigned char index){
	
	
	
}	

unsigned char Pattern1(unsigned char trigger){
	static  unsigned short Volume;
	static unsigned char counter;
	static unsigned char Stage;
	static unsigned short SquareCounter1,SquareCounter2;
	static unsigned char output;
	static unsigned short SquareSpacer1,SquareSpacer2;
	static unsigned char Attenuator;
	static unsigned int Longcount;
	
	if(trigger){
			Volume = 1;
			Stage = 0;
			counter = 0;
			output = 0;
			SquareCounter1 = 0;
			SquareCounter2 = 0;
			
			SquareSpacer1 = 700;
			SquareSpacer2 = 100;
				
	
	}
	
	if(Volume < 1)
		return 0;

	
	SquareCounter1 ++;
	SquareCounter2 ++;
	if(SquareCounter1 >  SquareSpacer1/2){
		SquareCounter1 = 0;
		
			if(output == 0)
				output = 1;
			else
				output = 0;
				
	}
	
	if(SquareCounter2 >  SquareSpacer2/3){
		SquareCounter2 = 0;
		
			if(output == 0)
				output = 1;
			else
				output = 0;
				
	}		
	

	if(Stage == 0){
	
		counter ++;
		if(counter > 10){
				SquareSpacer1-=1;
			counter = 0;
			Volume ++;
			if(Volume > 254)
				Stage = 1;	
		}	
	} else {
		counter ++;
	
		if(counter > 100){
			counter = 0;
				SquareSpacer1+=29;
			Volume --;
		}	
	}
			
		if(output)
			return Volume;
		return 0;
}
	





/*

unsigned char Pattern1(unsigned char trigger){
	static  unsigned short Volume;
	static unsigned char counter;
	static unsigned char Stage;
	static unsigned short SquareCounter;
	static unsigned char output;
	static unsigned short SquareSpacer;
	
	if(trigger){
			Volume = 100;
			Stage = 0;
			counter = 0;
			output = 0;
			SquareCounter = 0;
			SquareSpacer = 100;
	}
	
	if(Volume  == 0)
		return 0;
	
	SquareCounter ++;
	if(SquareCounter >  SquareSpacer){
		SquareSpacer+=10;
		SquareCounter = 0;
			if(output == 0)
				output = 1;
			else
				output = 0;
				
	}			
	

	if(Stage == 0){
		counter ++;
		if(counter > 100){
			counter = 0;
			Volume +=1;
			if(Volume > 240)
				Stage = 1;	
		}	
	} else {
		counter ++;
		if(counter > 10){
			counter = 0;
			Volume = Volume - 1;
		}	
	}
			
		if(output)
			return Volume;
		return 0;
}
	

*/


/*

unsigned char Pattern1(unsigned char trigger){
	static  unsigned char counter1,counter2;
	static unsigned char output;
	if(trigger){
		counter1 = 20;
		counter2 = 100;
	}
	

	if(counter1 < 150){
		counter2++;
		if(counter2 > counter1){
			counter2=0;
			counter1++;	
		}	
	
	} else {

		output = 0;
			return 0;
	}
	
//if(output > 400)
//	output = 0;
	
	return (counter2 | counter1) % 255 ;	
	
}
	
	*/
	
 /*
unsigned char Pattern1(unsigned char trigger){
	static  unsigned char counter1,counter2;
	static unsigned char output;
	if(trigger){
		counter1 = 255;
		counter2 = 0;
	}
	
	
	if(counter1 > 0){
		counter2--;
		if(counter2 < counter1){
			counter2=0;
			counter1--;	
			output +=100;
		}	
	
	} else {
		output = 0;
	}
	
if(output > 400)
	output = 0;
	
	return output > 100 ? 255 : 0;	
	
}
	*/
	
	
void main(void){

    unsigned long coloumn, row,band,i,j,temp,counter,y,index;
    
    unsigned int a,b;
    

    
    

    
    TRISE = 0;
   	TRISD = 0;
	TRISC = 0;
	TRISB = 0b00000000;
    TRISA = 0b00111111;
    PORTD = 0;
	PORTC = 0;
	PORTB = 0b00000000;
	PORTA = 0b00111111;
	PORTE = 0;
	

	
	
 //  ADCON1= 0b00001001;
//   UCON &= 0b11111011;
//   UCFG =  0b00000100;


  
  //	OSCTUNE = 0b00001111; 
  
  	OSCCON  = 0b11110000;
  	
  OSCTUNE = 0b11001011; 
  


	
	
//	 OpenADC( ADC_FOSC_64 & // A/D clock source set to 32Tosc
 //  ADC_RIGHT_JUST& // write the Digital result(10bits) from right, into ADRESH:ADRESL(16bits).
 //  ADC_20_TAD, // A/D acquisition time: 20TAD (for 10bit conversion at least12TAD)
 //  ADC_CH0 & //choose input channel (11), AN11
 //  ADC_INT_OFF& //ADC interrupt off
 //  ADC_VREFPLUS_VDD& // choose the supply voltage VDD as reference voltage, V+
//   ADC_VREFMINUS_VSS, // choose the supply voltage VSS as reference voltage, V-
//    ADC_6ANA  // an0 input, all others digital
//);
  
  
 
 PR2 = 0x3f;//0b01111100 ;
 //PWM is  156.25 kHz
 //8 bit resolution
 
T2CON = 0b00000100;
//enable timer 2. Prescaler is last two bits. 00 = Prescale 1.

CCPR1L = 0b00111110;
CCP1CON = 0b00101111;
//set for PWM
//PWM Period = [(PR2) + 1] • 4 • TOSC • (TMR2 Prescale Value)




	i=0;
	j=0;
	
	while(1){

		i++;
		if(i >= 6000){
			i= 0;
			index++;
			if(index >= 20)
				index = 0;
			
		
		}	
			

			SetSwitchLED(index,1);	
			
		DUTY(Pattern1(i==0));//coswave[i]*6);


  		Delay100TCYx(2);

 

	}

}