///////////////////////////////////////////////////////////////////////
// Universidad Del Valle De Guatemala
// IE2023: Electronica digital 2
// Autor: Alan Gomez - 22115
// Proyecto: Laboratorio 6 - UART con ATmega328P Microchips AVR, Lenguaje C.
// Hardware: ATmega328P
// Creado: 20/09/2024
///////////////////////////////////////////////////////////////////////

#define F_CPU 16000000

// Librerias
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/twi.h>

#include "UART/UART.h" //Incluir libreria de UART

// Definir los pines de los botones
#define BOTON_PIN1 PD2
#define BOTON_PIN2 PD3
#define BOTON_PIN3 PD4
#define BOTON_PIN4 PD5
#define BOTON_PIN5 PD6
#define BOTON_PIN6 PD7


// Variables
volatile uint8_t BTN_STATE_1 = 0;
volatile uint8_t BTN_STATE_2 = 0;
volatile uint8_t BTN_STATE_3 = 0;
volatile uint8_t BTN_STATE_4 = 0;
volatile uint8_t BTN_STATE_5 = 0;
volatile uint8_t BTN_STATE_6 = 0;

const uint8_t delay_time = 11;// tiempo de antirrebote

uint8_t dato=0;


void setup() {
	// Configura los pines de los botones como entrada con resistencia Pull-Up
	DDRD &= ~((1 << BOTON_PIN1) | (1 << BOTON_PIN2) | (1 << BOTON_PIN3) | (1 << BOTON_PIN4) | (1 << BOTON_PIN5) | (1 << BOTON_PIN6));
	PORTD |= ((1 << BOTON_PIN1) | (1 << BOTON_PIN2) | (1 << BOTON_PIN3) | (1 << BOTON_PIN4) | (1 << BOTON_PIN5) | (1 << BOTON_PIN6));

	// Configura el pin PB5 (LED integrado en el ATmega328P) como salida
	DDRB |= (1 << PB5);

	// Habilitar las interrupciones por cambio de estado para los pines PD2 a PD7
	PCICR |= (1 << PCIE2);
	PCMSK2 |= ((1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23));

	initUART9600();  // Iniciar UART
	
	// Habilitar interrupciones globales
	sei();
}
// Modulos

void TT_BOUNCE(void);
TT_BOUNCE(){
	if(BTN_STATE_1){
		_delay_ms(delay_time);// dealy de push
		BTN_STATE_1 = 0;
		// Accion 
		writeTextUART("1");
		writeTextUART("\n");
		}
	if(BTN_STATE_2){
		_delay_ms(delay_time);// dealy de push
		BTN_STATE_2 = 0;
		// Accion
		writeTextUART("2");
		writeTextUART("\n");
		}
	if(BTN_STATE_3){
		_delay_ms(delay_time);// dealy de push
		BTN_STATE_3 = 0;
		// Accion
		writeTextUART("3");
		writeTextUART("\n");
		}
	if(BTN_STATE_4){
		_delay_ms(delay_time);// dealy de push
		BTN_STATE_4 = 0;
		//Accion
		writeTextUART("4");
		writeTextUART("\n");
		}
	if(BTN_STATE_5){
		_delay_ms(1);// dealy de push
		BTN_STATE_5 = 0;
		//Accion
		writeTextUART("A");
		writeTextUART("\n");
		}
	if(BTN_STATE_6){
		_delay_ms(1);// dealy de push
		BTN_STATE_6 = 0;
		//Accion
		writeTextUART("B");
		writeTextUART("\n");
		_delay_ms(delay_time);// dealy de push
		}
}

//Vectores de interrupcion
ISR(PCINT2_vect) {
	// Detecta el cambio de estado en los pines PD2 a PD7

	// Detecta el cambio de estado en el pin PD2
	if (!(PIND & (1 << BOTON_PIN1))) {
		BTN_STATE_1 = 1; // Indica que se presionó un botón
	}
	// Detecta el cambio de estado en el pin PD3
	if (!(PIND & (1 << BOTON_PIN2))) {
		BTN_STATE_2 = 1; // Indica que se presionó un botón
	}
	// Detecta el cambio de estado en el pin PD4
	if (!(PIND & (1 << BOTON_PIN3))) {
		BTN_STATE_3 = 1; // Indica que se presionó un botón
	}
	// Detecta el cambio de estado en el pin PD5
	if (!(PIND & (1 << BOTON_PIN4))) {
		BTN_STATE_4 = 1; // Indica que se presionó un botón
	}
	// Detecta el cambio de estado en el pin PD6
	if (!(PIND & (1 << BOTON_PIN5))) {
		BTN_STATE_5 = 1; // Indica que se presionó un botón
	}
	// Detecta el cambio de estado en el pin PD7
	if (!(PIND & (1 << BOTON_PIN6))) {
		BTN_STATE_6 = 1; // Indica que se presionó un botón
	}
}
// Codigo Principal
int main(void) {
	setup();
	while (1) {
		TT_BOUNCE();
		_delay_ms(10);
	}
}
