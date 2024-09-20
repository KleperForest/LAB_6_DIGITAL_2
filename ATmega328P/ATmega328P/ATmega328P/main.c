#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>+
#include <avr/delay.h>

// Definir el pin del botón
#define BOTON_PIN_PD2 PD2
#define BOTON_PIN_PD3 PD3
#define BOTON_PIN_PD4 PD4
#define BOTON_PIN_PD5 PD5
#define BOTON_PIN_PD6 PD6
#define BOTON_PIN_PD7 PD7

// Variable para almacenar el estado del botón
volatile uint8_t state_B2 = 0;
volatile uint8_t state_B3 = 0;
volatile uint8_t state_B4 = 0;
volatile uint8_t state_B5 = 0;
volatile uint8_t state_B6 = 0;
volatile uint8_t state_B7 = 0;

void setup() {
	// Configura el pin del botón como entrada con resistencia Pull-Up
	DDRD &= ~(1 << BOTON_PIN_PD2);  // Configura el pin como entrada
	PORTD |= (1 << BOTON_PIN_PD2);  // Habilita la resistencia Pull-Up

	// Configura el pin PB5 (LED integrado en el ATmega328P) como salida
	DDRB |= (1 << PB5);

	// Habilitar las interrupciones por cambio de estado para el pin PD4
	PCICR |= (1 << PCIE2);  // Habilita las interrupciones por cambio de estado para PCINT[23:16] (PORTD)
	PCMSK2 |= (1 << PCINT20);  // Habilita la interrupción en el pin PD4

	// Habilitar interrupciones globales
	sei();
}

void loop() {
	// Controla el LED según el estado del botón
	if (state_B2) {
		PORTB |= (1 << PB5);  // Enciende el LED si el botón está en HIGH
		} else {
		PORTB &= ~(1 << PB5);  // Apaga el LED si el botón está en LOW
	}
}

// Esta es la rutina de servicio de interrupción para los pines de PORTD (pines D0 a D7)
ISR(PCINT2_vect) {
	// Detecta el cambio de estado en el pin PD4
	if (!(PIND & (1 << BOTON_PIN_PD2))) {
		state_B2 = !state_B2;  // Cambia el estado del botón
	}
}

int main(void) {
	setup();
	while (1) {
		loop();
		_delay_ms(100);
	}
}
