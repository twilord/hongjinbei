#include <reg52.h>
#include <lcd.h>

sbit K1=P3^1;//向上
sbit K2=P3^4;//向左
sbit K3=P3^5;//向右
sbit K4=P3^6;//向下
sbit K5=P3^7;//开启编辑功能
sbit K6=P3^3;//显示定时功能

void keyscan_speed()
{
	if(K1==0)//改变相应的值
	{
		delayms(10);
		while(K1==0);
		switch(sit[0])
		{
			case 0:
				if(data1[0][sit[0]]<99)
					data1[0][sit[0]]++; 
				display_speed(data1[0][0]);
				break;
			case 1:
				if(data1[0][sit[0]]<60)
					data1[0][sit[0]]++;
				display_duty(data1[0][1],data1[0][2]);
				break;
			case 2:
				if(data1[0][sit[0]]<9)
					data1[0][sit[0]]++;
				display_duty(data1[0][1],data1[0][2]);
				break;
		}
		
	}
	if(K2==0 && K5==0)//开启编辑功能的时候才能使用
	{
		LcdWriteCom(0xcb);
		delayms(10);
		cursor_move(2,0);
	}
	if(K3==0 && K5==0)//开启编辑功能的时候才能使用
	{
		delayms(10);
		cursor_move(1,0);
	}
	if(K4==0)
	{
		delayms(10);
		while(K4==0);
		switch(sit[0])
		{
			case 0:
				if(data1[0][sit[0]]>0) 
					data1[0][sit[0]]--;
				display_speed(data1[0][0]);
				break;
			case 1:
				if(data1[0][sit[0]]>0)
					data1[0][sit[0]]--;
				display_duty(data1[0][1],data1[0][2]);
				break;
			case 2:
				if(data1[0][sit[0]]>0)
					data1[0][sit[0]]--;
				display_duty(data1[0][1],data1[0][2]);
				break;
		}
	}
}

void keyscan_time()
{
	if(K1==0)//改变相应的值
	{
		delayms(10);
		while(K1==0);
		switch(sit[1])
		{
			case 0:
				if(data1[1][sit[1]]<24)
					data1[1][sit[1]]++; 
				display_time(data1[1][0],data1[1][1]);
				break;
			case 1:
				if(data1[1][sit[1]]<60)
					data1[1][sit[1]]++;
				display_time(data1[1][0],data1[1][1]);
				break;
			case 2:
				if(data1[1][2]==0)
					data1[1][2]=1;
				else 
					data1[1][2]=0;
				display_time(data1[1][0],data1[1][1]);
				break;
		}
		
	}
	if(K2==0)//开启编辑功能的时候才能使用
	{
		delayms(10);
		cursor_move(2,1);
	}
	if(K3==0)//开启编辑功能的时候才能使用
	{
		delayms(10);
		cursor_move(1,1);
	}
	if(K4==0)
	{
		delayms(10);
		while(K4==0);
		switch(sit[1])
		{
			case 0,1:
				if(data1[1][sit[1]]>0) 
					data1[1][sit[1]]--;
				display_time(data1[1][0],data1[1][1]);
				break;
			case 2:
				if(data1[1][2]==0)
					data1[1][2]=1;
				else 
					data1[1][2]=0;
				display_time(data1[1][0],data1[1][1]);
				break;
		}
	}
}

void Int0_init()
{
	IT0=1;
	EX0=1;
	EA=0;
}

void Int1_init()
{
	IT1=1;
	EX1=1;
	EA=0;
}

uchar string[]={""}
int main()
{
	LcdInit();
	display_line();
	display_speed(data1[0][0]);
	display_duty(data1[0][1],data1[0][2]);
	LcdWriteCom(next[0][sit[0]]);
	Int0_init();
	Int1_init();
	while(1)
	{
		if(K5==0 || key_status[6]==0) //根据K5判断是否显示光标
		LcdWriteCom(0x0d);
		else 
		LcdWriteCom(0x0c);
	}
}

void Int0() interrupt 0
{  	
	if(key_status[6]==1)
	{
		keyscan_speed();
		LcdWriteCom(next[0][sit[0]]);
	}
	else 
	{
		keyscan_time();
		LcdWriteCom(next[1][sit[1]]);
	}
}

void Int1() interrupt 2
{
	delayms(10);
	if(K6==0)
	if(key_status[6]==1)
	{
		key_status[6]=0;
		LcdWriteCom(0x01);//清屏
		delayms(1);
		display_line2();//显示初始时间
		display_time(data1[1][0],data1[1][1]);//显示定时时间
	}
	else {
		key_status[6]=1;
		LcdWriteCom(0x01);
		display_line();
		display_speed(data1[0][0]);
		display_duty(data1[0][1],data1[0][2]);
		LcdWriteCom(next[0][sit[0]]);
	}
	LcdWriteCom(next[1][sit[1]]);
}