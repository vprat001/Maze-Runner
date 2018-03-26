/*
 *
 * Created by Venkat Prathipati    861209938
 *
 */
unsigned char pattern[8] ={0b10000000,
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
//volatile unsigned short jpress = 0;
unsigned char outputIN;
unsigned char outred;
unsigned char outgreen;
unsigned char * finish = "Good job, you finished playing this map, move on to next map!";
unsigned char * finish1 = "Good job, Game over!";
unsigned char k;
int16_t A0;
int16_t A1;
unsigned char i;
unsigned char j;
unsigned char score;
unsigned short redcursorPos = 0;
unsigned short redcursorLoc = 0;


enum display_SM {init, display, incr} display_state;
void displaySM(){
	
	switch(display_state){//Transitions
		case init:
			display_state = display;
			break;
		case display:
			if (GetKeypadKey() == '1')
			{
				score = 0;
				j = 0;
			}
			else if (GetKeypadKey() == '2')
			{
				score = 0;
				j = 1;
			}
			else if (GetKeypadKey() == '3')
			{
				score = 0;
				j = 2;
			}
			else if (GetKeypadKey() == '4')
			{
				score = 0;
				j = 3;
			}
			else if (GetKeypadKey() == '5')
			{
				score = 0;
				j = 4;
			}	
			display_state = incr;
			break;
		case incr:
			if(i >= 8)
			{
				display_state = init;
			}
			else
			{
				display_state = display;
			}
			break;
		default:
			display_state = display;
			break;
	}
	
	switch(display_state){//Actions
		case init:
			i = 0;
			break;
		case display:
			if((redcursorPos == colSolution[j]) && (patternRed[j][redcursorPos] == rowSolution[j])){
				if (j == 0 ) {
					for (k = 0; k < 76; k++){
						USART_Send(finish[k], 0);
					}
					USART_Send('\n', 0);
				}
				if (j == 1 ) {
					USART_Send('2', 0);
					USART_Send('\n', 0);
				}	
				if (j == 2 ) {
					for (k = 0; k < 76; k++){
						USART_Send(finish[k], 0);
					}
					USART_Send('\n', 0);
				}
				if (j == 3 )
				{
					for (k = 0; k < 76; k++){
						USART_Send(finish[k], 0);
					}
					USART_Send('\n', 0);
				}
				if (j == 4 )
				{
					score = 1;
					USART_Send('5', 0);
					USART_Send('\n', 0);
				}
				else {
					score = 1;
					redcursorPos = 0;
				}
			}
			outputIN = pattern[i];
			outred = patternRed[j][i];
			outgreen = patternGreen[j][i];
			break;
		case incr:
			outred = 0xFF;
			outgreen = 0xFF;
			outputIN = 0x00;
			i++;
			break;
	}
}