/*
 * Atmega1.cpp
 *
 * Created: 18-12-2023 7.31.17 PM
 * Author : BIJO
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>





void display(const char *p);
void command(char a);
void data(char b);
void display(const char *p){
	while(*p!='\0'){
		data(*p);
		p++;
	}
}
void command(char a){
	PORTD = a;
	PORTC = 0x02;
	_delay_ms(100);
	PORTC = 0x00;
}

void data(char b){
	PORTD = b;
	PORTC = 0x03;
	_delay_ms(100);
	PORTC = 0x01;
}


int main(void)
{
	
    DDRC = 0xFF;
	DDRD = 0xFF;//Taken as output in the case of pic it will change
	DDRB = 0x0F;//TAKEN AS INPUT FOR MOTOR
	DDRA = 0x00;
	
	
	
	command(0x01);
	command(0x0E);
	command(0x06);
	command(0x38);
	command(0x84);
	display("ATMEGA");
	command(0xC3);
	display("CACULATOR");
	_delay_ms(2000);
	command(0x01);
	
	
	char pass[20];
	char get[20];
	int count = 0;		
	int minusof = 0;
	int plus = 0;
	int minus = 0;
	int mult = 0;
	int divs = 0;
	int flag = 0;
	char resultString[10];
    while (1) 
    {
		    if((PINA&0x01) == 0){
				flag = 1;
				display("(-)");
				minusof++;
			}
			PORTB = 0x0E;
			if((PINB&0x80) == 0){
				data('0');
				pass[count++] = '0';
				while((PINB&0x80)==0);
			}else if((PINB&0x40) == 0){
				data('1');
				pass[count++] = '1';
				while((PINB&0x40)==0);
			}else if((PINB&0x20) == 0){
				data('2');
				pass[count++] = '2';
				while((PINB&0x20)==0);
			}else if((PINB&0x10) == 0){
				data('3');
				pass[count++] = '3';
				while((PINB&0x10)==0);
			}
			PORTB = 0x0D;
			if((PINB&0x80) == 0){
				data('4');
				pass[count++] = '4';
				while((PINB&0x80)==0);
			}else if((PINB&0x40) == 0){
				data('5');
				pass[count++] = '5';
				while((PINB&0x40)==0);
			}else if((PINB&0x20) == 0){
				data('6');
				pass[count++] = '6';
				while((PINB&0x20)==0);
			}else if((PINB&0x10) == 0){
				data('7');
				pass[count++] = '7';
				while((PINB&0x10)==0);
			}
			PORTB = 0x0B;
			if((PINB&0x80) == 0){
				data('8');
				pass[count++] = '8';
				while((PINB&0x80)==0);
			}else if((PINB&0x40) == 0){
				data('9');
				pass[count++] = '9';
				while((PINB&0x40)==0);
			}else if((PINB&0x20) == 0){
				command(0x8F);
				plus =  1;
				data('+');
				for(int i = 0;i<count + 1;i++){
					get[i] = pass[i];
				}
				   
				for(int i = 0;i<count;i++){
					pass[i] = 0;
				}
				count = 0;
				while((PINB&0x20)==0);
				command(0xC0);
			}else if((PINB&0x10) == 0){
				command(0x8F);
				minus = 1;
				data('-');
				for(int i = 0;i<count + 1;i++){
					get[i] = pass[i];
				}
				
				for(int i = 0;i<count;i++){
					pass[i] = 0;
				}
				count = 0;
				while((PINB&0x10)==0);
				command(0xC0);
			}
			PORTB = 0x07;
			if((PINB&0x80) == 0){
				command(0x8F);
				mult = 1;
				data('X');
				for(int i = 0;i<count + 1;i++){
					get[i] = pass[i];
				}
				
				for(int i = 0;i<count;i++){
					pass[i] = 0;
				}
				count = 0;
				while((PINB&0x80)==0);
				command(0xC0);
			}else if((PINB&0x40) == 0){
				command(0x8F);
				divs = 1;
				data('/');
				for(int i = 0;i<count + 1;i++){
					get[i] = pass[i];
				}
				
				for(int i = 0;i<count;i++){
					pass[i] = 0;
				}
				count = 0;
				while((PINB&0x40)==0);
				command(0xC0);
			}else if((PINB&0x20) == 0){
				minusof = 0;
				command(0x01);
				display("CLEARING");
				_delay_ms(500);
				command(0x01);
				count = 0;
				for (int i = 0; i < 20; i++) {
					pass[i] = 0;
				}

			}else if((PINB&0x10) == 0){
				if(plus == 1){
					if(flag!=1){
						int sum = atoi(get);
						int sum1 = atoi(pass);
						int Result;
						Result = sum + sum1;
						sprintf(resultString, "%d", Result);
						command(0xC7);
						data('=');
						display(resultString);
						plus = 0;
					}
					else{
						int sum = atoi(get);
						int sum1 = atoi(pass);
						int Result;
						Result = (-sum) + sum1;
						sprintf(resultString, "%d", Result);
						command(0xC7);
						data('=');
						display(resultString);
						flag = 0;
					}
					
				}
				if(minus == 1){
					if(flag!=1){
						int min = atoi(get);
						int min1 = atoi(pass);
						int Result;
						Result = min - min1;
						sprintf(resultString, "%d", Result);
						command(0xC7);
						data('=');
						display(resultString);
						minus = 0;
					}
					else{
						int min = atoi(get);
						int min1 = atoi(pass);
						int Result;
						Result = (-min) - min1;
						sprintf(resultString, "%d", Result);
						command(0xC7);
						data('=');
						display(resultString);
						flag = 0;
					}
					
				}
				if(mult == 1){
					if(flag!=1){
						int mul = atoi(get);
						int mul1 = atoi(pass);
						int Result;
						Result = mul * mul1;
						sprintf(resultString, "%d", Result);
						command(0xC7);
						data('=');
						display(resultString);
						mult = 0;
						
					}
					 else{
						 if(minusof == 1){
							 int mul = atoi(get);
							 int mul1 = atoi(pass);
							 int Result;
							 Result = (-mul) * mul1;
							 sprintf(resultString, "%d", Result);
							 command(0xC7);
							 data('=');
							 display(resultString);
							 mult = 0;
							
						 }else if(minusof == 2){
							 int mul = atoi(get);
							 int mul1 = atoi(pass);
							 int Result;
							 Result = (-mul) * (-mul1);
							 sprintf(resultString, "%d", Result);
							 command(0xC7);
							 data('=');
							 display(resultString);
							 mult = 0;
							
						 }
						
					}
					
				}
				if(divs == 1){
					if(flag!=1){
						double div= atof(get);
						double div1 = atof(pass);
						double Result;
						if(div1>0){
							Result = div / div1;
							int scalingFactor = 100;  // for two decimal places
							int ResultInt = (int)(Result * scalingFactor);

							snprintf(resultString, 20, "%d.%02d", ResultInt / 100, ResultInt % 100);

							command(0xC7);
							data('=');
							display(resultString);
							divs = 0;
						}
						else{
							if(div1 == 0){
								command(0x01);
								command(0x80);
								display("VALUE NOT BE ");
								command(0xC4);
								display("CORRECT");
								_delay_ms(1500);
								command(0x01);
							}
							
						}
					 }
					  else if(flag == 1){
						  double div= atof(get);
						  double div1 = atof(pass);
						  double Result;
						  if(div1>0){
							  if(minusof == 1){
								  Result = (-div) / div1;
								  int scalingFactor = 100;  // for two decimal places
								  int ResultInt = (int)(Result * scalingFactor);

								  snprintf(resultString, 20, "%d.%02d", ResultInt / 100, ResultInt % 100);

								  command(0xC7);
								  data('=');
								  display(resultString);
								  flag = 0;
							  }if(minusof == 2){
								  Result = (-div) / (-div1);
								  int scalingFactor = 100;  // for two decimal places
								  int ResultInt = (int)(Result * scalingFactor);

								  snprintf(resultString, 20, "%d.%02d", ResultInt / 100, ResultInt % 100);

								  command(0xC7);
								  data('=');
								  display(resultString);
								  flag = 0;
							  }
							 
							 
						  }
						 
						   
						 
						   
					
					
					}
					
				}
			
					
	   
					
					
				
			}
    }
}

