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

        DIN_CHm_STATE stCurrentDinCh = {0, LOW};

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
                sprintf(sARGOUT_DinSingleChStateResponseMesg, "DIN %d:State HIGH", lDinCHn-1);
        }
        else if (stCurrentDinCh.eCHm_State == LOW)
        {
                sprintf(sARGOUT_DinSingleChStateResponseMesg, "DIN %d:State LOW", lDinCHn-1);
        }

/*******************************/
	return iError;
}

int handling_MultiCH_DIN_cmd(char       *ARGIN_DinMultiChCmdMesg,
                             char       *ARGOUT_DinMultiChStateResponseMesg)
{
	int iError = 0;


/*******************************/
	return iError;
}
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

