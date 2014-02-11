/***********************************************************************************
 * File name	: TAA0_timer_user.c
 * Description	: Imlement the timer TAA0 interrupt service routine in this file.
 * Creator	: XU ZAN	[HSE-OT]
 * Create date	: Sat.	Aug. 03, 2013
 * Copyright(C)		2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ***********************************************************************************/

#include "../macrodriver.h"
#include "../timer.h"
#include "PWM_Out_Generating_Option.h"
#include "PWM_Out.h"

#if (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION1)
	#pragma interrupt INTTAA0CC0 MD_INTTAA0CC0

	extern unsigned int g_uiTAA0_CycleCnt4PwmHigh, g_uiTAA0_CycleCnt4PwmLow;
	extern struct PwmOutChnSelector g_stPwmOut1Chn;
	/*
	**-----------------------------------------------------------------------------
	**
	**	Abstract:
	**		This function is INTTAA0CC0 interrupt service routine.
	**
	**	Parameters:
	**		None
	**
	**	Returns:
	**		None
	**
	**-----------------------------------------------------------------------------
	*/
	static unsigned int gs_uiTAA0CurrentCycleCnt4PwmHigh = 0,
			    gs_uiTAA0CurrentCycleCnt4PwmLow  = 0;
	
	__interrupt void MD_INTTAA0CC0(void)
	{
		if (gs_uiTAA0CurrentCycleCnt4PwmHigh < g_uiTAA0_CycleCnt4PwmHigh)
		{
			TAA0CCIF0 = 0;	/* Clear INTTAA0CC0 interrupt flag */
			gs_uiTAA0CurrentCycleCnt4PwmHigh++;			
			return;
		}
		else if (gs_uiTAA0CurrentCycleCnt4PwmHigh == g_uiTAA0_CycleCnt4PwmHigh)
		{
			TAA0CCIF0 = 0;
			gs_uiTAA0CurrentCycleCnt4PwmHigh++;
			// LOW
			{
				if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
					Set_PwmOutPrimaryChn_Level(g_stPwmOut1Chn.iPwmOutChn, LOW);
				else if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
					Set_PwmOutSecondaryChn_Level(g_stPwmOut1Chn.bytPwmOutBoardID, g_stPwmOut1Chn.iPwmOutChn, LOW);
			}
			gs_uiTAA0CurrentCycleCnt4PwmLow = 0;
			return;
		}
		
		if (gs_uiTAA0CurrentCycleCnt4PwmLow < g_uiTAA0_CycleCnt4PwmLow)
		{
			TAA0CCIF0 = 0;
			gs_uiTAA0CurrentCycleCnt4PwmLow++;
			return;
		}
		else if (gs_uiTAA0CurrentCycleCnt4PwmLow == g_uiTAA0_CycleCnt4PwmLow)
		{
			TAA0CCIF0 = 0;
			gs_uiTAA0CurrentCycleCnt4PwmLow++;
			// High
			{
				if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
					Set_PwmOutPrimaryChn_Level(g_stPwmOut1Chn.iPwmOutChn, HIGH);
				else if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
					Set_PwmOutSecondaryChn_Level(g_stPwmOut1Chn.bytPwmOutBoardID, g_stPwmOut1Chn.iPwmOutChn, HIGH);
			}
			gs_uiTAA0CurrentCycleCnt4PwmHigh = 0;
			return;
		}
	}
	
	void PWM_Out1_Stop(void)
	{
		TAA0_Stop();
		// Reset the cycle counts.
		gs_uiTAA0CurrentCycleCnt4PwmHigh = 0;
		gs_uiTAA0CurrentCycleCnt4PwmLow  = 0;
		
		if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
			Set_PwmOutPrimaryChn_Level(g_stPwmOut1Chn.iPwmOutChn, LOW);
		else if (g_stPwmOut1Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
			Set_PwmOutSecondaryChn_Level(g_stPwmOut1Chn.bytPwmOutBoardID, g_stPwmOut1Chn.iPwmOutChn, LOW);
		return;
	}

#endif

