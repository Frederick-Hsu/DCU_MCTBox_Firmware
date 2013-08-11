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
#include "../utility.h"

extern int g_iErrorCodeNo;

int handling_PWMOut_cmd(char* sPWMOutCmdMesg)
{
	int iResult = 0;

	unsigned int uiLen = strlen(sPWMOutCmdMesg),
				 uiPosOfCmdSeparator_Colon = strcspn(sPWMOutCmdMesg, ":"),
				 uiPosOfCmdSeparator_Space = strcspn(sPWMOutCmdMesg, " ");
	char sPwmOut_ActionName[16] = {0}, sPwmOut_RestString[64] = {0};
	
	if ( (uiPosOfCmdSeparator_Colon == uiLen) || (uiPosOfCmdSeparator_Space == uiLen) || (uiPosOfCmdSeparator_Colon <= uiPosOfCmdSeparator_Space) )
	{
		g_iErrorCodeNo = -25;
		return g_iErrorCodeNo;
	}
	strncpy(sPwmOut_ActionName, sPWMOutCmdMesg+uiPosOfCmdSeparator_Space+1, uiPosOfCmdSeparator_Colon-uiPosOfCmdSeparator_Space-1);
	strncpy(sPwmOut_RestString, sPWMOutCmdMesg+uiPosOfCmdSeparator_Colon+1, uiLen-uiPosOfCmdSeparator_Colon-1);
	
	ToUpperString(sPwmOut_ActionName);
	ToUpperString(sPwmOut_RestString);
	if (!strncmp(sPwmOut_ActionName, "CONF", 4))
	{
		iResult = handling_PWMOut_Config(sPwmOut_RestString);
	}
	else if (!strncmp(sPwmOut_ActionName, "START", 5))
	{
		iResult = handling_PWMOut_Start(sPwmOut_RestString);
	}
	else if (!strncmp(sPwmOut_ActionName, "STOP", 4))
	{
		iResult = handling_PWMOut_Stop(sPwmOut_RestString);
	}

	return iResult;
}

int handling_PWMOut_Config(char *sPWMOutConfigMesg)
{
	int iResult = 0;

	char sPwmOutN[8] = {0}, 
		 sPwmOutFreq[32] = {0}, 
		 sPwmOutDuty[32] = {0}, 
		 sPwmOutVolt[32] = {0};
	/*
	iResult = Parsing_PWMOut_Config_Param(sPWMOutConfigMesg, 
										  sPwmOutN, 
										  sPwmOutFreq, 
										  sPwmOutDuty, 
										  sPwmOutVolt);
	 */
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

#if 0	// Remark : this function has not implemented OK, because of bug.
	int Parsing_PWMOut_Config_Param(char sARGIN_PwmOutConfigStr[], 
									char *sARGOUT_PwmOutN,
									char *sARGOUT_PwmOutFreq,
									char *sARGOUT_PwmOutDuty,
									char *sARGOUT_PwmOutVolt)
	{
		int iResult = 0;
		unsigned int uiLen = strlen(sARGIN_PwmOutConfigStr),
							 uiPosOfColon = strcspn(sARGIN_PwmOutConfigStr, ":");
		char sTempStrToFetch[8][16] = {0}, sTempRestStr[32] = {0}, sTempRestRearStr[64] = {0};
		int iCnt = 0;
		strcpy(sTempRestStr, sARGIN_PwmOutConfigStr);
		strcpy(sTempRestRearStr, sTempRestStr);
		do
		{
			strncpy(sTempStrToFetch[iCnt], sTempRestRearStr, uiPosOfColon);
			strncpy(sTempRestRearStr, sTempRestStr+uiPosOfColon+1, strlen(sTempRestStr)-uiPosOfColon-1);

			uiPosOfColon = strcspn(sTempRestRearStr, ":");
			iCnt++;
		}
		while (strchr(sTempRestRearStr, ':') != NULL);

		return iResult;
	}
#endif

/***************************************************************************************************/
int handling_PWMIn_cmd(char* sPWMInCmdMesg)
{
	int iResult = 0;
	return iResult;
}

/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

