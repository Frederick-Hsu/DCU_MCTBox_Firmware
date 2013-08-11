/***********************************************************************************
 * File name	: TAA1_timer_user.c
 * Description	: Imlement the timer TAA1 interrupt service routine in this file.
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
	#pragma interrupt INTTAA1CC0 MD_INTTAA1CC0

	extern unsigned int g_uiTAA1_CycleCnt4PwmHigh, g_uiTAA1_CycleCnt4PwmLow;
	extern struct PwmOutChnSelector g_stPwmOut2Chn;
	/*
	**-----------------------------------------------------------------------------
	**
	**	Abstract:
	**		This function is INTTAA1CC0 interrupt service routine.
	**
	**	Parameters:
	**		None
	**
	**	Returns:
	**		None
	**
	**-----------------------------------------------------------------------------
	*/
	static unsigned int gs_uiTAA1CurrentCycleCnt4PwmHigh = 0,
			    gs_uiTAA1CurrentCycleCnt4PwmLow  = 0;
	
	__interrupt void MD_INTTAA1CC0(void)
	{
		if (gs_uiTAA1CurrentCycleCnt4PwmHigh < g_uiTAA1_CycleCnt4PwmHigh)
		{
			TAA1CCIF0 = 0;	/* clear INTTAA1CC0 interrupt flag */
			gs_uiTAA1CurrentCycleCnt4PwmHigh++;
			return;
		}
		else if (gs_uiTAA1CurrentCycleCnt4PwmHigh == g_uiTAA1_CycleCnt4PwmHigh)
		{
			TAA1CCIF0 = 0;
			gs_uiTAA1CurrentCycleCnt4PwmHigh++;
			// Pull Low
			{
				if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
					Set_PwmOutPrimaryChn_Level(g_stPwmOut2Chn.iPwmOutChn, LOW);
				else if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
					Set_PwmOutSecondaryChn_Level(g_stPwmOut2Chn.bytPwmOutBoardID, g_stPwmOut2Chn.iPwmOutChn, LOW);
			}
			gs_uiTAA1CurrentCycleCnt4PwmLow = 0;
			return;
		}
		
		if (gs_uiTAA1CurrentCycleCnt4PwmLow < g_uiTAA1_CycleCnt4PwmLow)
		{
			TAA1CCIF0 = 0;
			gs_uiTAA1CurrentCycleCnt4PwmLow++;
			return;
		}
		else if (gs_uiTAA1CurrentCycleCnt4PwmLow == g_uiTAA1_CycleCnt4PwmLow)
		{
			TAA1CCIF0 = 0;
			gs_uiTAA1CurrentCycleCnt4PwmLow++;
			// Push High
			{
				if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
					Set_PwmOutPrimaryChn_Level(g_stPwmOut2Chn.iPwmOutChn, HIGH);
				else if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
					Set_PwmOutSecondaryChn_Level(g_stPwmOut2Chn.bytPwmOutBoardID, g_stPwmOut2Chn.iPwmOutChn, HIGH);
			}
			gs_uiTAA1CurrentCycleCnt4PwmHigh = 0;
			return;
		}
	}
	
	void PWM_Out2_Stop(void)
	{
		TAA1_Stop();
		// Reset current cycle counts
		gs_uiTAA1CurrentCycleCnt4PwmHigh = 0;
		gs_uiTAA1CurrentCycleCnt4PwmLow  = 0;
		
		if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
			Set_PwmOutPrimaryChn_Level(g_stPwmOut2Chn.iPwmOutChn, LOW);
		else if (g_stPwmOut2Chn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
			Set_PwmOutSecondaryChn_Level(g_stPwmOut2Chn.bytPwmOutBoardID, g_stPwmOut2Chn.iPwmOutChn, LOW);
		
		return;
	}

#endif

