/*
*******************************************************************************
**
**  This device driver was created by Applilet2 for V850ES/Fx3
**  32-Bit Single-Chip Microcontrollers
**
**  Copyright(C) 2002, 2012 Renesas Electronics Corporation
**  All rights reserved by Renesas Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  Renesas Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	timer_user.c
**  Abstract :	This file implements device driver for Timer module.
**  APIlib :	Applilet2 for V850ES/Fx3 V2.43 [30 Mar 2011]
**
**  Device :	uPD70F3376
**
**  Compiler :	CA850
**
**  Creation date:	09/26/2012
**  
*******************************************************************************
*/

#pragma interrupt INTTAA0CC0 MD_INTTAA0CC0
#pragma interrupt INTTAA1CC0 MD_INTTAA1CC0
/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include "macrodriver.h"
#include "timer.h"
/* Start user code for include definition. Do not edit comment generated here */
/* End user code for include definition. Do not edit comment generated here */
#include "user_define.h"

#include "PWM/PWM_Out.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global definition. Do not edit comment generated here */
/* End user code for global definition. Do not edit comment generated here */

extern struct PwmOutChnSelector g_stPwmOutChn;

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
__interrupt void MD_INTTAA0CC0(void)
{
	/* Start user code. Do not edit comment generated here */	
	/* End user code. Do not edit comment generated here */
	
	//TAA0 timer ---> timeout
	TAA0_Stop();
	
	if (g_stPwmOutChn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
	{
		Set_PwmOutPrimaryChn_Level(g_stPwmOutChn.iPwmOutChn, LOW);
	}
	else if (g_stPwmOutChn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
	{
		Set_PwmOutSecondaryChn_Level(g_stPwmOutChn.bytPwmOutBoardID, g_stPwmOutChn.iPwmOutChn, LOW);
	}
	
	TAA1_Start();
}

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
__interrupt void MD_INTTAA1CC0(void)
{
	/* Start user code. Do not edit comment generated here */	
	/* End user code. Do not edit comment generated here */
	
	// TAA1 timer ---> timeout
	TAA1_Stop();
	
	if (g_stPwmOutChn.ePrimaryOrSecondary == PWM_ATTR_PRIMARY)
	{
		Set_PwmOutPrimaryChn_Level(g_stPwmOutChn.iPwmOutChn, HIGH);
	}
	else if (g_stPwmOutChn.ePrimaryOrSecondary == PWM_ATTR_SECONDARY)
	{
		Set_PwmOutSecondaryChn_Level(g_stPwmOutChn.bytPwmOutBoardID, g_stPwmOutChn.iPwmOutChn, HIGH);
	}
	
	TAA0_Start();	// Restart TAA0 timer
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


