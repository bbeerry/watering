#include<wiringPi.h>
#include<stdio.h>
unsigned char ndht(int,unsigned char * dat);
unsigned char main()
{
	int point=0;
	unsigned char dat[5],i=0;
	wiringPiSetup();
	while(ndht(point,dat)==33)
	{
		printf("try again");
		i++;
		if(i==5)
		{
			printf("data validation error");
			return 0;
		}
		delay(500);
	}
	i=0;
	while(ndht(point,dat)==21)
	{	
		printf("try again");
		i++;
		if(i==5)
		{
			printf("error in check.");
			return 0;
		}
		delay(500);
	}
		
	printf("humidity:%d.%d%\ntemp:%d.%d'C\n",dat[0],dat[1],dat[2],dat[3]);
	return 1;
}
