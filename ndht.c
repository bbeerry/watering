#include<wiringPi.h>
unsigned char ndht(int dhtpin,unsigned char * dat)
{
	unsigned int i=0,n=0,time,dtime[41],max,mid,min;
	unsigned char check=0;
	pinMode(dhtpin,OUTPUT);
	digitalWrite(dhtpin,0);
	delay(24);
	digitalWrite(dhtpin,1);
	pinMode(dhtpin,INPUT);
	time=micros();
	while(digitalRead(dhtpin)==1)
	{
		if((micros()-time)>50)
			return 33;
	}
	for(i=0;i<41;i++)
	{
		time=micros();
		while(digitalRead(dhtpin)==0);
		while(digitalRead(dhtpin)==1)
		{ if((micros()-time)>200)	break;}
		dtime[i]=(micros()-time);
	}
	max=min=dtime[1];
	for(i=1;i<41;i++)
	{
		if(max<dtime[i])max=dtime[i];
		if(min>dtime[i])min=dtime[i];
	}
	for(i=1,mid=0;i<41;i++)
		mid=mid+dtime[i];
	mid=(mid-min-max)/38;
	for(n=0,i=1;i<41;i++)
	{
		dat[n]<<=1;
		if(dtime[i]>mid) dat[n]|=0x01;
		else dat[n]&=~0x01;
		if(i%8==0)	n++;
	}
	if(dat[0]+dat[2]==dat[4])
		return 22;
	else return 21;
}
