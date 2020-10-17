/* Ruler 1         2         3         4         5         6         7        */
/*
 * Libreria para definiciones y configuraciones del Timer 1
 */

#include "nuestrostimers.h"

// Inicializacion de registros en el Timer 1
void Tm_Inicie_timer()
{
	TCNT1 = 0x0000; // Contador inicializado en 0
	// Modo de operacion = CTC Pags. 170 - 173. Tabla 20-6
	TCCR1A &= ~(1<<WGM10); // 0
	TCCR1A &= ~(1<<WGM11); // 0
	TCCR1B |=  (1<<WGM12); // 1
	TCCR1B &= ~(1<<WGM13); // 0
	
	// Valor de comparacion = OCR1A Pags. 162, 173. Tabla 20-7
	OCR1A = (F_CPU/1024/Fclk)-1;
	
	TCCR1B |=  (1<<CS10); // Preescalizador en 1024. F_CLK/1024
	TCCR1B &= ~(1<<CS11);
	TCCR1B |=  (1<<CS12);
	
	// Habilitar la interrupcion Pag. 184
	TIMSK1 |= (1<<OCIE1A); // Pone un 1 en el bit 1 del registro		
}

// Define el periodo dinamico a partir del timer
void Tm_Inicie_periodico (Tm_Periodico *ctp,unsigned int periodo)
{	
	ctp->banderas |=  TM_PER_B_ACTIVO; //activo el timer
	ctp->banderas &= ~TM_PER_B_TC; //bajo la bandera a 0
	ctp->contador  =  ctp->periodo = periodo;
	// Tanto el contador y el periodo inician en el mismo valor el contator decrece
	// y el periodo se usa para reset
}

// Conteo regresivo del periodo definido anteriormente
void Tm_Procese_tiempo (Tm_Periodico *ctp)
{
	if (ctp->banderas & TM_PER_B_ACTIVO)//si el timer est? activo
	{
		--(ctp->contador);//decremente en uno
		if ( !(ctp->contador) )//si lleg? a cero
		{
			ctp->banderas |= TM_PER_B_TC;//subo la bandera
			ctp->contador  = ctp->periodo;// lo reseteo
		}
	}
	if ((ctp->timeout))//si ya es cero, no entra
	ctp->timeout = ctp->timeout-1;//decremente tambi?n el timeout
}

// Bandera que dice cuando se completa un periodo
char Tm_Hubo_periodico (Tm_Periodico *ctp) //le ingreso la estructura
{
	return (ctp->banderas & TM_PER_B_TC);
}

// Reinicia la bandera y la establece en cero
void Tm_Baje_periodico (Tm_Periodico *ctp)  //envio la estructura
{
	ctp->banderas &= ~TM_PER_B_TC;
}

// Desactivo el timer una vez no lo necesito
void Tm_Termine_periodico (Tm_Periodico *ctp)  //envio la estructura
{
	ctp->banderas &= ~TM_PER_B_ACTIVO;
}

// Bajo la bandera de timeout
char Tm_Hubo_timeout (Tm_Periodico *ctp)  //revise si hubo timeout
{
	if((ctp->timeout == 0) && (ctp->banderas &= TM_PER_B_ACTIVO))
	// Compararlo con 0
	{
		ctp->banderas &=~TM_PER_B_ACTIVO;
		ctp->banderas &= TM_Out_B_TC;
		return 1; //si es 0 retorne 1
	}
	else
	{
		return 0;	//si no retorne 0
	}
}

void Tm_Inicie_timeout (Tm_Periodico *ctp,unsigned int tiempo)
{
	// La mascara de la bandera se crea en el tiempo.h
	// puede tomar el mismo valor de la mascara de periodicos
	ctp->banderas |=  TM_PER_B_ACTIVO;
	ctp->banderas &= ~TM_Out_B_TC;  //bajo la bandera de timeout
	ctp->timeout   =  tiempo; //inicio el contador en el numero de conteos deseado
}
void Tm_Baje_timeout (Tm_Periodico *ctp)  //envio la estructura
{
	ctp->banderas &= ~TM_Out_B_TC; //atiendo la bandera una vez hubo periodo
	ctp->banderas &= ~TM_PER_B_ACTIVO;
}