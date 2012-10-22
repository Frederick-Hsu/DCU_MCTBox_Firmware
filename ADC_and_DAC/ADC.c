/*****************************************************************************************
 * Program name	: ADC.c
 * Description	: Implementation of ADC functions.
 * Author	: XU ZAN
 * Date		: Thu.	July 26, 2012
 * Copyright(C)		2010 --- 2012		Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 *****************************************************************************************/
 
#include <stdarg.h>
#include <stdlib.h>
#include "ADC_and_DAC.h"
#include "..\bus.h"
#include "..\utility.h"

#pragma ioreg


static const float fADC_Ref_Voltage = 2.5000f;






/*===========================================================================================================================================*/
void Select_ADC_CHn_ON(E_ADC_CH enumCHn)
{
	/*
	 * How to select the corresponding ADC channel?
	 * It depends on the combination of 3 pins : ADD2, ADD1, ADD0.
	 * For the table illustates :
	 *	--------+-------+-------+---------
	 * 	CHn	| ADD2	| ADD1	| ADD0
	 *	--------+-------+-------+---------
	 *	CH0	| 0	| 0	| 0
	 *	--------+-------+-------+---------
	 *	CH1	| 0	| 0	| 1
	 *	--------+-------+-------+---------
	 *	CH2	| 0	| 1	| 0
	 *	--------+-------+-------+---------
	 *	CH3	| 0	| 1	| 1
	 *	--------+-------+-------+---------
	 *	CH4	| 1	| 0	| 0
	 *	--------+-------+-------+---------
	 *	CH5	| 1	| 0	| 1
	 *	--------+-------+-------+---------
	 *	CH6	| 1	| 1	| 0
	 *	--------+-------+-------+---------
	 *	CH7	| 1	| 1	| 1
	 *	--------+-------+-------+---------
	 */
	if (enumCHn >= 8)
	{
		/*
		 * No channel can be selected.
		 * This condition must be considered.
		 *
		 * Remarked by XU ZAN @2012-07-26
		 */
		return;
	}
	 
	/*
	 * First of all, enable the CH-selector chip : ADG1208
	 */
	ENAD = HIGH;	// P93  = ENAD pin
	/*
	 * Then setup the corresponding ports as below :
	 *
	 * ADC Input Channel Selector 
	 * AddressBus[2..0]
	 * -----+-----+-----+-----+-----+------+------+------+
	 *   7  |  6  |  5  |  4  |  3  |  2   |  1   |  0   |
	 * -----+-----+-----+-----+-----+------+------+------+
	 *      |     |     |     |     | ADD2 | ADD1 | ADD0 |
	 * -----+-----+-----+-----+-----+------+------+------+
	 *      |     |     |     |     | PCS1 | PCM0 | PCM1 |
	 * -----+-----+-----+-----+-----+------+------+------+
	 */
	ADD0 = (enumCHn & 0x01);
	ADD1 = (enumCHn & 0x02) >> 1;
	ADD2 = (enumCHn & 0x04) >> 2;
	
	return;
}

void Enable_ADC_CHNL_Selector_Chip_ADG1208()
{
	ENAD = HIGH;	// P93  = ENAD pin, while HIGH is active.
}

void Disable_ADC_CHNL_Selector_Chip_ADG1208()
{
	ENAD = LOW;	// P93  = ENAD pin
}






/**********************************************************************************************************
 * Notice : The ADC timing control need to be tested, and optimize it.
 * 	    These 2 functions were currently the draft version.  
 *
 *	    MUST CARRY OUT STRICTLY TESTING.
 *
 * Remarked by XU ZAN @2012-08-06
 **********************************************************************************************************/
void ADC_Acquiring_AnalogInputValue(E_ADC_CH 	eADC_CHn, 
 				    int 	*iAIN_Value)
/***********************************************************
 * Purpose : this function just acquired the sampling value 
*            of 1 specific analog input channel.
 ***********************************************************/
{
	int iTempAdcInValue = 0xFFFF;
	
	/*
	 * Shielding the influence on DAC and switch output
	 */
	CS0 = HIGH;
	ENDA = HIGH;
	
	PORT_ChangePCM3Input();		// Change BUSY port to Input mode
	
	{
		/*
		 * ADC acquiring and sampling process
		 * Please focus on the acquiring timing, refer to the chip AD976A datasheet.
		 */
		CON = HIGH;
		Select_ADC_CHn_ON(eADC_CHn);
		
		CON = LOW;
		NOP();		// When CON line was pulled LOW, 
		NOP();		// it must "Delay more than 50ns", the pull HIGH the CON line.
		NOP();		// Use multi NOP() empty instructions to delay.
		NOP();		// each NOP() will cost 1 instruction cycle.  (1 instruction cycle = 1/core_frequency (32MHz) = 31.25ns)
		CON = HIGH;
		
		do
		{			// Just waiting for the BUSY line, till it become HIGH. 
			NOP();		// Use NOP() is used to kill the time, nothing need to do.
		}			//
		while(BUSY != HIGH);	// Then start to acquire immediately BUSY line become HIGH, quit the loop.
		
		iTempAdcInValue = Read_ADCInput_DataBus_from_DB15_to_DB00();
	}
	Disable_ADC_CHNL_Selector_Chip_ADG1208();
	
	*iAIN_Value = iTempAdcInValue;
	return;
}


void ADC_Get_8CHs_AINValue(WORD wAIN_Value[])
{
	#if 0
	int i = 0;
	
	Control_Port_Mutx_From_ADC_to_Switch(CONTROL_PIN_HIGH);
	Control_Port_Mutx_From_ADC_to_DAC(CONTROL_PIN_HIGH);
	
	// Initialization
	ENAD = LOW;
	CON = HIGH;
	BUSY = HIGH;
	
	for (i=0; i<8; i++)
	{
		Select_ADC_CHn_ON(i);
		CON = LOW;	/* Pull low the CON pin. */
		// Delay less than 83ns, and more than 50ns.
		NOP();	// 1 assembly instruction will cost 1 clock cycle. 1 clock_cycle_time = 32.25ns
		NOP();
		BUSY = LOW;	/* Pull low the BUSY pin. */
		NOP();
		NOP();
		NOP();
		CON = HIGH;	/* Pull high the CON pin. */
		
		Precisely_Delay_Xus(4);
		BUSY = HIGH;	/* Pull high the BUSY pin. */
		
		NOP();
		NOP();
		NOP();
		wAIN_Value[i] = Read_ADC_Output_Data_Bus();
		
		Precisely_Delay_Xus(2);	// Delay 2us, waiting for the next acquisition.
	}
	Disable_ADC_CHNL_Selector_Chip_ADG1208();
	CON = HIGH;
	BUSY = HIGH;
	#endif
}


void Calculate_Analog_Input_Value_for_1Ch(E_ADC_CH 	eADC_CHn, 
					  float 	*fAnalogInputValue)
{
	int wADCValue = 0;
	float fTempValue = 0.0000f;
	
	
	ADC_Acquiring_AnalogInputValue(eADC_CHn, &wADCValue);
	
	if ( ((wADCValue & 0x8000)>>15) == 0x0001 )	// Under this condition, ADC Value is negative, expressed in Binary 1 complement.
	{
		fTempValue = ((~wADCValue + 1) * fADC_Ref_Voltage)/power(2, 16);
		fTempValue = (-1) * fTempValue;
	}
	else
	{
		fTempValue = wADCValue * fADC_Ref_Voltage / power(2, 16);
	}
	fAnalogInputValue = &fTempValue;
	return;
}


void Calculate_Analog_Input_Value_for_8Chs(float fAnalogInputValues8Chs[])
{
	int i = 0;
	
	WORD wTempADCValue[8] = {0x0000};
	float fTempValue[8] = {0.0000f};
	
	ADC_Get_8CHs_AINValue(wTempADCValue);
	
	for (i=0; i<8; i++)
	{
		if ( ((wTempADCValue[i] & 0x8000)>>15) == 0x0001 )
		{
			fTempValue[i] = ((~wTempADCValue[i] + 1) * fADC_Ref_Voltage)/power(2, 16);
			fTempValue[i] = (-1)*fTempValue[i];
		}
		else
		{
			fTempValue[i] = wTempADCValue[i] * fADC_Ref_Voltage / power(2, 16);
		}
	}
	
	for (i=0; i<8; i++)
	{
		fAnalogInputValues8Chs[i] = fTempValue[i];
	}
	
	return;
}


void Calculate_Multi_CHs_ADC_AIN_Values(PST_ADC_Values_Multi_CHs pADC_CHi_Value, ...)
{
	va_list marker;
	
	PST_ADC_Values_Multi_CHs next = pADC_CHi_Value;
	
	va_start(marker, pADC_CHi_Value);
	while (next != NULL)
	{
		Calculate_Analog_Input_Value_for_1Ch(next->eADC_CHn, 
						     next->fAnalogInputValue);
		
		next = va_arg(marker, PST_ADC_Values_Multi_CHs);
	}
	va_end(marker);
	
	return;
}

