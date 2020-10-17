/* Ruler 1         2         3         4         5         6         7        */
/*
 * Libreria para la comunicacion serial asincrona entre el microcontrolador y 
 * el computador con una velocidad de 9600 baudios
 */

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "definiciones_y_configuraciones.h" // Para la FSM

/* ********************** Definicion de tipos de datos ********************** */

/* ------------------------ Definiciones de la UART ------------------------- */
#define INICIO '#'
#define DORMIR 'S'
#define s0 0
#define s1 1

/* ------------------------------- Estructura ------------------------------- */

typedef struct UART_T UART_T;
struct UART_T
{
	unsigned char bandera,
				  dato_serial,
				  fsm;
};

void UART_init(void);
void UART_write_txt(char* cadena);

void UART_read(UART_T *comunicacion);
char Hubo_RX(UART_T *comunicacion);
void Baje_RX(UART_T *comunicacion);
void UART_write_DEC(uint8_t *temperatura, char *dato_tx);
void UART_write_UNI(uint8_t *temperatura, char *dato_tx);
void Inicie_estado_write(UART_T *comunicacion);
void FSM_UART(UART_T *ptr_char);
void ConfigurarModo(void);
#endif /* UART_H_ */