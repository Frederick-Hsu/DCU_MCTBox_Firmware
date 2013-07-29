/**************************************************************************************************
 * File name	: handling_PWM_cmd.c
 * Description	: The file pair (handling_PWM_cmd.c / .h) will implement the handling and parsing
 *		  for PWM commands.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Mon.	June 24, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ***************************************************************************************************/
#include <string.h>

#include "handling_PWM_cmd.h"

int handling_PWMOut_cmd(char* sPWMOutCmdMesg)
{
	int iResult = 0;

	unsigned int uiLen = strlen(sPWMOutCmdMesg),
				 uiPosOfCmdSeparator_Colon = strcspn(sPWMOutCmdMesg, ":"),
				 uiPosOfCmdSeparator_Space = strcspn(sPWMOutCmdMesg, " ");
	return iResult;
}

int handling_PWMIn_cmd(char* sPWMInCmdMesg)
{
	int iResult = 0;
	return iResult;
}

int handling_PWMOut_Config(char *sPWMOutConfigMesg)
{
	int iResult = 0;
	return iResult;
}

int handling_PWMOut_Start(char *sPWMOutStartMesg)
{
	int iResult = 0;
	return iResult;
}

int handling_PWMOut_Stop(char *sPWMOutStopMesg)
{
	int iResult = 0;
	return iResult;
}


/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

