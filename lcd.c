#include <lcd.h>

void Lcd_Delay1ms(uint c)
{
	uchar a,b;
	for(;c>0;c--)
	{
		for(b=199;b>0;b--)
		{
			for(a=1;a>0;a--);
		}
	}
}

void LcdWriteCom(uchar com)
{
	LCD_E = 0;
	LCD_RS = 0;
	LCD_RW = 0;

	LCD_DATAPINS = com;
	Lcd_Delay1ms(1);

	LCD_E = 1;
	Lcd_Delay1ms(5);
	LCD_E =	0;
}

void LcdWriteData(uchar dat)
{
	LCD_E = 0;
	LCD_RS = 1;
	LCD_RW = 0;

	LCD_DATAPINS = dat;
	Lcd_Delay1ms(1);

	LCD_E = 1;
	Lcd_Delay1ms(5);
	LCD_E = 0;
}

void LcdInit()
{
	LcdWriteCom(0x38);
	LcdWriteCom(0x0d); //光标所在位置的字符闪烁
	LcdWriteCom(0x06);
	LcdWriteCom(0x01);
	LcdWriteCom(0x80);
}

uchar ones,tens;
uchar data1[2][3]={50,60,0,0,0,0};//速度 占空 占空 小时 分钟 定时开关
uchar line1[5]={"speed"};
uchar line2[10]={"duty cycle"};
uchar line3[14]={"timeset   h  m"};
uchar line4[]={"close open "};
uchar ascii[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
uchar next[2][3]={0x87,0xcb,0xcd,0x89,0x8c,0xc0};
uchar sit[]={0,0};
uchar key_status[]={0,1,1,1,1,1,1};//按键状态

void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

void cursor_move(uchar direction,uchar s)//光标移动
{
	switch(direction)
	{
		case 1:
			sit[s]++;
			sit[s]=sit[s]%3;
			LcdWriteCom(next[s][sit[s]]);	
			break;
		case 2:
			sit[s]+=3;
			sit[s]--;
			sit[s]%=3;
			LcdWriteCom(next[s][sit[s]]);
			break;
	}
}

void display_line()//把要显示的东西先显示到屏幕上
{
	uchar i;
	LcdWriteCom(0x80);
	delayms(1);
	for(i=0;i<5;i++)
	{	
		LcdWriteData(line1[i]);
	}
	LcdWriteCom(0xc0);
	for(i=0;i<10;i++)
	{
		LcdWriteData(line2[i]);
	}
	LcdWriteCom(0xcc);
	LcdWriteData('/');
	LcdWriteCom(0x85);
}

void display_line2()//将定时功能开启时的东西显示到屏幕上
{
	uchar i;
	LcdWriteCom(0x80);
	delayms(1);
	for(i=0;i<14;i++)
	{
		LcdWriteData(line3[i]);
	}
}

void display_time(uchar dat1,uchar dat2)//显示时间
{
	uchar i;
	if(dat1>24) dat1=24;
	if(dat2>60) dat2=60;
	if(dat1<0) dat1=0;
	if(dat2<0) dat2=0;
	tens = dat1/10;
	ones = dat1%10;
	LcdWriteCom(0x0c);//先关闭光标
	LcdWriteCom(0x88);
	LcdWriteData(ascii[tens]);
	LcdWriteData(ascii[ones]);
	tens = dat2/10;
	ones = dat2%10;
	LcdWriteCom(0x8b);
	LcdWriteData(ascii[tens]);
	LcdWriteData(ascii[ones]);
	LcdWriteCom(0xc0);
	if(data1[1][2]==1)//开启
		for(i=6;i<=10;i++)
			LcdWriteData(line4[i]);
	else 
		for(i=0;i<5;i++)
			LcdWriteData(line4[i]);
	LcdWriteCom(0x0d);
	LcdWriteCom(next[1][sit[1]]);
}

void display_speed(uchar dat)//显示速度
{  		
	if(dat>=100)
	{
		LcdWriteCom(next[0][0]-2);
		LcdWriteData(ascii[1]);
		LcdWriteData(ascii[0]);
		LcdWriteData(ascii[0]);
		return;
	}
	tens = dat/10;
	ones = dat%10;
	if(tens==0)
	{
		LcdWriteCom(next[0][0]-1);
		LcdWriteData(ascii[ones]);
	}
	else 
	{
		LcdWriteCom(next[0][0]-1);
		LcdWriteData(ascii[tens]);
		LcdWriteData(ascii[ones]);
	}	 
}

void display_duty(uchar dat1,uchar dat2)//显示占空比
{
	tens=dat1/10;
	ones=dat1%10;
	LcdWriteCom(next[0][1]-1);
	LcdWriteData(ascii[tens]);
	LcdWriteData(ascii[ones]);
	LcdWriteCom(next[0][2]);
	LcdWriteData(ascii[dat2]);
}
		   `