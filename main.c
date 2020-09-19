/*
 * Practica1.c
 *
 * Created: 11/9/2020 23:38:08
 * Author : HewlettPackard
 */ 

/*???????????? LIBRERIAS ????????????*/
#include <avr/io.h>
#include <stdlib.h>

#include "nuestrostimers.h"
#include "definiciones_y_configuraciones.h"
#include "display.h"
	
// En nuestra implementacion esta deberia ser un global
// si vamos a trabajar por interrupción o deberia estar en el
// espacio de memoria del main.
int8_t temperatura;  //ya va a estar en celsius
int8_t tempUnidades; //vamos a guardar el BCD de unidades
int8_t tempDecenas;  //vamos a guardar el BCD de decenas

Tm_Periodico sondeoADC,sondeoDisplay,baseTiempo;
Tm_Inicie_periodico (&sondeoADC,TIEMPOADC);         // iniciar periodico de ADC
Tm_Inicie_periodico (&sondeoDisplay,TIEMPODISPLAY); // iniciar periodico de Display
Tm_Inicie_periodico (&baseTiempo,TIEMPOBASE);       // iniciar periodico de ADC

void main (void)
{
	for(;;)
	{
		if(Tm_Hubo_periodico (&baseTiempo))
		{
			Tm_Baje_periodico (&baseTiempo);//reset de condicion baseTiempo
			Tm_Procese_tiempo (&sondeoADC);
			Tm_Procese_tiempo (&sondeoDisplay);
		}
		if(Tm_Hubo_periodico (&sondeoADC))// condicion de ADC
		{
			Tm_Baje_periodico (&sondeoADC);//reset de condicion ADC
			// Aquí se lee el ADC y se convierte el valor leido a grados y unidades y decenas para el display
			DyC_Procese_ADC(&temperatura,&tempUnidades,&tempDecenas); // En definiciones y configuraciones
		}

		if(Tm_Hubo_periodico (&sondeoDisplay))// condicion de Display entra cada TIEMPODISPLAY 10 milisegundos
		
		{ 
			Tm_Baje_periodico (&sondeoDisplay);//reset de condicion
			D_Procese_display(&tempUnidades,&tempDecenas);
		}
	} /*for*/
} /*main*/
