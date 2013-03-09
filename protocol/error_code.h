/**************************************************************************************************
 * Program name	: error_code.h
 * Description	: List all error-code numbers in this file, these error-code numbers could be
 *		  referred and called in the entire project.
 * Author	: XU ZAN
 * Date		: Thur.		Sept. 13, 2012
 * Copyright(C)		2010 --- 2012		Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 **************************************************************************************************/


#ifndef _ERROR_CODE_H
#define _ERROR_CODE_H

	char sErrorCode[][128] =
	{
		"Error code # 000: No error!\n",
		"Error code # 001: Command Syntax error\n",
		"Error code # 002: Not exists this action catalog\n",
		"Error code # 003: Switch Board ID error\n",
		"Error code # 004: Switch action command error\n",
		"Error code # 005: Switch channel error\n",
		"Erroe code # 006: Switch state error\n",
		"Error code # 007: Action keyword 'SWITCH' error\n",
		"Error code # 008: Switch Group 24 bits length error\n",
		"Error code # 009: ADC command format error, no Qmark(?).\n",
		"Error code # 010: ADC command syntax error, no colon(:).\n",
		"Error code # 011: ADC measure type error.\n",
		"Error code # 012: ADC channel error.\n",
		"Error code # 013: Digital-IN command format error, no Qmark(?).\n",
		"Error code # 014: DAC output type error. Only support voltage outputing.\n",
		"Error code # 015: DAC output voltage is beyond the range. Not support.\n",
		"Error code # 016: Digital-IN channel number error.\n",
		"Error code # 017: Digital-IN unknown command.\n",
		"Error code # 018: Digital-IN command format error.\n",
		"Error code # 019: Digital-OUT command format error.\n",
		"Error code # 020: Digital-OUT board ID error.\n",
		""
	};


#endif	/*    _ERROR_CODE_H    */

