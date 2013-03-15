/********************************************************************************************
 * File name	: handling_command.h
 * Description	: Declare and implement to handle command for all action catelogues.
 * Creator	: XU ZAN [HSE-OT]
 * Creation date: Fri.	March 1, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ********************************************************************************************/

#ifndef _HANDLING_COMMAND_H
#define _HANDLING_COMMAND_H

	/*===================================================================*/
	//  Function prototypes :

	int handling_Switch_Relay_Control_cmd(char* sSwitch_Relay_Ctrl_cmd_Mesg);

	int handling_ADC_cmd(char* sADC_cmd_Mesg);

	int handling_DAC_cmd(char* sDAC_cmd_Mesg);

	int handling_DIN_cmd(char* sDIN_cmd_Mesg);

	int handling_DOUT_cmd(char* sDOUT_cmd_Mesg);

	int handling_CAN_cmd(char* sCAN_cmd_Mesg);

	int handling_LIN_cmd(char* sLIN_cmd_Mesg);

	int handling_PWM_cmd(char* sPWM_cmd_Mesg);

	/***********************************************************************************/

#endif	/*  _HANDLING_COMMAND_H  */

