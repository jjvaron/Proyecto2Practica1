
#include "display.h"

void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas)
{
    short adcval=leaADC();
    *temperatura=convierta_a_Celsius(adcval);
    *tempUnidades=saqueUnidades(*temperatura);
    *tempDecenas=saqueDecenas(*temperatura);
    *tempUnidades=num2BCD(*tempUnidades);
    *tempDecenas=num2BCD(*tempDecenas);
}

short leaADC(void)
{
  ADMUX |=  (1<<REF1)|(1<<REF0) /* Lectura ADC con referencia de 1.1V */
  ADMUX |=  (1<<MUX3) /* habilita el sensor de temperatura */
  ADCSRA |= (1 << ADSC) | (1 << ADEN); /* comienza conversión ADC */ 
  while (bit_is_set(ADCSRA,ADSC)); /* Detect end-of-conversion */

  return ADCL | (ADCH << 8);; /* retorna la temperatura en celsios */
}

int8_t convierta_a_Celsius(short temp_value)
{
	/* f(x) = (raw_value - offset) / coeff */
  return int8_t ((temp_value - 324.31)/1.22);
}