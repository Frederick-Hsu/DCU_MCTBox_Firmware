/********************************************************************************************
 * File name	: PWM_Out.c
 * Description	: PWM module, implements the PWM Out functions.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Fri.	June 07, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 * 
 ********************************************************************************************/

#include "PWM.h"

/*===========================================================================================
 * Introduce the 2 timer : TAA0 & TAA1.
 * By virtual of the alternative Start/Stop transition of 2 timer in different interval, 
 * to implement the PWM ON / OFF transition. 
 * Please see the below transition diagram with the 2 timer :
 * 
 *    TAA0 start to count timing : 
 *                                 (PWM ON)      TAA0 timeout (TAA0 stop)                                              TAA0 start again
 *                               -----------------------------                                                          ------------------------------
 *                                                           |                                                          |                            |
 *                                                           | TTA1 start to count timing (PWM OFF)                     |                            | TAA1 start again
 *                                                           ------------------------------------------------------------                            ------------------------------------------------------------
 *                                                                                                       TAA1 timeout (TAA1 stop)
 */
#include "../timer.h"

int iCurrentActivePwmOutCh = 0;

void Set_Config_PWM_Out_Param(ST_PWM_OUT stPwmParam)
{
	/*
	 * Firstly do initialization for the 2 timer.
	 */
	TAA0_Init();
	TAA1_Init();
	
	/*
	 * Then set the interval respectively for each timer, 
	 * according to both PWM frequency and duty-cycle parameters.
	 */
}


void PWM_Out_Start(void)
{
	TAA0_Start();
	if (iCurrentActivePwmOutCh == 1)
	{
		PWM_OUT1 = HIGH;
	}
	else if (iCurrentActivePwmOutCh == 2)
	{
		PWM_OUT2 = HIGH;
	}	
}


void PWM_Out_Stop(void)
{
	TAA0_Stop();
	TAA1_Stop();
	
	iCurrentActivePwmOutCh = 0;	// Reset
	
	/*
	 * Reset 2 PWM out channels to low level mandantorily,
	 * whatever condition the external environment is.
	 */
	PWM_OUT1 = LOW;
	PWM_OUT2 = LOW;
}

void Calculate_Timer_Interval(float 	fARGIN_PWM_Freq_InHz, 
			      float 	fARGIN_PWM_DutyCycle_InPercentage,
			      USHORT 	*ushrtARGOUT_pInterval_For_PwmON,
			      USHORT 	*ushrtARGOUT_pInterval_For_PwmOFF)
{
}

/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

