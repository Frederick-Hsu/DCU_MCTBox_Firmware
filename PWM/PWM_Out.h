/********************************************************************************************
 * File name	: PWM_Out.h
 * Description	: PWM Out module, declares the prototypes in this file.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Fri.	June 07, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 * 
 ********************************************************************************************/

#ifndef PWM_OUT_H
#define PWM_OUT_H

#if !defined (FW_SIMULATION_TESTING_BASED_ON_VISUAL_STUDIO)
	#include "../macrodriver.h"
	#include "../bus.h"
#else
	#include "../FW_Simulation_Testing/configuration.h"
#endif
	#include "PWM_Out_Generating_Option.h"
	
	typedef struct stPWM_Param
	{
		float fPWM_Freq_InHz;
		float fPWM_DutyCycle_InPercentage;
		float fPWM_Amplitude_InVolt;	// This parameter could be reserved for extension in future.	
	}
	ST_PWM_PARAM, *PST_PWM_PARAM;
	
	enum Pwm_Out_No
	{
		PWM_OUT1 = 1,
		PWM_OUT2 = 2
	};
	/*
	 * Note : 
	 * Since the original PWM ports of Renesas uPD70F3376 chip had been occupied as the GPIOs in the schematics, 
	 * in order to output PWM signal or acquire the external PWM signal, it was constrained to simulate to implement
	 * the PWM function through software. 
	 *
	 * Providing 2 kinds of PWM-outputting approaches :
	 *	1. Choose CS2(i.e. port P97), CS3(port P96), CS4(i.e. P95), CS5(i.e. P94) ports to output PWM signals
	 *	   but here only select 2 ports : CS4 and CS5, other ports are not enabled.
	 *
	 *	2. Choose one pin of DataBus [D0..D23] to output PWM signal.
	 */
	enum PwmOutAttribute
	{
		PWM_ATTR_PRIMARY = 0,
		PWM_ATTR_SECONDARY
	};
	// PWM Approach 1 :
	typedef enum PwmOutPrimaryChn
	{
		PwmOutChn_Primary0 = 0,	// CS4
		PwmOutChn_Primary1 = 1	// CS5
	}
	PWM_OUT_PRIMARY_CHN;
	// PWM Approach 2 :
	typedef enum PwmOutSecondaryChn
	{
		PwmOutChn_Secondary0 = 0,	// DB00
		PwmOutChn_Secondary1,	// DB01
		PwmOutChn_Secondary2,	// DB02
		PwmOutChn_Secondary3,	// DB03
		PwmOutChn_Secondary4,	// DB04
		PwmOutChn_Secondary5,	// DB05
		PwmOutChn_Secondary6,	// DB06
		PwmOutChn_Secondary7,	// DB07
		PwmOutChn_Secondary8,	// DB08
		PwmOutChn_Secondary9,	// DB09
		PwmOutChn_Secondary10,	// DB10
		PwmOutChn_Secondary11,	// DB11
		PwmOutChn_Secondary12,	// DB12
		PwmOutChn_Secondary13,	// DB13
		PwmOutChn_Secondary14,	// DB14
		PwmOutChn_Secondary15,	// DB15
		PwmOutChn_Secondary16,	// DB16
		PwmOutChn_Secondary17,	// DB17
		PwmOutChn_Secondary18,	// DB18
		PwmOutChn_Secondary19,	// DB19
		PwmOutChn_Secondary20,	// DB20
		PwmOutChn_Secondary21,	// DB21
		PwmOutChn_Secondary22,	// DB22
		PwmOutChn_Secondary23	// DB23
	}
	PWM_OUT_SECONDARY_CHN;
	/************************************************************************************************/
	struct PwmOutChnSelector
	{
		enum PwmOutAttribute 	ePrimaryOrSecondary;
		int  			iPwmOutChn;
		BYTE 			bytPwmOutBoardID;
	};
	
#if 0
	enum PWM_OUT_CURRENT_STATE
	{
		PWM_STATE_STOPPED = 0,
		PWM_STATE_RUNNING = 1
	};
	enum PWM_OUTn_CURRENT_ENABLED
	{
		PWM_OUT1_DISABLED = 0,
		PWM_OUT2_DISABLED = 0,
		
		PWM_OUT1_ENABLED = 1,
		PWM_OUT2_ENABLED = 1
	};
	
	typedef struct tPwmOut_FSM
	{
		// enum Pwm_Out_No eCurrentActivePwmOutNr;
		enum PWM_OUT_CURRENT_STATE ePwmCurrentState;
		struct PwmOutChnSelector tPWM_Out_CH;		
	}
	ST_PWMOUT_FSM, *PST_PWMOUT_FSM;
#endif
	
	/*===============================================================================================
	 * Functions prototype relative to PWM :
	 */
	void Set_PwmOutPrimaryChn_Level(PWM_OUT_PRIMARY_CHN ePrimaryChn, enum LEVEL eLevel);
	void Set_PwmOutSecondaryChn_Level(BYTE 				bytPwmBoardID, 
					  PWM_OUT_SECONDARY_CHN 	ePwmSecondaryChn, 
					  enum LEVEL 			eLevel);
	
	#if (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION2)
		void Set_Config_PWM_Out_Param(ST_PWM_PARAM stPwmParam);
		
		void PWM_Out_Start(int ePrimaryOrSecondary, int ePwmChn, BYTE bytPwmBoardID);
		void PWM_Out_Stop(void);
		
		void Calculate_Timer_Interval(float 	fARGIN_PWM_Freq_InHz, 
					      float 	fARGIN_PWM_DutyCycle_InPercentage,
					      USHORT 	*ushrtARGOUT_pInterval_For_PwmON,
					      USHORT 	*ushrtARGOUT_pInterval_For_PwmOFF);
	#elif (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION1)
		void Set_Config_PWM_Out_Param(enum Pwm_Out_No ePwmOutNr, ST_PWM_PARAM stPwmParam);
		
		void Calculate_Timer_Interval(float fARGIN_PWM_Freq_InHz,
					      float fARGIN_PWM_DutyCycle_InPercentage,
					      unsigned int *uiARGOUT_pCycleCnt_For_PwmHigh,
					      unsigned int *uiARGOUT_pCycleCnt_For_PwmLow);
		
		void PWM_Out_Start(enum Pwm_Out_No ePwmOutNr, int ePrimaryOrSecondary, int ePwmChn, BYTE bytPwmBoardID);
		void PWM_Out_Stop(enum Pwm_Out_No ePwmOutNr);
		void PWM_Out1_Stop(void);
		void PWM_Out2_Stop(void);
	#endif
	

#endif	/*  PWM_OUT_H  */

