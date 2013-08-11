/*****************************************************************************************************
 * File name	: Calculate_PWM_Timer_Count.h
 * Description	: Study and demonstrate how to calculate the count of PWM_Timer
 * Creator		: XU ZAN
 * Creation date: Thu.	Aug. 01, 2013
 * Copyright(C)		2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 *****************************************************************************************************/

#ifndef CALCULATE_PWM_TIMER_COUNT_H
#define CALCULATE_PWM_TIMER_COUNT_H

	typedef unsigned short	USHORT;

	typedef struct PWM_Param
	{
		float fPWMFreqInHz;
		float fPWMDutyCycleInPercentage;
	}
	ST_PWM_PARAM, *PST_PWM_PARAM;


	void Calculate_Timer_Interval(float		fARGIN_PWM_Freq_InHz,
								  float		fARGIN_PWM_DutyCycle_InPercentage,
								  USHORT	*ushrtARGOUT_pInterval_For_PwmON,
								  USHORT	*ushrtARGOUT_pInterval_For_PwmOFF);

	void Test_TimerIntervalCalculation(void);

#endif	/*  CALCULATE_PWM_TIMER_COUNT_H  */