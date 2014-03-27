/******************************************************************************************
 * Program name	: Digital_IN.h
 * Description	: The declaration for Digital_IN module.
 * Author	: XU ZAN
 * Date		: Thu.	July 26, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 ******************************************************************************************/

#ifndef _DIGITAL_IN_H
#define _DIGITAL_IN_H

        #if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
                #include "../macrodriver.h"
        #else
                #include "../FW_Simulation_Testing/configuration.h"
        #endif  /*  FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO  */

	typedef enum Digital_IN_CHm
	{
		DIN_CH0 = 0,
		DIN_CH1 = 1,
		DIN_CH2 = 2,
		DIN_CH3 = 3,
		DIN_CH4 = 4,
		DIN_CH5 = 5,
		DIN_CH6 = 6,
		DIN_CH7 = 7,
		DIN_CH8 = 8,
		DIN_CH9 = 9,
		DIN_CH10 = 10,
		DIN_CH11 = 11,
		DIN_CH12 = 12,
		DIN_CH13 = 13,
		DIN_CH14 = 14,
		DIN_CH15 = 15,
		DIN_CH16 = 16,
		DIN_CH17 = 17,
		DIN_CH18 = 18,
		DIN_CH19 = 19,
		DIN_CH20 = 20,
		DIN_CH21 = 21,
		DIN_CH22 = 22,
		DIN_CH23 = 23
	}
	DIGITAL_IN_CHm;

	typedef struct Digital_IN_CHm_State
	{
		DIGITAL_IN_CHm 	eCHm;
		enum LEVEL 	eCHm_State;
	}
	DIN_CHm_STATE, *P_DIN_CHm_STATE;

        #if !defined(FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
                void Read_DIN_CHn_State(P_DIN_CHm_STATE pDInCHmState);
                void Read_DIN_Multi_CHs_State(P_DIN_CHm_STATE pCHiState, ...);
		void Read_DIN_1GroupOfCHs_State(char *sARGOUT_24ChsStates);
		
		int Read_DinBoard_CHn_State(BYTE ucDinBoardID, DIGITAL_IN_CHm eDinChn);
		void Read_DinBoard_24Chs_State(BYTE ucDinBoardID, char *sARGOUT_24ChsStates);
		
        #endif  /*  FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO  */




#endif	/*    _DIGITAL_IN_H    */

