#define F_CPU 16000000UL
#include "ADC lib.h"
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

void capturarADC(unsigned char* res){


	if (ADCSRA&0x10)
	{
		tmp0=ADCL;
		tmp1=ADCH&0x03;
		*res=(unsigned char)((((unsigned int)tmp1<<8)|((unsigned int)tmp0))>>2);
		ADCSRA &= ~(1<<ADIF);
	}

}

void prom_10datos(float* d, float* v){
	*d=0.0;

	for (int i=0; i<10; i++)
	{
		*d=*d+v[i];
	}

	*d=*d/10.0;

}




void calcDeviation(float prom, unsigned char* v, float* final)
{
	/*El cuadrado de la desviación estandar S es igual al cociente entre la sumatoria del cuadrado de cada elemento menos el promedio y la cantidad de elementos menos 1.
	http://en.wikipedia.org/wiki/Standard_deviation
	es decir, para éste contexto se tiene S^2 = S(tmp[i]-prom)/7 */
	float sum =0;
	for(int i=0; i<10; i++)
		sum += pow((v[i]-prom)/100, 2);
	*final= sqrt(sum/10);

}

float des(unsigned char *v, float* prom){


	float final=0;

	float sum =0;
	for(int i=0; i<10; i++)
	sum += pow((v[i]-*prom)/100, 2);
	final= sqrt(sum/10);
	return final;
}

void mostrarLamp(unsigned char d){

	switch ((int)d)
	{int k;
		case 0:
		_delay_ms(260);
		_delay_ms(260);
		_delay_ms(260);
		_delay_ms(260);
		_delay_ms(260);
		PORTD=0b00000000;
		d=0;
		break;

		case 1:
		PORTD=0b00000001;
				k=1;
				d=1;
		break;

		default:
		PORTD=0b01010101;
		break;
	}
}
