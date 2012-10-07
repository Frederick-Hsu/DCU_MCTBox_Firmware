/********************************************************************************************************
 * Program name	: bus.h
 * Description	: Construct the bus, such as 24-bit Data bus, 8-bit Address bus and 24-bit Digit IN
 *		: Control bus, etc.
 *		: And the manipulation on these bus.
 * Author	: XU ZAN
 * Date		: Wed.	July 25, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 ********************************************************************************************************/

#ifndef _BUS_H
#define _BUS_H

	/********************************************************************************/
	// Included files
	#pragma ioreg
	
	#include "macrodriver.h"
	
	
	
	/********************************************************************************/
	// Macros
	// Define the control port
	#define CS0	(P9H.1)
	#define CS1	(P9H.0)
	#define CS2	(P9L.7)
	#define CS3	(P9L.6)
	#define CS4	(P9L.5)
	#define CS5	(P9L.4)
	
	#define ENAD	(P9L.3)
	#define ENDA	(PCT.1)
	#define RW	(PCT.0)
	#define BUSY	(PCM.3)
	#define CON	(PCM.2)
	
	#define NSLP	(P3L.2)
	#define ERR	(P4.0)
	#define EN	(P4.1)
	#define STB	(P4.2)
	
	
	
	/*
	 * Specify the Data Bus[23..0], and its corresponding pin.
	 */
	#define DB00	(P7L.0)
	#define DB01	(P7L.1)
	#define	DB02	(P7L.2)
	#define	DB03	(P7L.3)
	#define	DB04	(P7L.4)
	#define DB05	(P7L.5)
	#define DB06	(P7L.6)
	#define DB07	(P7L.7)
	#define DB08	(P7H.0)
	#define DB09	(P7H.1)
	#define DB10	(P7H.2)
	#define DB11	(P7H.3)
	#define DB12	(P7H.4)
	#define DB13	(P7H.5)
	#define DB14	(P7H.6)
	#define DB15	(P7H.7)
	#define DB16	(PDLH.5)
	#define DB17	(PDLH.4)
	#define DB18	(PDLH.3)
	#define DB19	(PDLH.2)
	#define DB20	(PDLH.1)
	#define DB21	(PDLH.0)
	#define DB22	(PDLL.7)
	#define DB23	(PDLH.6)
	
	
	typedef enum CONTROL_PIN_LEVEL
	{
		CONTROL_PIN_LOW = 0,
		CONTROL_PIN_HIGH = 1
	}
	E_CTRL_PIN_LEVEL;
	
	
	
	
/*======================================================================================================================================================================================================*/
	void Write_Address_Bus(BYTE btAddrBus_bit7_0);
	
	BYTE Read_Address_Bus();
	
	void Set_Data_Bus_bit7_0(BYTE btLowByte_Value);
	void Set_Data_Bus_bit15_8(BYTE btMiddleByte_Value);
	void Set_Data_Bus_bit23_16(BYTE btHighByte_Value);
	
	void Write_DataBus_Single_CHn(     DWORD   dwCHn,
				      enum LEVEL   eSpecificLevelValue);
	
	
	DWORD Read_Digital_In_Bus_Value();
	
	void Set_DataBus_to_Initial_Default_After_Power_ON();
	void Set_AddressBus_to_Initial_Default_After_Power_ON();
	void Set_ControlBus_to_Initial_Default_After_Power_ON();
	void Set_Initial_Default_After_Power_ON();
	
	
	/*****************************************************************/
	void Control_Port_Mutx_From_Switch_to_ADC(E_CTRL_PIN_LEVEL eCtrl_Pin_CS0);
	void Control_Port_Mutx_From_ADC_to_Switch(E_CTRL_PIN_LEVEL eCtrl_Pin_ENAD);
	void Control_Port_Mutx_From_ADC_to_DAC(E_CTRL_PIN_LEVEL eCtrl_Pin_ENAD);
	void Control_Port_Mutx_From_DAC_to_ADC(E_CTRL_PIN_LEVEL eCtrl_Pin_ENDA);
	
	


#endif	/*    _BUS_H    */

