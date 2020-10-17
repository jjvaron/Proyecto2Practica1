/* Ruler 1         2         3         4         5         6         7        */
/*
 * Libreria para definiciones y configuraciones del ADC
 */

/* ********************************* Headers ******************************** */
#ifndef DEFINICIONES_Y_CONFIGURACIONES_H_
#define DEFINICIONES_Y_CONFIGURACIONES_H_

/* --------------------- Inclusion de librerias estandar -------------------- */

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint-gcc.h>
#include <avr/sfr_defs.h>

/* ****************************** Definiciones ****************************** */
#define TIEMPOADC     1000 // tiempo en ms
#define TIEMPODISPLAY 2000

/* ------------------------------- Estructura ------------------------------- */
typedef struct ADC_T ADC_T;
struct ADC_T
{
	unsigned char bandera;
};

/* ************************* Prototipos de Funciones ************************ */

/* ---------------------------------- ADC ----------------------------------- */
void DyC_ResetADC(void);
void DyC_Inicie_ADC(void);
void DyC_HabiliteADC(void);
void DyC_DeshabiliteADC(void);
  
void DyC_Procese_ADC (uint8_t *temperatura);
  
char DyC_Hubo_Conversion(ADC_T *ptr);
char DyC_Baje_Conversion(ADC_T *ptr);
#endif /* DEFINICIONES_Y_CONFIGURACIONES_H_ */