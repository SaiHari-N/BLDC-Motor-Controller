/*
 * main.h
 *
 * Created: 2/24/2020 10:59:33 PM
 *  Author: Mohamed_Hassanin
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/* pins and ports */

/* hall sensors */
#define HALL_SENSORS_MODER DDRB
#define HALL_SENSORS_IDR PINB
#define HALL_U_PIN 0x01
#define HALL_V_PIN 0x02
#define HALL_W_PIN 0x04

/* gates signals */
#define UL_MODER DDRB
#define UL_ODR PORTB
#define UL_PIN 0x40

#define UH_MODER DDRB
#define UH_ODR PORTB
#define UH_PIN 0x20

#define VL_MODER DDRE
#define VL_ODR PORTE
#define VL_PIN 0x10

#define VH_MODER DDRE
#define VH_ODR PORTE
#define VH_PIN 0x08

#define WL_MODER DDRH
#define WL_ODR PORTH
#define WL_PIN 0x10

#define WH_MODER DDRH
#define WH_ODR PORTH
#define WH_PIN 0x08

#define HS_PWM_MODER DDRH 
#define HS_PWM_PIN 0x40

#define LS_PWM_MODER DDRB
#define LS_PWM_ODR PORTB
#define LS_PWM_PIN 0x10


#ifdef DIRECTION_CCW
unsigned char Hall_DIR_sequence[] = { 0x00,
									 // Note:		Hall	WVU
	HS_U|LS_W,       // Hall position 001
	HS_V|LS_U,       // Hall position 010
	HS_V|LS_W,       // Hall position 011
	HS_W|LS_V,       // Hall position 100
	HS_U|LS_V,       // Hall position 101
	HS_W|LS_U,       // Hall position 110
0x00   };
#endif

#ifdef DIRECTION_CW
unsigned char Hall_DIR_sequence[] = {  0x00,
									 // Note:		Hall	WVU
	HS_W|LS_U,       // Hall position 001
	HS_U|LS_V,       // Hall position 010
	HS_W|LS_V,       // Hall position 011
	HS_V|LS_W,       // Hall position 100
	HS_V|LS_U,       // Hall position 101
	HS_U|LS_W,       // Hall position 110
0x00   };
#endif

// Motor and Commutation Variables
unsigned int Desired_PWM_DutyCycle, Current_PWM_DutyCycle, 
PWM_BucketStep, PWM_Update_Counter, ADC_Sample_Counter, Temp_DutyCycle;
unsigned char PreDriver_Sequence, Hall_IN, Motor_Status;

// ADC Variables
volatile int curADC, prevADC = 0;
volatile unsigned char SampleADC = false;

// Function definition
void PWM_update (unsigned char Next_Hall_Sequence);
void Start_Motor(void);
void Stop_Motor(void);
void Start_ADC_Conversation(void);

//peripherals
static void GpioInit(void);
static void PCINT0_init(void);
static void ADC_init(void);
static void Timer2_config(void);

#endif /* MAIN_H_ */