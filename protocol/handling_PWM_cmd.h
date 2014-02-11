/**************************************************************************************************
 * File name	: handling_PWM_cmd.h
 * Description	: The file pair (handling_PWM_cmd.c / .h) will implement the handling and parsing
 *		  for PWM commands.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Mon.	June 24, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ***************************************************************************************************/


#ifndef HANDLING_PWM_CMD_H
#define HANDLING_PWM_CMD_H

	#include "../PWM/PWM_Out.h"

	int handling_PWMOut_cmd(char* sPWMOutCmdMesg);
	int handling_PWMOut_Config(char *sPWMOutConfigMesg);
	int handling_PWMOut_Start(char *sPWMOutStartMesg);
	int handling_PWMOut_Stop(char *sPWMOutStopMesg);

	int Parsing_PWMOut_Config_Param(char  sARGIN_PwmOutConfigStr[], 
					char *sARGOUT_PwmOutN,
					char *sARGOUT_PwmOutFreq,
					char *sARGOUT_PwmOutDuty,
					char *sARGOUT_PwmOutVolt);
	
	int Get_PwmOutConfig_AttributeGroupValue(char sARGIN_PwmOutConfig_AttributeGroup[],
						 char *sARGOUT_PwmOutConfig_OutN,
						 char *sARGOUT_PwmOutConfig_Freq,
						 char *sARGOUT_PwmOutConfig_Duty,
						 char *sARGOUT_PwmOutConfig_Volt);
	
	int Check_PwmOutConfig_Param(PST_PWM_PARAM 	pARGOUT_PwmOutParam, 
				     char		sPwmOutFreq[], 
				     char 		sPwmOutDuty[], 
				     char 		sPwmOutVolt[]);
	
	int Get_PwmOutStart_AttributeGroupValue(char sARGIN_PwmOutStart_AttributeGroup[],
						char *sARGOUT_PwmOutStart_OutN,
						char *sARGOUT_PwmOutStart_PriSec,
						char *sARGOUT_PwmOutStart_BoardID,
						char *sARGOUT_PwmOutStart_Chnl);
	
	int Check_PwmOutStart_Param(char sARGIN_PwmOutStart_OutN[],
				    char sARGIN_PwmOutStart_PriSec[],
				    char sARGIN_PwmOutStart_BoardID[],
				    char sARGIN_PwmOutStart_Chnl[],
				    enum Pwm_Out_No *pePwmOutNr,
				    int *pePrimOrSecn,
				    int *pePwmChn,
				    BYTE *pbytPwmBoardID);


/**********************************************************************************************/
	int handling_PWMIn_cmd(char* sPWMInCmdMesg);

	

#endif	/*  HANDLING_PWM_CMD_H  */

