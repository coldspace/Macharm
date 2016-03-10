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

	Error = pp->SetPoint[num] - NextPoint;          // ƫ��
	pp->SumError[num] += Error;                      // ����
	dError = pp->LastError - pp->PrevError;     // ��ǰ΢��
	pp->PrevError[num] = pp->LastError[num];
	pp->LastError[num] = Error;
	return (pp->Proportion * Error              // ������
		+ pp->Integral * pp->SumError[num]         // ������
		+ pp->Derivative * dError             // ΢����
		);
}

void MashPid::PIDInit(PID *pp)
{
	memset(pp, 0, sizeof(PID));
}
