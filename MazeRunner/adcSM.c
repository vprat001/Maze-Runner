/*
 *
 * Created by Venkat Prathipati    861209938
 *
 */

void ADC_init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

//Read from the ADC channel ch
uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel
	ch &= 0x07;
	ADMUX = (ADMUX & 0xF8) | ch;

	//Start Single conversion
	ADCSRA |= (1 << ADSC);

	//Wait for conversion to complete
	while(ADCSRA & (1 << ADSC));

	//Clear ADIF
	ADCSRA |= (1<<ADIF);
	return (ADC);
}



volatile unsigned short joystick = 0;
int16_t A0;
int16_t A1;

enum adcstates {first} adcstate;
void adcSM()
{
	switch(adcstate){//transitions
		A0 = ReadADC(0);
		A1 = ReadADC(1);
		
		case first:
			if(A0 > 600)//right
			{
				joystick = 0x01;
			}
			else if (A0 < 500)//left
			{
				joystick = 0x02;
			}
			else if (A1 > 600)//up
			{
				joystick = 0x04;
			}
			else if (A1 < 500)//down
			{
				joystick = 0x08;
			}
			else
			{
				joystick = 0x00;
			}
			adcstate = first;
			break;
		default:
			adcstate = first;
			break;
	}
	switch(adcstate){//actions
		case first:
			break;
		default:
			break;
	}
}