/***********************************************************************************************************
 * Program name	: Just_test_and_debug.c
 * Description	: This file is used to dedicatedly test and debug all function modules. 
 *		: You can write the test functions freely.
 * Author	: XU ZAN
 * Date		: Sat.	Oct. 27, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 ***********************************************************************************************************/


#include "Just_test_and_debug.h"
 
#include "Switch_Relay_Control/Switch_Relay_Control.h"
#include "bus.h"
#include "DIO/Digital_IN.h"
#include "PWM/PWM_Out.h"


/*************************************************************************/
//  External global variables :



/*************************************************************************/
//  Functions implementation :
#if defined (TEST_DEBUG_PURPOSE)

	void SetInitialState_TurnOffAllSwitches(void)
	{
		int i = 0;
		CS0 = LOW;
		RW = HIGH;
		
		/*
		 * Shielding the influence on ADC and DAC
		 */
		ENDA = HIGH;
		ENAD = LOW;
		
		for (i=0x00; i<=0xFF; i++)
		{
			Write_Address_Bus(i);
			Write_All_DataBus(0x00000000);
		}
		
		/*
		 * At last, you should disable the Address selector chip 74ALS520.
		 * In order to keep the current state of switches.
		 */
		for (i=0; i<100; i++)
		{
			NOP();
			NOP();
			NOP();
			NOP();
			NOP();
			NOP();
			NOP();
		}
		CS0 = HIGH;
		
		return;
	}


	 
	void Test_Switch_Ctrl(void)
	{
		#if 1
			int i = 0;
			PST_Access_Ctrl_SwitchRelayMatrix pSwitch1  = (PST_Access_Ctrl_SwitchRelayMatrix)malloc(sizeof(ST_Access_Ctrl_SwitchRelayMatrix)),
							  pSwitch2 = (PST_Access_Ctrl_SwitchRelayMatrix)malloc(sizeof(ST_Access_Ctrl_SwitchRelayMatrix)),
							  pSwitch3 = (PST_Access_Ctrl_SwitchRelayMatrix)malloc(sizeof(ST_Access_Ctrl_SwitchRelayMatrix)),
							  pSwitch4 = (PST_Access_Ctrl_SwitchRelayMatrix)malloc(sizeof(ST_Access_Ctrl_SwitchRelayMatrix)),
							  pSwitch5 = (PST_Access_Ctrl_SwitchRelayMatrix)malloc(sizeof(ST_Access_Ctrl_SwitchRelayMatrix)),
							  pSwitch6 = (PST_Access_Ctrl_SwitchRelayMatrix)malloc(sizeof(ST_Access_Ctrl_SwitchRelayMatrix));
			
			pSwitch1->byteBoardID = 0x01;
			pSwitch1->dwSwitch_Relay_CHn = 1;
			pSwitch1->eOpen_Close_State = CLOSE;
			
			/**************************************************************************/
			pSwitch2->byteBoardID = 0x01;
			pSwitch2->dwSwitch_Relay_CHn = 3;
			pSwitch2->eOpen_Close_State = CLOSE;
			
			pSwitch3->byteBoardID = 0x01;
			pSwitch3->dwSwitch_Relay_CHn = 23;
			pSwitch3->eOpen_Close_State = CLOSE;
			
			pSwitch4->byteBoardID = 0x01;
			pSwitch4->dwSwitch_Relay_CHn = 17;
			pSwitch4->eOpen_Close_State = CLOSE;
			
			pSwitch5->byteBoardID = 0x01;
			pSwitch5->dwSwitch_Relay_CHn = 10;
			pSwitch5->eOpen_Close_State = CLOSE;
			
			pSwitch6->byteBoardID = 0x01;
			pSwitch6->dwSwitch_Relay_CHn = 5;
			pSwitch6->eOpen_Close_State = OPEN;
			/**************************************************************************/
			
			
			
			Control_Single_Switch(pSwitch1);
			
			// Control_Multi_Switch(pSwitch2, pSwitch3, pSwitch4, pSwitch5, pSwitch6, NULL);
			
			free(pSwitch1);
			free(pSwitch2);
			free(pSwitch3);
			free(pSwitch4);
			free(pSwitch5);
			free(pSwitch6);
		#endif
		
		
		
		
		
		
	/*******************************************************/
		return;
	}
	
	void Enable_All_Switches()
	{
		CS0 = LOW;
		RW = HIGH;
	
		ENDA = HIGH;
		ENAD = LOW;
		
		Write_Address_Bus(0x01);
		Write_All_DataBus(0x0000FFFF);
		CS0 = HIGH;
		
		return;
	}
	
	void Print_ADC_Values_8Channels(void)
	{
		int iADCValue[8] = {0};
		char sADC_8CHs_Value[512] = {0};
		
		ADC_Get_8CHs_AINValue(iADCValue);
		
		sprintf(sADC_8CHs_Value,
			"ADC Value : %d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n\n", 
			iADCValue[0], 
			iADCValue[1], 
			iADCValue[2],
			iADCValue[3],
			iADCValue[4],
			iADCValue[5],
			iADCValue[6],
			iADCValue[7]);
			
		UARTD2_SendData(sADC_8CHs_Value, strlen(sADC_8CHs_Value)*sizeof(char));
		
		return;
	}
	
	void Test_MCU_Chip_System(void)
	{
		int iBit = 12315;
		char sTemp[128] = {0};
		
		sprintf(sTemp, "This MCU chip is a %d-byte system.\n\n", sizeof(iBit));
		UARTD2_SendData(sTemp, strlen(sTemp)*sizeof(char));
		
		return;
	}
	
	void Test_1Ch_ADC_Voltage_Measurement(void)
	{
		int iADC_CHn = 1;
		int iAdcCntValue = 0;
		float fVoltage = 0.0000f;
		
		char sAdcMeasureResult[128] = {0};
		
		ADC_Acquiring_AnalogInputValue(iADC_CHn, &iAdcCntValue);	
		Calculate_Analog_Input_Value_for_1Ch(iADC_CHn, &fVoltage);
		
		sprintf(sAdcMeasureResult, 
			"ADC %d:COUNT %d\tADC %d:VOLT  %f\n",
			iADC_CHn, iAdcCntValue, fVoltage);
		UARTD2_SendData(sAdcMeasureResult, strlen(sAdcMeasureResult)*sizeof(char));
		
		return;
	}
	
	void Test_1Ch_DIN_Measurement(void)
	{
		DIN_CHm_STATE tCurrentDinCh = {23, LOW};
		char sDinMeasureResult[128] = {0};
		
		Read_DIN_CHn_State(&tCurrentDinCh);
		if (LOW == tCurrentDinCh.eCHm_State)
		{
			sprintf(sDinMeasureResult, "DIN %d:STATE LOW\n", tCurrentDinCh.eCHm);
		}
		else
		{
			sprintf(sDinMeasureResult, "DIN %d:STATE HIGH\n", tCurrentDinCh.eCHm);
		}
		UARTD2_SendData(sDinMeasureResult, strlen(sDinMeasureResult)*sizeof(char));
		
		return;
	}
	
	void Test_PWM_Out(void)
	{
	#if (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION2)
		ST_PWM_PARAM tPwmParam = {10,	// PWM_Freq = 1Hz
					  20,	// PWM_DutyCyle = 40%
					  5};	// PWM_Amplitude = 5V
		// Configure PWM parameter
		Set_Config_PWM_Out_Param(tPwmParam);
		// Activate PWM_Out
		PWM_Out_Start(PWM_ATTR_SECONDARY, PwmOutChn_Secondary11, 0x01);
	#elif (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION1)
		ST_PWM_PARAM tPwmParam1 = {10, 40, 5}, tPwmParam2 = {4, 80, 5};
		/* Configure the PWM_Out1 parameter */
		Set_Config_PWM_Out_Param(PWM_OUT1, tPwmParam1);
		/* Activate PWM_Out1 */
		PWM_Out_Start(PWM_OUT1, PWM_ATTR_SECONDARY, PwmOutChn_Secondary11, 0x01);
		
		Set_Config_PWM_Out_Param(PWM_OUT2, tPwmParam2);
		PWM_Out_Start(PWM_OUT2, PWM_ATTR_SECONDARY, PwmOutChn_Secondary21, 0x01);
		/* Stop PWM_Out1 */
		// PWM_Out_Stop(PWM_OUT1);
		/* Stop PWM_Out2 */
		// PWM_Out_Stop(PWM_OUT2);
	#endif
	}

#endif	/*    TEST_DEBUG_PURPOSE    */

