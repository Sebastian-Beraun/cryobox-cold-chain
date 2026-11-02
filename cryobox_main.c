/*
 * CryoBox - Portable cold-chain vaccine carrier
 * MCU: PIC18F57Q43 | IDE: MPLAB X IDE | Compiler: XC8
 *
 * Transcribed from screenshots in the project report (Figure 2).
 * Required local headers (not shown in the report, must come from the
 * original project): cabeza.h, LCD.h, DHT22.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f57q43.h>
#include "cabeza.h"
#include "LCD.h"
#include "DHT22.h"
#include <xc.h>

#define _XTAL_FREQ 16000000UL

// --- PD ---
#define MOTOR_IN1  LATAbits.LATA0
#define MOTOR_IN2  LATAbits.LATA1
#define MOTOR_STBY LATAbits.LATA2

float KP          = 12.0;
float KD          = 3.0;
float setpoint    = 31.5;   // NOTE: value shown in the report capture (test value?)
float ultimo_error = 0;

void Ventilador(int pwm)
{
    MOTOR_STBY = 1;
    MOTOR_IN1 = 1;
    MOTOR_IN2 = 0;
    if(pwm > 255) pwm = 255;
    if(pwm < 0)   pwm = 0;
    CCPR1L = pwm;
}
// ---------

void configuro(void)
{
    OSCCON1 = 0x60;
    OSCFRQ  = 0x05;
    OSCEN   = 0x40;

    // LCD
    TRISD  = 0x00;
    ANSELD = 0x00;
    LATD   = 0x00;

    // Motor direction
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;

    // PWM RC2 - CCP1
    TRISCbits.TRISC2   = 0;
    ANSELCbits.ANSELC2 = 0;
    RC2PPS             = 0x15;
    CCP1CON            = 0x9C;
    CCPTMRS0bits.C1TSEL = 0x1;
    PR2                = 0xFF;
    CCPR1L             = 0;
    T2CLKCON           = 0x01;
    T2CON              = 0xC0;
    T2CONbits.TMR2ON   = 1;

    // DHT22
    TRISF  = 0xFF;
    ANSELF = 0x00;

    // Alert outputs
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;
    ANSELA = 0;
}

void LCDinit(void)
{
    __delay_ms(20);
    LCD_init();
}

float Tprom, Hprom;

void main(void)
{
    configuro();
    LCDinit();
    MOTOR_STBY = 1;

    while(1)
    {
        DHT22_Leer(1);
        __delay_ms(2000);
        DHT22_Leer(2);
        __delay_ms(2000);

        LCDCLC();
        if(DHT1_error == 0 && DHT2_error == 0)
        {
            Tprom = (DHT1_temperatura + DHT2_temperatura) / 2.0;
            Hprom = (DHT1_humedad     + DHT2_humedad)     / 2.0;

            LCDXY(0,0);
            LCDTEXTO("Temp: ");
            LCDREAL(Tprom);
            LCDCHAR(0xDF);
            LCDCHAR('C');
            LCDXY(1,0);
            LCDTEXTO("Hum: ");
            LCDREAL(Hprom);
            LCDCHAR('%');

            // --- Control PD ---
            float proporcional  = Tprom - setpoint;
            float derivativa    = proporcional - ultimo_error;
            ultimo_error        = proporcional;
            float poder_control = (KP * proporcional) + (KD * derivativa);

            if(poder_control > 255) poder_control = 255;
            if(poder_control < 0)   poder_control = 0;

            if(Tprom > setpoint)
                Ventilador((int)poder_control);
            else
                Ventilador(0);
        }
        else
        {
            LCDXY(0,0);
            LCDTEXTO("Err1:");
            LCDENTERO(DHT1_error);
            LCDXY(1,0);
            LCDTEXTO("Err2:");
            LCDENTERO(DHT2_error);

            Ventilador(0); // Safety: turn off if there is a sensor error
        }

        // Over-temperature alert (> 8 C)
        if(Tprom > 8)
        {
            LATAbits.LATA3 = 1;
            LATAbits.LATA4 = 1;
        }
        else
        {
            LATAbits.LATA3 = 0;
            LATAbits.LATA4 = 0;
        }
    }
}
