#if !defined(DEFYCONF_H)
#define DEFYCONF_H

#define TIEMPOADC 1000
#define TIEMPODISPLAY 10

#ifdef __cplusplus
extern "C" {
#endif
//aqui ponemos nuestras funciones de C varias
typedef short int8_t
short leaADC(void);
int8_t convierta_a_Celsius(short val);
void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas);

#ifdef __cplusplus
} // extern "C"
#endif
