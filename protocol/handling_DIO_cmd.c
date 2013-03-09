/**********************************************************************************
 * Module	: DIO command-handling module
 *
 * File name	: handling_DIO_cmd.c
 * Description	: In this module, its purpose is to dedicatedly parse and handle
 *				: Digital-IN & Digital-OUT catalog of commands.
 *				: this file implements these functions.
 * Creator	: XU ZAN [HSE-OT]
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

        char sDinCHn[8] = {0}, sDinState[16] = {0};
        long  lDinCHn = 25;

        DIN_CHm_STATE stCurrentDinCh = {25, 5};

        if ((uiPosOfCmdSeparator_Qmark == uiLen) ||
            (uiPosOfCmdSeparator_Colon == uiLen) ||
            (uiPosOfCmdSeparator_Space == uiLen) )
        {
                g_iErrorCodeNo = -18;
                return g_iErrorCodeNo;
        }

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
                        g_iErrorCodeNo = -18;
                        return g_iErrorCodeNo;
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
        while (NULL !=strchr(sRestSubstringOfDinCmdMesg, ';'));

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

int handling_Single_DOUT_CHn_cmd(char *sDoutSingleChCmdUnitMesg)
{
        int iError = 0;

        unsigned int uiLen = strlen(sDoutSingleChCmdUnitMesg),
                     uiPosOfCmdSeparator_Colon = strcspn(sDoutSingleChCmdUnitMesg, ":"),
                     uiPosOfCmdSeparator_Space = strcspn(sDoutSingleChCmdUnitMesg, " ");

        char sDoutBoardID[16] = {0}, sChNr[8] = {0}, sChnState[16] = {0}, sAttribute[32] = {0};
        long lDoutBoardID = 0, lChNr = 0;

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

        strncpy(sAttribute, sDoutSingleChCmdUnitMesg+uiPosOfCmdSeparator_Colon+1, uiLen-2);
        uiPosOfCmdSeparator_Space = strcspn(sDoutSingleChCmdUnitMesg, " ");
        if (uiPosOfCmdSeparator_Space == strlen(sDoutSingleChCmdUnitMesg))
        {
                iError = handling_Dout_1AttributeGroup(lDoutBoardID, sAttribute);
        }

/*********************************/
        return iError;
}

int handling_Multi_DOUT_CHn_cmd(char *sDoutMultiChCmdMesg)
{
        int iError = 0;

/*********************************/
        return iError;
}

int handling_Dout_1AttributeGroup(BYTE bDoutBoardID, char *sDoutChnGroup24Bits)
{
        int iError = 0;

/*********************************/
        return iError;
}
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

