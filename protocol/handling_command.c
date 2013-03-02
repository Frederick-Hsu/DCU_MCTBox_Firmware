/********************************************************************************************
 * File name	: handling_command.c
 * Description	: Declare and implement to handle command for all action catelogues.
 * Creator	: XU ZAN [HSE-OT]
 * Creation date: Fri.	March 1, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ********************************************************************************************/

/****************************************************************************/
// Includeds :
#include "handling_command.h"
#include "handling_Switch_Relay_Control_cmd.h"
#include "handling_ADC_DAC_cmd.h"


/****************************************************************************/
// Variables :
extern int g_iErrorCodeNo;



/****************************************************************************/
// Functions implementation :
int handling_Switch_Relay_Control_cmd(char* sSwitch_Relay_Ctrl_cmd_Mesg)
{
	int iError = 0;
	// char *pCmdStringHeadPtr = sSwitch_Relay_Ctrl_cmd_Mesg;

	const int iCmdSeparator_Semicolon = ';'	,
		  iCmdSeparator_Colon 	 = ':'	,
		  iCmdSeparator_Comma 	 = ','	,
		  iCmdSeparator_Space 	 = ' '	,
		  iCmdSeparator_Sigh	 = '!'	,
		  iCmdSeparator_Qmark	 = '?'	;

	unsigned int uiPosOfCmdSeparator_Semicolon 	= 0,	// :
		     uiPosOfCmdSeparator_Colon		= 0,	// " "
		     uiPosOfCmdSeparator_Comma		= 0,	// ,
		     uiPosOfCmdSeparator_Space 		= 0,	// ;
		     uiPosOfCmdSeparator_Sigh		= 0,	// !
		     uiPosOfCmdSeparator_Qmark		= 0;	// ?
		     
	char sTemp_1CmdUnit_String[64] = {0},
	     sTempSubString[128] = {0};
	
	unsigned int uiLen = strlen(sSwitch_Relay_Ctrl_cmd_Mesg);
	/*
	 * Search and locate the position of these command separator.
	 * For command separator ";" and "," it can judge the command kind.
	 */
	uiPosOfCmdSeparator_Semicolon = strcspn(sSwitch_Relay_Ctrl_cmd_Mesg, ";");	// Note : 
	uiPosOfCmdSeparator_Colon = strcspn(sSwitch_Relay_Ctrl_cmd_Mesg, ":");		// If the specified separator was not found, it will 
	uiPosOfCmdSeparator_Comma = strcspn(sSwitch_Relay_Ctrl_cmd_Mesg, ",");		// return the length of string = strlen(sCmdMesg)
	uiPosOfCmdSeparator_Space = strcspn(sSwitch_Relay_Ctrl_cmd_Mesg, " ");		// Not the negative value.
	uiPosOfCmdSeparator_Sigh = strcspn(sSwitch_Relay_Ctrl_cmd_Mesg, "!");		// Remarked by XUZAN@2013-02-21
	uiPosOfCmdSeparator_Qmark = strcspn(sSwitch_Relay_Ctrl_cmd_Mesg, "?");
	
#if 1
	/*
	 * How to differentiate the command kind? 
	 * please refer to the explanantion from file "How_to_implement_command_message_parsing.txt"
	 */
	
	/*START : Kind 2=========================================================*/
	// Corresponding to Kind 2: Multi parameters <----> Multi attributes
	if (uiPosOfCmdSeparator_Semicolon != uiLen)	// Search ";"
	{
		iError = handling_Multi_Switches(sSwitch_Relay_Ctrl_cmd_Mesg);
		if (iError)
		{
			g_iErrorCodeNo = iError;
			return g_iErrorCodeNo;
		}
	}
	/*END : Kind 2=========================================================*/
	
	/*START : Kind 3=========================================================*/
	// Corresponding to Kind 3: Single parameter <----> Multi attributes
	else if (uiPosOfCmdSeparator_Comma != uiLen)	// Search ","
	{
		iError = handling_Batch_Switches(sSwitch_Relay_Ctrl_cmd_Mesg);
		if (iError)
		{
			g_iErrorCodeNo = iError;
			return g_iErrorCodeNo;
		}
	}
	/*END : Kind 3=========================================================*/

	/*START : Kind 1=========================================================*/
	// Corresponding to Kind 1: Single parameter <----> Single attribute
	else
	{
		strncpy(sTemp_1CmdUnit_String, sSwitch_Relay_Ctrl_cmd_Mesg, uiLen-1);	// don't copy the last char '!'
		iError = handling_SwitchAction_1Command_unit(sTemp_1CmdUnit_String);
		if (iError)
		{
			g_iErrorCodeNo = iError;
			return g_iErrorCodeNo;
		}
	}
	/*END : Kind 1=========================================================*/
#endif
	return 0;
}


int handling_ADC_cmd(char* sADC_cmd_Mesg)
{
	int iResult = 0;
	
	
/***************************************/
	return iResult;
}


int handling_DAC_cmd(char* sDAC_cmd_Mesg)
{
	int iResult = 0;
	
/***************************************/
	return iResult;
}
/*
 * END OF FILE  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
