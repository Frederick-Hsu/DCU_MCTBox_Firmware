/**********************************************************************************
 * Module		: DIO command-handling module
 *
 * File name	: handling_DIO_cmd.h
 * Description	: In this module, its purpose is to dedicatedly parse and handle
 *				: Digital-IN & Digital-OUT catalog of commands.
 *				: this file declares the functions prototype.
 * Creator		: XU ZAN [HSE-OT]
 * Creation date: Wed.	March 6, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 **********************************************************************************/

#ifndef _HANDLING_DIO_CMD_H
#define _HANDLING_DIO_CMD_H

        #if !defined(FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
		#include "../macrodriver.h"
	#else
		#include "../FW_Simulation_Testing/configuration.h"
	#endif

	int handling_SingleCH_DIN_cmd(char	*sARGIN_DinSingleChCmdMesg,
                                      char	*sARGOUT_DinSingleChStateResponseMesg);

	int handling_MultiCH_DIN_cmd(char       *ARGIN_DinMultiChCmdMesg,
                                     char       *ARGOUT_DinMultiChStateResponseMesg);

        int handling_Single_DOUT_CHn_cmd(char   *sDoutSingleChCmdUnitMesg);

        int handling_Multi_DOUT_CHn_cmd(char    *sDoutMultiChCmdMesg);

        int handling_Dout_1AttributeGroup(BYTE bDoutBoardID, char *sDoutChnGroup24Bits);

#endif	/*  _HANDLING_DIO_CMD_H  */

