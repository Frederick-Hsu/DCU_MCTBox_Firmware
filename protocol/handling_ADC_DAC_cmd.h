/**********************************************************************************************
 * File name	: handling_ADC_DAC_cmd.h
 *
 * Description	: Declare functions prototype, 
 *		  dedicatedly for handling_ADC_DAC_cmd module, this file is only
 *		  used to serve for 'ADC measuring voltage and DAC outputing voltage' 
 *		  command parsing & handling task.
 *
 * Creator	: XU ZAN [HSE-OT]
 * Creation date: Fri.	March 1, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 **********************************************************************************************/

#ifndef _HANDLING_ADC_DAC_CMD_H
#define _HANDLING_ADC_DAC_CMD_H

	int handling_Voltage_Measurement(char *sVoltageMeasurementCmdMesg);
	int handling_Current_Measurement(char *sCurrentMeasurementCmdMesg);

#endif	/*  _HANDLING_ADC_DAC_CMD_H  */

