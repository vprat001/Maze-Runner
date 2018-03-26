/*
 *
 * Created By: Venkat Prathipati
 *
 */

#include <avr/io.h>
#include "timer.h"
#include "io.c"
#include "bit.h"
#include "keypad.h"
#include "usart.h"
#include "transmitDataLED.c"
#include "adcSM.c"
#include "displaySM.c"
#include "MazeSM.c"



//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------






unsigned char pattern[8] =
	{0b10000000,
	0b01000000,
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b00000001,};

unsigned char patternRed[5][8] ={{0b01111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,},
{0b11011111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,},
{0b01111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,},
{0b11111110,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,},
{0b11110111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,}};
unsigned char patternGreen[5][8] ={{0b10000010, 0b10111111, 0b10100001, 0b10101111, 0b10101000, 0b11101111, 0b00000001, 0b11111111},
{0b00101001, 0b11101001, 0b10001001, 0b11111001, 0b10000001, 0b11111111, 0b00010000, 0b11110000},
{0b10000000, 0b11001111, 0b01011100, 0b01110101, 0b11001101, 0b01011001, 0b01010001, 0b11011111},
{0b00001111, 0b10111111, 0b00001111, 0b11111111, 0b11100000, 0b11101011, 0b11110100, 0b11111111},
{0b11101001, 0b10111011, 0b10000010, 0b11111010, 0b00001010, 0b11111010, 0b10000010, 0b11111111}};

unsigned char colSolution[6] = {7,0,3,6,0,1};
unsigned char rowSolution [6] = {0b01111111,0b11111110,0b11111110,0b11111011,0b11111110,0b11111110};
//volatile unsigned short joystick = 0;
unsigned char outputIN;
unsigned char outred;
unsigned char outgreen;
int16_t A0;
int16_t A1;
unsigned char i;
unsigned char j;
unsigned char score;
unsigned short redcursorPos = 0;
unsigned short redcursorLoc = 0;
unsigned char * finish = "Good job! You finished playing this map, choose another map to keep playing.";
unsigned char k;

	


int main(void)
{
	DDRA = 0xFC; PORTA = 0x03;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	

	unsigned long int SMTick1_calc = 1;//joystick
	unsigned long int SMTick2_calc = 1;//display
	unsigned long int SMTick3_calc = 150;//maze

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;

	
	static task task1, task2, task3;
	task *tasks[] = { &task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = first;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &adcSM;//Function pointer for the tick.

	// Task 2
	task2.state = init;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &displaySM;//Function pointer for the tick.

	// Task 3
	task3.state = wait;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period; // Task current elasped time.
	task3.TickFct = &mazeSM; // Function pointer for the tick.


	TimerSet(GCD);
	TimerOn();
	
	unsigned char * finish1 = "Welcome to Maze Runner! Choose a map and begin.";
	unsigned char m;
	initUSART(0);
	
	ADC_init();
	
	for (m = 0; m < 47; m++){
		USART_Send(finish1[m], 0);
	}
	USART_Send('\n', 0);
		
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}

		
		if (score == 1){
			PORTA = 0x08;
		}
		else if(score == 0){
			PORTA = 0;
		}
		
		transmit_data_IN(outputIN);
		transmit_data_Green(outgreen);
		transmit_data_Red(outred);
		
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		
	}
	
}