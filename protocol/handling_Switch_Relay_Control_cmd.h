/**********************************************************************************************
 * File name	: handling_Switch_Relay_Control_cmd.h
 * Description	: Declare functions prototype,
 *		  dedicatedly for handling_Switch_Relay_Control_cmd module, this file is only
 *		  used to serve for Switch_Relay_Control command parsing & handling task.
 * Creator	: XU ZAN [HSE-OT]
 * Creation date: Fri.	March 1, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 **********************************************************************************************/

#ifndef _HANDLING_SWITCH_RELAY_CONTROL_CMD_H
#define _HANDLING_SWITCH_RELAY_CONTROL_CMD_H

	/*===================================================================*/
	//  Includes :
	#include "../Switch_Relay_Control/Switch_Relay_Control.h"


	/*===================================================================*/
	//  Function prototypes :
	int handling_Multi_Switches(char* sMulti_Switch_Ctrl_Cmd__Mesg);
	int handling_SwitchAction_1Command_unit(char* s1Command_unit);
	int handling_Batch_Switches(char* sBatch_Switch_Ctrl_Cmd_Mesg);
	int handling_SwitchAction_1Attribute_unit(char* s1Attribute_unit, PSTSwitch_CHn_State pCHn_State);
	int handling_SwitchAction_1AttributeGroup(BYTE byteBoardID, char *sSwitchGroup24Bits);

#endif	/*  _HANDLING_SWITCH_RELAY_CONTROL_CMD_H  */

