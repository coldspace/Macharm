#include "mashpropery.h"
# include "SSD1306.h"
/*OLED�ܽ�����*/
#define OLED_DC 11
#define OLED_CS 9
#define OLED_CLK 10
#define OLED_MOSI 12
#define OLED_RESET 13

/*ע�ͣ�����OLED��ӡ����*/
#define OLEDPRINT

int MashinePropery::angle[6] = { 90, 120, 0, 60, 99, 10 }; //��ǰ����ĽǶ�
int MashinePropery::angle_init[6] = { 90, 120, 0, 60, 99, 10 }; //�Ծ�̬����ֵ���г�ʼ��
int MashinePropery::goal_angle[6] = { 90, 120, 0, 60, 99, 10 };//Ŀ�����ĽǶ�
unsigned int MashinePropery::speed = 1;

#ifdef OLEDPRINT
SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#endif // OLEDPRINT

MashinePropery::MashinePropery()    
{
}

MashinePropery::~MashinePropery()
{
}

#ifdef OLEDPRINT

void MashinePropery::oledprintgoal()
{
	oled.drawchar(30, 2, goal_angle[0] / 100 + 0x30);
	oled.drawchar(36, 2, (goal_angle[0] / 10) % 10 + 0x30);
	oled.drawchar(42, 2, goal_angle[0] % 10 + 0x30);

	oled.drawchar(30, 3, goal_angle[1] / 100 + 0x30);
	oled.drawchar(36, 3, (goal_angle[1] / 10) % 10 + 0x30);
	oled.drawchar(42, 3, goal_angle[1] % 10 + 0x30);

	oled.drawchar(30, 4, goal_angle[2] / 100 + 0x30);
	oled.drawchar(36, 4, (goal_angle[2] / 10) % 10 + 0x30);
	oled.drawchar(42, 4, goal_angle[2] % 10 + 0x30);

	oled.drawchar(30, 5, goal_angle[3] / 100 + 0x30);
	oled.drawchar(36, 5, (goal_angle[3] / 10) % 10 + 0x30);
	oled.drawchar(42, 5, goal_angle[3] % 10 + 0x30);

	oled.drawchar(30, 6, goal_angle[4] / 100 + 0x30);
	oled.drawchar(36, 6, (goal_angle[4] / 10) % 10 + 0x30);
	oled.drawchar(42, 6, goal_angle[4] % 10 + 0x30);

	oled.drawchar(30, 7, goal_angle[5] / 100 + 0x30);
	oled.drawchar(36, 7, (goal_angle[5] / 10) % 10 + 0x30);
	oled.drawchar(42, 7, goal_angle[5] % 10 + 0x30);

	oled.display();
}

void MashinePropery::oledprintnow()
{
	/*��ӡ��ǰ�Ƕ���Ϣ*/
	oled.drawchar(66, 2, angle[0] / 100 + 0x30);
	oled.drawchar(72, 2, (angle[0] / 10) % 10 + 0x30);
	oled.drawchar(78, 2, angle[0] % 10 + 0x30);

	oled.drawchar(66, 3, angle[1] / 100 + 0x30);
	oled.drawchar(72, 3, (angle[1] / 10) % 10 + 0x30);
	oled.drawchar(78, 3, angle[1] % 10 + 0x30);

	oled.drawchar(66, 4, angle[2] / 100 + 0x30);
	oled.drawchar(72, 4, (angle[2] / 10) % 10 + 0x30);
	oled.drawchar(78, 4, angle[2] % 10 + 0x30);

	oled.drawchar(66, 5, angle[3] / 100 + 0x30);
	oled.drawchar(72, 5, (angle[3] / 10) % 10 + 0x30);
	oled.drawchar(78, 5, angle[3] % 10 + 0x30);

	oled.drawchar(66, 6, angle[4] / 100 + 0x30);
	oled.drawchar(72, 6, (angle[4] / 10) % 10 + 0x30);
	oled.drawchar(78, 6, angle[4] % 10 + 0x30);

	oled.drawchar(66, 7, angle[5] / 100 + 0x30);
	oled.drawchar(72, 7, (angle[5] / 10) % 10 + 0x30);
	oled.drawchar(78, 7, angle[5] % 10 + 0x30);

	oled.display();
}

#endif // OLEDPRINT

void MashinePropery::Mashine_Init(PID *pp)
{
	for (int i = 0; i < 6; i++)
	{
		angle[i] = angle_init[i];
		goal_angle[i] = angle_init[i];
	}
#ifdef OLEDPRINT
	oled.ssd1306_init(SSD1306_SWITCHCAPVCC);
	oled.clear();

	oled.drawstring(0, 0, "Mesh");
	oled.drawstring(30, 0, "goal");
	oled.drawstring(66, 0, "now");
	oled.drawstring(0, 2, "M0");
	oled.drawstring(0, 3, "M1");
	oled.drawstring(0, 4, "M2");
	oled.drawstring(0, 5, "M3");
	oled.drawstring(0, 6, "M4");
	oled.drawstring(0, 7, "M5");
	oled.display();

	oledprintgoal();
#endif // OLEDPRINT

	PIDInit(pp);
}

void MashinePropery::Angle_Init()
{
	for (int i = 0; i < 6; i++)
	{
		goal_angle[i] = angle_init[i];
		spid.SetPoint[i] = angle_init[i];
	}
}

int MashinePropery::GetAngle(unsigned int num)
{
	if (angle[num] != goal_angle[num])
	{
		if (angle[num] > goal_angle[num])
			angle[num] --;
		else
			angle[num] ++;
	}
	return angle[num];
	/*double n ;
	n = PIDCalc(&spid, goal_angle[num], num);
	n = 0;
	angle[num] = n + angle[num];
	spid.SetPoint[num] = angle[num];
	return angle[num];*/
}

int MashinePropery::GetGoalAngle(unsigned int num)
{
	return goal_angle[num];
}

/*���о���λ�ÿ���*/
void MashinePropery::SetAngle(unsigned int num, unsigned int inputangle)
{
	goal_angle[num] = inputangle;

	/*��ⷶΧ�Ƿ����*/
	if (goal_angle[num] > 180)
		goal_angle[num] = 180;
	if (goal_angle[num] < 0)
		goal_angle[num] = 0;
#ifdef OLEDPRINT
	oledprintgoal();
#endif // OLEDPRINT
}

/*�������λ�ÿ���*/
void MashinePropery::SetAngle(unsigned int num, unsigned int flag, unsigned int inputangle) 
{
	if (!flag)
		goal_angle[num] = goal_angle[num] + inputangle;
	else
		goal_angle[num] = goal_angle[num] - inputangle;

	/*��ⷶΧ�Ƿ����*/
	if (goal_angle[num] > 180)
		goal_angle[num] = 180;
	if (goal_angle[num] < 0)
		goal_angle[num] = 0;
	//���5��ץȡ������Ƕ����ֻ����65���������������ȷ��ȣ�
	//�ڴ˼�һ������
	if (goal_angle[5] > 65)
		goal_angle[num] = 65;

#ifdef OLEDPRINT
	oledprintgoal();
#endif // OLEDPRINT

	
}

unsigned int MashinePropery::GetSpeed()
{
	return speed;
}

//�����ٶȣ��趨ÿ��������ٶ���ͬ
void MashinePropery::SetSpeed(unsigned int sp)
{
	speed = 50 - sp;
}

/*��ӡÿ�������Ŀ��ǶȺ͵����ת���ٶ�*/
void MashinePropery::PrintfAngle()
{
	Serial.println("GoalAngle");
	for (int i = 0; i < 6; i++)
	{
		Serial.print("M");
		Serial.print(i);
		Serial.print(":");
		Serial.print(goal_angle[i]);
		Serial.print(" ");
	}
	Serial.print("\r\n");
	Serial.println("Angle");
	for (int i = 0; i < 6; i++)
	{
		Serial.print("M");
		Serial.print(i);
		Serial.print(":");
		Serial.print(angle[i]);
		Serial.print(" ");
	}
	Serial.print("\r\n");
	Serial.print("Speed:");
	Serial.println(50-speed);
	Serial.println();
}
