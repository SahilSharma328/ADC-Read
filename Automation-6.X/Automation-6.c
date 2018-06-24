/*
 * File:   Automation-6.c
 * Author: Sahil
 *
 * Created on 12 January, 2018, 1:10 PM
 */

// PIC18F45K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = HSMP      // Oscillator Selection bits (HS oscillator (medium power 4-16 MHz))
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = OFF   // Primary clock enable bit (Primary clock can be disabled by software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = OFF     // HFINTOSC Fast Start-up (HFINTOSC output and ready status are delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//Working With Accelerometer

#define _XTAL_FREQ 16000000
#include <string.h>
#include <xc.h>

#define led1 PORTAbits.RA4

void uart_init1(unsigned long);
void uart_num1(unsigned int);
void uart_string1(unsigned char*);
void adc_init();
unsigned int adc_read(unsigned char);
void uart_send1(unsigned char);

void uart_init1(unsigned long baud)
{
    unsigned int n;
    TXSTA1 = 0x00;						//Transmit States
    RCSTA1 = 0x00;						//Receive States 
    TXSTAbits.SYNC = 0;               // Asynchronous mode
    BAUDCON1bits.BRG16 = 1;          // Use SPBREGH and SPBREG
    RCSTA1bits.SPEN = 1;           // Enable tx and rx pins
    RCSTA1bits.CREN = 1;           // Enable continous receive mode
    TXSTA1bits.BRGH = 1;           // High Speed transmission enable
    TXSTA1bits.TXEN = 1;           // Enable transmission
    
    n = ((_XTAL_FREQ/4)/baud) - 1;
    SPBRGH1 = (n>>8);
    SPBRG1 = n & 0xFF;
    
   
}

void uart_string1(unsigned char *str)
{
	while((*str)!='\0')
	{
		while(!TX1IF);
		TXREG1=(*str);
		str++;
	}
}

void uart_send1(unsigned char ch)
{
	while(!TX1IF);
	TXREG1=ch;
}

void uart_num(unsigned int num)
{
    unsigned char n[16], i, j;
    i = 0;
    do
    {
        n[i] = (num % 10) + 48;
        num = num / 10;
        i++;
    }while(num!=0);
	
	for(j=i;j>0;j--)
	{
		uart_send1(n[j-1]);
	}
}

void adc_init()
{
    ADCON1 = 0x00;
    ADCON2 = 0x8A;
    ADCON3 = 0x8A;
}

unsigned int adc_read(unsigned char channel)
{
  if(channel > 5)  
    return 0;     

  // ADCON0 &= 0xC5; 
  ADCON0 = 0x00;
  ADON = 1;
  ADCON0 |= (channel<<2); 
  __delay_ms(2); 
  ADCON0bits.GO = 1; 
  while(ADCON0bits.DONE == 1); 
  ADON = 0;
  return (ADRES); 
}

void main() 
{
    unsigned int x,y,z,i,j,k=64;
    ANSELC=0;
    uart_init1(9600);
    adc_init();
    while(1)
    {
        x=adc_read(0);
        uart_string1("x=");
        uart_num(x);
        __delay_ms(300);
    
        y=adc_read(1);
        uart_string1("y=");
        uart_num(y);
        __delay_ms(300);
    
        z=adc_read(2);
        uart_string1("z=");
        uart_num(z);
        __delay_ms(300);
    }
}