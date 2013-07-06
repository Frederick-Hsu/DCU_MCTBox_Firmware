/********************************************************************************************
 * File name	: PWM.h
 * Description	: PWM module, declares the prototypes in this file.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Fri.	June 07, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 * 
 ********************************************************************************************/

#ifndef PWM_H
#define PWM_H

	typedef struct stPWM_Param
	{
		float fPWM_Freq_InHz;
		float fPWM_DutyCycle_InPercentage;
		float fPWM_MaxAmplitude_InVoltage;
	}
	ST_PWM_Param, *PST_PWM_Param;
	

#endif	/*  PWM_H  */

