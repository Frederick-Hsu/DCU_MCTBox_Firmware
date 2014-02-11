/*****************************************************************************************************
 * File name	: Calculate_PWM_Timer_Count.C
 * Description	: Study and demonstrate how to calculate the count of PWM_Timer
 * Creator		: XU ZAN
 * Creation date: Thu.	Aug. 01, 2013
 * Copyright(C)		2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 *****************************************************************************************************/

#include <string.h>
#include "Calculate_PWM_Timer_Count.h"

int iTAA0_FlagCycleCnt = 0, iTAA1_FlagCycleCnt = 0;

void Calculate_Timer_Interval(float		fARGIN_PWM_Freq_InHz,
							  float		fARGIN_PWM_DutyCycle_InPercentage,
							  USHORT	*ushrtARGOUT_pInterval_For_PwmON,
							  USHORT	*ushrtARGOUT_pInterval_For_PwmOFF)
{
	const double dTimer_Tcycle = 4.0e-006,								// unit : second 定时器时钟周期 T = 128 / 32M = 4us
				 dTimer_Time1CompleteCycle = dTimer_Tcycle * 65536;		// unit : second 计数器走完1个完整的Cycle耗时：4us * 65536 = 262.144ms

	double dPwm_On_Time = 0.00f, dPwm_Off_Time = 0.00f;
	double dTempPwmOn = 0.00f, dTempPwmOff = 0.00f;

	USHORT ushrtPwmOnInterval = 0, ushrtPwmOffInterval = 0;

	dPwm_On_Time  = fARGIN_PWM_DutyCycle_InPercentage / (100 * fARGIN_PWM_Freq_InHz);
	dPwm_Off_Time = (100.00 - fARGIN_PWM_DutyCycle_InPercentage) / (100 * fARGIN_PWM_Freq_InHz);

	dTempPwmOn = dPwm_On_Time / dTimer_Time1CompleteCycle;
	dTempPwmOff = dPwm_Off_Time / dTimer_Time1CompleteCycle;

	iTAA0_FlagCycleCnt = (int)(dPwm_On_Time / dTimer_Time1CompleteCycle);
	iTAA1_FlagCycleCnt = (int)(dPwm_Off_Time / dTimer_Time1CompleteCycle);

	ushrtPwmOnInterval = (int)((dTempPwmOn - iTAA0_FlagCycleCnt)*65536) - 1;
	ushrtPwmOffInterval = (int)((dTempPwmOff - iTAA1_FlagCycleCnt)*65536) - 1;

	memcpy(ushrtARGOUT_pInterval_For_PwmON, &ushrtPwmOnInterval, sizeof(USHORT));
	memcpy(ushrtARGOUT_pInterval_For_PwmOFF, &ushrtPwmOffInterval, sizeof(USHORT));
	return;
}

void Test_TimerIntervalCalculation(void)
{
	ST_PWM_PARAM tPwmParam = {10, 40};

	USHORT ushrtPwmOn = 0, ushrtPwmOff = 0;
	Calculate_Timer_Interval(tPwmParam.fPWMFreqInHz,
							 tPwmParam.fPWMDutyCycleInPercentage,
							 &ushrtPwmOn,
							 &ushrtPwmOff);
	return;
}