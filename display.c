/* Ruler 1         2         3         4         5         6         7        */
/*
 * Declaración de funciones previamente enunciadas en la libreria de
 * definiciones y configuraciones
 */

 /* ********************************* Headers ******************************** */

 /* --------------------- Inclusion de librerias estandar -------------------- */
 #include "display.h"
 
 /* ******************************** Funciones ******************************* */
 
 /* ----------------------------- Manejo Display  ---------------------------- */
// Separa las unidades y decenas del valor de temperatura
void D_inicie_display (D_Display *disp, uint8_t *temperatura)
{
	disp->tempUnidades = *temperatura%10;
    disp->tempDecenas  = *temperatura/10;
}
   
// Apaga el display antes de cambiar de numero
void D_apague_display() 
{
	// baja todos los pines definidos en los puertos
	PORTB &= 0x00;
	PORTC &= 0x00; 
	PORTD &= 0x00; 
}
// Esto es para definir el estado inicial
void D_Inicie_Estado(D_Display *disp)
{
	disp->mostrando    = UNIDADES;
}

 /* -------------------------------- Display --------------------------------- */
// Mostrar los numeros en el 7-seg
void D_Procese_display (D_Display *disp) //, int8_t valor)
{
	switch(disp->mostrando)
	{
		case UNIDADES: //mostramos unidades
			D_apague_display();
		    // Puerto en 1 desde protoboard
		    PORTD |= TRANDISPUNIDADES;
		    PORTC &=~ TRANDISPDECENAS;
		    PORTB |= num2BCDPUERTOB(disp->tempUnidades);
		    PORTC |= num2BCDPUERTOC(disp->tempUnidades);
		    PORTD |= num2BCDPUERTOD(disp->tempUnidades);
		  
            disp->mostrando=DECENAS;// cambiar estado
        break;
	  
        case DECENAS: //mostrar decenas
			D_apague_display();
			// Puerto en 1 desde protoboard
			PORTD &=~ TRANDISPUNIDADES; //0b00010000;
			PORTC |= TRANDISPDECENAS;
			PORTB |= num2BCDPUERTOB(disp->tempDecenas);
			PORTC |= num2BCDPUERTOC(disp->tempDecenas);
			PORTD |= num2BCDPUERTOD(disp->tempDecenas);
			
            disp->mostrando=UNIDADES;// cambiar estado
	    break;
	  
        default:
            disp->mostrando=DECENAS;
	}
}
 
/* -----------------------------  Look-up tables ----------------------------- */

uint8_t num2BCDPUERTOB(uint8_t num)
{
	//PORB SEG                  XXXXXFAB
	//PORB PIN                  76543210
	uint8_t tablaPuertoB[12] = {0b00000111, // 0
		0b00000001,  // 1
		0b00000011,  // 2
		0b00000011,  // 3
		0b00000101,  // 4
		0b00000110,  // 5
		0b00000110,  // 6
		0b00000011,  // 7
		0b00000111,  // 8
		0b00000111,  // 9
		0b00000000,  // Signo menos -
	0b00000110}; // E de error
	
	if(num>=12||num<0)
	return tablaPuertoB[11];/*en la pos 11 de la tabla esta el error*/
	return tablaPuertoB[num];
}

uint8_t num2BCDPUERTOC(uint8_t num)
{
	//PORC PIN                 XXXXXX+G // +=DECENAS
	//PORC PIN                 76543210
	uint8_t tablaPuertoC[12] = {0b00000000, // 0
		0b00000000,  // 1
		0b00000001,  // 2
		0b00000001,  // 3
		0b00000001,  // 4
		0b00000001,  // 5
		0b00000001,  // 6
		0b00000000,  // 7
		0b00000001,  // 8
		0b00000001,  // 9
		0b00000001,  // -
	0b00000001}; // E
	
	if(num>=12||num<0)
	return tablaPuertoC[11];/*en la pos 11 de la tabla esta el error*/
	return tablaPuertoC[num];
}

uint8_t num2BCDPUERTOD(uint8_t num)
{
	//PORD PIN                CDE-XXXX // -=UNIDADES
	//PORD PIN                76543210
	uint8_t tablaPuertoD[12] = {0b11100000, // 0
		0b10000000,  // 1
		0b01100000,  // 2
		0b11000000,  // 3
		0b10000000,  // 4
		0b11000000,  // 5
		0b11100000,  // 6
		0b10000000,  // 7
		0b11100000,  // 8
		0b11000000,  // 9
		0b00000000,  // -
	0b01100000}; // E
	
	if(num>=12||num<0)
	return tablaPuertoD[11];/*en la pos 11 de la tabla esta el error*/
	return tablaPuertoD[num];
}