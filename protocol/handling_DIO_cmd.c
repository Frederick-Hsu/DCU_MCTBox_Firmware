/**********************************************************************************
 * Module		: DIO command-handling module
 *
 * File name	: handling_DIO_cmd.c
 * Description	: In this module, its purpose is to dedicatedly parse and handle
 *				: Digital-IN & Digital-OUT catalog of commands.
 *				: this file implements these functions.
 * Creator		: XU ZAN [HSE-OT]
 * Creation date: Wed.	March 6, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 **********************************************************************************/



/***********************************************************************/
// Includes :
#include <string.h>
#include <stdio.h>
#include "handling_DIO_cmd.h"
#include "../utility.h"

#include "../DIO/Digital_IN.h"
#include "../DIO/Digital_OUT.h"


/***********************************************************************/
// Global variables :
extern int g_iErrorCodeNo;


/***********************************************************************/
// Function implementation
int handling_SingleCH_DIN_cmd(char	*sARGIN_DinSingleChCmdMesg,
                              char	*sARGOUT_DinSingleChStateResponseMesg)
{
	int iError = 0;

	unsigned int uiLen = strlen(sARGIN_DinSingleChCmdMesg),
                 uiPosOfCmdSeparator_Colon = strcspn(sARGIN_DinSingleChCmdMesg, ":"),
                 uiPosOfCmdSeparator_Space = strcspn(sARGIN_DinSingleChCmdMesg, " "),
                 uiPosOfCmdSeparator_Qmark = strcspn(sARGIN_DinSingleChCmdMesg, "?");

    char sDinCHn[8] = {0}, sDinState[16] = {0}, sDinBoardID[16] = {0};
    long lDinCHn = 25, lDinBoardID = 0x00;
	char *cColonSubStr = NULL;
	int iDinBoardCHnState = 0;

    DIN_CHm_STATE stCurrentDinCh = {25, 5};

    if ((uiPosOfCmdSeparator_Qmark == uiLen) ||
        (uiPosOfCmdSeparator_Colon == uiLen) ||
        (uiPosOfCmdSeparator_Space == uiLen) )
    {
        g_iErrorCodeNo = -18;
        return g_iErrorCodeNo;
    }

	if (strstr(sARGIN_DinSingleChCmdMesg, ":STA"))
	{
        strncpy(sDinCHn, sARGIN_DinSingleChCmdMesg+uiPosOfCmdSeparator_Space+1, uiPosOfCmdSeparator_Colon-uiPosOfCmdSeparator_Space-1);
        strncpy(sDinState, sARGIN_DinSingleChCmdMesg+uiPosOfCmdSeparator_Colon+1, uiPosOfCmdSeparator_Qmark-uiPosOfCmdSeparator_Colon-1);
        Convert_Str_To_Int(sDinCHn, &lDinCHn);
        ToUpperString(sDinState);

        if ((lDinCHn>24) || (lDinCHn<0))
        {
			g_iErrorCodeNo = -16;
			return g_iErrorCodeNo;
        }
        if (strncmp(sDinState, "STATE", 5))
        {
			g_iErrorCodeNo = -17;
			return g_iErrorCodeNo;
        }

        stCurrentDinCh.eCHm = lDinCHn-1;
        #if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
        	Read_DIN_CHn_State(&stCurrentDinCh);
        #endif  /*  FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO  */

        if (stCurrentDinCh.eCHm_State == HIGH)
        {
			sprintf(sARGOUT_DinSingleChStateResponseMesg, "DIN %ld:State HIGH", lDinCHn);
        }
        else if (stCurrentDinCh.eCHm_State == LOW)
        {
			sprintf(sARGOUT_DinSingleChStateResponseMesg, "DIN %ld:State LOW", lDinCHn);
        }
	}
	else if (strstr(sARGIN_DinSingleChCmdMesg, " STA"))
	{
		cColonSubStr = strstr(sARGIN_DinSingleChCmdMesg, ":");
		
		strncpy(sDinBoardID, sARGIN_DinSingleChCmdMesg+uiPosOfCmdSeparator_Space+1, uiPosOfCmdSeparator_Colon-uiPosOfCmdSeparator_Space-1);
		Convert_Str_To_Int(sDinBoardID, &lDinBoardID);

		strncpy(sDinCHn, cColonSubStr+1, strcspn(cColonSubStr, " ")-1);
		Convert_Str_To_Int(sDinCHn, &lDinCHn);

		#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
			iDinBoardCHnState = Read_DinBoard_CHn_State(lDinBoardID, lDinCHn-1);	// Channel # starting from 1 to 24
		#endif
		sprintf(sARGOUT_DinSingleChStateResponseMesg, 
				"DIN 0x%02X:%ld %s", 
				lDinBoardID, 
				lDinCHn, 
				iDinBoardCHnState==1?"HIGH":"LOW");
	}
/*******************************/
	return iError;
}

int handling_MultiCH_DIN_cmd(char       *ARGIN_DinMultiChCmdMesg,
                             char       *ARGOUT_DinMultiChStateResponseMesg)
{
	int iError = 0;

	unsigned int uiLen = strlen(ARGIN_DinMultiChCmdMesg),
                 uiPosOfCmdSeparator_Semicolon = strcspn(ARGIN_DinMultiChCmdMesg, ";"),
                 uiPosOfCmdSeparator_Qmark = strcspn(ARGIN_DinMultiChCmdMesg, "?");

    char s1DinCmdUnit[24]                   = {0},
         s1DinCmdUnitResponse[32]           = {0},
         sRestSubstringOfDinCmdMesg[256]    = {0},
         sTempRestSubstring[256]            = {0};

    if (uiPosOfCmdSeparator_Semicolon == uiLen)
    {
        g_iErrorCodeNo = -18;
        return g_iErrorCodeNo;
    }


    strncpy(s1DinCmdUnit, ARGIN_DinMultiChCmdMesg, uiPosOfCmdSeparator_Semicolon);
    iError = handling_SingleCH_DIN_cmd(s1DinCmdUnit, s1DinCmdUnitResponse);
    if (iError)
        return iError;
    strcat(ARGOUT_DinMultiChStateResponseMesg, s1DinCmdUnitResponse);
    strcat(ARGOUT_DinMultiChStateResponseMesg, ";");

    strncpy(sRestSubstringOfDinCmdMesg, ARGIN_DinMultiChCmdMesg+uiPosOfCmdSeparator_Semicolon+1, uiLen-uiPosOfCmdSeparator_Semicolon);
    do
    {
        uiPosOfCmdSeparator_Semicolon = strcspn(sRestSubstringOfDinCmdMesg, ";");
        if (uiPosOfCmdSeparator_Semicolon == strlen(sRestSubstringOfDinCmdMesg))
        {
			break;
        }

        memset(s1DinCmdUnit, 0, 24*sizeof(char));
        memset(s1DinCmdUnitResponse, 0, 32*sizeof(char));
        strncpy(s1DinCmdUnit, sRestSubstringOfDinCmdMesg, uiPosOfCmdSeparator_Semicolon);
        iError = handling_SingleCH_DIN_cmd(s1DinCmdUnit, s1DinCmdUnitResponse);
        if (iError)
			return iError;
        strcat(ARGOUT_DinMultiChStateResponseMesg, s1DinCmdUnitResponse);
        strcat(ARGOUT_DinMultiChStateResponseMesg, ";");

        memset(sTempRestSubstring, 0, 256*sizeof(char));
        strncpy(sTempRestSubstring,
                sRestSubstringOfDinCmdMesg+uiPosOfCmdSeparator_Semicolon+1,
                strlen(sRestSubstringOfDinCmdMesg)-uiPosOfCmdSeparator_Semicolon);
        memset(sRestSubstringOfDinCmdMesg, 0, 256*sizeof(char));
        sprintf(sRestSubstringOfDinCmdMesg, "%s", sTempRestSubstring);
    }
    while (NULL != strchr(sRestSubstringOfDinCmdMesg, ';'));

    memset(s1DinCmdUnit, 0, 24*sizeof(char));
    memset(s1DinCmdUnitResponse, 0, 32*sizeof(char));
    strncpy(s1DinCmdUnit, sRestSubstringOfDinCmdMesg, strlen(sRestSubstringOfDinCmdMesg)-1);
    iError = handling_SingleCH_DIN_cmd(s1DinCmdUnit, s1DinCmdUnitResponse);
    if (iError)
		return iError;
    strcat(ARGOUT_DinMultiChStateResponseMesg, s1DinCmdUnitResponse);

/*******************************/
	return iError;
}

int handling_1GroupOfChs_DIN_cmd(char *sARGIN_1GroupDinChsCmdMesg,
				 				 char *sARGOUT_1GroupDinChsStateResponseMesg)
{
	char sDin24ChsStates[32] = {0}, sDinBoardID[16] = {0};
	long lDinBoardID = 0x00;
	unsigned int uiPosOfSpace = strcspn(sARGIN_1GroupDinChsCmdMesg, " "),
		     	 uiPosOfColon = strcspn(sARGIN_1GroupDinChsCmdMesg, ":");
	
	if (strstr(sARGIN_1GroupDinChsCmdMesg, "*:"))
	{
		#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
			Read_DIN_1GroupOfCHs_State(sDin24ChsStates);
		#endif
		sprintf(sARGOUT_1GroupDinChsStateResponseMesg, "DIN *:State %s", sDin24ChsStates);
	}
	else if (strstr(sARGIN_1GroupDinChsCmdMesg, ":*"))
	{
		strncpy(sDinBoardID, sARGIN_1GroupDinChsCmdMesg+uiPosOfSpace+1, uiPosOfColon-uiPosOfSpace-1);
		Convert_Str_To_Int(sDinBoardID, &lDinBoardID);
		
		#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
			Read_DinBoard_24Chs_State((BYTE)lDinBoardID, sDin24ChsStates);
		#endif
		sprintf(sARGOUT_1GroupDinChsStateResponseMesg, "DIN 0x%02X:* %s", lDinBoardID, sDin24ChsStates);
	}
	return 0;
}
					 
int handling_Single_DOUT_CHn_cmd(char *sDoutSingleChCmdUnitMesg)
{
    int iError = 0;

    unsigned int uiLen = strlen(sDoutSingleChCmdUnitMesg),
                 uiPosOfCmdSeparator_Colon = strcspn(sDoutSingleChCmdUnitMesg, ":"),
                 uiPosOfCmdSeparator_Space = strcspn(sDoutSingleChCmdUnitMesg, " ");

    char sDoutBoardID[16] = {0}, sChNr[8] = {0}, sChnState[16] = {0}, sAttribute[32] = {0};
    long lDoutBoardID = 0, lChNr = 0;

    ST_Access_Ctrl_SwitchRelayMatrix stCurrentDoutPort = {0x00, 0, LOW};

    if ((uiPosOfCmdSeparator_Colon == uiLen) ||
        (uiPosOfCmdSeparator_Space == uiLen) ||
        (uiPosOfCmdSeparator_Colon <= uiPosOfCmdSeparator_Space))
    {
        g_iErrorCodeNo = -19;
        return g_iErrorCodeNo;
    }

    strncpy(sDoutBoardID,
            sDoutSingleChCmdUnitMesg+uiPosOfCmdSeparator_Space+1,
            uiPosOfCmdSeparator_Colon-uiPosOfCmdSeparator_Space-1);
    iError = Convert_Str_To_Int(sDoutBoardID, &lDoutBoardID);
    if (iError)
            return iError;
    if ((lDoutBoardID>0xFF) || (lDoutBoardID<0x00))
    {
        g_iErrorCodeNo = -20;
        return g_iErrorCodeNo;
    }
    stCurrentDoutPort.byteBoardID = lDoutBoardID;

    strncpy(sAttribute, sDoutSingleChCmdUnitMesg+uiPosOfCmdSeparator_Colon+1, uiLen-uiPosOfCmdSeparator_Colon-1);
    uiPosOfCmdSeparator_Space = strcspn(sAttribute, " ");
    if (uiPosOfCmdSeparator_Space == strlen(sAttribute))
    {
        iError = handling_Dout_1AttributeGroup(lDoutBoardID, sAttribute);
        g_iErrorCodeNo = iError;
	return iError;
    }
    strncpy(sChNr, sAttribute, uiPosOfCmdSeparator_Space);
    iError = Convert_Str_To_Int(sChNr, &lChNr);
    if (iError)
            return iError;
    if ((lChNr>24) || (lChNr<0))
    {
        g_iErrorCodeNo = -21;
        return g_iErrorCodeNo;
    }
    stCurrentDoutPort.dwSwitch_Relay_CHn = lChNr;

    strncpy(sChnState, sAttribute+uiPosOfCmdSeparator_Space+1, strlen(sAttribute)-uiPosOfCmdSeparator_Space-1);
    ToUpperString(sChnState);
    if (!strncmp(sChnState, "HIGH", 4) ||
        !strncmp(sChnState, "HI", 2)   ||
        !strncmp(sChnState, "1", 1)    ||
        !strncmp(sChnState, "H", 1)    ||
        !strncmp(sChnState, "ON", 2)   ||
        !strncmp(sChnState, "CLOSE", 5))
    {
		stCurrentDoutPort.eOpen_Close_State = HIGH;
    }
    else if (!strncmp(sChnState, "LOW", 3) ||
             !strncmp(sChnState, "LO", 2)  ||
             !strncmp(sChnState, "0", 1)   ||
             !strncmp(sChnState, "L", 1)   ||
             !strncmp(sChnState, "OFF", 3) ||
             !strncmp(sChnState, "OPEN", 4))
    {
		stCurrentDoutPort.eOpen_Close_State = LOW;
    }
    else
    {
        g_iErrorCodeNo = -22;
        return g_iErrorCodeNo;
    }
    #if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
    	DOUT_Single_CHn(&stCurrentDoutPort);
    #endif  /*  FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO  */
/*********************************/
    return iError;
}

int handling_Multi_DOUT_CHn_cmd(char *sDoutMultiChCmdMesg)
{
    int iError = 0;
    unsigned int uiLen = strlen(sDoutMultiChCmdMesg),
                 uiPosOfCmdSeparator_Semicolon = strcspn(sDoutMultiChCmdMesg, ";");

    char s1DoutCmdUnit[48]                  = {0},
         sRestSubstringOfDoutCmdMesg[256]   = {0},
         sTempSubstring[256]                = {0};

    if (uiPosOfCmdSeparator_Semicolon == uiLen)
    {
        g_iErrorCodeNo = -19;
        return g_iErrorCodeNo;
    }
    strncpy(s1DoutCmdUnit, sDoutMultiChCmdMesg, uiPosOfCmdSeparator_Semicolon);
    iError = handling_Single_DOUT_CHn_cmd(s1DoutCmdUnit);
    if (iError)
		return iError;

    strncpy(sRestSubstringOfDoutCmdMesg,
            sDoutMultiChCmdMesg+uiPosOfCmdSeparator_Semicolon+1,
            uiLen-uiPosOfCmdSeparator_Semicolon);
    do
    {
        uiPosOfCmdSeparator_Semicolon = strcspn(sRestSubstringOfDoutCmdMesg, ";");
        if (uiPosOfCmdSeparator_Semicolon == strlen(sRestSubstringOfDoutCmdMesg))
			break;

        memset(s1DoutCmdUnit, 0, 48*sizeof(char));
        strncpy(s1DoutCmdUnit, sRestSubstringOfDoutCmdMesg, uiPosOfCmdSeparator_Semicolon);
        iError = handling_Single_DOUT_CHn_cmd(s1DoutCmdUnit);
        if (iError)
			return iError;

        memset(sTempSubstring, 0, 256*sizeof(char));
        strncpy(sTempSubstring,
                sRestSubstringOfDoutCmdMesg+uiPosOfCmdSeparator_Semicolon+1,
                strlen(sRestSubstringOfDoutCmdMesg)-uiPosOfCmdSeparator_Semicolon);
        memset(sRestSubstringOfDoutCmdMesg, 0, 256*sizeof(char));
        sprintf(sRestSubstringOfDoutCmdMesg, "%s", sTempSubstring);
    }
    while (NULL != strchr(sRestSubstringOfDoutCmdMesg, ';'));

    memset(s1DoutCmdUnit, 0, 48*sizeof(char));
    strncpy(s1DoutCmdUnit, sRestSubstringOfDoutCmdMesg, strlen(sRestSubstringOfDoutCmdMesg)-1);
    iError = handling_Single_DOUT_CHn_cmd(s1DoutCmdUnit);
/*********************************/
    return iError;
}

int handling_Dout_1AttributeGroup(BYTE bDoutBoardID, char *sDoutChnGroup24Bits)
{
    int iError = 0;

    int iCnt = -1;
    unsigned int uiLen = strlen(sDoutChnGroup24Bits),
				 uiPosOfCmdSeparator_Sigh = strcspn(sDoutChnGroup24Bits, "!");
	
	char sTrueAttributeGroup24Bits[48] = {0};
    
	ST_Access_Ctrl_SwitchRelayMatrix DoutPort = {0x00, 0, LOW};

	if (uiPosOfCmdSeparator_Sigh != uiLen)
	{
		strncpy(sTrueAttributeGroup24Bits, sDoutChnGroup24Bits, 24);
	}

#if 0
    if (24 != uiLen)
    {
        g_iErrorCodeNo = -19;
        return g_iErrorCodeNo;
    }
#endif
    DoutPort.byteBoardID = bDoutBoardID;

    for (iCnt=strlen(sTrueAttributeGroup24Bits)-1; iCnt>=0; iCnt--)
    {
        if (('1' == *(sTrueAttributeGroup24Bits+iCnt)) || ('0' == *(sTrueAttributeGroup24Bits+iCnt)))
        {
            DoutPort.dwSwitch_Relay_CHn = 24-iCnt;
            DoutPort.eOpen_Close_State = ('1' == *(sTrueAttributeGroup24Bits+iCnt))? HIGH : LOW;
            #if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
            DOUT_Single_CHn(&DoutPort);
            #endif  /*  FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO  */
        }
    }
/*********************************/
    return iError;
}
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

