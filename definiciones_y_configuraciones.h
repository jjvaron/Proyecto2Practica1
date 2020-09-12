#if !defined(DEFYCONF_H)
#define DEFYCONF_H

#define TIEMPOADC 1000
#define TIEMPODISPLAY 10
#define TIEMPOBASE 9

#ifdef __cplusplus
extern "C" {
#endif
/*
typedef unsigned short uint8_t;
typedef short int8_t;
*/
void resetADC(void);
short leaADC(void);

int8_t convierta_a_Celsius(short val);
int8_t convierta_a_Celsius(short adcval);
int8_t obtenerUnidades(int8_t *temperatura);
int8_t obtenerDecenas(int8_t *temperatura);
int8_t num2BCD(int8_t *tempUnidades);
int8_t num2BCD(int8_t *tempDecenas);
void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas);

//void Inicializacion(void);

#ifdef __cplusplus
} // extern "C"
#endif
