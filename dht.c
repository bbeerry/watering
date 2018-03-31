#include<wiringPi.h>
#define POINT 0
unsigned char dht11(unsigned char * dat)
{
	unsigned int i=0,time,bitTime[41],max,min,mid=0;
	unsigned char check=0;
	pinMode(POINT,OUTPUT);
	digitalWrite(POINT,0);
	delay(30);
	digitalWrite(POINT,1);
	pinMode(POINT,INPUT);
	time=micros();
	while(digitalRead(POINT)==1)
	{
		if((micros()-time)>50) return 2;
	}
	for(i=0;i<41;i++)
	{
		time=micros();
		while(digitalRead(POINT)==0);
		while(digitalRead(POINT)==1)
		{
			if((micros()-time)>300) break;
		}
		bitTime[i]=micros()-time;
		//bitTime is origin 40bit digital
	}
	max=min=bitTime[1];
	for(i=1;i<41;i++)
	{
		if(bitTime[i]>max)max=bitTime[i];
		if(bitTime[i]<min)min=bitTime[i];
	}
	for(i=1;i<41;i++)
		mid+=bitTime[i];
	mid-=max;
	mid-=min;
	mid/=38;
	for(i=1;i<41;i++)
	{
		if(bitTime[i]>(mid)) bitTime[i]=1;
		else bitTime[i]=0;
	}

	for(i=0;i<8;i++)
	{
		dat[0]<<=1;
		bitTime[i+1]?(dat[0]|=0x01):(dat[0]&=~0x01);
		dat[1]<<=1;
		bitTime[i+9]?(dat[1] |= 0x01):(dat[1] &= ~0x01);
		dat[2]<<=1;
    bitTime[i+17]?(dat[2] |= 0x01):(dat[2] &= ~0x01);
		dat[3]<<=1;
    bitTime[i+25]?(dat[3] |= 0x01):(dat[3] &= ~0x01);
		check<<=1;
    bitTime[i+33]?(check |= 0x01):(check &= ~0x01);
	}
	if(dat[0]+dat[2]==check)
	{
		return 1;
	}
	else 
		return 0;
}	
