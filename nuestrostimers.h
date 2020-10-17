/* Ruler 1         2         3         4         5         6         7        */
/*
 * Libreria para definiciones y configuraciones del Timer 1
 */

#ifndef NUESTROSTIMERS_H_
#define NUESTROSTIMERS_H_

/* --------------------- Inclusion de librerias estandar -------------------- */
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

/* ********************** Definicion de tipos de datos ********************** */

/* ------------------------- Definiciones del timer ------------------------- */

#define TM_PER_B_ACTIVO		0x01U
#define TM_PER_B_TC			0x02U
#define TM_Out_B_TC			0x04U

#define	Fclk      1000UL // Frecuencia base en Hz del timer
#define F_CPU 16000000UL // Frecuencia del oscilador externo. 16 MHz

/* ------------------------------- Estructura ------------------------------- */
typedef struct Tm_Periodico Tm_Periodico;
struct Tm_Periodico
{
	unsigned int	contador, // van los conteos temporales por timer multiplo
	                periodo;  //hasta donde quiero contar
	unsigned long	timeout;  //ac? va un timeout
	unsigned char   banderas; //el primer bit me dice si est? activo este timer el segundo me dice si se da periodo
};

/* ************************* Prototipos de Funciones ************************ */

/* --------------------------------- Timer1 --------------------------------- */

void Tm_Inicie_timer(void);

char Tm_Hubo_timer(void);
void Tm_Baje_timer(void);

void Tm_Procese_tiempo (Tm_Periodico *ctp);

void Tm_Inicie_periodico (Tm_Periodico *ctp,unsigned int periodo);
char Tm_Hubo_periodico (Tm_Periodico *ctp);
void Tm_Baje_periodico (Tm_Periodico *ctp);
void Tm_Termine_periodico (Tm_Periodico *ctp);

void Tm_Inicie_timeout (Tm_Periodico *ctp,unsigned int tiempo);
char Tm_Hubo_timeout (Tm_Periodico *ctp);
void Tm_Baje_timeout (Tm_Periodico *ctp);

#endif /* NUESTROSTIMERS_H_ */