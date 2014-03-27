/******************************************************************************************
 * Program name	: Digital_IN.C
 * Description	: The implementation for Digital_IN module.
 * Author	: XU ZAN
 * Date		: Thu.	July 26, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 ******************************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "Digital_IN.h"
#include "../bus.h"
#include "../serial.h"


	void Read_DIN_CHn_State(P_DIN_CHm_STATE pDInCHmState)
	{
		unsigned int uiDIN_CHn = 30;
		enum LEVEL eDIN_CHm_LEVEL = 10;

		unsigned char sErrMesg[] = "The Digital IN channel is wrong.\n CH state is unknown.\n";

		DWORD dwDigitalInBusValue = 0x00000000;
		dwDigitalInBusValue = Read_Digital_In_Bus_Value();

		uiDIN_CHn = pDInCHmState->eCHm;
		if (uiDIN_CHn >= 24)
		{
			pDInCHmState->eCHm_State = 3;
			UARTD2_SendData(sErrMesg, sizeof(sErrMesg));
			return;
		}

		if ( ((dwDigitalInBusValue & (0x00000001 << uiDIN_CHn)) >> uiDIN_CHn) == HIGH )
		{
			eDIN_CHm_LEVEL = HIGH;
		}
		else if ( ((dwDigitalInBusValue & (0x00000001 << uiDIN_CHn)) >> uiDIN_CHn) == LOW )
		{
			eDIN_CHm_LEVEL = LOW;
		}
		else
		{
			eDIN_CHm_LEVEL = 3;	// Unknown state
		}

		pDInCHmState->eCHm_State = eDIN_CHm_LEVEL;
		return;
	}


	void Read_DIN_Multi_CHs_State(P_DIN_CHm_STATE pCHiState, ...)
	{
		va_list marker;

		P_DIN_CHm_STATE next = pCHiState;

		va_start(marker, pCHiState);
		while (next != NULL)
		{
			Read_DIN_CHn_State(next);
			next = va_arg(marker, P_DIN_CHm_STATE);
		}
		va_end(marker);

		return;
	}

	void Read_DIN_1GroupOfCHs_State(char *sARGOUT_24ChsStates)
	{
		DWORD dwDinBusValue = 0x00000000;
		dwDinBusValue = Read_Digital_In_Bus_Value();
		
		sprintf(sARGOUT_24ChsStates, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
			((dwDinBusValue & 0x00800000)>>23),
			((dwDinBusValue & 0x00400000)>>22),
			((dwDinBusValue & 0x00200000)>>21),
			((dwDinBusValue & 0x00100000)>>20),
			((dwDinBusValue & 0x00080000)>>19),
			((dwDinBusValue & 0x00040000)>>18),
			((dwDinBusValue & 0x00020000)>>17),
			((dwDinBusValue & 0x00010000)>>16),
			((dwDinBusValue & 0x00008000)>>15),
			((dwDinBusValue & 0x00004000)>>14),
			((dwDinBusValue & 0x00002000)>>13),
			((dwDinBusValue & 0x00001000)>>12),
			((dwDinBusValue & 0x00000800)>>11),
			((dwDinBusValue & 0x00000400)>>10),
			((dwDinBusValue & 0x00000200)>>9),
			((dwDinBusValue & 0x00000100)>>8),
			((dwDinBusValue & 0x00000080)>>7),
			((dwDinBusValue & 0x00000040)>>6),
			((dwDinBusValue & 0x00000020)>>5),
			((dwDinBusValue & 0x00000010)>>4),
			((dwDinBusValue & 0x00000008)>>3),
			((dwDinBusValue & 0x00000004)>>2),
			((dwDinBusValue & 0x00000002)>>1),
			((dwDinBusValue & 0x00000001)   ));
		return;
	}
	
	int Read_DinBoard_CHn_State(BYTE ucDinBoardID, DIGITAL_IN_CHm eDinChn)
	{
		int iState = 0;
		DWORD dw24PortsState = 0x00000000;
		
		CS1 = LOW;	// Enable chip(U4) 74ALS520
		Write_Address_Bus(ucDinBoardID);
		dw24PortsState = Read_DataBus_State_from_DB23_to_DB00();
		
		iState = (((0x00000001 << eDinChn) & dw24PortsState) >> eDinChn);
		
		CS1 = HIGH;	// Remember to disable chip(U4)
		
		return iState;
	}
	
	void Read_DinBoard_24Chs_State(BYTE ucDinBoardID, char *sARGOUT_24ChsStates)
	{
		DWORD dw24PortsState = 0x00000000;
		
		CS1 = LOW;
		Write_Address_Bus(ucDinBoardID);
		dw24PortsState = Read_DataBus_State_from_DB23_to_DB00();
		CS1 = HIGH;
		
		sprintf(sARGOUT_24ChsStates, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
			((dw24PortsState & 0x00800000)>>23),
			((dw24PortsState & 0x00400000)>>22),
			((dw24PortsState & 0x00200000)>>21),
			((dw24PortsState & 0x00100000)>>20),
			((dw24PortsState & 0x00080000)>>19),
			((dw24PortsState & 0x00040000)>>18),
			((dw24PortsState & 0x00020000)>>17),
			((dw24PortsState & 0x00010000)>>16),
			((dw24PortsState & 0x00008000)>>15),
			((dw24PortsState & 0x00004000)>>14),
			((dw24PortsState & 0x00002000)>>13),
			((dw24PortsState & 0x00001000)>>12),
			((dw24PortsState & 0x00000800)>>11),
			((dw24PortsState & 0x00000400)>>10),
			((dw24PortsState & 0x00000200)>>9 ),
			((dw24PortsState & 0x00000100)>>8 ),
			((dw24PortsState & 0x00000080)>>7 ),
			((dw24PortsState & 0x00000040)>>6 ),
			((dw24PortsState & 0x00000020)>>5 ),
			((dw24PortsState & 0x00000010)>>4 ),
			((dw24PortsState & 0x00000008)>>3 ),
			((dw24PortsState & 0x00000004)>>2 ),
			((dw24PortsState & 0x00000002)>>1 ),
			((dw24PortsState & 0x00000001)    ));
	}
	
	
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
 