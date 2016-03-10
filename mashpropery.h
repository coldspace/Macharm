# ifndef _MASHPROPERY_H_
# define _MASHPROPERY_H_
# include <arduino.h>
# include "MashPid.h"

class MashinePropery : public MashPid
{
public:
	MashinePropery();
	~MashinePropery();
	void SetAngle(unsigned int num, unsigned int inputangle);
	void SetAngle(unsigned int num, unsigned int flag, unsigned int inputangle);
	int GetAngle(unsigned int num);
	int GetGoalAngle(unsigned int num);
	void SetSpeed(unsigned int sp);
	unsigned int GetSpeed();
	void PrintfAngle();
	void Mashine_Init(PID *pp);
	void Angle_Init();
	void oledprintnow();
	PID spid;
private:
	void oledprintgoal();
	static unsigned int speed;		//电机的转动速度
	static int angle[6];	//当前电机的角度
	static int goal_angle[6]; //电机的目标角度
	static int angle_init[6];	//初始化电机角度
};

#endif