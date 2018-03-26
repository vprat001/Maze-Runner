void transmit_data_IN(unsigned char data) { int i;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTB = 0x00;
		// set SER = next bit of data to be sent.
		PORTB |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTB |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTB |= 0x04;
	// clears all lines in preparation of a new transmission PORTB = 0x00;
}

void transmit_data_Red(unsigned char data) { int k;
	for (k = 0; k < 8 ; ++k) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTA = 0x00;
		// set SER = next bit of data to be sent.
		PORTA |= (((data >> k) & 0x01) << 4);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTA |= 0x20;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTA |= 0x40;
	// clears all lines in preparation of a new transmission PORTC = 0x00;
}



void transmit_data_Green(unsigned char data) { int j;
	for (j = 0; j < 8 ; ++j) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTB = 0x00;
		// set SER = next bit of data to be sent.
		PORTB |= (((data >> j) & 0x01) << 4);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTB |= 0x20;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTB |= 0x40;
	// clears all lines in preparation of a new transmission PORTC = 0x00;
}