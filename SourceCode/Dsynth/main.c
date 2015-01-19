#include "p18f4520.h"
#include <adc.h> 


//global options, read from ADC. used in ISR generated patterns.
unsigned char opt_blue,opt_green,opt_red,opt_white,optx,slider1,slider2,trigger;


//duty cycle of PWM output. ie, sound wave amplitude
#define  DUTY(data)               \
	        CCPR1L  = data >> 2; \
	        CCP1CON =   ((data & 0b00000011)<<4) | 0b00001100;
	        


unsigned int safesub(unsigned int a, unsigned int b){
if(b>a)
  return 0;
return a-b;	
}

unsigned short  abs(unsigned short a, unsigned short b){
if(b>a)
  return b-a;
return a-b;	
}


unsigned char SafeByte(unsigned int input){
   if(input > 255)
      return 255;
      return input;	
	
}
	
	
/* definitions of control knobs*/
	
#define SLIDE1 ADC_CH0
#define SLIDE2 ADC_CH1


#define OPTION_BLUE ADC_CH3//BLUE
#define OPTION_GREEN ADC_CH5//GREEN
#define OPTION_RED ADC_CH6//RED
#define OPTION_WHITE ADC_CH7//WHITE
#define OPTIONX ADC_CH4


unsigned char loadADCchannel(unsigned char channel){
	   SetChanADC (channel);
   //	   Delay10TCYx(60);
       ConvertADC ();	// Start an A/D conversion				
       while(BusyADC()){	// Wait for the conversion to complete
       //maybe some code for while were waiting (could turn an led on maybe)
        }
     //   Delay10TCYx(60);
        return  (unsigned char)(ReadADC()>>2);
}
	




#define PLEFT1 PORTDbits.RD5 //GROUPS OF 4
#define PLEFT2 PORTDbits.RD6 //controlls 5-8
#define PLEFT3 PORTDbits.RD7
#define PLEFT4 PORTDbits.RD0
#define PLEFT5 PORTDbits.RD1

#define PRIGHT1  PORTDbits.RD4 
#define PRIGHT2  PORTCbits.RC4 //POINT WITHIN GROUP OF FOUR
#define PRIGHT3  PORTDbits.RD3
#define PRIGHT4  PORTDbits.RD2


#define LLEFT1 LATDbits.LATD5 //GROUPS OF 4
#define LLEFT2 LATDbits.LATD6 //controlls 5-8
#define LLEFT3 LATDbits.LATD7
#define LLEFT4 LATDbits.LATD0
#define LLEFT5 LATDbits.LATD1

#define LRIGHT1  LATDbits.LATD4 
#define LRIGHT2  LATCbits.LATC4 //POINT WITHIN GROUP OF FOUR
#define LRIGHT3  LATDbits.LATD3
#define LRIGHT4  LATDbits.LATD2

//              TRISDbits.TRISD7 = 0;
#define TLEFT1 TRISDbits.TRISD5  //GROUPS OF 4
#define TLEFT2 TRISDbits.TRISD6 //controlls 5-8
#define TLEFT3 TRISDbits.TRISD7
#define TLEFT4 TRISDbits.TRISD0
#define TLEFT5 TRISDbits.TRISD1

#define TRIGHT1  TRISDbits.TRISD4 
#define TRIGHT2  TRISCbits.TRISC4 //POINT WITHIN GROUP OF FOUR
#define TRIGHT3  TRISDbits.TRISD3
#define TRIGHT4  TRISDbits.TRISD2



#define LEFT1_ON 0b00111100
#define LEFT2_ON 0b01011100
#define LEFT3_ON 0b10011100
#define LEFT4_ON 0b00011101
#define LEFT5_ON 0b00011110







#define TRIS_LEFT1_ON 0b11000011
#define TRIS_LEFT2_ON 0b10100011
#define TRIS_LEFT3_ON 0b01100011
#define TRIS_LEFT4_ON 0b11100010
#define TRIS_LEFT5_ON 0b11100001




#define RIGHT1_ON 0b11101111
#define RIGHT2_ON 0b11111111 //CONTROLLED BY rc4
#define RIGHT3_ON 0b11110111
#define RIGHT4_ON 0b11111011






#define INPUT 1
#define OUTPUT 0

#define HIGH 1
#define LOW 0

//give index. get back if that switch is closed.
unsigned char GetSwitchValue(unsigned char index){
//	unsigned char output;
	
	//set everything to input
/*
	TLEFT1 =  INPUT;
	TLEFT2 =  INPUT;
	TLEFT3 =  INPUT;
	TLEFT4 =  INPUT;
	TLEFT5 =  INPUT;

	TRIGHT1 =  INPUT;
	TRIGHT2 =  INPUT;
	TRIGHT3 =  INPUT;
	TRIGHT4 =  INPUT;
*/
	TRISD = 0xFF;
	TRISC = TRISC | 0b00010000;
	
	/*

	//set everything to 0. //something gets through the Z it looks like
	LLEFT1 = 0;
	LLEFT2 = 0;
	LLEFT3 = 0;
	LLEFT4 = 0;
	LLEFT5 = 0;

	LRIGHT1 = 0;
	LRIGHT2 = 0;
	LRIGHT3 = 0;
	LRIGHT4 = 0;
	*/
	
		
	
	LATD = 0;
	LATC = LATC & 0b11101111;
	
	
	switch(index){ //Set the left side to 0
		case 0:
			TRIGHT1 = OUTPUT;
			LRIGHT1 = HIGH;
			return PLEFT1;	
		break;
		case 1:
			TRIGHT2 = OUTPUT;
			LRIGHT2 = HIGH;
			return PLEFT1;
		break;
		case 2:
			TRIGHT3 = OUTPUT;
			LRIGHT3 = HIGH;
			return PLEFT1;
		break;
		case 3:
			TRIGHT4 = OUTPUT;
			LRIGHT4 = HIGH;
			return PLEFT1;
		break;
		
		//*************************************************//
		
		case 4:
			TRIGHT1 = OUTPUT;
			LRIGHT1 = HIGH;
			return PLEFT2;	
		break;
		case 5:
			TRIGHT2 = OUTPUT;
			LRIGHT2 = HIGH;
			return PLEFT2;
		break;
		case 6:
			TRIGHT3 = OUTPUT;
			LRIGHT3 = HIGH;
			return PLEFT2;
		break;
		case 7:
			TRIGHT4 = OUTPUT;
			LRIGHT4 = HIGH;
			return PLEFT2;
		break;
		
		
			//*************************************************//
			
		case 8:
			TRIGHT1 = OUTPUT;
			LRIGHT1 = HIGH;
			return PLEFT3;	
		break;
		case 9:
			TRIGHT2 = OUTPUT;
			LRIGHT2 = HIGH;
			return PLEFT3;
		
		break;
		case 10:
			TRIGHT3 = OUTPUT;
			LRIGHT3 = HIGH;
		
			return PLEFT3;
		break;
		case 11:
			TRIGHT4 = OUTPUT;
			LRIGHT4 = HIGH;
			
			return PLEFT3;
		break;
		
		
			//*************************************************//
			
		case 12:
			TRIGHT1 = OUTPUT;
			LRIGHT1 = HIGH;
			return PLEFT4;	
		break;
		case 13:
			TRIGHT2 = OUTPUT;
			LRIGHT2 = HIGH;
			return PLEFT4;
		break;
		case 14:
			TRIGHT3 = OUTPUT;
			LRIGHT3 = HIGH;
			return PLEFT4;
		break;
		case 15:
			TRIGHT4 = OUTPUT;
			LRIGHT4 = HIGH;
			return PLEFT4;
		break;
		
			//*************************************************//
		
		case 16:
			TRIGHT1 = OUTPUT;
			LRIGHT1 = HIGH;
			return PLEFT5;	
		break;
		case 17:
			TRIGHT2 = OUTPUT;
			LRIGHT2 = HIGH;
			return PLEFT5;
		break;
		case 18:
			TRIGHT3 = OUTPUT;
			LRIGHT3 = HIGH;
			return  PLEFT5;
		break;
		case 19:
			TRIGHT4 = OUTPUT;
			LRIGHT4 = HIGH;
			return PLEFT5;
		break;
		

	}


	
	
}	





void SetAllWhite(){
		//default all lefts 0 and all rights 1
	TRISD = 0;//don't allow any of portd as output
	TRISC = 0;
	
	PORTD = 0b00011100;
	PORTC = PORTC & 0b11101111;
}



//give index. That LED goes pink. The rest go black
void SetPink(unsigned char index){
	unsigned char output;
	
	TRISD = 0b11111111;//don't allow any of portd as output
	TRISC = 0b00000000;
	PORTD = 0;
	//default portd state
	//lefts
	//PORTD = 0bxxx000xx;	
	//rights
	//PORTD = 0b000yyy00
	
	//default all lefts 0 and all rights 1
	//PORTD = 0b00011100;
	

	
	PORTC = PORTC | 0b00010000;
	
	// 1 set portd and trisd to turn on LED
	// 2 set trisd for inpuy
	// if led is connected, input should have discharged to ground, otherwise it should still be high 

	
	switch(index){ //Set the left side to 0
		case 0: //0-3
		
			PORTD = LEFT1_ON & RIGHT1_ON;
			TRISD =  TRIS_LEFT1_ON;
				
		break;
		case 1:
			
			PORTD = LEFT1_ON & RIGHT2_ON;
			PORTC = PORTC & 0b11101111;
			TRISD =  TRIS_LEFT1_ON;
			
		break;
		case 2:
			
			PORTD = LEFT1_ON & RIGHT3_ON;
			TRISD =  TRIS_LEFT1_ON;
		break;
		case 3:
			
			PORTD = LEFT1_ON & RIGHT4_ON;
			TRISD =  TRIS_LEFT1_ON;
		break;
		
		
		case 4:
			
			PORTD = LEFT2_ON & RIGHT1_ON;
			TRISD =  TRIS_LEFT2_ON;
		break;
		case 5:
		
			PORTD = LEFT2_ON & RIGHT2_ON;
			PORTC = PORTC & 0b11101111;
			TRISD =  TRIS_LEFT2_ON;
		break;
		case 6:
			
			PORTD = LEFT2_ON & RIGHT3_ON;
			TRISD =  TRIS_LEFT2_ON;
		break;
		case 7:
			
			PORTD = LEFT2_ON & RIGHT4_ON;
			TRISD =  TRIS_LEFT2_ON;
		break;
		case 8:
			
			PORTD = LEFT3_ON & RIGHT1_ON;
			TRISD =  TRIS_LEFT3_ON;
		break;
		case 9:
		
			PORTD = LEFT3_ON & RIGHT2_ON;
			PORTC = PORTC & 0b11101111;
			TRISD =  TRIS_LEFT3_ON;
		break;
		case 10:
			
			PORTD = LEFT3_ON & RIGHT3_ON;
			TRISD =  TRIS_LEFT3_ON;
		break;
		case 11:
			
			PORTD = LEFT3_ON & RIGHT4_ON;
			TRISD =  TRIS_LEFT3_ON;
		break;
		case 12:
			
			PORTD = LEFT4_ON & RIGHT1_ON;
			TRISD =  TRIS_LEFT4_ON;
		break;
		case 13:
			
			PORTD = LEFT4_ON & RIGHT2_ON;
			PORTC = PORTC & 0b11101111;
			TRISD =  TRIS_LEFT4_ON;
		break;
		case 14:
			
			PORTD = LEFT4_ON & RIGHT3_ON;
			TRISD =  TRIS_LEFT4_ON;
		break;
		case 15:
			
			PORTD = LEFT4_ON & RIGHT4_ON;
			TRISD =  TRIS_LEFT4_ON;
		break;
		case 16:
			
			PORTD = LEFT5_ON & RIGHT1_ON;
			TRISD =  TRIS_LEFT5_ON;
		break;
		case 17:
			
			PORTD = LEFT5_ON & RIGHT2_ON;
			PORTC = PORTC & 0b11101111;
			TRISD =  TRIS_LEFT5_ON;
		break;
		case 18:
			
			PORTD = LEFT5_ON & RIGHT3_ON;
			TRISD =  TRIS_LEFT5_ON;
		break;
		case 19:
			
			PORTD = LEFT5_ON & RIGHT4_ON;
			TRISD =  TRIS_LEFT5_ON;
		break;
	}	
}	



/*
#define OPTION1 ADC_CH3//BLUE
#define OPTION2 ADC_CH5//GREEN
#define OPTION3 ADC_CH6//RED
#define OPTION4 ADC_CH7//WHITE
*/





    
    

unsigned char Pattern1(){
	static  unsigned char  Volume;
	static unsigned short counter;
	static unsigned char Stage;
	static unsigned short SquareCounter1,SquareCounter2;
	static unsigned char output;
	//static unsigned char SquareSpacer1,SquareSpacer2;
	//static unsigned char Attenuator;
//	static unsigned int Longcount;
	
	if(trigger){
			Volume = opt_blue;
		//	Stage = 0;
			counter = 0;
			output = 0;
			SquareCounter1 = 0;
			SquareCounter2 = 0;
			
			trigger = 0;
	} else if(Volume  == 0){
		return 0;
	}	

	
	SquareCounter1 +=3;

	if(SquareCounter1 > (Volume & opt_red)){
		SquareCounter1 = 0;
		output  = !output;
		//output =!output;
	}
	
	
	SquareCounter2 +=5;

	if(SquareCounter2 > (unsigned short)( (opt_green & Volume) )){
		SquareCounter2 = 0;
		output  = !output;
	//	output =!output;
	}




	counter+=5;
	if(counter > (unsigned short)opt_white + (unsigned short)50){
		counter = 0;
		Volume --;
	}

if(output)
	return Volume;
return 0;
}
	
	
	
unsigned char Pattern8(){
	static unsigned short LowTime;
	static unsigned short HighTime;
	static unsigned short counter;
	static unsigned short lhcounter;
	static unsigned char output;
	static unsigned char val2;
	
	if(trigger){
		LowTime = 	((unsigned short)opt_blue*2);
		HighTime = (512-((unsigned short)opt_blue*2));
		val2=0;
		counter  = 0;
		lhcounter = 0;
		output = 0;
		trigger = 0;
	}	else if(HighTime == 0)
		return 0;
	
	counter+= 16-(opt_white>>4);
	if(counter > (HighTime)){
		counter -= LowTime;
		LowTime++;
		HighTime--;
		val2+=(opt_red);
	}
	
	lhcounter+=(opt_green>>4);
	if(output){
		if(lhcounter > HighTime){
			output = 0;
			lhcounter  -=HighTime;	
		}	
	}  else {
		if (lhcounter > LowTime){
			output = 1;
			lhcounter  -= LowTime;
		}	
	}
	
	if(output){
		return val2&(HighTime/2);
		
	}	
	return 0;
	
}	
		
unsigned char Pattern7(){
	static  unsigned char  MainCount;
	static unsigned short TriCounter1,TriCounter2;
	static unsigned short count;
	static unsigned char output;
	
	if(trigger){
			MainCount = opt_blue;
			count  = 0;
			trigger = 0;
	} else if(MainCount <= 255-opt_green) {
		return 0;
	}
		
	count+=5;
	if(count > opt_white){
		count =0;
		MainCount--;
	}

	
//	TriCounter1 +=2;
//	if(TriCounter1 > (opt_green )){
//		TriCounter1 = 0;
	//	output = !output;
//	}	
		
	TriCounter2 +=2;
	if(TriCounter2 > (opt_red & MainCount)){
	//	TriCounter1 = 0;
		TriCounter2  =0;
		output = !output;
	}	
	
//	if(TriCounter2 > opt_red>>1 |  TriCounter1 > opt_green>>1)
	if(output)
		return MainCount;
	return 0;
	
//	return 0;
	

	}
	
	
	
	
		
unsigned char Pattern6(){
	static  unsigned short  MainCount;
	static unsigned short TriCounter1,TriCounter2;
	static unsigned short count;
	
	if(trigger){
			MainCount = opt_blue;
			count  = 0;
			trigger = 0;
	}// else if(MainCount  == 900){
	//	return 0;
	//}	
	
	count+=5;
	if(count > opt_white){
		count -=opt_white;
		MainCount++;
	}
	if(MainCount >= 400)
		return 0;
	
	TriCounter1 +=2;
	if(TriCounter1 > (opt_green))
		TriCounter1 -= opt_green;
		
	TriCounter2 +=2;
	if(TriCounter2 > (opt_red)){
		TriCounter1 = 0;
		TriCounter2  -=opt_red;
		
	}	
	
//	if(TriCounter2 > opt_red>>1 |  TriCounter1 > opt_green>>1)
		return ((unsigned short)(TriCounter1+TriCounter2)<<6)/MainCount;
//	return 0;
	

	}
	
	
	
unsigned char Pattern5(){
	static  unsigned short  MainCount;
	static unsigned short TriCounter1,TriCounter2;
	static unsigned short count;
	
	if(trigger){
			MainCount = opt_blue;
			count  = 0;
			trigger = 0;
	}// else if(MainCount  == 900){
	//	return 0;
	//}	
	
	count+=4;
	if(count > opt_white){
		count -=opt_white;
		MainCount++;
	}
	
	TriCounter1 +=2;
	if(TriCounter1 > (opt_green))
		TriCounter1 -= opt_green;
		
	TriCounter2 +=2;
	if(TriCounter2 > (opt_red))
		TriCounter2  -=opt_red;
	
//	if(TriCounter2 > opt_red>>1 |  TriCounter1 > opt_green>>1)
		return abs(MainCount,TriCounter1+TriCounter2);
//	return 0;
	

	}
	
unsigned char Pattern4(){
	static  unsigned short  MainCount;
	static unsigned short TriCounter1,TriCounter2;
	static unsigned short count;
	
	if(trigger){
			MainCount = opt_blue;
			count  = 0;
			trigger = 0;
	} else if(MainCount  == 512){
		return 0;
	}	
	
	count+=25;
	if(count > opt_white){
		count -=opt_white;
		MainCount++;
	}
	
	TriCounter1 +=33-(MainCount>>4);
	if(TriCounter1 > (opt_green)*2)
		TriCounter1 = 0;
		
	TriCounter2 +=33-(MainCount>>4);
	if(TriCounter2 > (opt_red)*2)
		TriCounter2 = 0;
	
	return (abs(TriCounter1,opt_green) + abs(TriCounter2,opt_red))/2;

	

	}
	
	
	
	
	
unsigned char Pattern3(){
	static  unsigned short  MainCount;
	static unsigned short TriCounter1,TriCounter2;
	static unsigned char count;
	
	if(trigger){
			MainCount = opt_blue;
			count  = 0;
			trigger = 0;
	} else if(MainCount  == 512){
		return 0;
	}	
	
	count+=30;
	if(count > opt_white){
		count = 0;
		MainCount++;
	}
	
	TriCounter1 ++;
	if(TriCounter1 > (opt_green))
		TriCounter1 = 0;
		
	TriCounter2 ++;
	if(TriCounter2 > (opt_red))
		TriCounter2 = 0;
	
	return (abs(MainCount,255) * ((unsigned short)TriCounter1 + (unsigned short)TriCounter2))>>4;

	

	}
	
    

unsigned char Pattern2(){
	static  unsigned char  Volume;
	static unsigned short counter;
	static unsigned char Stage;
	static unsigned short SquareCounter1,SquareCounter2;
	static signed short output;
	//static unsigned char SquareSpacer1,SquareSpacer2;
	//static unsigned char Attenuator;
//	static unsigned int Longcount;
	
	if(trigger){
			Volume = opt_blue;
			Stage = 0;
			counter = 0;
			output = 0;
			SquareCounter1 = 0;

			
			trigger = 0;
	} else if(Volume  == 0){
		return 0;
	}	

	
//	SquareCounter1 +=30;

//	if(SquareCounter1 > (opt_red)){
	//	SquareCounter1 -=;
		output  -= 100>>1+ (Volume/opt_green);
		if(output < -opt_red)
			output = 255;
		
		
//		output  -= opt_red>>4;
		
		//output =!output;
//	}
	





	counter++;//=10;
	if(counter > (unsigned short)opt_white){
	//	if(Stage == 0){
	//		Volume ++;
	//		if(Volume == 255)
	//			Stage = 1;
	//	} else {
			Volume =((unsigned short)Volume*(unsigned short)31)/32;
	//	}
			
		counter = 0;
		
	}

	if(output < 0)
		return Volume;
		
	if(output < Volume)
		return output;
//	return Volume ^ output;
	return (Volume & output);
//return 0;
}
	
	
	
	
#define CONTROLPOD_output PORTAbits.RA4


#pragma code
#pragma interrupt InterruptHandlerHigh

void InterruptHandlerHigh()
{
	unsigned char result;
	static unsigned char shiftIndexCount,index,oldresult,xtimer=0;
	//static unsigned char holdem = 0;
	
 if (INTCONbits.TMR0IF)
 {
	   WriteTimer0(0xFFA0); //2485 too slow. 2475 too fast
	    INTCONbits.TMR0IF=0;         //clear interrupt flag
	   // if(index %2 )
	  //  result = Pattern2();
	  //  else
	    result = Pattern8();
	   // holdem = (holdem*7 + result)/8;
	    CONTROLPOD_output = result > oldresult;
	    oldresult = result;
		DUTY(result);
		
		
		xtimer ++;
		if(xtimer %8)
		
			SetPink(index);
		else
			SetAllWhite();
		
			
	//	if(xtimer == 1){
	//		if(shiftIndexCount < 20) //only turn on pink for first steps of cycle
	//			SetPink(index);
	//		else
	//			SetAllWhite();//all black
	//	} else if (xtimer == 50){
		//	SetAllWhite();
		 if (xtimer == 51){
			xtimer = 0;
		//		SetPink(25);//all black
			shiftIndexCount++;
		

				
			
				shiftIndexCount++;
				if(shiftIndexCount > ((unsigned short)slider1*4)+20){
					shiftIndexCount = 0;
					index++;
					if(index >= 20)
						index = 0;
					
						
				}	else if (shiftIndexCount == 20){
						trigger = GetSwitchValue(index);
							
				}


  
 }
}
}


#pragma code InterruptVectorHigh = 0x08
void IntereuptVectorHigh(void)
{
_asm
	goto InterruptHandlerHigh
_endasm	
}


#pragma code

void main(void){


	unsigned char level,index = 0,CycleDecider;
	unsigned short shiftIndexCount = 0;
    
    TRISE = 0B00000111;
   	TRISD = 0;
	TRISC = 0;
	TRISB = 0b00000000;
    TRISA = 0b11101111;
    PORTD = 0;
	PORTC = 0;
	PORTB = 0b00000000;
	PORTA = 0b00111111;
	PORTE = 0;
	

	



  
  	OSCCON  = 0b01110100;
  	
  OSCTUNE = 0b10001111; 
  


	
	
	 OpenADC( ADC_FOSC_16 & // A/D clock source set to 32Tosc
   ADC_RIGHT_JUST& // write the Digital result(10bits) from right, into ADRESH:ADRESL(16bits).
   ADC_12_TAD, // A/D acquisition time: 20TAD (for 10bit conversion at least12TAD)
   ADC_CH4 & //choose input channel (11), AN11
   ADC_INT_OFF& //ADC interrupt off
   ADC_VREFPLUS_VDD& // choose the supply voltage VDD as reference voltage, V+
   ADC_VREFMINUS_VSS, // choose the supply voltage VSS as reference voltage, V-
    ADC_8ANA  // an0 input, all others digital
);
  
  
 
 PR2 = 0x3f;//0b01111100 ;
 //PWM is  156.25 kHz
 //8 bit resolution
 
T2CON = 0b00000100;
//enable timer 2. Prescaler is last two bits. 00 = Prescale 1.

CCPR1L = 0b00111110;
CCP1CON = 0b00101111;
//set for PWM
//PWM Period = [(PR2) + 1] • 4 • TOSC • (TMR2 Prescale Value)



SSPCON1 = 0;


 //init_timers();
	
	   WriteTimer0(0);

    INTCON=0x20;
    INTCON2=0x04;
    RCONbits.IPEN = 1;        //enable high interrupts
   
  
   T0CON=0x82; //prescaler 1:8
   INTCONbits.GIEH = 1;
   
	while(1){

			CycleDecider++;
		switch(CycleDecider){ //non- time critical routines
			case 1:
				opt_blue = loadADCchannel(OPTION_BLUE);
			break;
			case 2:
				opt_green = loadADCchannel(OPTION_GREEN);
			break;
			case 3:
				opt_red = loadADCchannel(OPTION_RED);
			break;
			case 4:
				opt_white = loadADCchannel(OPTION_WHITE);
			break;
			case 5:
				optx = loadADCchannel(OPTIONX);
			break;
			case 6:
				slider1 = loadADCchannel(SLIDE1);
			break;
			case 7:
				slider2 = loadADCchannel(SLIDE2);
			
				CycleDecider = 0;
			break;
		}
		
		
	
	
			
//	level = Pattern1(returnvalue,opt1,opt2,opt3,opt4);

		
//	DUTY(level);

//DUTY(Pattern1(returnvalue,opt1,opt2,opt3,opt4));

//	returnvalue = 0;

 

	}//while 1

}//main

