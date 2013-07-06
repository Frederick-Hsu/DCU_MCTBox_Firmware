/********************************************************************************************
 * File name	: PWM.h
 * Description	: PWM module, declares the prototypes in this file.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Fri.	June 07, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 * 
 ********************************************************************************************/

#ifndef PWM_H
#define PWM_H


	#include "../macrodriver.h"
	#include "../bus.h"
	
	typedef struct stPWM_Param
	{
		float fPWM_Freq_InHz;
		float fPWM_DutyCycle_InPercentage;
		float fPWM_Amplitude_InVolt;	// This parameter could be reserved for extension in future.	
	}
	ST_PWM_PARAM, *PST_PWM_PARAM;
	
	
	/*
	 * Note : 
	 * Since the original PWM ports of Renesas uPD70F3376 chip had been occupied as the GPIOs in the schematics, 
	 * in order to output PWM signal or acquire the external PWM signal, it was constrained to simulate to implement
	 * the PWM function through software. 
	 *
	 * Providing 2 kinds of PWM-outputting approaches :
	 *	1. Choose CS2(i.e. port P97), CS3(port P96), CS4(i.e. P95), CS5(i.e. P94) ports to output PWM signals
	 *	   but here only select 2 ports : CS4 and CS5, other ports are not enabled.
	 *
	 *	2. Choose one pin of DataBus [D0..D23] to output PWM signal.
	 */
	// PWM Approach 1 :
	#define PWM_OUT1	CS4
	#define PWM_OUT2	CS5
	typedef struct stPWM_Out
	{
		BYTE bytPWM_Out_PortNr;
		ST_PWM_PARAM stPWM_Out_Param;
	}
	ST_PWM_OUT, *PST_PWM_OUT;
	
	
	// PWM Approach 2 :
	typedef struct stPWM_Out_Port
	{
		BYTE bytBoardID;
		DWORD dwOutCHn;
	}
	ST_PWM_OUT_PORT, *PST_PWM_OUT_PORT;
	
	typedef struct stPWM_Out_DB
	{
		ST_PWM_OUT_PORT stPWM_Out_Pin;
		ST_PWM_PARAM stPWM_Out_Param;
	}
	ST_PWM_OUT_DB, *PST_PWM_OUT_DB;
	
	
	
	/*==============================================================================================================
	 * Functions prototype relative to PWM :
	 */
	void Set_Config_PWM_Out_Param(ST_PWM_OUT stPwmParam);
	void PWM_Out_Start(void);
	void PWM_Out_Stop(void);
	

#endif	/*  PWM_H  */

