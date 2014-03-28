/*************************************************************************************
 * File name	: handling_System_cmd.c
 * Description	: Dedicatedly handle the system catalog of commands in this file-pair.
 * Creator		: XU ZAN [HSE-OT]
 * Creation date: Mon.	March 11, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 *************************************************************************************/

#include "handling_System_cmd.h"
#include "error_code.h"
#include "../version.h"

/****************************************************************************/
// Global variables :
extern int g_iErrorCodeNo;

/****************************************************************************/
// Static global variables :
	static char sErrorCode[][128] =
	{
		"Error code #  000: No error",
		"Error code # -001: Command Syntax error",
		"Error code # -002: Not exists this action catalog",
		"Error code # -003: Switch Board ID error",
		"Error code # -004: Switch action command error",
		"Error code # -005: Switch channel error",
		"Erroe code # -006: Switch state error",
		"Error code # -007: Action keyword 'SWITCH' error",
		"Error code # -008: Switch Group 24 bits length error",
		"Error code # -009: ADC command format error, no Qmark(?)",
		"Error code # -010: ADC command syntax error, no colon(:)",
		"Error code # -011: ADC measure type error",
		"Error code # -012: ADC channel error",
		"Error code # -013: Digital-IN command format error, no Qmark(?)",
		"Error code # -014: DAC output type error. Only support voltage outputing",
		"Error code # -015: DAC output voltage is beyond the range. Not support",
		"Error code # -016: Digital-IN channel number error",
		"Error code # -017: Digital-IN unknown command",
		"Error code # -018: Digital-IN command format error",
		"Error code # -019: Digital-OUT command format error",
		"Error code # -020: Digital-OUT board ID error",
		"Error code # -021: Digital-OUT channel number error",
		"Error code # -022: Digital-OUT channel state error",
		"Error code # -023: PWM command type error",
		"Error code # -024: Duplicate PWM output on the same port",
		"Error code # -025: PWM Out command format error. No space or colon found",
		"Error code # -026: Attribute string format error",
		"Error code # -027: The duty cycle format of PWM Out command is wrong. No '%' sign",
		"Error code # -028: The frequency value of PWM Out is out of range (0, 100000)Hz",
		"Error code # -029: The duty cycle value of PWM Out is out of range (0%, 100%)",
		"Error code # -030: The PWM Output channel number is wrong",
		"Error code # -031: No keyword 'OUT1' or 'OUT2' in PWM_Out command",
		"Error code # -032: No keyword 'PRIM' or 'SECN' in PWM_Out Start command",
		"Error code # -033: No keyword 'CHNL' in PWM_Out Start command",
		"Error code # -034: No keyword 'FREQ' in PWM_Out Config command",
		"Error code # -035: No keyword 'DUTY' in PWM_Out Config command",
		"Error code # -036: PWM_Out channel is wrong in Primary mode",
		"Error code # -037: PWM_Out channel is wrong in Secondary mode",
		"Error code # -038: PWM_Out board id is wrong in Secondary mode",
		"Error code # -039: Command syntax of System catalogue is wrong. No ':'",
		"Error code # -040: FIN command format error. No ':'",
		"Error code # -041: FIN board ID error",
		"Error code # -042: FIN channel number error",
		"Error code # -043: FIN command format error. No ' STATe'"
	};
/****************************************************************************/

int handling_System_Error_cmd(char sSystemErrorCmd[])
{
	char sErrorMesg[128] = {0};
	
	int iPosOfColon = strcspn(sSystemErrorCmd, ":"), iPosOfQmark = strcspn(sSystemErrorCmd, "?");
	if ( (iPosOfColon == strlen(sSystemErrorCmd)) || (iPosOfQmark == strlen(sSystemErrorCmd)) )
	{
		g_iErrorCodeNo = ERROR_CODE_MINUS_001;
	}
	
	sprintf(sErrorMesg, "$SYSTem:%s!", sErrorCode[g_iErrorCodeNo * (-1)]);
	#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
		UARTD2_SendData(sErrorMesg, strlen(sErrorMesg));	// Response the error message from COM port.
	#endif
	
	/* Clean the error code to 0 (i.e. No error.)
	 * Please note that current MCTBox firmware system does not support error code queue. 
	 * 
	 * Remarked bu XU ZAN@2013-12-14
	 */
	g_iErrorCodeNo = ERROR_CODE_000;
	
	return g_iErrorCodeNo;
}

int handling_System_IDN_cmd(char sSystemIdnCmd[])
{
	int iResult = 0;
	char sIDN[128] = {0};
	int iPosOfColon = strcspn(sSystemIdnCmd, ":"), iPosOfQmark = strcspn(sSystemIdnCmd, "?");
	if ( (iPosOfColon == strlen(sSystemIdnCmd)) || (iPosOfQmark == strlen(sSystemIdnCmd)) )
	{
		g_iErrorCodeNo = ERROR_CODE_MINUS_001;
		return g_iErrorCodeNo;
	}
	
	sprintf(sIDN, "$SYSTem:Hello, this is MCTBox. Welcome to call me!");
	#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
		UARTD2_SendData(sIDN, strlen(sIDN));
	#endif
	
	return iResult;
}

int handling_System_Version_cmd(char sSystemVerCmd[])
{
	int iResult = 0;
	char sVersion[128] = {0};
	int iPosOfColon = strcspn(sSystemVerCmd, ":"), iPosOfQmark = strcspn(sSystemVerCmd, "?");
	if ( (iPosOfColon == strlen(sSystemVerCmd)) || (iPosOfQmark == strlen(sSystemVerCmd)) )
	{
		g_iErrorCodeNo = ERROR_CODE_MINUS_001;
		return g_iErrorCodeNo;
	}
	
	sprintf(sVersion, "$SYSTem:%s!", CURRENT_VERSION);
	#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
		UARTD2_SendData(sVersion, strlen(sVersion));
	#endif
	
	return iResult;
}

/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

