/*************************************************************************************************************************
 * File name		: handling_FIN_cmd.h
 * Description		: To handle the "FIN" type of commands, in order to support the Optical-couple-INput board.
 * Creator			: XU ZAN	[HSE-OT]
 * Creation date	: Thur.		March 27, 2014
 * Copyright(C)		2014	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 *************************************************************************************************************************/


#ifndef HANDLING_FIN_CMD_H
#define HANDLING_FIN_CMD_H

	int handling_1GroupOfChs_FIN_cmd(char sARGIN_1GroupFinCmdMesg[], char *sARGOUT_1GroupFinChnState);
	
	int handling_SingleCH_FIN_cmd(char sARGIN_FinSingleChnCmdMesg[], char *sARGOUT_FinSingleChnState);

#endif	/*  HANDLING_FIN_CMD_H  */
