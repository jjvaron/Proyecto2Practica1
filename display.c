
#include "display.h"

void D_inicie_display (D_Display *disp,int8_t *tempUnidades,int8_t *tempDecenas)
{
    disp->tempUnidades=tempUnidades;
    disp->tempDecenas=tempDecenas;
    disp->mostrando=UNIDADES;
}


void D_Procese_display (D_Display *disp)
{
    switch(disp->mostrando){
        case UNIDADES://mostramos unidades
            disp->mostrando=DECENAS;// cambiar estado
            //muestro la conversión a unidades leidas por el ADC y convertidas
            muestre_en_display(*disp->tempUnidades,UNIDADES );

        break;
        case DECENAS://mostrar decenas
            disp->mostrando=UNIDADES;// cambiar estado
            muestre_en_display(*disp->tempDecenas,DECENAS );
        default:
            disp->mostrando=DECENAS;
    }
}

void muestre_en_display(uint8_t digito  , uint8_t display )
{
    /*configura el puerto o los puertos que esten usando para los display*/
	DDRD = 0b01111111; //Se establecen los pines de salida del puerto D
	DDRB = 0b01111111; //Se establecen los pines de salida del puerto B

    /*primero bajar todos los bits del display en el puerto*/
    PORTB &= 0b00000000;
    /*subir solo los necesarios*/
	PORTB |= 0b01111111;
    /*primero bajar todos los bits del display en el puerto*/
    PORTD &= 0b00000000;
    /*subir solo los necesarios*/
	PORTD |= 0b01111111;
    /*y dependiendo del valor de display enciende el transistor correspondiente*/
    if(display==UNIDADES){
        PUERTOBDISPDECENAS &= ~TRANDISPDECENAS;
        PUERTODISPUNIDADES |= TRANDISPUNIDADES;
    }
    if(display==DECENAS){
        PUERTODISPUNIDADES &= ~TRANDISPUNIDADES;
        PUERTOBDISPDECENAS |= TRANDISPDECENAS;
    }
}
/*
     a
     _
f | g_  | b
e |  _  | c
     d
anodo o catodo?? prende con 1 o con 0??? prende con 1
0b00111111  // muestro un 0
0b00000110  // muestro un 1
0b01001011 //2
0b01001111 //3
0b01100110 //4
0b01101101 //5
0b01111101 //6
0b01000111 //7
0b01111111 //8
0b01101111 //9
0b01000000 // signo menos
0b01111001 // Error
*/
tablaBCD[12]={0b0011111,
              0b00000110,
              0b01001011,
              0b01001111,
              0b01100110,
              0b01101101,
              0b01111101,
              0b01000111,
              0b01111111,
              0b01101111,
              0b01000000,
              0b01111001};/*del 0 al 9 y menos y error*/

int8_t num2BCD(int8_t num)
{
    if(num>=12||num<0)
        return tablaBCD[11];/*en la pos 11 de la tabla esta el error*/
    return tablaBCD[num];
}
