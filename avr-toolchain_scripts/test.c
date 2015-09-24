#define F_CPU 11059200

#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>

int main(void) {

	sei();

	uint8_t A_MSK = 0;

	while (1) {
		PORTF ^= 0xFF;
		PORTA ^= A_MSK++;
		_delay_ms(500);
	}

	return 0;
}
