#include <SSD1306.h>
#include <Servo.h>
#include "mashpropery.h"
#include <string.h>
#include "MashPid.h"

//是否打印数据
//# define DebugMash

/*注释，启动OLED打印数据*/
//#define OLEDPRINT
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean stringStart = false;

Servo myserov0;
Servo myserov1;
Servo myserov2;
Servo myserov3;
Servo myserov4;
Servo myserov5;

unsigned int M0angle = 0;
unsigned int M1angle = 0;
unsigned int M2angle = 0;
unsigned int M3angle = 0;
unsigned int M4angle = 0;
unsigned int M5angle = 0;

MashinePropery MyMashine;

void setup()
{
	Serial.begin(9600);

	myserov0.attach(2);
	myserov1.attach(3);
	myserov2.attach(4);
	myserov3.attach(5);
	myserov4.attach(6);
	myserov5.attach(7);
	MyMashine.Mashine_Init(&MyMashine.spid);

	/*PID参数设置*/
	MyMashine.spid.Proportion = 0.5;
	MyMashine.spid.Derivative = 0.5;
	MyMashine.spid.Integral = 0.5;
#ifdef DebugMash
	Serial.println("Init Success!");
#endif
	myserov0.write(MyMashine.GetAngle(0));
	myserov1.write(MyMashine.GetAngle(1));
	myserov2.write(MyMashine.GetAngle(2));
	myserov3.write(MyMashine.GetAngle(3));
	myserov4.write(MyMashine.GetAngle(4));
	myserov5.write(MyMashine.GetAngle(5));

#ifdef DebugMash
	MyMashine.PrintfAngle();
#endif

#ifdef OLEDPRINT
	MyMashine.oledprintnow();
#endif // OLEDPRINT

	
}

void loop()
{
	M0angle = MyMashine.GetAngle(0);
	M1angle = MyMashine.GetAngle(1);
	M2angle = MyMashine.GetAngle(2);
	M3angle = MyMashine.GetAngle(3);
	M4angle = MyMashine.GetAngle(4);
	M5angle = MyMashine.GetAngle(5);
	for (int a = 0; a < 20; a++)
	{
		myserov0.write(M0angle);
		myserov1.write(M1angle);
		myserov2.write(M2angle);
		myserov3.write(M3angle);
		myserov4.write(M4angle);
		myserov5.write(M5angle);
		delay(MyMashine.GetSpeed());
	}
#ifdef OLEDPRINT
	MyMashine.oledprintnow();
#endif // OLEDPRINT

#ifdef DebugMash
	if (!((MyMashine.GetAngle(0) == MyMashine.GetGoalAngle(0)) && \
		(MyMashine.GetAngle(1) == MyMashine.GetGoalAngle(1)) && \
		(MyMashine.GetAngle(2) == MyMashine.GetGoalAngle(2)) && \
		(MyMashine.GetAngle(3) == MyMashine.GetGoalAngle(3)) && \
		(MyMashine.GetAngle(4) == MyMashine.GetGoalAngle(4)) && \
		(MyMashine.GetAngle(5) == MyMashine.GetGoalAngle(5))) )
		MyMashine.PrintfAngle();
#endif
}

/*
*串口接收函数
*按照规定格式接收舵机的角度
*例如 #1#090* 表示给1号舵机设置为90度。
*例如 #20* 表示给电机设置转速20，最大50
*例如 #* 表示给电机角度初始化
*例如 #1#0090* 进行相对位置变化，表示角度加90度
*例如 ##* 打印当前角度数据
*/
void serialEvent()
{
	while (Serial.available())
	{
		char inChar = (char)Serial.read();
		if ((inChar == '#') || (stringStart))
		{
			inputString += inChar;
			stringStart = true;
		}
		else
			inputString = "";
		if (inChar == '*')
		{
			stringComplete = true;
			stringStart = false;
		}
		if (stringComplete)
		{
			if (!DealSerialEvent(inputString))
			{
#ifdef DebugMash
				Serial.println("Data error!");
#endif 
			}
			else
			{
#ifdef DebugMash
				Serial.println("Send Success!");
				MyMashine.PrintfAngle();
#endif 
			}
			stringComplete = false;
			inputString = "";
		}
	}
}

/*
*辅助函数
*将串口得到的数据进行判断，处理。写入舵机的角度。
*/
int DealSerialEvent(String input1String)
{
	int Strnumber = 0;
	unsigned int setangletext = 0;
	Strnumber = input1String.length();
	//例如 #1#090 * 表示给1号舵机设置为90度。
	if ((input1String[0] == '#') && (input1String[2] == '#') && (Strnumber == 7) && (input1String[6] == '*'))
	{
		if (((input1String[1] - 0x30) >= 0) && ((input1String[1] - 0x30) <= 5)) 
		{
			setangletext = (input1String[3] - 0x30) * 100 + (input1String[4] - 0x30) * 10 + (input1String[5] - 0x30);
			if ((setangletext >= 0) && (setangletext <= 180))
			{
				if ((input1String[1] - 0x30) == 5)
				{
					if ((setangletext >= 0) && (setangletext <= 65))
					{
						MyMashine.SetAngle(input1String[1] - 0x30, setangletext);
						return 1;
					}
					else
						return 0;
				}
				else
				{
					MyMashine.SetAngle(input1String[1] - 0x30, setangletext);
					return 1;
				}
			}
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	//例如 #20 * 表示给电机设置转速20，最大50
	if ((input1String[0] == '#') && (input1String[3] == '*') && (Strnumber == 4))
	{
		setangletext = (input1String[1] - 0x30) * 10 + (input1String[2] - 0x30) * 1;
		if ((setangletext >= 0) && (setangletext <= 50))
		{
			MyMashine.SetSpeed(setangletext);
			return 1;
		}
		else
			return 0;
	}
	//例如 #* 表示给电机角度初始化
	if ((input1String[0] == '#') && (input1String[1] == '*') && (Strnumber == 2))
	{
		MyMashine.Angle_Init();
		return 1;
	}
	//例如 ##* 打印当前角度信息
	if ((input1String[0] == '#') && (input1String[1] == '#') && (input1String[2] == '*')  && (Strnumber == 3))
	{
		MyMashine.PrintfAngle();
		return 1;
	}

	//例如 #1#0090 * 进行相对位置变化，表示角度加90度
	if ((input1String[0] == '#') && (input1String[2] == '#') && (Strnumber == 8) && (input1String[7] == '*'))
	{
		if (((input1String[1] - 0x30) >= 0) && ((input1String[1] - 0x30) <= 5))
		{
			setangletext = (input1String[4] - 0x30) * 100 + (input1String[5] - 0x30) * 10 + (input1String[6] - 0x30);
			if ((setangletext >= 0) && (setangletext <= 180))
			{
				if (input1String[1] == 5)
				{
					if ((setangletext >= 0) && (setangletext <= 65))
					{
						MyMashine.SetAngle(input1String[1] - 0x30, input1String[3] - 0x30,setangletext);
						return 1;
					}
					else
						return 0;
				}
				else
				{
					MyMashine.SetAngle(input1String[1] - 0x30, input1String[3] - 0x30, setangletext);
					return 1;
				}
			}
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	
	return 0;
}
