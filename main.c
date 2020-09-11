#include "nuestrostimers.h"
#include "definiciones_y_configuraciones.h"

// En nuestra implementacion esta deberia ser un global
// si vamos a trabajar por interrupción o deberia estar en el
// espacio de memoria del main.
int8_t temperatura;//ya va a estar en celsius
int8_t tempUnidades;//vamos a guardar el BCD de unidades
int8_t tempDecenas;//vamos a guardar el BCD de decenas

Tm_Periodico sondeoADC,sondeoDisplay;
Tm_Inicie_periodico (&sondeoADC,TIEMPOADC);// iniciar periodico de ADC
Tm_Inicie_periodico (&sondeoDisplay,TIEMPODISPLAY);// iniciar periodico de Display


void main (void)
{
    Tm_Procese_tiempo (&sondeoADC);
    Tm_Procese_tiempo (&sondeoDisplay);

}

if(Tm_Hubo_periodico (&sondeoADC))// condicion de ADC
        {
            Tm_Baje_periodico (&sondeoADC);//reset de condicion ADC
            adcval = leaADC(); /*lectura ADC*/
			temperatura = convierta_a_Celsius(adcval); /*conversión del valor a celsius*/
            DyC_Procese_ADC(&temperatura,&tempUnidades,&tempDecenas);
        }

  if(Tm_Hubo_periodico (&sondeoDisplay))// condicion de Display entra cada
                //TIEMPODISPLAY 10 milisegundos
    { Tm_Baje_periodico (&sondeoDisplay);//reset de condicion
            DyC_Procese_Display(&tempUnidades,&tempDecenas);
        }
