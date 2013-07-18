/********************************************************************************************
 * File name	: PWM_Out.c
 * Description	: PWM module, implements the PWM Out functions.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Fri.	June 07, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 * 
 ********************************************************************************************/

#include "PWM_Out.h"

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
#include "../port.h"
#include "../Switch_Relay_Control/Switch_Relay_Control.h"

struct PwmOutChnSelector g_stPwmOutChn = {PWM_ATTR_PRIMARY, 0, 0x00};

void Set_Config_PWM_Out_Param(ST_PWM_PARAM stPwmParam)
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

void PWM_Out_Start(int ePrimaryOrSecondary, int ePwmChn, BYTE bytPwmBoardID)
{	
	TAA0_Start();

	if (ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
	{
		Set_PwmOutPrimaryChn_Level(ePwmChn, HIGH);
	}
	else if (ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
	{
		Set_PwmOutSecondaryChn_Level(bytPwmBoardID, ePwmChn, HIGH);
	}
	
	g_stPwmOutChn.ePrimaryOrSecondary = ePrimaryOrSecondary;
	g_stPwmOutChn.iPwmOutChn = ePwmChn;
	g_stPwmOutChn.bytPwmOutBoardID = bytPwmBoardID;
	
	return;
}


void PWM_Out_Stop(void)
{
	TAA0_Stop();
	TAA1_Stop();
	
	/*
	 * Reset 2 PWM out channels to low level mandantorily,
	 * whatever condition the external environment is.
	 */
	if (g_stPwmOutChn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
	{
		Set_PwmOutPrimaryChn_Level(g_stPwmOutChn.iPwmOutChn, LOW);
	}
	else if (g_stPwmOutChn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
	{
		Set_PwmOutSecondaryChn_Level(g_stPwmOutChn.bytPwmOutBoardID, g_stPwmOutChn.iPwmOutChn, LOW);
	}
	return;
}

/**************************************************************************************************************************/
void Calculate_Timer_Interval(float 	fARGIN_PWM_Freq_InHz, 
			      float 	fARGIN_PWM_DutyCycle_InPercentage,
			      USHORT 	*ushrtARGOUT_pInterval_For_PwmON,
			      USHORT 	*ushrtARGOUT_pInterval_For_PwmOFF)
{
	float fTcycle = 0.00f,
	      fPwmOnTime = 0.00f,
	      fPwmOffTime = 0.00f;
	
	fTcycle= fARGIN_PWM_Freq_InHz;
	if ((fARGIN_PWM_DutyCycle_InPercentage>=0.00) && (fARGIN_PWM_DutyCycle_InPercentage<=1.00))
	{
		fPwmOnTime = fTcycle * fARGIN_PWM_DutyCycle_InPercentage;
		fPwmOffTime = fTcycle * (1 - fARGIN_PWM_DutyCycle_InPercentage);
	}
	else
	{
		// Error
	}
	
	
}

void Set_PwmOutPrimaryChn_Level(PWM_OUT_PRIMARY_CHN ePrimaryChn, enum LEVEL eLevel)
{
	switch (ePrimaryChn)
	{
	case PwmOutChn_Primary0 :
		// CS4 = eLevel;
		PORT_ChangeP95Output(eLevel);
		break;
	case PwmOutChn_Primary1 :
		// CS5 = eLevel;
		PORT_ChangeP94Output(eLevel);
		break;
	default :
		break;
	}
	return;
}

void Set_PwmOutSecondaryChn_Level(BYTE 				bytPwmBoardID, 
				  PWM_OUT_SECONDARY_CHN 	ePwmSecondaryChn, 
				  enum LEVEL 			eLevel)
{
	PST_Access_Ctrl_SwitchRelayMatrix pPwmOut;
	
	pPwmOut->byteBoardID = bytPwmBoardID;
	pPwmOut->dwSwitch_Relay_CHn = (DWORD)ePwmSecondaryChn;
	pPwmOut->eOpen_Close_State = eLevel;
	
	Control_Single_Switch(pPwmOut);
	return;
}
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

