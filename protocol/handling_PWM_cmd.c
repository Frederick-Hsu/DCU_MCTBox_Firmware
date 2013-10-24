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
#include <stdio.h>
#include <stdlib.h>

#include "handling_PWM_cmd.h"
#include "../utility.h"
#include "../PWM/PWM_Out.h"

extern int g_iErrorCodeNo;

int handling_PWMOut_cmd(char* sPWMOutCmdMesg)
{
	int iResult = 0;

	unsigned int uiLen = strlen(sPWMOutCmdMesg),
		     uiPosOfCmdSeparator_Colon = strcspn(sPWMOutCmdMesg, ":"),
		     uiPosOfCmdSeparator_Space = strcspn(sPWMOutCmdMesg, " ");
	char sPwmOut_ActionName[16] = {0}, sPwmOut_RestString[64] = {0};
	
	if ( (uiPosOfCmdSeparator_Colon == uiLen) || 
	     (uiPosOfCmdSeparator_Space == uiLen) || 
	     (uiPosOfCmdSeparator_Colon <= uiPosOfCmdSeparator_Space) )
	{
		g_iErrorCodeNo = -25;
		return g_iErrorCodeNo;
	}
	strncpy(sPwmOut_ActionName, 
		sPWMOutCmdMesg+uiPosOfCmdSeparator_Space+1, 
		uiPosOfCmdSeparator_Colon-uiPosOfCmdSeparator_Space-1);

	strncpy(sPwmOut_RestString, 
		sPWMOutCmdMesg+uiPosOfCmdSeparator_Colon+1, 
		uiLen-uiPosOfCmdSeparator_Colon-1-1);	// Discard the last char '!'
	
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

	ST_PWM_PARAM tPwmOutParam = {0.00};
	enum Pwm_Out_No ePwmOutNr = 0;

	iResult = Get_PwmOutConfig_AttributeGroupValue(sPWMOutConfigMesg, 
						       sPwmOutN,
						       sPwmOutFreq, 
						       sPwmOutDuty, 
						       sPwmOutVolt);
	
	iResult = Check_PwmOutConfig_Param(&tPwmOutParam, 
					   sPwmOutFreq, 
					   sPwmOutDuty, 
					   sPwmOutVolt);
	if (!strcmp(sPwmOutN, "OUT1"))
	{
		ePwmOutNr = PWM_OUT1;
	}
	else if (!strcmp(sPwmOutN, "OUT2"))
	{
		ePwmOutNr = PWM_OUT2;
	}
	else
	{
		g_iErrorCodeNo = -30;
		return g_iErrorCodeNo;
	}
#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	Set_Config_PWM_Out_Param(ePwmOutNr, tPwmOutParam);
#endif

	return iResult;
}

int handling_PWMOut_Start(char *sPWMOutStartMesg)
{
	int iResult = 0;
	char sPwmOutN[8] = {0},
	     sPwmOutPriSec[16] = {0},
	     sPwmOutBoardID[16] = {0},
	     sPwmOutChnl[16] = {0};
	
	enum Pwm_Out_No ePwmOutNr = 0;
	int ePrimOrSecn = 0;
	int ePwmChn = 0;
	BYTE bytPwmBoardID = 0;

	iResult = Get_PwmOutStart_AttributeGroupValue(sPWMOutStartMesg, 
						      sPwmOutN, 
						      sPwmOutPriSec, 
						      sPwmOutBoardID, 
						      sPwmOutChnl);
	iResult = Check_PwmOutStart_Param(sPwmOutN,
					  sPwmOutPriSec,
					  sPwmOutBoardID,
					  sPwmOutChnl,
					  &ePwmOutNr,
					  &ePrimOrSecn,
					  &ePwmChn,
					  &bytPwmBoardID);

#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	PWM_Out_Start(ePwmOutNr, ePrimOrSecn, ePwmChn, bytPwmBoardID);
#endif
	return iResult;
}

int handling_PWMOut_Stop(char *sPWMOutStopMesg)
{
	int iResult = 0;
	enum Pwm_Out_No ePwmOutN = 0;
	if (!strcmp(sPWMOutStopMesg, "OUT1"))
	{
		ePwmOutN = PWM_OUT1;
	}
	else if (!strcmp(sPWMOutStopMesg, "OUT2"))
	{
		ePwmOutN = PWM_OUT2; 
	}
	else
	{
		g_iErrorCodeNo = -31;
		return -31;
	}
#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	PWM_Out_Stop(ePwmOutN);
#endif
	return iResult;
}

#if 1
	/* This function has no flexibility when parsing other types of attributes group 
	 * in the command string. But it is easy to understand.
	 * So it is not recommended to use it.
	 */
	int Parsing_PWMOut_Config_Param(char sARGIN_PwmOutConfigStr[], 
	 				char *sARGOUT_PwmOutN,
					char *sARGOUT_PwmOutFreq,
					char *sARGOUT_PwmOutDuty,
					char *sARGOUT_PwmOutVolt)
	{
		int iResult = 0;
		unsigned int uiLen = strlen(sARGIN_PwmOutConfigStr),
			     uiPosOfColon = strcspn(sARGIN_PwmOutConfigStr, ":"),
			     uiPosOfSpace = 0;
		char sTempStrToFetch[8][32] = {0}, sTempRestStr[128] = {0}, sTempRestRearStr[128] = {0};
		char sPwmAttributes[4][32] = {0};
		int iCnt = 0;
		strcpy(sTempRestStr, sARGIN_PwmOutConfigStr);
		strcpy(sTempRestRearStr, sTempRestStr);
		do
		{
			strncpy(sTempStrToFetch[iCnt], sTempRestStr, uiPosOfColon);
			memset(sTempRestRearStr, 0, 128*sizeof(char));
			
			strncpy(sTempRestRearStr, 
				sTempRestStr+uiPosOfColon+1, 
				strlen(sTempRestStr)-uiPosOfColon-1);
			memset(sTempRestStr, 0, 128*sizeof(char));
			memcpy(sTempRestStr, sTempRestRearStr, strlen(sTempRestRearStr));

			uiPosOfColon = strcspn(sTempRestStr, ":");
			iCnt++;
		}
		while (strchr(sTempRestStr, ':') != NULL);
		strncpy(sTempStrToFetch[iCnt], sTempRestStr, strlen(sTempRestStr));
		while (iCnt >= 0)
		{
			if (!strncmp(sTempStrToFetch[iCnt], "OUT", 3))
				sprintf(sPwmAttributes[0], "%s", sTempStrToFetch[iCnt]);
			if (!strncmp(sTempStrToFetch[iCnt], "FREQ", 4))
				sprintf(sPwmAttributes[1], "%s", sTempStrToFetch[iCnt]);
			if (!strncmp(sTempStrToFetch[iCnt], "DUTY", 4))
				sprintf(sPwmAttributes[2], "%s", sTempStrToFetch[iCnt]);
			if (!strncmp(sTempStrToFetch[iCnt], "VOLT", 4))
				sprintf(sPwmAttributes[3], "%s", sTempStrToFetch[iCnt]);
			iCnt--;
		}

		strcpy(sARGOUT_PwmOutN, sPwmAttributes[0]);
		uiPosOfSpace = strcspn(sPwmAttributes[1], " ");
		strncpy(sARGOUT_PwmOutFreq, 
			sPwmAttributes[1]+uiPosOfSpace+1, 
			strlen(sPwmAttributes[1])-uiPosOfSpace-1);
		uiPosOfSpace = strcspn(sPwmAttributes[2], " ");
		strncpy(sARGOUT_PwmOutDuty, 
			sPwmAttributes[2]+uiPosOfSpace+1, 
			strlen(sPwmAttributes[2])-uiPosOfSpace-1);
		uiPosOfSpace = strcspn(sPwmAttributes[3], " ");
		strncpy(sARGOUT_PwmOutVolt, 
			sPwmAttributes[3]+uiPosOfSpace+1, 
			strlen(sPwmAttributes[3])-uiPosOfSpace-1);

		return iResult;
	}
#endif

	int Get_PwmOutConfig_AttributeGroupValue(char sARGIN_PwmOutConfig_AttributeGroup[],
						 char *sARGOUT_PwmOutConfig_OutN,
						 char *sARGOUT_PwmOutConfig_Freq,
						 char *sARGOUT_PwmOutConfig_Duty,
						 char *sARGOUT_PwmOutConfig_Volt)
	{
		int iResult = 0;
	#if defined (EXPERIMENT_DEBUG_PURPOSE)
		PAttributeList pAttrGrp = (PAttributeList)malloc(sizeof(AttributeList));
		void *pHead = pAttrGrp, *pTemp = NULL;
		Experiment_FetchSegmentAttributeGroup(sARGIN_PwmOutConfig_AttributeGroup, ":", pAttrGrp);

		do
		{
			if (!strncmp(pAttrGrp->Attr.sAttributeName, "OUT", 3))
			{
				sprintf(sARGOUT_PwmOutConfig_OutN, "%s", pAttrGrp->Attr.sAttributeName);
			}
			if (!strncmp(pAttrGrp->Attr.sAttributeName, "FREQ", 4))
			{
				sprintf(sARGOUT_PwmOutConfig_Freq, "%s", pAttrGrp->Attr.sAttributeValue);
			}
			if (!strncmp(pAttrGrp->Attr.sAttributeName, "DUTY", 4))
			{
				sprintf(sARGOUT_PwmOutConfig_Duty, "%s", pAttrGrp->Attr.sAttributeValue);
			}
			if (!strncmp(pAttrGrp->Attr.sAttributeName, "VOLT", 4))
			{
				sprintf(sARGOUT_PwmOutConfig_Volt, "%s", pAttrGrp->Attr.sAttributeValue);
			}
			pAttrGrp = pAttrGrp->pNextNode;
		}
		while (pAttrGrp != NULL);

		// Destroy the whole list node by node.  It is a must.
		pAttrGrp = (PAttributeList)pHead;
		#if 0									// Compare the 2 methods of destroying a list:
		ReleaseAttributeList(pAttrGrp);						// Method 1: Cannot absolutely destroy the whole list, including the head pointer
		#else									// His head pointer is still alive, even though it has occupied the memory space of a pointer (4 bytes).
		do									// Method 2: Absolutely destroy the whole list, including the head pointer
		{									// His head pointer was assigned to NULL. 
			pTemp = pAttrGrp->pNextNode;
			free(pAttrGrp);
			pAttrGrp = (PAttributeList)pTemp;
		}
		while (pAttrGrp != NULL);
		#endif
	/*****************************************************************************************************************/
	#else
		#if 0
			iResult = Parsing_PWMOut_Config_Param(sARGIN_PwmOutConfig_AttributeGroup, 
							      sARGOUT_PwmOutConfig_OutN, 
							      sARGOUT_PwmOutConfig_Freq, 
							      sARGOUT_PwmOutConfig_Duty, 
							      sARGOUT_PwmOutConfig_Volt);
		#else
			int iCnt = 0;
			Attribute t_PwmOutConfigAttrGroup[4] = {NULL};
			iResult = Fetch_SegmentAttributesGroup_From_SeparatorStr(sARGIN_PwmOutConfig_AttributeGroup, ":", t_PwmOutConfigAttrGroup);
			for (iCnt=0; iCnt<4; iCnt++)
			{
				if (!strncmp(t_PwmOutConfigAttrGroup[iCnt].sAttributeName, "OUT", 3))
					sprintf(sARGOUT_PwmOutConfig_OutN, "%s", t_PwmOutConfigAttrGroup[iCnt].sAttributeName);
				if (!strncmp(t_PwmOutConfigAttrGroup[iCnt].sAttributeName, "FREQ", 4))
					sprintf(sARGOUT_PwmOutConfig_Freq, "%s", t_PwmOutConfigAttrGroup[iCnt].sAttributeValue);
				if (!strncmp(t_PwmOutConfigAttrGroup[iCnt].sAttributeName, "DUTY", 4))
					sprintf(sARGOUT_PwmOutConfig_Duty, "%s", t_PwmOutConfigAttrGroup[iCnt].sAttributeValue);
				if (!strncmp(t_PwmOutConfigAttrGroup[iCnt].sAttributeName, "VOLT", 4))
					sprintf(sARGOUT_PwmOutConfig_Volt, "%s", t_PwmOutConfigAttrGroup[iCnt].sAttributeValue);
			}
			/* Here you cannot destroy the variable, because it was assigned in the heap memory,
			 * it will be automatically freed after quit the current function.
			 *
			 * Please remember that only the memory space assigned at stack need to be freed manually by programmer.
			 */
			// free(t_PwmOutConfigAttrGroup);	
		#endif
	#endif	/*  EXPERIMENT_DEBUG_PURPOSE  */
		if (0 == strlen(sARGOUT_PwmOutConfig_OutN))
		{
			g_iErrorCodeNo = -31;
			iResult = g_iErrorCodeNo;
		}
		if (0 == strlen(sARGOUT_PwmOutConfig_Freq))
		{
			g_iErrorCodeNo = -34;
			iResult = g_iErrorCodeNo;
		}
		if (0 == strlen(sARGOUT_PwmOutConfig_Duty))
		{
			g_iErrorCodeNo = -35;
			iResult = g_iErrorCodeNo;
		}
		return iResult;
	}

	int Check_PwmOutConfig_Param(PST_PWM_PARAM 	pARGOUT_PwmOutParam, 
				     char 		sPwmOutFreq[], 
				     char 		sPwmOutDuty[], 
				     char 		sPwmOutVolt[])
	{
		int iResult = 0;
		float fPwmOutFreq = 0.00f, fPwmOutDutyCycle = 0.00f, fPwmOutVolt = 0.00f;
		unsigned int uiPosOfPercentageSign = 0;
		char sPwmOutDutyValue[8] = {0};

	#if defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
		fPwmOutFreq = atof(sPwmOutFreq);
	#else
		fPwmOutFreq = atoff(sPwmOutFreq);
	#endif

		uiPosOfPercentageSign = strcspn(sPwmOutDuty, "%");
		if (uiPosOfPercentageSign == strlen(sPwmOutDuty))	// Check whether the duty_cycle string contained a '%' sign
		{
			g_iErrorCodeNo = -27;
			return g_iErrorCodeNo;
		}
		strncpy(sPwmOutDutyValue, sPwmOutDuty, uiPosOfPercentageSign);
	#if defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
		fPwmOutDutyCycle = atof(sPwmOutDutyValue);
		fPwmOutVolt = atof(sPwmOutVolt);
	#else
		fPwmOutDutyCycle = atoff(sPwmOutDutyValue);
		fPwmOutVolt = atoff(sPwmOutVolt);
	#endif
		/*
		 * Check the PWM parameters' range.
		 */
		if ( (fPwmOutFreq<0.00) || (fPwmOutFreq>100000.00) )	// PWM frequence range: (0, 100000)Hz
		{
			g_iErrorCodeNo = -28;
			return g_iErrorCodeNo;
		}
		if ( (fPwmOutDutyCycle<0.00) || (fPwmOutDutyCycle>100.00) )	// PWM duty cycle range: (0%, 100%)
		{
			g_iErrorCodeNo = -29;
			return g_iErrorCodeNo;
		}

		// Assign these values to PwmOutParam
		pARGOUT_PwmOutParam->fPWM_Freq_InHz = fPwmOutFreq;
		pARGOUT_PwmOutParam->fPWM_DutyCycle_InPercentage = fPwmOutDutyCycle;
		pARGOUT_PwmOutParam->fPWM_Amplitude_InVolt = fPwmOutVolt;

		return iResult;
	}

	int Get_PwmOutStart_AttributeGroupValue(char sARGIN_PwmOutStart_AttributeGroup[],
						char *sARGOUT_PwmOutStart_OutN,
						char *sARGOUT_PwmOutStart_PriSec,
						char *sARGOUT_PwmOutStart_BoardID,
						char *sARGOUT_PwmOutStart_Chnl)
	{
		int iResult = 0;
		PAttributeList pAttrGrp = (PAttributeList)malloc(sizeof(AttributeList));
		void *pHead = pAttrGrp, *pTemp = NULL;

		Experiment_FetchSegmentAttributeGroup(sARGIN_PwmOutStart_AttributeGroup, ":", pAttrGrp);
		do
		{
			if (!strncmp(pAttrGrp->Attr.sAttributeName, "OUT", 3))
				sprintf(sARGOUT_PwmOutStart_OutN, "%s", pAttrGrp->Attr.sAttributeName);

			if (!strncmp(pAttrGrp->Attr.sAttributeName, "PRIM", 4))
				sprintf(sARGOUT_PwmOutStart_PriSec, "%s", pAttrGrp->Attr.sAttributeName);
			else if (!strncmp(pAttrGrp->Attr.sAttributeName, "SECN", 4))
				sprintf(sARGOUT_PwmOutStart_PriSec, "%s", pAttrGrp->Attr.sAttributeName);

			if (!strncmp(pAttrGrp->Attr.sAttributeName, "BDID", 4))
				sprintf(sARGOUT_PwmOutStart_BoardID, "%s", pAttrGrp->Attr.sAttributeValue);

			if (!strncmp(pAttrGrp->Attr.sAttributeName, "CHNL", 4))
				sprintf(sARGOUT_PwmOutStart_Chnl, "%s", pAttrGrp->Attr.sAttributeValue);

			pAttrGrp = pAttrGrp->pNextNode;
		}
		while (pAttrGrp != NULL);
		// Destroy the pAttrGrp list
		pAttrGrp = (PAttributeList)pHead;
		do
		{
			pTemp = pAttrGrp->pNextNode;
			// free(pAttrGrp->Attr.sAttributeName);
			// free(pAttrGrp->Attr.sAttributeValue);
			free(pAttrGrp);
			pAttrGrp = (PAttributeList)pTemp;
		}
		while (pAttrGrp != NULL);
		
		if (0 == strlen(sARGOUT_PwmOutStart_OutN))
		{
			g_iErrorCodeNo = -31;
			iResult = g_iErrorCodeNo;
		}
		if (0 == strlen(sARGOUT_PwmOutStart_PriSec))
		{
			g_iErrorCodeNo = -32;
			iResult = g_iErrorCodeNo;
		}
		if (0 == strlen(sARGOUT_PwmOutStart_Chnl))
		{
			g_iErrorCodeNo = -33;
			iResult = g_iErrorCodeNo;
		}
		return iResult;
	}

	int Check_PwmOutStart_Param(char sARGIN_PwmOutStart_OutN[],
				    char sARGIN_PwmOutStart_PriSec[],
				    char sARGIN_PwmOutStart_BoardID[],
				    char sARGIN_PwmOutStart_Chnl[],
				    enum Pwm_Out_No *pePwmOutNr,
				    int *pePrimOrSecn,
				    int *pePwmChn,
				    BYTE *pbytPwmBoardID)
	{
		int iResult = 0;

		enum Pwm_Out_No eOutNr = 0;
		int iPriOrSec = 0, iPwmChn = 0;
		// BYTE bPwmBID = 0;
		long lBoardID = 0;

		if (!strcmp(sARGIN_PwmOutStart_OutN, "OUT1"))
			eOutNr = PWM_OUT1;
		else if (!strcmp(sARGIN_PwmOutStart_OutN, "OUT2"))
			eOutNr = PWM_OUT2;
		else
		{
			g_iErrorCodeNo = -31;
			iResult = g_iErrorCodeNo;
		}
		memcpy(pePwmOutNr, &eOutNr, sizeof(eOutNr));
		// pePwmOutNr = &eOutNr;

		if (!strcmp(sARGIN_PwmOutStart_PriSec, "PRIM"))
			iPriOrSec = PWM_ATTR_PRIMARY;
		else if (!strcmp(sARGIN_PwmOutStart_PriSec, "SECN"))
			iPriOrSec = PWM_ATTR_SECONDARY;
		else
		{
			g_iErrorCodeNo = -32;
			iResult = g_iErrorCodeNo;
		}
		memcpy(pePrimOrSecn, &iPriOrSec, sizeof(iPriOrSec));
		// pePrimOrSecn = &iPriOrSec;

		if (0 != strlen(sARGIN_PwmOutStart_BoardID))
		{
			Convert_Str_To_Int(sARGIN_PwmOutStart_BoardID, &lBoardID);
			// bPwmBID = atoi(sARGIN_PwmOutStart_BoardID);
			memcpy(pbytPwmBoardID, &lBoardID, sizeof(BYTE));
			// pbytPwmBoardID = &bPwmBID;
		}

		iPwmChn = atoi(sARGIN_PwmOutStart_Chnl);
		memcpy(pePwmChn, &iPwmChn, sizeof(iPwmChn));
		// pePwmChn = &iPwmChn;

		if (!strcmp(sARGIN_PwmOutStart_PriSec, "PRIM"))
		{
			if ( (iPwmChn!=PwmOutChn_Primary0) || (iPwmChn!=PwmOutChn_Primary1) )
			{
				g_iErrorCodeNo = -36;
				iResult = g_iErrorCodeNo;
			}
		}
		else if (!strcmp(sARGIN_PwmOutStart_PriSec, "SECN"))
		{
			if ( (iPwmChn<PwmOutChn_Secondary0) || (iPwmChn>24) )
			{
				g_iErrorCodeNo = -37;
				iResult = g_iErrorCodeNo;
			}
		}

		if ( (lBoardID<0) || (lBoardID>0xFF) )
		{
			g_iErrorCodeNo = -38;
			iResult = g_iErrorCodeNo;
		}

		return iResult;
	}
/***************************************************************************************************/
int handling_PWMIn_cmd(char* sPWMInCmdMesg)
{
	int iResult = 0;
	return iResult;
}

/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

