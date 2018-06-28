/*
 * Motorfree.c
 *
 * Created: 16/11/2014 10:20:05 p.m.
 *  Author: Administrador
 */ 


//#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include <util/delay.h>
#include <stdio.h>




int c[101];
int a;
int b;
int d;
int delay1;
int delay2;
int delay3;

ISR(TIMER0_OVF_vect)
{TCNT0 = 0xEF;
	PORTC ^= (1 << 7);
}

void controlMotor(void) //Tarea para activar componentes de los sensores no activos
{
int k=0,i=0,h=0,j=0;
	while(1){
	
	if ((PINC) & (1<<PC0)) //verifico si hay un uno en el PINA1, pin de seno.
		{int e1, e2, e3;
			k=0;
			i=0;
			j=0;
			h=0;
			a=0;
			b=0;
			d=0;
			c[0]=0;
			PORTK=0X00;
			PORTL=0X00;
			PORTA=0X00;
			PORTJ=0X00;
			TCNT1=0x00;
			
			if ((PINC) & (1<<PC1)) //verifico si hay un uno en el PINA1, pin de seno.
			{	e1=1;
				
			}
			else{
				e1=0;
			}
			
			if ((PINC) & (1<<PC2)) //verifico si hay un uno en el PINA1, pin de seno.
			{	e2=1;
				
			}
			else{
				e2=0;
			}
			
			if ((PINC) & (1<<PC3)) //verifico si hay un uno en el PINA1, pin de seno.
			{	e3=1;
				
			}
			else{
				e3=0;
			}
			delay1=25+(e1*40)+(e2*64)+(e3*78);
			delay2=32+(e1*32)+(e2*138)+(e3*168);
			delay3=32+(e1*114)+(e1*202)+(e1*254);
			//delay1=128+(e1*320)+(e2*512)+(e3*624);
			//delay2=128+(e1*656)+(e2*1104)+(e3*1344);
			//delay3=128+(e1*912)+(e2*1616)+(e3*2032);
			//
		}
		else{
		
		
		
		
		
		
		if (TCNT1>=delay1)
		{a=1;
		}
		if (TCNT1>=delay2)
		{b=1;
		}
		if (TCNT1>=delay3)
		{d=1;
		}
	if (a==1)
		{PORTL=c[i];
			i++;
			if (i>=99)
			{i=0;
			}
		}
		if (b==1)
		{PORTA=c[j];
			j++;
			if (j>=99)
			{j=0;
			}
		}
		if (d==1)
		{PORTJ=c[h];
			h++;
			if (h>=99)
			{h=0;
			}
		}
		
		PORTK=c[k];
		k++;
		if (k>=99)
		{k=0;
		}
	}
	}
		
	}


void getData(void) // Tarea para sumar componetes de los sensores no activos,girar el eje cordenado con la matriz y cambiar la velocidad de los motores
{
	int m=0;
	int temperature;
	char garbage;
	while (1)
	{
		ADCSRA |= (1 << ADSC);
		while(!(ADCSRA & (1<<ADIF)))	// Wait ADC...
		{
			
		}
		ADCSRA|=(1<<ADIF);				// Clear EOC
		//temperature = ADCL>>2;
		temperature = (ADCH<<6)|(ADCL>>2);			// Saving ADC
		garbage = ADCL;
		garbage= ADCH;
		c[m]=(temperature);
		vTaskDelay(50);
		
	}
		

	}






int main(void)
{
	//inicializacion de puertos
	
	DDRC = 0b11110000;
	DDRL = 0xFF;
	DDRK = 0xFF;
	DDRJ = 0xFF;
	DDRA = 0xFF;
	DDRF |= 0b11110000;
	/* adc initialization */
	ADCSRA |= (1 << ADPS0);
	ADMUX &= 0x00;
	//ADMUX |= ((0<< REFS1)) | (1 << REFS0);
	
	TCCR0A |= ((1 << WGM00)|(1 << WGM01));
	TCCR0B |= (1 << CS01);
	TIMSK0 |= (1 << TOIE0);
	//TCCR1B |= (1 << CS10);
	TCCR1B |= 0x02;
	
	
	
	
	//Set ADC to free run mode
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
		
	// Enables ADC for use
	ADCSRA |= (1 << ADEN);
	
	// ADC Auto Trigger Enable
	ADCSRA |= (1 << ADATE);
	
	sei();
	
	//Inicializacion de las tareas
	xTaskCreate( controlMotor, (signed portCHAR *)"motor", configMINIMAL_STACK_SIZE,
	NULL, tskIDLE_PRIORITY, ( xTaskHandle * ) NULL ); 
	
	xTaskCreate( getData, (signed portCHAR *)"data", configMINIMAL_STACK_SIZE,
	NULL, tskIDLE_PRIORITY, ( xTaskHandle * ) NULL ); 
		
	vTaskStartScheduler(); 

	return 0;
}
