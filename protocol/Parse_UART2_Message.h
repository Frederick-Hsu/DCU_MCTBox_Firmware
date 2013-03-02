/*****************************************************************************************
 * Program name	: Parse_UART2_Message.h
 *
 * Description	: Declaration and Prototype file.
 *		  The 2 files (header file and source file) is used to parse the message, 
 *		  which UART2 (i.e. the serial port COM2 communicating with PC) received, 
 *		  then catalog into various kinds of command/instruction types.
 *
 * Author	: XU ZAN
 * Date		: Mon.	Aug. 13, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 *****************************************************************************************/
 

#ifndef _PARSE_UART2_MESSAGE_H
#define _PARSE_UART2_MESSAGE_H

	/*===================================================================*/
	//  Includes :
	
	
	/*===================================================================*/
	//  Macros :
	
	
	
	
	/*===================================================================*/
	//  Global variables :
	
	

	/*===================================================================*/
	//  Function prototypes :
	int Parse_UART2_Received_Message(char *sMesg);

	
#endif	/*    _PARSE_UART2_MESSAGE_H    */

