/*
Nuno Guterres Nogueira 
The fun of making something work
*/

#define F_CPU 1000000UL			

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCD16x2_4bit.h"

#define DHT11_PIN 6

#define FOSC 1000000 // Clock Speed
#define BAUD 4800
#define MYUBRR FOSC/16/BAUD-1

uint8_t c=0,I_RH=0,D_RH=0,I_Temp=0,D_Temp=0,CheckSum=0,k;

void USART_Init(uint32_t ubrr)
{
	/*Set baud rate */
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}


void USART_Transmit(uint8_t *data )
{
	k=0;
	while( data[k] != '\0' )
	{
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) );
		/* Put data into buffer, sends the data */
		UDR0 = data[k];
		k++;
	}
}

void Request()				/* Microcontroller send start pulse/request */
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
}

void Response()				/* receive response from DHT11 */
{
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

uint8_t Receive_data()			/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}

int main(void)
{
	USART_Init(MYUBRR);
	char data[5];
	lcdinit();			/* Initialize LCD */
	lcd_clear();			/* Clear LCD */
	lcd_gotoxy(0,0);		/* Enter column and row position */
	lcd_print("Humi.=");
	lcd_gotoxy(0,1);
	lcd_print("Temp.=");
	uint8_t teste[10];

  while(1)
	{

		Request();		/* send start pulse */
		Response();		/* receive response */
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */


		if ((I_RH + D_RH + I_Temp + D_Temp) == CheckSum)
		{
			itoa(I_RH,data,10);
			lcd_gotoxy(7,0);
			lcd_print(data);

			lcd_print("%");

			itoa(I_Temp,data,10);
			lcd_gotoxy(7,1);
			lcd_print(data);
			lcd_print(".");

			itoa(D_Temp,data,10);
			lcd_print(data);
			lcddata(0xDF);
			lcd_print("C");

		}

		_delay_ms(2000);


	}
}
