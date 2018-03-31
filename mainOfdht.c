#include<wiringPi.h>
#include<stdio.h>
unsigned char dht11(unsigned char * dat);
unsigned char main()
{
	unsigned char dat[4],i=0;
	wiringPiSetup();
	while(!dht11(dat))
	{
		i++;
		if(i==5)
		{
			printf("data validation error");
			return 0;
		}
		delay(1000);
	}
	printf("humidity:%d.%d%\ntemp:%d.%d'C\n",dat[0],dat[1],dat[2],dat[3]);
	return 1;
}
