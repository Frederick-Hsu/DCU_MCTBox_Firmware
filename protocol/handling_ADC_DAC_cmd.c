/**********************************************************************************************
 * File name	: handling_ADC_DAC_cmd.c
 *
 * Description	: Implement functions, 
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

#include <string.h>
#include <stdio.h>
#include "handling_ADC_DAC_cmd.h"
#include "..\utility.h"
#include "..\ADC_and_DAC\ADC_to_Measure_Voltage_Current.h"

extern int g_iErrorCodeNo;

/******************************************************************/
// Functins implementation :
int handling_Voltage_Measurement(char *sVoltageMeasurementCmdMesg)
{
	int iError = 0;

	unsigned int uiLen = strlen(sVoltageMeasurementCmdMesg),
				 uiPosOfCmdSeparator_Colon	= strcspn(sVoltageMeasurementCmdMesg, ":"),
				 uiPosOfCmdSeparator_Semicolon	= strcspn(sVoltageMeasurementCmdMesg, ";"),
				 uiPosOfCmdSeparator_Space	= strcspn(sVoltageMeasurementCmdMesg, " ");
	char sADCVoltageMeasureResponseMesg[256] = {0};

	if (uiPosOfCmdSeparator_Semicolon != uiLen)
	{
		iError = handling_MultiVoltageMeasureUnits(sVoltageMeasurementCmdMesg, sADCVoltageMeasureResponseMesg);
		g_iErrorCodeNo = iError;
		return g_iErrorCodeNo;
	}
	else
	{
		iError = handling_1VoltageMeasureUnit(sVoltageMeasurementCmdMesg, sADCVoltageMeasureResponseMesg);
		g_iErrorCodeNo = iError;
		return g_iErrorCodeNo;
	}
/*****************************/
	return iError;
}

int handling_Current_Measurement(char *sCurrentMeasurementCmdMesg)
{
	int iError = 0;


/*****************************/
	return iError;
}

int handling_MultiVoltageMeasureUnits(char *sARGIN_MultiVoltageMeasureUnits_Mesg,
				      char *sARGOUT_MultiVoltageMeasureResponse)
{
	int iError = 0;

/*****************************/
	return iError;
}

int handling_1VoltageMeasureUnit(char *sARGIN_1VoltageMeasureUnit_Mesg, 
				 char *sARGOUT_VoltageMeasureResponse)
{
	int iError = 0;

	unsigned int uiLen = strlen(sARGIN_1VoltageMeasureUnit_Mesg),
		     uiPosOfCmdSeparator_Space = strcspn(sARGIN_1VoltageMeasureUnit_Mesg, " "),
		     uiPosOfCmdSeparator_Qmark = strcspn(sARGIN_1VoltageMeasureUnit_Mesg, "?"),
		     uiPosOfCmdSeparator_Colon = strcspn(sARGIN_1VoltageMeasureUnit_Mesg, ":");
	char sADCMeasureCHn[8]  = {0}, sADCMeasureType[16] = {0};
	long lADCMeasureCHn	=  0;
	float fADCVoltage = 0.0000f;

	if (uiPosOfCmdSeparator_Qmark == uiLen)
	{
		g_iErrorCodeNo = -9;
		return g_iErrorCodeNo;
	}
	strncpy(sADCMeasureType,
		(sARGIN_1VoltageMeasureUnit_Mesg+uiPosOfCmdSeparator_Colon+1),
		(uiPosOfCmdSeparator_Qmark-uiPosOfCmdSeparator_Colon-1));
	ToUpperString(sADCMeasureType);
	if (strncmp(sADCMeasureType, "VOLT", 4))
	{
		g_iErrorCodeNo = -11;
		return g_iErrorCodeNo;
	}
	strncpy(sADCMeasureCHn, 
		(sARGIN_1VoltageMeasureUnit_Mesg+uiPosOfCmdSeparator_Space+1), 
		(uiPosOfCmdSeparator_Colon-uiPosOfCmdSeparator_Space-1));
	if (!strcmp(sADCMeasureCHn, "*"))
	{
		iError = handling_8CHVoltageMeasurement(sARGOUT_VoltageMeasureResponse);
		g_iErrorCodeNo = iError;
		return g_iErrorCodeNo;
	}
	Convert_Str_To_Int(sADCMeasureCHn, &lADCMeasureCHn);
	if ((lADCMeasureCHn<1) || (lADCMeasureCHn>8))
	{
		g_iErrorCodeNo = -12;
		return g_iErrorCodeNo;
	}

#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	fADCVoltage = Measure_Single_CHn_Voltage_Value(lADCMeasureCHn-1, 10.00);
#endif
	sprintf(sARGOUT_VoltageMeasureResponse, "ADC %d:VOLT %.06f", lADCMeasureCHn, fADCVoltage);
/*****************************/
	return iError;
}

int handling_8CHVoltageMeasurement(char *ARGOUT_8ChVoltageMeasureResponse)
{
	int iError = 0;
	float f8ChVoltages[8] = {0.0000};
#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	Measure_8CHs_Voltage_Values(10.00, f8ChVoltages);
#endif
	sprintf(ARGOUT_8ChVoltageMeasureResponse, 
		#if defined (TESTING_EXPERIMENT)
			"ADC 1:VOLT %.06f;ADC 2:VOLT %.06f;ADC 3:VOLT %.06f;ADC 4:VOLT %.06f;ADC 5:VOLT %.06f;ADC 6:VOLT %.06f;ADC 7:VOLT %.06f;ADC 8:VOLT %.06f",
		#else
			"ADC 1:VOLT %.06f;\nADC 2:VOLT %.06f;\nADC 3:VOLT %.06f;\nADC 4:VOLT %.06f;\nADC 5:VOLT %.06f;\nADC 6:VOLT %.06f;\nADC 7:VOLT %.06f;\nADC 8:VOLT %.06f",
		#endif
		f8ChVoltages[0],
		f8ChVoltages[1],
		f8ChVoltages[2],
		f8ChVoltages[3],
		f8ChVoltages[4],
		f8ChVoltages[5],
		f8ChVoltages[6],
		f8ChVoltages[7]);

/***********************************/
	return iError;
}
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

