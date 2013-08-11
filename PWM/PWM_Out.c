/********************************************************************************************
 * File name	: PWM_Out.c
 * Description	: PWM module, implements the PWM Out functions.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Fri.	June 07, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 * 
 ********************************************************************************************/


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
#include <math.h>
#include "PWM_Out.h"
#include "../timer.h"
#include "../port.h"
#include "../Switch_Relay_Control/Switch_Relay_Control.h"

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
	ST_Access_Ctrl_SwitchRelayMatrix tPwmOut = {0x00, 0, 0};
	
	tPwmOut.byteBoardID = bytPwmBoardID;
	tPwmOut.dwSwitch_Relay_CHn = (DWORD)ePwmSecondaryChn;
	tPwmOut.eOpen_Close_State = eLevel;
	
	Control_Single_Switch(&tPwmOut);
	return;
}

#if (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION2)

	struct PwmOutChnSelector g_stPwmOutChn = {PWM_ATTR_PRIMARY, 0, 0x00};
	unsigned int iTAA0_FlagCycleCnt = 0, iTAA1_FlagCycleCnt = 0;
	USHORT ushrtPWMOnCnt_TAA0CCR0  = 0, ushrtPWMOffCnt_TAA1CCR0 = 0;

	void Set_Config_PWM_Out_Param(ST_PWM_PARAM stPwmParam)
	{
		USHORT ushrtTempPwmOnCnt_TAA0CCR0 = 0;
		USHORT ushrtTempPwmOffCnt_TAA1CCR0 = 0;
		/*
		 * Firstly do initialization for the 2 timer.
		 */
		TAA0_Init();
		TAA1_Init();
		
		/*
		 * Then set the interval respectively for each timer, 
		 * according to both PWM frequency and duty-cycle parameters.
		 */
		Calculate_Timer_Interval(stPwmParam.fPWM_Freq_InHz, 
		 			 stPwmParam.fPWM_DutyCycle_InPercentage,
					 &ushrtPWMOnCnt_TAA0CCR0,
					 &ushrtPWMOffCnt_TAA1CCR0);
		if (iTAA0_FlagCycleCnt >= 1)
		{
			ushrtTempPwmOnCnt_TAA0CCR0 = 0xFFFF;
			TAA0_ChangeTimerCondition(&ushrtTempPwmOnCnt_TAA0CCR0, 1);
		}
		else
		{
			TAA0_ChangeTimerCondition(&ushrtPWMOnCnt_TAA0CCR0, 1);
		}
		
		if (iTAA1_FlagCycleCnt >= 1)
		{
			ushrtTempPwmOffCnt_TAA1CCR0 = 0xFFFF;
			TAA1_ChangeTimerCondition(&ushrtTempPwmOffCnt_TAA1CCR0, 1);
		}
		else
		{
			TAA1_ChangeTimerCondition(&ushrtPWMOffCnt_TAA1CCR0, 1);
		}
		
		return;
	}

	void PWM_Out_Start(int ePrimaryOrSecondary, int ePwmChn, BYTE bytPwmBoardID)
	{	
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
		
		TAA0_Start();		
		return;
	}
	
	void PWM_Out_Stop(void)
	{	
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
		TAA0_Stop();
		TAA1_Stop();
		return;
	}

	/**************************************************************************************************************************/
	void Calculate_Timer_Interval(float 	fARGIN_PWM_Freq_InHz,
				      float	fARGIN_PWM_DutyCycle_InPercentage,
				      USHORT	*ushrtARGOUT_pInterval_For_PwmON,
				      USHORT	*ushrtARGOUT_pInterval_For_PwmOFF)
	{
		const double dTimer_Tcycle = 4.0e-006,								// unit : second ??????? T = 128 / 32M = 4us
			     dTimer_Time1CompleteCycle = dTimer_Tcycle * 65536;		// unit : second ?????1????Cycle??:4us * 65536 = 262.144ms

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

#elif (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION1)
	extern int g_iErrorCodeNo;
	
	unsigned int g_uiTAA0_CycleCnt4PwmHigh = 0, g_uiTAA0_CycleCnt4PwmLow = 0,
		     g_uiTAA1_CycleCnt4PwmHigh = 0, g_uiTAA1_CycleCnt4PwmLow = 0;
	
	struct PwmOutChnSelector g_stPwmOut1Chn = {PWM_ATTR_PRIMARY, 0, 0x00},
				 g_stPwmOut2Chn = {PWM_ATTR_SECONDARY, 1, 0x00};
	
	// ST_PWMOUT_FSM g_tPwmOutFSM = {PWM_STATE_STOPPED, {PWM_ATTR_PRIMARY, PwmOutChn_Primary0, 0x00}};
		     
	void Set_Config_PWM_Out_Param(enum Pwm_Out_No ePwmOutNr, ST_PWM_PARAM stPwmParam)
	{
		// const USHORT FIXED_TIMER_INTERVAL = 31;
		const USHORT FIXED_TIMER_INTERVAL = 9;
		
		// Do initialization for TAA0 and TAA1 timers
		if (ePwmOutNr == PWM_OUT1)
		{
			TAA0_Init();
			TAA0_ChangeTimerCondition(&FIXED_TIMER_INTERVAL, 1);	// Directly set the timer interval : 1us
			Calculate_Timer_Interval(stPwmParam.fPWM_Freq_InHz, 	stPwmParam.fPWM_DutyCycle_InPercentage,
						 &g_uiTAA0_CycleCnt4PwmHigh, 	&g_uiTAA0_CycleCnt4PwmLow);
		}
		else if (ePwmOutNr == PWM_OUT2)
		{
			TAA1_Init();
			TAA1_ChangeTimerCondition(&FIXED_TIMER_INTERVAL, 1);
			Calculate_Timer_Interval(stPwmParam.fPWM_Freq_InHz, 	stPwmParam.fPWM_DutyCycle_InPercentage,
						 &g_uiTAA1_CycleCnt4PwmHigh, 	&g_uiTAA1_CycleCnt4PwmLow);
		}
		else
		{
			g_iErrorCodeNo = -24;
		}
		return;
	}
	
	void Calculate_Timer_Interval(float fARGIN_PWM_Freq_InHz,
				      float fARGIN_PWM_DutyCycle_InPercentage,
				      unsigned int *uiARGOUT_pCycleCnt_For_PwmHigh,
				      unsigned int *uiARGOUT_pCycleCnt_For_PwmLow)
	{
		unsigned int uiCycleCnt4PwmHigh = 0, uiCycleCnt4PwmLow = 0;
		// uiCycleCnt4PwmHigh = (unsigned int)(10000 * (fARGIN_PWM_DutyCycle_InPercentage/fARGIN_PWM_Freq_InHz)); 
		// uiCycleCnt4PwmLow  = (unsigned int)(10000 * ((100-fARGIN_PWM_DutyCycle_InPercentage)/fARGIN_PWM_Freq_InHz));
		uiCycleCnt4PwmHigh = (unsigned int)(1000 * (fARGIN_PWM_DutyCycle_InPercentage/fARGIN_PWM_Freq_InHz)); 
		uiCycleCnt4PwmLow  = (unsigned int)(1000 * ((100-fARGIN_PWM_DutyCycle_InPercentage)/fARGIN_PWM_Freq_InHz));
		
		memcpy(uiARGOUT_pCycleCnt_For_PwmHigh, &uiCycleCnt4PwmHigh, sizeof(unsigned int));
		memcpy(uiARGOUT_pCycleCnt_For_PwmLow, &uiCycleCnt4PwmLow, sizeof(unsigned int));
		return;
	}
	
	void PWM_Out_Start(enum Pwm_Out_No ePwmOutNr, int ePrimaryOrSecondary, int ePwmChn, BYTE bytPwmBoardID)
	{
		if (ePwmOutNr == PWM_OUT1)
		{
			if (ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
				Set_PwmOutPrimaryChn_Level(ePwmChn, HIGH);
			else if (ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
				Set_PwmOutSecondaryChn_Level(bytPwmBoardID, ePwmChn, HIGH);
			g_stPwmOut1Chn.ePrimaryOrSecondary = ePrimaryOrSecondary;
			g_stPwmOut1Chn.iPwmOutChn = ePwmChn;
			g_stPwmOut1Chn.bytPwmOutBoardID = bytPwmBoardID;
			
			TAA0_Start();
		}
		else if (ePwmOutNr == PWM_OUT2)
		{
			if (ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
				Set_PwmOutPrimaryChn_Level(ePwmChn, HIGH);
			else if (ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
				Set_PwmOutSecondaryChn_Level(bytPwmBoardID, ePwmChn, HIGH);
			g_stPwmOut2Chn.ePrimaryOrSecondary = ePrimaryOrSecondary;
			g_stPwmOut2Chn.iPwmOutChn = ePwmChn;
			g_stPwmOut2Chn.bytPwmOutBoardID = bytPwmBoardID;
			
			TAA1_Start();
		}
		else
		{
			g_iErrorCodeNo = -24;
		}
		return;
	}
	
	void PWM_Out_Stop(enum Pwm_Out_No ePwmOutNr)
	{
		if (ePwmOutNr == PWM_OUT1)
		{
			PWM_Out1_Stop();
			#if 0
			TAA0_Stop();
			if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
				Set_PwmOutPrimaryChn_Level(g_stPwmOut1Chn.iPwmOutChn, LOW);
			else if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
				Set_PwmOutSecondaryChn_Level(g_stPwmOut1Chn.bytPwmOutBoardID, g_stPwmOut1Chn.iPwmOutChn, LOW);
			#endif
		}
		else if (ePwmOutNr == PWM_OUT2)
		{
			PWM_Out2_Stop();
			#if 0
			TAA1_Stop();
			if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
				Set_PwmOutPrimaryChn_Level(g_stPwmOut2Chn.iPwmOutChn, LOW);
			else if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
				Set_PwmOutSecondaryChn_Level(g_stPwmOut2Chn.bytPwmOutBoardID, g_stPwmOut2Chn.iPwmOutChn, LOW);
			#endif
		}
		else
		{
			g_iErrorCodeNo = -24;
		}
		return;
	}
#endif
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

