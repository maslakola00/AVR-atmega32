#include <avr/io.h> //zdefiniowanie portow
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

//7SEG DISPLAY
#define INIT_SEG_A      DDRA |= (1<<PA0)//ustawienie jako wyjscie
#define ON_SEG_A      PORTA |=(1<<PA0)
#define OFF_SEG_A    PORTA &= ~(1<<PA0)

#define INIT_SEG_B   DDRA |= (1<<PA1)//ustawienie jako wyjscie
#define ON_SEG_B       PORTA |=(1<<PA1)
#define OFF_SEG_B    PORTA &= ~(1<<PA1)

#define INIT_SEG_C   DDRA |= (1<<PA2)//ustawienie jako wyjscie
#define ON_SEG_C       PORTA |=(1<<PA2)
#define OFF_SEG_C    PORTA &= ~(1<<PA2)

#define INIT_SEG_D   DDRA |= (1<<PA3)//ustawienie jako wyjscie
#define ON_SEG_D       PORTA |=(1<<PA3)
#define OFF_SEG_D    PORTA &= ~(1<<PA3)

#define INIT_SEG_E   DDRA |= (1<<PA4)//ustawienie jako wyjscie
#define ON_SEG_E      PORTA |=(1<<PA4)
#define OFF_SEG_E    PORTA &= ~(1<<PA4)

#define INIT_SEG_F   DDRA |= (1<<PA5)//ustawienie jako wyjscie
#define ON_SEG_F       PORTA |=(1<<PA5)
#define OFF_SEG_F    PORTA &= ~(1<<PA5)

#define INIT_SEG_G   DDRA |= (1<<PA6)//ustawienie jako wyjscie
#define ON_SEG_G       PORTA |=(1<<PA6)
#define OFF_SEG_G    PORTA &= ~(1<<PA6)

//LED OPERATIONS
#define INIT_LED_PLUS   DDRD |= (1<<PD7)//ustawienie jako wyjscie
#define ON_LED_PLUS      PORTD |=(1<<PD7)
#define OFF_LED_PLUS    PORTD &= ~(1<<PD7)

#define INIT_LED_MINUS   DDRC |= (1<<PC0)//ustawienie jako wyjscie
#define ON_LED_MINUS      PORTC |=(1<<PC0)
#define OFF_LED_MINUS    PORTC &= ~(1<<PC0)

#define INIT_LED_MULTI  DDRC |= (1<<PC1)//ustawienie jako wyjscie
#define ON_LED_MULTI      PORTC |=(1<<PC1)
#define OFF_LED_MULTI    PORTC &= ~(1<<PC1)

#define INIT_LED_DIV  DDRC |= (1<<PC2)//ustawienie jako wyjscie
#define ON_LED_DIV      PORTC |=(1<<PC2)
#define OFF_LED_DIV    PORTC &= ~(1<<PC2)

#define INIT_LED_DIV  DDRC |= (1<<PC2)//ustawienie jako wyjscie
#define ON_LED_DIV      PORTC |=(1<<PC2)
#define OFF_LED_DIV    PORTC &= ~(1<<PC2)

#define INIT_LED_MOD  DDRC |= (1<<PC5)//ustawienie jako wyjscie
#define ON_LED_MOD      PORTC |=(1<<PC5)
#define OFF_LED_MOD    PORTC &= ~(1<<PC5)

//TRANSISTORS
#define INIT_TRAN1       DDRA |= (1<<PA7)//ustawienie jako wyjscie
#define ON_TRAN1     PORTA |=(1<<PA7)
#define OFF_TRAN1    PORTA &= ~(1<<PA7)

#define INIT_TRAN2       DDRC |= (1<<PC4)//ustawienie jako wyjscie
#define ON_TRAN2         PORTC |=(1<<PC4)
#define OFF_TRAN2        PORTC &= ~(1<<PC4)

#define INIT_TRAN3       DDRC |= (1<<PC3)//ustawienie jako wyjscie
#define ON_TRAN3         PORTC |=(1<<PC3)
#define OFF_TRAN3        PORTC &= ~(1<<PC3)


//SWITCHES - OUTPUT
//1
#define INIT_B0  DDRB |= (1<<PB0)//ustawienie jako wyjscie
#define ON_B0       PORTB |=(1<<PB0)
#define OFF_B0    PORTB &= ~(1<<PB0)
//2
#define INIT_B1  DDRB |= (1<<PB1)//ustawienie jako wyjscie
#define ON_B1       PORTB |=(1<<PB1)
#define OFF_B1    PORTB &= ~(1<<PB1)
//3
#define INIT_B2  DDRB |= (1<<PB2)//ustawienie jako wyjscie
#define ON_B2       PORTB |=(1<<PB2)
#define OFF_B2    PORTB &= ~(1<<PB2)


//SWITCHES - PULLUP
//1
#define INIT_PULLUP_B3 DDRB &= ~(1<<PB3) //wejscie
#define SW_CONFIG_PULLUP_B3 PORTB |= (1<<PB3) //wewnetrzny pullup
//2
#define INIT_PULLUP_B7 DDRB &= ~(1<<PB7) //wejscie
#define SW_CONFIG_PULLUP_B7 PORTB |= (1<<PB7) //wewnetrzny pullup
//3
#define INIT_PULLUP_D2 DDRD &= ~(1<<PD2) //wejscie
#define SW_CONFIG_PULLUP_D2 PORTD |= (1<<PD2) //wewnetrzny pullup
//4
#define INIT_PULLUP_D5 DDRD &= ~(1<<PD5) //wejscie
#define SW_CONFIG_PULLUP_D5 PORTD |= (1<<PD5) //wewnetrzny pullup
//5
#define INIT_PULLUP_D6 DDRD &= ~(1<<PD6) //wejscie
#define SW_CONFIG_PULLUP_D6 PORTD |= (1<<PD6) //wewnetrzny pullup

//VARIABLES
volatile uint8_t transistor=1;
uint8_t off_number=0, on_number=0;
int counter=0;
uint8_t tab[3]={0,0,0};
int number1;
int number2;
int change = 0;
int i=0; // do operacji jaka wykonujemy 
int score;

ISR(TIMER2_OVF_vect){

	TCNT2=6;
	if(transistor>3){ 
		transistor = 1;
	}
	else {
		transistor++;	
	}
}


void init(void){

//DISPLAY
    INIT_SEG_A;
	INIT_SEG_B;
	INIT_SEG_C;
	INIT_SEG_D;
	INIT_SEG_E;
	INIT_SEG_F;
	INIT_SEG_G;

	OFF_SEG_A;
	OFF_SEG_B;
	OFF_SEG_C;
	OFF_SEG_D;
	OFF_SEG_E;
	OFF_SEG_F;
	OFF_SEG_G;

//TRANSISTORS
    INIT_TRAN1;
    INIT_TRAN2;
    INIT_TRAN3;

    OFF_TRAN1; 
    OFF_TRAN2; 
    OFF_TRAN3; 

//LED
    INIT_LED_PLUS;
    INIT_LED_MINUS;
    INIT_LED_MULTI;
    INIT_LED_DIV;
    INIT_LED_MOD;

    OFF_LED_PLUS; 
    OFF_LED_MINUS;
    OFF_LED_MULTI;
    OFF_LED_DIV;
    OFF_LED_MOD;   

//SWITCHES OUTPUT
    INIT_B0;
    INIT_B1;
    INIT_B2;

    ON_B0;
    ON_B1;
    ON_B2;

//SWITCHES PULLUP
    INIT_PULLUP_B3;
    INIT_PULLUP_B7;
    INIT_PULLUP_D2;
    INIT_PULLUP_D5;
    INIT_PULLUP_D6;

    SW_CONFIG_PULLUP_B3;
    SW_CONFIG_PULLUP_B7;
    SW_CONFIG_PULLUP_D2;
    SW_CONFIG_PULLUP_D5;
    SW_CONFIG_PULLUP_D6;


}

void select_led(int led){

    switch(led){

    case 6:
        ON_LED_PLUS;
        OFF_LED_MINUS;
        OFF_LED_MULTI;
        OFF_LED_DIV;
        OFF_LED_MOD;
        break;
    

    case 7:
        ON_LED_MINUS;
        OFF_LED_PLUS;
        OFF_LED_MULTI;
        OFF_LED_DIV;
        OFF_LED_MOD;
        break;
    

    case 10:
        ON_LED_MULTI;
        OFF_LED_MINUS;
        OFF_LED_PLUS;
        OFF_LED_DIV;
        OFF_LED_MOD;
        break;
    
      case 11:
        ON_LED_DIV;
        OFF_LED_MINUS;
        OFF_LED_MULTI;
        OFF_LED_PLUS;
        OFF_LED_MOD;
        break;
     
     
      case 12:
        ON_LED_MOD;
        OFF_LED_MINUS;
        OFF_LED_MULTI;
        OFF_LED_PLUS;
        OFF_LED_DIV;
        break;


    }
}

void display_number(int on){


	switch(on){
	
	case 0:
	OFF_SEG_G;
	ON_SEG_A;
	ON_SEG_B;
	ON_SEG_C;
	ON_SEG_D;
	ON_SEG_E;
	ON_SEG_F;
	break;
	
	case 1:
	OFF_SEG_A;
	OFF_SEG_D;
	OFF_SEG_E;
	OFF_SEG_F;
	OFF_SEG_G;
	ON_SEG_B;
	ON_SEG_C;
	break;
	
	case 2:
	OFF_SEG_F;
	OFF_SEG_C;
	ON_SEG_A;
	ON_SEG_B;
	ON_SEG_G;
	ON_SEG_D;
	ON_SEG_E;
	break;
	
	
	case 3:
	OFF_SEG_F;
	OFF_SEG_E;
	ON_SEG_A;
	ON_SEG_B;
	ON_SEG_C;
	ON_SEG_D;
	ON_SEG_G;
	break;
	
	case 4:
	OFF_SEG_A;
	OFF_SEG_D;
	OFF_SEG_E;
	ON_SEG_B;
	ON_SEG_C;
	ON_SEG_F;
	ON_SEG_G;
	break;
	
	
	case 5:
	OFF_SEG_B;
	OFF_SEG_E;
	ON_SEG_A;
	ON_SEG_F;
	ON_SEG_G;
	ON_SEG_C;
	ON_SEG_D;
	break;
	
	case 6:
	OFF_SEG_B;
	ON_SEG_A;
	ON_SEG_F;
	ON_SEG_G;
	ON_SEG_C;
	ON_SEG_D;
	ON_SEG_E;
	break;

    case 7:
	OFF_SEG_F;
	OFF_SEG_G;
	OFF_SEG_E;
	OFF_SEG_D;
	ON_SEG_A;
	ON_SEG_B;
	ON_SEG_C;
	break;
	
	case 8:
	ON_SEG_A;
	ON_SEG_B;
	ON_SEG_C;
	ON_SEG_D;
	ON_SEG_E;
	ON_SEG_F;
	ON_SEG_G;
	break;
	
	case 9:
	OFF_SEG_E;
	ON_SEG_A;
	ON_SEG_B;
	ON_SEG_C;
	ON_SEG_D;
	ON_SEG_F;
	ON_SEG_G;
	break;
	
	
    }
}

void select_transistor(int number){

switch(number){

	case 1:

    ON_TRAN1;
    OFF_TRAN2;
    OFF_TRAN3;
	break;
	
	case 2:
    ON_TRAN2;
    OFF_TRAN1;
    OFF_TRAN3;

	break;
	
	case 3:
    ON_TRAN3;
    OFF_TRAN1;
    OFF_TRAN2;
	break;
	
    }

}

void output_off(int x){

    if(x==0) return OFF_B0;
    if(x==1) return OFF_B1;
    if(x==2) return OFF_B2;

}

void output_on(int x){

    if(x==0) return ON_B0;
    if(x==1) return ON_B1;
    if(x==2) return ON_B2;

}

void check_switches(){

 
// while(off_number<3){
 
   output_off(off_number);

     if(!(PINB &(1<<PB3))){  
        tab[counter]=off_number+1; 
           counter++; 
      }  

       if(!(PINB &(1<<PB7))){
         tab[counter]=off_number+4;     
            counter++; 
      }  

       if(!(PIND &(1<<PD2))){
          tab[counter]=off_number+7;    
             counter++;  
      } 
         

      if(!(PIND &(1<<PD5))){
          i=off_number+5;
          select_led(i);
            for(int j=0;j<3;j++){
                tab[j]=0;
            }
      }
       if(!(PIND &(1<<PD6))){
          i=off_number+10;
          select_led(i);
            for(int j=0;j<3;j++){
                tab[j]=0;
            }
      }

      if(counter>2){
         if(i!=0){
            number2=100*tab[0]+10*tab[1]+tab[2];
            score = number1+number2;
            for(int j=0;j<3;j++){
                tab[j]=0;
            }
            i=0;
          }
        
          if(i==0){
            number1=100*tab[0]+10*tab[1]+tab[2];
        }
            counter=0;

      }   

    output_on(off_number);
    off_number++;
 //}

}


int main(void){

        init();
        sei();

        TCCR2 |=((1<<CS22)| (1<<CS21));
        TCNT2 = 6;
        TIMSK |= (1<<TOIE2); //timer 8 bit 

     
       
    
        while(1){


            check_switches();
          
            display_number(tab[transistor-1]);
            select_transistor(transistor);
            

            change++;
            on_number++;
          
          // off_number=0;
          if(off_number==3){
              off_number=0;
          }
          
        }
        






    return 0;
}