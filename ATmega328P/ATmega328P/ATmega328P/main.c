///////////////////////////////////////////////////////////////////////
// Universidad Del Valle De Guatemala
// IE2023: Electronica digital 2
// Autor: Alan Gomez - 22115
// Proyecto: Laboratorio 6 - UART con ATmega328P Microchips AVR, Lenguaje C.
// Hardware: ATmega328P
// Creado: 20/09/2024
///////////////////////////////////////////////////////////////////////

////////////////////////////////////////////
// F_CPU ATmega
////////////////////////////////////////////
#define F_CPU 16000000 // Definimos la frecuencia con la que trabaja el ATmega328P
////////////////////////////////////////////
// Librerias
////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

////////////////////////////////////////////
// Definiciones de Pines
////////////////////////////////////////////
#define BOTON_PIN_PD2 PD2

////////////////////////////////////////////
// Variables
////////////////////////////////////////////
volatile uint8_t state_D2 = 0;

////////////////////////////////////////////
// SETUP
////////////////////////////////////////////
void setup() {
	// Configurar pin PD2 como entrada con resistencia Pull-Up
	DDRD &= ~(1 << BOTON_PIN_PD2);  // Configura PD2 como entrada
	PORTD |= (1 << BOTON_PIN_PD2);  // Activa resistencia Pull-Up

	// Configurar PB5 como salida (LED integrado)
	DDRB |= (1 << PB5);

	// Habilitar interrupci�n externa para PD2
	EICRA |= (1 << ISC01);  // Flanco de bajada para INT0
	EIMSK |= (1 << INT0);   // Habilitar interrupci�n INT0

	// Habilitar interrupciones globales
	sei();
}

////////////////////////////////////////////
// LOOP Secundario
////////////////////////////////////////////
void loop() {
	// Controlar el LED seg�n el estado del bot�n en PD2
	if (state_D2) {
		PORTB |= (1 << PB5);  // Enciende el LED si el bot�n est� presionado
		} else {
		PORTB &= ~(1 << PB5); // Apaga el LED si el bot�n no est� presionado
	}
}

////////////////////////////////////////////
// Rutina de servicio de interrupci�n (ISR)
////////////////////////////////////////////
// ISR para INT0 (PD2)
ISR(INT0_vect) {
	state_D2 = !state_D2;  // Cambia el estado de D2
}
