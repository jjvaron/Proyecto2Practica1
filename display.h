/* Ruler 1         2         3         4         5         6         7        */
/*
 * Libreria para el manejo del display donde se visualiza la temperatura
 * medida por el micro controlador ATMega328P.
 * Se usa un display 7 segmentos de catodo comun (tierra comun)
 */

/* ********************************* Headers ******************************** */
#ifndef DISPLAYDIGITOS_H_
#define DISPLAYDIGITOS_H_
/* --------------------- Inclusion de librerias estandar -------------------- */
#include <avr/io.h>
#include "definiciones_y_configuraciones.h"

/* ****************************** Definiciones ****************************** */

#define UNIDADES 1U// Si el estado se encuentra aca, muestra unidades
#define DECENAS  2U// Si el estado se encuentra aca, muestra decenas

// El transistor que deshabilita el display de unidades esta en el puerto D bit 5
#define TRANDISPUNIDADES  0b00010000 //PD4
// El transistor que deshabilita el display de decenas esta en el puerto b bit 2
#define TRANDISPDECENAS   0b00000010 //PC1
  
/* ------------------------------- Estructuras ------------------------------ */
  typedef struct D_Display D_Display;
  struct D_Display
  {
    uint8_t tempUnidades;
    uint8_t tempDecenas;
    uint8_t mostrando; // Estado
  };

/* ************************* Prototipos de Funciones ************************ */

void D_inicie_display (D_Display *disp, uint8_t *temperatura);
void D_apague_display (void);
void D_Inicie_Estado(D_Display *disp);

void D_Procese_display (D_Display *disp); 

uint8_t num2BCDPUERTOB(uint8_t num);
uint8_t num2BCDPUERTOC(uint8_t num);
uint8_t num2BCDPUERTOD(uint8_t num);

#endif /* DISPLAYDIGITOS_H_ */