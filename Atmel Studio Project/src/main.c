/*
 * main.c
 *
 * Created: 9/16/2018 2:43:15 PM
 * Created by : Mohamed Hassanin
 * 
 * Description: control sensored BLDC motor with openloop method
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "hall_sensor.h"
#include "main_open_loop.h"
#include "main.h"

int 
main(void)
{	     
  /* Peripherals initializations */
	GpioInit();
  ADC_init();
  PCINT0_init();
  Timer2_config();
  
  /* start the motor */
  Start_Motor();
	
	/* enable global interrupts  */
  sei();	
  
	while (1)
	{
		if(SampleADC == true)
		{
			Start_ADC_Conversation();
			SampleADC = false;
		}
	}
}   


void 
Start_Motor(void)
{
  Start_ADC_Conversation(); 
  
  Hall_IN = HALL_SENSORS_IDR & 0x07;  
  PreDriver_Sequence = Hall_DIR_sequence[Hall_IN];
  PWM_update(PreDriver_Sequence); 
  
  TCCR2B |= 1 << CS20 | 1 << CS21; //enable the timer with Prescaler = 32 
}

void 
Stop_Motor(void)
{
  cli(); //disable global interrupt
  TCCR2B &= ~(1 << CS20 | 1 << CS21);
  Motor_Status = Stopped;
}

void 
PWM_update (unsigned char Next_Hall_Sequence)
{
  switch(Next_Hall_Sequence)
		{
			case HS_W|LS_V:
				UL_ODR &= ~UL_PIN;
				UH_ODR &= ~UH_PIN;
				VH_ODR &= ~VH_PIN;
				WL_ODR &= ~WL_PIN;
				
				WH_ODR |= WH_PIN;
				VL_ODR |= VL_PIN;
			break;
    
			case HS_V|LS_U: 
				UH_ODR &= ~UH_PIN;
				VL_ODR &= ~VL_PIN;
				WH_ODR &= ~WH_PIN;
				WL_ODR &= ~WL_PIN;
				
				VH_ODR |= VH_PIN;
				UL_ODR |= UL_PIN;
			break;
    
			case HS_W|LS_U:
				UH_ODR &= ~UH_PIN;
				VL_ODR &= ~VL_PIN;
				VH_ODR &= ~VH_PIN;
				WL_ODR &= ~WL_PIN;
			
				WH_ODR |= WH_PIN;
				UL_ODR |= UL_PIN;
			break;
    
			case HS_U|LS_W:    
				UL_ODR &= ~UL_PIN;
				VL_ODR &= ~VL_PIN;
				VH_ODR &= ~VH_PIN;
				WH_ODR &= ~WH_PIN;
				
				UH_ODR |= UH_PIN;
				WL_ODR |= WL_PIN;
			break;
    
			case HS_U|LS_V:
				UL_ODR &= ~UL_PIN;
				VH_ODR &= ~VH_PIN;
				WL_ODR &= ~WL_PIN;
				WH_ODR &= ~WH_PIN;
				 
				UH_ODR |= UH_PIN;
				VL_ODR |= VL_PIN;
			break;
    
			case HS_V|LS_W:            
				UL_ODR &= ~UL_PIN;
				UH_ODR &= ~UH_PIN;
				VL_ODR &= ~VL_PIN;
				WH_ODR &= ~WH_PIN;
				
				VH_ODR |= VH_PIN;
				WL_ODR |= WL_PIN;
			break;
    
			default:
			break;
		}
}

void 
GpioInit(void)
{
	/* hall sensors input mode */
	HALL_SENSORS_MODER &= ~(HALL_U_PIN | HALL_V_PIN | HALL_W_PIN);
	
	/* gates signals output mode */
	UL_MODER |= UL_PIN;
	UH_MODER |= UH_PIN;
	VL_MODER |= VL_PIN;
	VH_MODER |= VH_PIN;
	WL_MODER |= WL_PIN;
	WH_MODER |= WH_PIN;
	
	/* PWM IOs */
	HS_PWM_MODER |= HS_PWM_PIN;
	
	LS_PWM_MODER |= LS_PWM_PIN;
	LS_PWM_ODR |= LS_PWM_PIN;
}

void 
PCINT0_init(void)
{
  PCICR |= 1 << PCIE0;
  PCMSK0 |= 1 << PCINT0 | 1 << PCINT1 | 1 << PCINT2;	
}

void 
ADC_init(void)
{
  ADMUX = 1 << REFS0; //AVCC with external capacitor at AREF pin
										   // MUX5:0 = 100000, ArduinoUno "A8"
	ADCSRB |= 1 << MUX5;
  ADCSRA |= 1 << ADEN | 1 << ADPS2;  //ADC sampling rate = 16MHz/16 = 1 MHz
}

void 
Timer2_config(void)
{
  TCCR2A |= 1 << WGM20 | 1 << WGM21;
  TCCR2B |= 1 << WGM22; //fast mode OCRA = PWM period
  OCR2A = TIMER_PWM_PERIOD;
  Current_PWM_DutyCycle = MIN_PWM_DUTYCYCLE; // Initial Duty cycle
  OCR2B = Current_PWM_DutyCycle;
  TCCR2A |= 1 << COM2B1;  // clear at compare 
  TIMSK2 |= 1 << TOIE2; // Timer2 overflow interrupt enabled
}

ISR
(PCINT0_vect)
{
  Hall_IN = HALL_SENSORS_IDR & 0x07;  
  PreDriver_Sequence = Hall_DIR_sequence[Hall_IN];
  PWM_update(PreDriver_Sequence); 
}

ISR
(TIMER2_OVF_vect)
{
  ADC_Sample_Counter++; 
  
  if(ADC_Sample_Counter > ADC_SAMPLING_PWM_PERIODS)
		{
			ADC_Sample_Counter = 0x0;
			SampleADC = true;
		}
			
	if (Desired_PWM_DutyCycle > Current_PWM_DutyCycle)
		{
			// Increment duty cycle or change duty cycle in +VE direction
			Current_PWM_DutyCycle = Current_PWM_DutyCycle + MAIN_PWM_BUCKET_DC;
		}
	else if (Desired_PWM_DutyCycle < Current_PWM_DutyCycle)
		{
			// Decrement duty cycle or change duty cycle in -VE direction
			Current_PWM_DutyCycle = Current_PWM_DutyCycle - MAIN_PWM_BUCKET_DC;
		}
    
	// Update PWM duty cycle values
	OCR2B = Current_PWM_DutyCycle;
}

void 
Start_ADC_Conversation(void)
{
  ADCSRA |= 1 << ADSC;	//start conversion
  while(ADCSRA & (1 << ADSC));
  curADC = ADC;
  if ((curADC > (prevADC + 10)) || (curADC < (prevADC - 10)))
		{
			prevADC = curADC;
			Temp_DutyCycle = ((prevADC-200)/655.0) * TIMER_PWM_PERIOD; //to not get to the top value

			Desired_PWM_DutyCycle = Temp_DutyCycle;
		}
}