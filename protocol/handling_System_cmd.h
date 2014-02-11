/*************************************************************************************
 * File name	: handling_System_cmd.h
 * Description	: Dedicatedly handle the system catalog of commands in this file-pair.
 * Creator		: XU ZAN [HSE-OT]
 * Creation date: Mon.	March 11, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 *************************************************************************************/

#ifndef _HANDLING_SYSTEM_COMMAND_H
#define _HANDLING_SYSTEM_COMMAND_H

	int handling_System_Error_cmd(char sSystemErrorCmd[]);
	int handling_System_IDN_cmd(char sSystemIdnCmd[]);
	int handling_System_Version_cmd(char sSystemVerCmd[]);

#endif	/*  _HANDLING_SYSTEM_COMMAND_H  */

