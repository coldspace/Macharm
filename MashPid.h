// MashPid.h

#ifndef _MASHPID_h
#define _MASHPID_h


#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include "string.h"
#else
	#include "WProgram.h"
#endif



class MashPid
{
public:
	typedef struct PID {

		double  SetPoint[6];           //  设定目标 Desired Value

		double  Proportion;         //  比例常数 Proportional Const
		double  Integral;           //  积分常数 Integral Const
		double  Derivative;         //  微分常数 Derivative Const

		double  LastError[6];          //  Error[-1]
		double  PrevError[6];          //  Error[-2]
		double  SumError[6];           //  Sums of Errors
	}PID;

	MashPid();
	~MashPid();
	double PIDCalc(PID *pp, double NextPoint, unsigned int num);
	void PIDInit(PID *pp);

private:

};


#endif

