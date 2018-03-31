/*调用时提供一个四个元素的unsigned char数组的地址*/ 
#include<wiringPi.h>
#define POINT 0
unsigned char dht11(unsigned char * dat)
{
	unsigned int i=0,time,bitTime[41],max,min,mid=0;
	unsigned char check=0;
	pinMode(POINT,OUTPUT);//启用引脚，现在是输出模式
	//printf("press ENTER to send start signal\n");
	//printf("starting!\n");
	digitalWrite(POINT,0);//主机发送开始信号
	delay(30);//拉低总线后等待一段时间，以确保DHT能检测到起始信号,delay的单位是ms
	digitalWrite(POINT,1);//主机拉高总线，结束开始信号
	//printf("start signal over,waiting for get messages...\n");
	pinMode(POINT,INPUT);//切换到输入模式,在输入模式，平时是高电平
	time=micros();//micros()返回unsigned int的从setup到现在的时间，单位是us
	while(digitalRead(POINT)==1)//有这一段代码是因为传感器会发送低电平响应信号
	{
		if((micros()-time)>50) return 2;//延时等待读取回应信号，如果超过50us没回应可能就有问题
	}
	//printf("arready get response signal\n");
	//dht发送响应信号后，再把总线拉高，准备发送数据
	//发送的数据，每一位(bit)都以低电平开始
	for(i=0;i<41;i++)//有41个元素是因为要用一个循环读取dht的响应信号
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
	mid/=38;//用平均数区分0和1
	for(i=1;i<41;i++)
	{
		if(bitTime[i]>(mid)) bitTime[i]=1;
		else bitTime[i]=0;
	}

	for(i=0;i<8;i++)//把二进制数据写入
	{
		dat[0]<<=1;
		bitTime[i+1]?(dat[0]|=0x01)/*在最后一位写入1*/:(dat[0]&=~0x01)/*在最后一位写入2*/;
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
		return 1;
	else 
		return 0;
}	
