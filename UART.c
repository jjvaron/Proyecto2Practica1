/* Ruler 1         2         3         4         5         6         7        */
/*
 * Libreria para la comunicacion serial asincrona entre el microcontrolador y 
 * el computador con una velocidad de 9600 baudios
 */

#include "UART.h"

/* UART */
void UART_init()
{
	DDRD |= (1<<1);							//PD1	COMO SALIDA TXa
	DDRD &= ~(1<<0);						//PD0	COMO ENTRADA RX
	UCSR0A = (0<<TXC0)|(0<<U2X0)|(0<<MPCM0);
	UCSR0B = (1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)|(0<<TXB80);
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
	UBRR0 = 103;								//NO DUPLICA VELOCIDAD 9600B A 160000
}

void UART_write(unsigned char caracter){
	while(!(UCSR0A&(1<<5)));    // mientras el registro UDR0 esté lleno espera
		UDR0 = caracter;            //cuando el el registro UDR0 está vacio se envia el caracter
}

// Funcion para la recepcion de caracteres
void UART_read(UART_T *comunicacion)
{
	comunicacion->bandera |= 0x01;
	comunicacion->dato_serial = UDR0;
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = comunicacion->dato_serial;
}

char Hubo_RX(UART_T *comunicacion)
{
	return (comunicacion->bandera & 0x01);
}

void Baje_RX(UART_T *comunicacion)
{
	comunicacion->bandera &=~ 0x01;
}

void UART_write_txt(char* cadena) //cadena de caracteres de tipo char
{			
	while(*cadena !=0x00) //mientras el último valor de la cadena sea diferente a el caracter nulo
	{				
		UART_write(*cadena);			//transmite los caracteres de cadena
		cadena++;						//incrementa la ubicación de los caracteres en cadena
		//para enviar el siguiente caracter de cadena
	}
}

// Funcion para la transmision del digito de decenas
void UART_write_DEC(uint8_t *temperatura, char *dato_tx)
{
    uint8_t decenas;
	decenas  = *temperatura/10;
	*dato_tx = decenas + '0';   

	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = *dato_tx;
}

// Funcion para la transmision del digito de unidades
void UART_write_UNI(uint8_t *temperatura, char *dato_tx)
{
	uint8_t unidades;
	unidades = *temperatura%10;
	*dato_tx = unidades + '0';
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = *dato_tx;
}

void Inicie_estado_write(UART_T *comunicacion)
{
	comunicacion->fsm = s0;
}

void FSM_UART(UART_T *ptr_char)
{
	switch (ptr_char->fsm)
	{
		case s0:
		if (ptr_char->dato_serial==INICIO)
		{
			ptr_char->fsm= s1;
		}
		else
		{
			ptr_char->fsm = s0;	
		} /* endif */
		break;

		case s1:
		if (ptr_char->dato_serial==DORMIR)
		{	
			ptr_char->fsm = s0;
			
			DyC_DeshabiliteADC(); //ADCSRA &=~ (1<<ADEN);
			// Todos los pines se apagan
			PORTB = 0x00;
			PORTC = 0x00;
			PORTD = 0x00;
			
			// Modo Idle
			/*
			SMCR &=~ (1<<SM2);
			SMCR &=~ (1<<SM1);
			SMCR &=~ (1<<SM0);
			*/
			// Ajusta el registro para ponerlo en modo Power-Down/Baja Potencia
			SMCR &=~ (1<<SM2);
			SMCR |=  (1<<SM1);
			SMCR &=~ (1<<SM0);
			// Ajusta el registro en modo de Power-save/Ahorro de potencia
			/*
			SMCR &=~ (1<<SM2);
			SMCR |=  (1<<SM1);
			SMCR |=  (1<<SM0);
			*/
			// Modo Standby
			/*
			SMCR |=  (1<<SM2);
			SMCR |=  (1<<SM1);
			SMCR &=~ (1<<SM0);
			*/
			// Modo Extended Standby
			/*
			SMCR |= (1<<SM2);
			SMCR |= (1<<SM1);
			SMCR |= (1<<SM0);
			*/
			SMCR |= (1<<SE);
			__asm__ __volatile__ ( "sleep" );
		}
		else
		{
			ptr_char->fsm = s0;
		}
		break;
	} /* endif */
} /* FSM_UART */

void ConfigurarModo()
{
	SMCR = 0x00;
	EIMSK |= (1<<INT0); //pin 4 INT0
	EICRA |= (1 << ISC01) || ( 1 << ISC00 ); // Rising edge on both
}