// 
// 
// 

#include "MashPid.h"


MashPid::MashPid()
{
}

MashPid::~MashPid()
{
}

double MashPid::PIDCalc(PID *pp, double NextPoint,unsigned int num)
{
	double  dError,Error;

	Error = pp->SetPoint[num] - NextPoint;          // 偏差
	pp->SumError[num] += Error;                      // 积分
	dError = pp->LastError - pp->PrevError;     // 当前微分
	pp->PrevError[num] = pp->LastError[num];
	pp->LastError[num] = Error;
	return (pp->Proportion * Error              // 比例项
		+ pp->Integral * pp->SumError[num]         // 积分项
		+ pp->Derivative * dError             // 微分项
		);
}

void MashPid::PIDInit(PID *pp)
{
	memset(pp, 0, sizeof(PID));
}
