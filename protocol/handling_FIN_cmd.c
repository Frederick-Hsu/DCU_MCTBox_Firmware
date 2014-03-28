/*************************************************************************************************************************
 * File name		: handling_FIN_cmd.h
 * Description		: To handle the "FIN" type of commands, in order to support the Optical-couple-INput board.
 * Creator			: XU ZAN	[HSE-OT]
 * Creation date	: Thur.		March 27, 2014
 * Copyright(C)		2014	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 *************************************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "handling_FIN_cmd.h"
#include "../DIO/Digital_IN.h"

/****************************************************************************/
// Global variables :
extern int g_iErrorCodeNo;

int handling_1GroupOfChs_FIN_cmd(char sARGIN_1GroupFinCmdMesg[], char *sARGOUT_1GroupFinChnState)
{
	int iResult = 0;
	char sFin24ChnStates[32] = {0}, sFinBoardID[16] = {0};
	long lFinBoardID = 0x00;
	
	unsigned int uiPosOfSpace = strcspn(sARGIN_1GroupFinCmdMesg, " "),
				 uiPosOfColon = strcspn(sARGIN_1GroupFinCmdMesg, ":");
	
	if (strstr(sARGIN_1GroupFinCmdMesg, ":*"))
	{
		strncpy(sFinBoardID, sARGIN_1GroupFinCmdMesg+uiPosOfSpace+1, uiPosOfColon-uiPosOfSpace-1);
		Convert_Str_To_Int(sFinBoardID, &lFinBoardID);
		
		#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
			Read_DinBoard_24Chs_State(lFinBoardID, sFin24ChnStates);
		#endif
		sprintf(sARGOUT_1GroupFinChnState, "FIN 0x%02X:* %s", lFinBoardID, sFin24ChnStates);
	}
	else
	{
		g_iErrorCodeNo = -40;
		return g_iErrorCodeNo;
	}
	
	return iResult;
}

int handling_SingleCH_FIN_cmd(char sARGIN_FinSingleChnCmdMesg[], char *sARGOUT_FinSingleChnState)
{
	int iResult = 0;
	unsigned int uiLen 			= strlen(sARGIN_FinSingleChnCmdMesg),
				 uiPosOfColon 	= strcspn(sARGIN_FinSingleChnCmdMesg, ":"),
				 uiPosOfSpace 	= strcspn(sARGIN_FinSingleChnCmdMesg, " ");
	
	char sFinChn[8] = {0},  sFinBoardID[16] = {0}, sFinChnState[8] = {0};
	long lFinChn    = 0x00, lFinBoardID     = 0x00;
	char *cColonSubStr = NULL;
	int iFinBoardCHnState = 0;
	
	if (strstr(sARGIN_FinSingleChnCmdMesg, " STA"))
	{
		cColonSubStr = strstr(sARGIN_FinSingleChnCmdMesg, ":");
		strncpy(sFinBoardID, sARGIN_FinSingleChnCmdMesg+uiPosOfSpace+1, uiPosOfColon-uiPosOfSpace-1);
		Convert_Str_To_Int(sFinBoardID, &lFinBoardID);
		
		strncpy(sFinChn, cColonSubStr+1, strcspn(cColonSubStr, " ")-1);
		Convert_Str_To_Int(sFinChn, &lFinChn);
		
		if ( (lFinBoardID<0x00) || (lFinBoardID>0xFF) )
		{
			g_iErrorCodeNo = -41;
			return g_iErrorCodeNo;
		}
		if ( (lFinChn<1) || (lFinChn>24) )
		{
			g_iErrorCodeNo = -42;
			return g_iErrorCodeNo;
		}
		
		#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
			iFinBoardCHnState = Read_DinBoard_CHn_State(lFinBoardID, lFinChn-1);	// Channel # starting from 1 to 24
		#endif
		sprintf(sARGOUT_FinSingleChnState, 
				"FIN 0x%02X:%ld %s",
				lFinBoardID,
				lFinChn,
				(iFinBoardCHnState == 1?"HIGH":"LOW"));
	}
	else
	{
		g_iErrorCodeNo = -43;
		return g_iErrorCodeNo;
	}
	
	return iResult;
}


/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
