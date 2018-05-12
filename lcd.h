#ifndef __LCD_H_
#define __LCD_H_

#include <reg52.h>
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#define LCD_DATAPINS P0

sbit LCD_E=P2^2;
sbit LCD_RW=P2^1;
sbit LCD_RS=P2^0;

void Lcd_Delay1ms(uint);
void LcdWriteCom(uchar);
void LcdWriteData(uchar);
void LcdInit();
void display_line();
void display_line2();
void display_speed(uchar);
void display_duty(uchar,uchar);
void display_time(uchar,uchar);
void delayms(uint);
void cursor_move(uchar ,uchar );

extern uchar sit[];
extern uchar ones,tens;
extern uchar line1[5];
extern uchar line2[10];
extern uchar ascii[10];
extern uchar next[2][3];
extern uchar data1[2][3];
extern uchar key_status[];

#endif