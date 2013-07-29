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

	int handling_PWMOut_cmd(char* sPWMOutCmdMesg);
	int handling_PWMOut_Config(char *sPWMOutConfigMesg);
	int handling_PWMOut_Start(char *sPWMOutStartMesg);
	int handling_PWMOut_Stop(char *sPWMOutStopMesg);
	
	int handling_PWMIn_cmd(char* sPWMInCmdMesg);

#endif	/*  HANDLING_PWM_CMD_H  */

