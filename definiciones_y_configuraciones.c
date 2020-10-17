/* Ruler 1         2         3         4         5         6         7        */
/*
 * Libreria para definiciones y configuraciones del ADC
 */

#include "definiciones_y_configuraciones.h"
 
/* ******************************** Funciones ******************************* */

/* ---------------------------------- ADC ----------------------------------- */
// Funcion para reiniciar el ADC
void DyC_ResetADC()
{
	ADCSRA = 0x00;
    ADMUX  = 0x00;
}

// Funcion para configurar la inicializacion del ADC
void DyC_Inicie_ADC()
{
	// Ajusta el resultado a la derecha Pag. 317
    ADMUX &=~ (1<<ADLAR);

    // Voltaje de referencia = 1.1 V Pag. 317 Tabla 28-3
    ADMUX |= (1<<REFS0); // 1
    ADMUX |= (1<<REFS1); // 1

    // Ahora se utiliza un divisor de frecuencias para cumplir con
    // la siguiente condicion 50 kHz < f < 200 kHz Pag. 308
    // Divisor de frecuencia = 128 --> 16000 kHz/128 = 125 kHz.
    //Pags. 319, 320 Tabla 28-5
    ADCSRA |= (1<<ADPS0);  // 1
    ADCSRA |= (1<<ADPS1);  // 1
    ADCSRA |= (1<<ADPS2);  // 1

    // Habilitando el sensor de temperatura Pags. 317, 318
    ADMUX  &=~ (1<<MUX0); // 0
    ADMUX  &=~ (1<<MUX1); // 0
	ADMUX  &=~ (1<<MUX2); // 0
    ADMUX  |=  (1<<MUX3); // 1
}

// Habilitar conversion del ADC
void DyC_HabiliteADC()
{
	ADCSRA |= (1<<ADEN) | (1<<ADSC);
}

// Deshabilita la conversion ADC
void DyC_DeshabiliteADC()
{
	ADCSRA &=~ (1<<ADEN);
	ADCSRA &=~ (1<<ADSC);
}

// Toma los datos, los convierte en Celsius
void DyC_Procese_ADC (uint8_t *temperatura)
{
	*temperatura= ADC - ((1024*0.7)/5);
	*temperatura= (*temperatura *99)/1024;
}

// Funcion para activar una bandera y evaluar si el ADC completo una conversion
char DyC_Hubo_Conversion(ADC_T *ptr)
{
	while (bit_is_set(ADCSRA,ADSC))
	{
		return (ptr->bandera | 0x01);
	}
	return (ptr->bandera & 0x00);
}

// Funcion para bajar la bandera de conversion
char DyC_Baje_Conversion(ADC_T *ptr)
{
	return (ptr->bandera & 0x00);
}