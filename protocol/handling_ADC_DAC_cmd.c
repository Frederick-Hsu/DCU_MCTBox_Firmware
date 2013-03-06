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

#define	ADC_OUTPUT_FORMAT_INTEGER	1
#define	ADC_OUTPUT_FORMAT_FLOAT		2
#define	ADC_OUTPUT_FORMAT			ADC_OUTPUT_FORMAT_FLOAT

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "handling_ADC_DAC_cmd.h"
#include "..\utility.h"
#include "..\ADC_and_DAC\ADC_to_Measure_Voltage_Current.h"

extern int g_iErrorCodeNo;

/******************************************************************/
// Functins implementation :
int handling_Voltage_Measurement(char *sVoltageMeasurementCmdMesg, char *sARGOUT_VoltageMeasureResultResponse)
{
	int iError = 0;

	unsigned int uiLen = strlen(sVoltageMeasurementCmdMesg),
				 uiPosOfCmdSeparator_Colon	= strcspn(sVoltageMeasurementCmdMesg, ":"),
				 uiPosOfCmdSeparator_Semicolon	= strcspn(sVoltageMeasurementCmdMesg, ";"),
				 uiPosOfCmdSeparator_Space	= strcspn(sVoltageMeasurementCmdMesg, " ");
	char sADCVoltageMeasureResponseMesg[8*48] = {0};

	if (uiPosOfCmdSeparator_Semicolon != uiLen)
	{
		iError = handling_MultiVoltageMeasureUnits(sVoltageMeasurementCmdMesg, sADCVoltageMeasureResponseMesg);
		sprintf(sARGOUT_VoltageMeasureResultResponse, "$%s!", sADCVoltageMeasureResponseMesg);
		g_iErrorCodeNo = iError;
		return g_iErrorCodeNo;
	}
	else
	{
		iError = handling_1VoltageMeasureUnit(sVoltageMeasurementCmdMesg, sADCVoltageMeasureResponseMesg);
		sprintf(sARGOUT_VoltageMeasureResultResponse, "$%s!", sADCVoltageMeasureResponseMesg);
		g_iErrorCodeNo = iError;
		return g_iErrorCodeNo;
	}
/*****************************/
	return iError;
}

int handling_Current_Measurement(char *sCurrentMeasurementCmdMesg, char *sARGOUT_CurrentMeasureResultResponse)
{
	int iError = 0;

	unsigned int uiLen = strlen(sCurrentMeasurementCmdMesg),
				 uiPosOfCmdSeparator_Semicolon = strcspn(sCurrentMeasurementCmdMesg, ";");

	char sADCCurrentMeasureResponse[256] = {0};
	if (uiPosOfCmdSeparator_Semicolon != uiLen)
	{
		iError = handling_MultiCurrentMeasureUnits(sCurrentMeasurementCmdMesg, sADCCurrentMeasureResponse);
		sprintf(sARGOUT_CurrentMeasureResultResponse, "$%s!", sADCCurrentMeasureResponse);
		g_iErrorCodeNo = iError;
	}
	else
	{
		iError = handling_1CurrentMeasureUnit(sCurrentMeasurementCmdMesg, sADCCurrentMeasureResponse);
		sprintf(sARGOUT_CurrentMeasureResultResponse, "$%s!", sADCCurrentMeasureResponse);
		g_iErrorCodeNo = iError;
	}

/*****************************/
	return iError;
}

int handling_MultiVoltageMeasureUnits(char *sARGIN_MultiVoltageMeasureUnits_Mesg,
				      char *sARGOUT_MultiVoltageMeasureResponse)
{
	int iError = 0;

	unsigned int uiLen = strlen(sARGIN_MultiVoltageMeasureUnits_Mesg),
		     uiPosOfCmdSeparator_Semicolon = strcspn(sARGIN_MultiVoltageMeasureUnits_Mesg, ";"),
		     uiPosOfCmdSeparator_Qmark = 0;
	char sRestSubstringOfCmdMesg[256]		= {0},
	     sTempResetSubstring[256]			= {0},
	     s1VoltageMeasureUnitString[32]		= {0},
	     sVoltageMeasureResultResponse[8*48]		= {0},
	     s1VoltageMeasureUnitResultResponse[8][48]	= {0};
	int  iCnt = 0;

	strncpy(s1VoltageMeasureUnitString, sARGIN_MultiVoltageMeasureUnits_Mesg, uiPosOfCmdSeparator_Semicolon);
	if (strcspn(s1VoltageMeasureUnitString, "?") == strlen(s1VoltageMeasureUnitString))	// It shows no '?' in the 1 voltage measurement unit string
	{
		g_iErrorCodeNo = -9;
		return g_iErrorCodeNo;
	}
	handling_1VoltageMeasureUnit(s1VoltageMeasureUnitString, s1VoltageMeasureUnitResultResponse[iCnt]);
	strcat(sVoltageMeasureResultResponse, s1VoltageMeasureUnitResultResponse[iCnt]);
	#if defined (TESTING_EXPERIMENT)
		strcat(sVoltageMeasureResultResponse, ";");
	#else
		strcat(sVoltageMeasureResultResponse, ";\n");
	#endif
	
	strncpy(sRestSubstringOfCmdMesg, sARGIN_MultiVoltageMeasureUnits_Mesg+uiPosOfCmdSeparator_Semicolon+1, uiLen-uiPosOfCmdSeparator_Semicolon);
	do
	{
		uiPosOfCmdSeparator_Semicolon = strcspn(sRestSubstringOfCmdMesg, ";");
		if (uiPosOfCmdSeparator_Semicolon == strlen(sRestSubstringOfCmdMesg))
		{
			break;
		}
		memset(s1VoltageMeasureUnitString, 0, 32*sizeof(char));
		strncpy(s1VoltageMeasureUnitString, sRestSubstringOfCmdMesg, uiPosOfCmdSeparator_Semicolon);
		if (strcspn(s1VoltageMeasureUnitString, "?") == strlen(s1VoltageMeasureUnitString))
		{
			g_iErrorCodeNo = -9;
			return g_iErrorCodeNo;	// If detecting the command format error, directly quit.
		}
		iCnt++;
		handling_1VoltageMeasureUnit(s1VoltageMeasureUnitString, s1VoltageMeasureUnitResultResponse[iCnt]);
		strcat(sVoltageMeasureResultResponse, s1VoltageMeasureUnitResultResponse[iCnt]);
		#if defined (TESTING_EXPERIMENT)
			strcat(sVoltageMeasureResultResponse, ";");
		#else
			strcat(sVoltageMeasureResultResponse, ";\n");
		#endif

		memset(sTempResetSubstring, 0, 256*sizeof(char));
		strncpy(sTempResetSubstring, 
				sRestSubstringOfCmdMesg+uiPosOfCmdSeparator_Semicolon+1, 
				strlen(sRestSubstringOfCmdMesg)-uiPosOfCmdSeparator_Semicolon);
		memset(sRestSubstringOfCmdMesg, 0, 256*sizeof(char));
		sprintf(sRestSubstringOfCmdMesg, "%s", sTempResetSubstring);
	}
	while (strchr(sRestSubstringOfCmdMesg, ';') != NULL);

	uiPosOfCmdSeparator_Qmark = strcspn(sRestSubstringOfCmdMesg, "?");
	if (uiPosOfCmdSeparator_Qmark == strlen(sRestSubstringOfCmdMesg))
	{
		g_iErrorCodeNo = -9;
		return g_iErrorCodeNo;
	}
	memset(s1VoltageMeasureUnitString, 0, 32*sizeof(char));
	// memcpy(void* _Dst, const* _Src, size_t _Size);
	strncpy(s1VoltageMeasureUnitString, sRestSubstringOfCmdMesg, uiPosOfCmdSeparator_Qmark+1);
	handling_1VoltageMeasureUnit(s1VoltageMeasureUnitString, s1VoltageMeasureUnitResultResponse[iCnt+1]);
	strcat(sVoltageMeasureResultResponse, s1VoltageMeasureUnitResultResponse[iCnt+1]);

	sprintf(sARGOUT_MultiVoltageMeasureResponse, "%s", sVoltageMeasureResultResponse);
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
#if (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_FLOAT)
	float fADCVoltage = 0.0000f;
#elif (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_INTEGER)
	int iADCValue = 0x0000;
#endif

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
	#if (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_FLOAT)
		fADCVoltage = Measure_Single_CHn_Voltage_Value(lADCMeasureCHn-1, 10.00);
	#elif (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_INTEGER)
		ADC_Acquiring_AnalogInputValue(lADCMeasureCHn-1, &iADCValue);
	#endif
#endif
	#if (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_FLOAT)
	sprintf(sARGOUT_VoltageMeasureResponse, "ADC %d:VOLT %.09f", lADCMeasureCHn, fADCVoltage);
	#elif (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_INTEGER)
		sprintf(sARGOUT_VoltageMeasureResponse, "ADC %d:VOLT %d", lADCMeasureCHn, iADCValue);
	#endif
/*****************************/
	return iError;
}

int handling_8CHVoltageMeasurement(char *ARGOUT_8ChVoltageMeasureResponse)
{
	int iError = 0;
	#if (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_FLOAT)
		float f8ChVoltages[8] = {0.0000};
	#elif (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_INTEGER)
		int i8ChADCValue[8] = {0x0000};
	#endif
#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	#if (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_FLOAT)
		Measure_8CHs_Voltage_Values(10.00, f8ChVoltages);
	#elif (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_INTEGER)
		ADC_Get_8CHs_AINValue(i8ChADCValue);
	#endif
#endif
	#if (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_FLOAT)
		sprintf(ARGOUT_8ChVoltageMeasureResponse, 
			#if defined (TESTING_EXPERIMENT)
				"ADC 1:VOLT %.09f;ADC 2:VOLT %.09f;ADC 3:VOLT %.09f;ADC 4:VOLT %.09f;ADC 5:VOLT %.09f;ADC 6:VOLT %.09f;ADC 7:VOLT %.09f;ADC 8:VOLT %.09f",
			#else
				"ADC 1:VOLT %.09f;\nADC 2:VOLT %.09f;\nADC 3:VOLT %.09f;\nADC 4:VOLT %.09f;\nADC 5:VOLT %.09f;\nADC 6:VOLT %.09f;\nADC 7:VOLT %.09f;\nADC 8:VOLT %.09f",
			#endif
			f8ChVoltages[0],
			f8ChVoltages[1],
			f8ChVoltages[2],
			f8ChVoltages[3],
			f8ChVoltages[4],
			f8ChVoltages[5],
			f8ChVoltages[6],
			f8ChVoltages[7]);
	#elif (ADC_OUTPUT_FORMAT == ADC_OUTPUT_FORMAT_INTEGER)
		sprintf(ARGOUT_8ChVoltageMeasureResponse, 
			#if defined (TESTING_EXPERIMENT)
				"ADC 1:VOLT %d;ADC 2:VOLT %d;ADC 3:VOLT %d;ADC 4:VOLT %d;ADC 5:VOLT %d;ADC 6:VOLT %d;ADC 7:VOLT %d;ADC 8:VOLT %d",
			#else
				"ADC 1:VOLT %d;\nADC 2:VOLT %d;\nADC 3:VOLT %d;\nADC 4:VOLT %d;\nADC 5:VOLT %d;\nADC 6:VOLT %d;\nADC 7:VOLT %d;\nADC 8:VOLT %d",
			#endif
			i8ChADCValue[0],
			i8ChADCValue[1],
			i8ChADCValue[2],
			i8ChADCValue[3],
			i8ChADCValue[4],
			i8ChADCValue[5],
			i8ChADCValue[6],
			i8ChADCValue[7]);
	#endif

/***********************************/
	return iError;
}

int handling_1CurrentMeasureUnit(char *sARGIN_1CurrentMeasureUnit_Mesg,
				 char *sARGOUT_CurrentMeasureResponse)
{
	int iError = 0;

	unsigned int uiLen = strlen(sARGIN_1CurrentMeasureUnit_Mesg),
		     uiPosOfCmdSeparator_Colon = strcspn(sARGIN_1CurrentMeasureUnit_Mesg, ":"),
		     uiPosOfCmdSeparator_Space = strcspn(sARGIN_1CurrentMeasureUnit_Mesg, " "),
		     uiPosOfCmdSeparator_Qmark = strcspn(sARGIN_1CurrentMeasureUnit_Mesg, "?");
	char sADC_CHn[8] = {0},  sSamplingResistorInOhm[16] = {0}, sCurrentAttribute[32] = {0};
	long iADC_CHn = 0;
	double dSamplingResistorInOhm = 0.0000f, dCurrentValue = 0.0000f;

	if (uiPosOfCmdSeparator_Qmark == uiLen)
	{
		g_iErrorCodeNo = -9;
		return g_iErrorCodeNo;
	}
	strncpy(sADC_CHn, 
		sARGIN_1CurrentMeasureUnit_Mesg+uiPosOfCmdSeparator_Space+1, 
		uiPosOfCmdSeparator_Colon-uiPosOfCmdSeparator_Space-1);
	Convert_Str_To_Int(sADC_CHn, &iADC_CHn);
	if ((iADC_CHn<1) || (iADC_CHn>8))
	{
		g_iErrorCodeNo = -12;
		return g_iErrorCodeNo;
	}

	strncpy(sCurrentAttribute, 
		sARGIN_1CurrentMeasureUnit_Mesg+uiPosOfCmdSeparator_Colon+1, 
		uiPosOfCmdSeparator_Qmark-uiPosOfCmdSeparator_Colon);
	ToUpperString(sCurrentAttribute);
	if (strncmp(sCurrentAttribute, "CURR", 4))
	{
		g_iErrorCodeNo = -11;
		return g_iErrorCodeNo;
	}
	uiPosOfCmdSeparator_Space = strcspn(sCurrentAttribute, " ");
	uiPosOfCmdSeparator_Qmark = strcspn(sCurrentAttribute, "?");
	strncpy(sSamplingResistorInOhm, 
		sCurrentAttribute+uiPosOfCmdSeparator_Space+1, 
		uiPosOfCmdSeparator_Qmark-uiPosOfCmdSeparator_Space-1);
#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	dSamplingResistorInOhm = atoff(sSamplingResistorInOhm);
	dCurrentValue = Measure_Single_CHn_Current(iADC_CHn-1, 10.00, dSamplingResistorInOhm);
#endif
	sprintf(sARGOUT_CurrentMeasureResponse, "ADC %d:CURR %.09f", iADC_CHn, dCurrentValue);
/**************************/
	return iError;
}


int handling_MultiCurrentMeasureUnits(char *sARGIN_MultiCurrentMeasureUnits_Mesg,
				      char *sARGOUT_MultiCurrentMeasureResponse)
{
	int iError = 0;

	unsigned int uiLen = strlen(sARGIN_MultiCurrentMeasureUnits_Mesg),
				 uiPosOfCmdSeparator_Semicolon = strcspn(sARGIN_MultiCurrentMeasureUnits_Mesg, ";"),
				 uiPosOfCmdSeparator_Qmark = 0;
	char sRestSubstringOfCmdMesg[256]				= {0},
		 sTempRestSubstring[256]					= {0},
		 s1CurrentMeasureUnitString[48]				= {0},
		 sCurrentMeasureResultResponse[256]			= {0},
		 s1CurrentMeasureUnitResultResponse[8][48]	= {0};
	int iCnt = 0;

	strncpy(s1CurrentMeasureUnitString, sARGIN_MultiCurrentMeasureUnits_Mesg, uiPosOfCmdSeparator_Semicolon);
	iError = handling_1CurrentMeasureUnit(s1CurrentMeasureUnitString, s1CurrentMeasureUnitResultResponse[iCnt]);
	if (iError)
		return iError;
	strcat(sCurrentMeasureResultResponse, s1CurrentMeasureUnitResultResponse[iCnt]);
	#if defined (TESTING_EXPERIMENT)
		strcat(sCurrentMeasureResultResponse, ";");
	#else
		strcat(sCurrentMeasureResultResponse, ";\n");
	#endif
	strncpy(sRestSubstringOfCmdMesg, sARGIN_MultiCurrentMeasureUnits_Mesg+uiPosOfCmdSeparator_Semicolon+1, uiLen-uiPosOfCmdSeparator_Semicolon);
	do
	{
		uiPosOfCmdSeparator_Semicolon = strcspn(sRestSubstringOfCmdMesg, ";");
		if (uiPosOfCmdSeparator_Semicolon == strlen(sRestSubstringOfCmdMesg))
			break;
		memset(s1CurrentMeasureUnitString, 0, 48*sizeof(char));
		strncpy(s1CurrentMeasureUnitString, sRestSubstringOfCmdMesg, uiPosOfCmdSeparator_Semicolon);
		iCnt++;
		iError = handling_1CurrentMeasureUnit(s1CurrentMeasureUnitString, s1CurrentMeasureUnitResultResponse[iCnt]);
		if (iError)
			return iError;
		strcat(sCurrentMeasureResultResponse, s1CurrentMeasureUnitResultResponse[iCnt]);
		#if defined (TESTING_EXPERIMENT)
			strcat(sCurrentMeasureResultResponse, ";");
		#else
			strcat(sCurrentMeasureResultResponse, ";\n");
		#endif

		memset(sTempRestSubstring, 0, 256*sizeof(char));
		strncpy(sTempRestSubstring, 
				sRestSubstringOfCmdMesg+uiPosOfCmdSeparator_Semicolon+1, 
				strlen(sRestSubstringOfCmdMesg)-uiPosOfCmdSeparator_Semicolon);
		memset(sRestSubstringOfCmdMesg, 0, 256*sizeof(char));
		sprintf(sRestSubstringOfCmdMesg, "%s", sTempRestSubstring);
	}
	while (strchr(sRestSubstringOfCmdMesg, ';') != NULL);

	memset(s1CurrentMeasureUnitString, 0, 48*sizeof(char));
	uiPosOfCmdSeparator_Qmark = strcspn(sRestSubstringOfCmdMesg, "?");
	strncpy(s1CurrentMeasureUnitString, sRestSubstringOfCmdMesg, uiPosOfCmdSeparator_Qmark+1);
	iError = handling_1CurrentMeasureUnit(s1CurrentMeasureUnitString, s1CurrentMeasureUnitResultResponse[iCnt+1]);
	strcat(sCurrentMeasureResultResponse, s1CurrentMeasureUnitResultResponse[iCnt+1]);

	sprintf(sARGOUT_MultiCurrentMeasureResponse, "%s", sCurrentMeasureResultResponse);
/***************************/
	return iError;
}
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

