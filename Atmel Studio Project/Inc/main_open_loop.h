//****************************************************************************//
// Clock Timing Definitions
//****************************************************************************//

#define SYSTEM_FREQ 16000000UL          // (in Hz) System Freq, Timer PWM Time Base
#define PWM_FREQ 5000                  // (in Hz) PWM Freq for switching the FETs 
#define TIMER2_PRESCALER 32
#define TIMER_PWM_PERIOD  ((SYSTEM_FREQ)/PWM_FREQ/TIMER2_PRESCALER)
                                        // Timer PWM Counts = System Freq/(2xPWM Freq)

#define SPEEDIN_PWM_FACTOR  4           // Calculate this value and define the constant 
                                        // = 2^12/TIMER_PWM_PERIOD, (as ADC12 is being used) rounded off to the closest whole digit

#define DUTYCYCLE_MIN 5                 // (in %age) Min Dutycycle percentage - latching percentage used initially
#define MIN_PWM_DUTYCYCLE   ((TIMER_PWM_PERIOD)*(DUTYCYCLE_MIN))/100
                                        // Min PWM Dutycycle; also used for configuring initial dutycycle 

//****************************************************************************//
// Motor Speed Control - Open Loop Definitions
//****************************************************************************//

#define MOTOR_STARTUP_TIME  100        // (in ms) Motor Start-Up time 
#define DUTYCYCLE_CHANGE_PERIODS  1000    // #PWM periods dutycycle change is expected, used for start-up routine only
#define STARTUP_STEPS ((unsigned long)PWM_FREQ*MOTOR_STARTUP_TIME)/(DUTYCYCLE_CHANGE_PERIODS*1000)
                                        // For e.g: PWM freq = 15kHz, Startup time=100ms, 
                                        // then #steps for ramping = (15*100)/10 = 150 steps

#define ADC_SAMPLING_PWM_PERIODS 10   // Every #PWM periods when ADC is sampled

#define MAIN_PWM_BUCKET_DC 1 // PWM dutycycle change resolution in timer counts
                                        
//****************************************************************************//
// Motor Status Definitions
//****************************************************************************//

#define Stopped 0x00
#define StartUp 0x01
#define Running 0x02

#define DIRECTION_CCW 
//#define DIRECTION_CW

#define true 0x01
#define false 0x0


