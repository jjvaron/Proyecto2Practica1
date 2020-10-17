/* Ruler 1         2         3         4         5         6         7        */
/******************************  main_v2.5.2.c  *******************************/
/*                                                                            */
/*   Proposito: Sensor de temperatura en un microcontrolador cuyo valor se    */
/*              observa en dos pantallas 7 segmentos y mediante comunicacion  */
/*              serial.                                                       */
/*  Programado usando interrupciones.                                         */
/*                                                                            */
/*   Origin:  Escrito y probado por                                           */
/*            Cristian-Ovidio  Duran-Celis                                    */
/*            Juan-Sebastian   Parrado-Gutierrez                              */
/*            Jeliza           Varon-Heredia                                  */
/*            Septiembre 9, 2020                                              */
/*                                                                            */
/*   e-mail:  cristianduran@javeriana.edu.co                                  */
/*                parrado-j@javeriana.edu.co                                  */
/*            jelitza.varon@javeriana.edu.co                                  */
/*                                                                            */
/*   Notas: Los autores utilizaron el siguiente datasheet                     */
/* https://cdn.sparkfun.com/assets/c/a/8/e/4/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf */
/*            Rev. B – Nov/2016                                               */
/*                                                                            */
/******************************************************************************/

/* ******************************** Headers ********************************* */

/* --------------------- Inclusion de librerias estandar -------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/* ---------------------- Inclusion de librerias propias -------------------- */
#include "nuestrostimers.h" // Timer 1
#include "definiciones_y_configuraciones.h" // ADC cuya entrada es el sensor de
// temperatura
#include "display.h" // Dos displays 7 segmentos
#include "UART.h" // Comunicacion serial asincrona

Tm_Periodico digitos, adc; // Estrucutura con informacion del contador, periodo
// y las banderas
D_Display display; // Estructura con informacion de la temperatura y el estado
// de las pantallas
ADC_T conversion; // Estructura con una bandera
UART_T Recepcion; // Estructura con informacion del serial y una bandera
char Transmicion;

uint8_t temperatura_main = 0; // Inicializacion de la temperatura

int main()
{
	cli(); // Deshabilita interrupciones
	
	// Inicializacion de los pines como salida
	// Habilita los pines 16, 15, 14
	DDRB |= 0b00000111; //(1<<DDB2) | (1<<DDB1) | (1<<DDB0);
	// Habilita los pines 23, 24
	DDRC |= 0b0000011; //(1<<DDC1) | (1<<DDC0);
	// Habilita los pines 13, 12, 11, 6
	DDRD |= 0b11110000; //(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4);
	
	// Iniciando funciones
	Tm_Inicie_timer();
	Tm_Inicie_periodico(&digitos, 8); // Periodo en ms
	Tm_Inicie_periodico(&adc, 1000);  // Periodo en ms
	
	DyC_ResetADC();
	DyC_Inicie_ADC();
	
	D_Inicie_Estado(&display); // Define cual 7 segmentos enciende primero
	
	UART_init();
	Inicie_estado_write(&Recepcion);
	ConfigurarModo();
	
	sei(); // Habilita interrupciones
	
	
    for(;;) // while(1)  
	{	
		if(Tm_Hubo_periodico(&adc)) // Condicion de ADC
		{
			Tm_Baje_periodico(&adc);// Reset de condicion ADC
			DyC_HabiliteADC();
			
			if (conversion.bandera == 0x01) // Similar a hacer
			                                // <<if(Hubo_Conversion(&bandera_ADC))>>
			{
				DyC_Baje_Conversion(&conversion); // Reset de condicion conversion
				DyC_DeshabiliteADC();			
			}
			UART_write_DEC(&temperatura_main, &Transmicion);
			UART_write_UNI(&temperatura_main, &Transmicion);
		}
		
		// Inicia display despues de haber obtenido un dato del ADC
		D_inicie_display(&display, &temperatura_main);
		
		if(Tm_Hubo_periodico(&digitos)) // Condicion de display
		{
			Tm_Baje_periodico(&digitos); //Reset de condicion display
			D_Procese_display(&display);
		}
		
		if(Hubo_RX(&Recepcion))
		{
			Baje_RX(&Recepcion);
			FSM_UART(&Recepcion);
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	Tm_Procese_tiempo (&adc);
	Tm_Procese_tiempo (&digitos);
}

ISR(ADC_vect)
{
	DyC_Procese_ADC(&temperatura_main);
	DyC_Hubo_Conversion(&conversion);
}

ISR(USART_RX_vect)
{
	UART_read(&Recepcion);
}

ISR(INT0_vect)
{} //SMCR &=~ (1<<SE); // Deshabilita el modo sueno