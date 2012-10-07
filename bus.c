/********************************************************************************************************
 * Program name	: bus.c
 * Description	: Construct the bus, such as 24-bit Data bus, 8-bit Address bus and 24-bit Digit IN
 *		: Control bus, etc.
 *		: And the manipulation on these bus.
 *		: This file is the implematation.
 *
 * Author	: XU ZAN
 * Date		: Wed.	July 25, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 ********************************************************************************************************/

#include <string.h>
#include "bus.h"
#include "serial.h"
#include "port.h"

#pragma ioreg
	/*******************************************************************************/
	// Global variables
	
	/*
	 * DataBus[7..0]
	 * -----+-----+-----+-----+-----+-----+-----+-----|
	 *   7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	 * -----+-----+-----+-----+-----+-----+-----+-----|
	 *  P77 | P76 | P75 | P74 | P73 | P72 | P71 | P70 |
	 * -----+-----+-----+-----+-----+-----+-----+-----+
	 */
	volatile BYTE g_btDataBus_Bit7_0 = 0x00; // = (BYTE)(P7L & 0xFF);
	
	/*
	 * DataBus[15..8]
	 * ------+------+------+------+------+------+-----+-----+
	 *   15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |
	 * ------+------+------+------+------+------+-----+-----+
	 *  P715 | P714 | P713 | P712 | P711 | P710 | P79 | P78 |
	 * ------+------+------+------+------+------+-----+-----+
	 */
	volatile BYTE g_btDataBus_Bit15_8 = 0x00; //  = (BYTE)(((P7H & 0xFF00) >> 8) & 0xFF);
	
	
	
	/*
	 * PDL[13..6]
	 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
	 *   15  |  14  |  13   |  12   |  11   |  10   |  9   |  8   |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
	 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
	 *       |      | PDL13 | PDL12 | PDL11 | PDL10 | PDL9 | PDL8 | PDL7 | PDL6 | PDL5 | PDL4 | PDL3 | PDL2 | PDL1 | PDL0 |
	 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
	 *    0  |  0   |   1   |   1   |   1   |   1   |   1  |  1   |  1   |  1   |  0   |   0  |   0  |   0  |  0   |   0  |
	 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
	 */
	BYTE btTempDataBus; //  = (BYTE)(((PDL & 0x3FC0) >> 6) & 0xFF);
	/*
	 * DataBus[23..16]
	 * ------+------+------+------+-------+-------+-------+-------+
	 *   23  |  22  |  21  |  20  |  19   |  18   |  17   |  16   |
	 * ------+------+------+------+-------+-------+-------+-------+
	 *  PDL6 | PDL7 | PDL8 | PDL9 | PDL10 | PDL11 | PDL12 | PDL13 |
	 * ------+------+------+------+-------+-------+-------+-------+
	 */
	volatile BYTE g_btDataBus_Bit23_16 = 0x00;
	#if 0
						  = ((btTempDataBus & 0x01) << 7) + 
						    ((btTempDataBus & 0x02) << 5) +
						    ((btTempDataBus & 0x04) << 3) +
						    ((btTempDataBus & 0x08) << 1) +
						    ((btTempDataBus & 0x10) >> 1) +
						    ((btTempDataBus & 0x20) >> 3) +
						    ((btTempDataBus & 0x40) >> 5) +
						    ((btTempDataBus & 0x80) >> 7);
	#endif
	
	
	/*
	 * AddressBus[7..0]
	 * ------+------+------+------+------+------+------+------+
	 *   7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
	 * ------+------+------+------+------+------+------+------+
	 *  P910 | P911 | P912 | P913 | PCS0 | PCS1 | PCM0 | PCM1 |
	 * ------+------+------+------+------+------+------+------+
	 *
	 */
	volatile BYTE g_btAddressBus_Bit7_0 = 0x00;
	#if 0
					    = ((P9H.2 & 0x01) << 7) + 
					      ((P9H.3 & 0x01) << 6) +
					      ((P9H.4 & 0x01) << 5) +
					      ((P9H.5 & 0x01) << 4) +
					      ((PCS.0 & 0x01) << 3) +
					      ((PCS.1 & 0x01) << 2) +
					      ((PCM.0 & 0x01) << 1) +
					      ((PCM.1 & 0x01));
	#endif
	
	/*
	 * ADC Data Bus[15..0]
	 * ------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 *   15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	 * ------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 *  P715 | P714 | P713 | P712 | P711 | P710 | P79 | P78 | P77 | P76 | P75 | P74 | P73 | P72 | P71 | P70 |
	 * ------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 *                          P7H                         |                      P7L                      |
	 * ------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 *
	 */
	volatile WORD g_wADC_Value_DataBus_16bit = 0x0000;
	#if 0
					       = (((WORD)(P7H & 0x00FF)) << 8) +
						 ( (WORD)(P7L &   0xFF));
	#endif
	
	
	/*
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
	BYTE g_btADC_Chnl_Selector_AddressBus_bit2_0 = 0x00; 
	#if 0
						     = ((PCS.1 & 0x01) << 2) +
						       ((PCM.0 & 0x01) << 1) +
						       ((PCM.1 & 0x01));
	#endif
	
	
	
	
	/*
	 * Digital In Bus[23..0]
	 * ------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 *   31  |  30  |  29  |  28  |  27  |  26  |  25  |  24  |  23  |  22  |  21  |  20  |  19  |  18  |  17  |  16  |  15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	 * ------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 *       |      |      |      |      |      |      |      | DI23 | DI22 | DI21 | DI20 | DI19 | DI18 | DI17 | DI16 | DI15 | DI14 | DI13 | DI12 | DI11 | DI10 | DI9 | DI8 | DI7 | DI6 | DI5 | DI4 | DI3 | DI2 | DI1 | DI0 |
	 * ------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 *       |      |      |      |      |      |      |      | PDL5 | PDL4 | PDL3 | PDL2 | PDL1 | PDL0 | PCT6 | PCT4 | P92  | P91  | P90  | P51  | P50  | P37  | P36 | P35 | P06 | P04 | P03 | P02 | P01 | P00 | P11 | P10 |
	 * ------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	 */
	volatile DWORD g_dwDigitalIN_Bus_bit23_0 = 0x00000000;
	#if 0
						 = (
						   	(
						   		(DWORD)(PDL & 0x003F)
							) << 18
						   )
						   +
						   (
						   	(
						   		(DWORD)
								(
						   			((PCT.6 & 0x01) << 1) + (PCT.4 & 0x01)
								)
							) << 16
						   )
						   +
						   (
						   	(
						   		(DWORD)(P9L & 0x07)
							) << 13
						   )
						   +
						   (
						   	(DWORD)
						   	(
						   		((P5.1 & 0x01) << 1) + (P5.0 & 0x01)
							) << 11
						   )
						   +
						   (
						   	(DWORD)
							(
						   		(P3L & 0xE0)
							) << 8
						   )
						   +
						   (
						   	(
						   		(DWORD)
								(
						   			((P0.6 & 0x01) << 5) + (P0 & 0x1F)
								)
							) << 2
						   )
						   +
						   (
						   	(DWORD)
							(
						   		(P1 & 0x03)
							)
						   );
	#endif
	
	
	/*
	 * DAC Input Data Bus[7..0]
	 * -----+-----+-----+-----+-----+-----+-----+-----+
	 *   7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	 * -----+-----+-----+-----+-----+-----+-----+-----+
	 *  P77 | P76 | P75 | P74 | P73 | P72 | P71 | P70 |
	 * -----+-----+-----+-----+-----+-----+-----+-----+
	 */
	BYTE g_btDAC_In_Databus_bit7_0 = 0x00; // = P7L;
	
	
	
	
/*======================================================================================================================================================================================================*/





void Write_Address_Bus(BYTE btAddrBus_bit7_0)
/*
 * AddressBus[7..0]
 * ------+------+------+------+------+------+------+------+
 *   7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
 * ------+------+------+------+------+------+------+------+
 *  P910 | P911 | P912 | P913 | PCS0 | PCS1 | PCM0 | PCM1 |
 * ------+------+------+------+------+------+------+------+
 *
 */
{
	/*
	 * First of all, you must change these ports to output mode by software
	 */
	PORT_ChangePCM1Output(0);	// For A0
	PORT_ChangePCM0Output(0);	//     A1
	PORT_ChangePCS1Output(0);	//     A2
	PORT_ChangePCS0Output(0);	//     A3
	PORT_ChangeP913Output(0);	//     A4
	PORT_ChangeP912Output(0);	//     A5
	PORT_ChangeP911Output(0);	//     A6
	PORT_ChangeP910Output(0);	//     A7
	
	/*
	 * Start to set the value for corresponding port.
	 */	
	A0 = (btAddrBus_bit7_0 & 0x01);		// PCM1 ----- bit 0
	A1 = (btAddrBus_bit7_0 & 0x02) >> 1;		// PCM0 ----- bit 1
	A2 = (btAddrBus_bit7_0 & 0x04) >> 2;		// PCS1 ----- bit 2
	A3 = (btAddrBus_bit7_0 & 0x08) >> 3;		// PCS0 ----- bit 3
	A4 = (btAddrBus_bit7_0 & 0x10) >> 4;		// P913 ----- bit 4
	A5 = (btAddrBus_bit7_0 & 0x20) >> 5;		// P912 ----- bit 5
	A6 = (btAddrBus_bit7_0 & 0x40) >> 6;		// P911 ----- bit 6
	A7 = (btAddrBus_bit7_0 & 0x80) >> 7;		// P910 ----- bit 7
	return;
}

BYTE Read_Address_Bus()
{
	/*
	 * Change these ports to input mode by software
	 */
	PORT_ChangePCM1Input();		// For A0
	PORT_ChangePCM0Input();		//     A1
	PORT_ChangePCS1Input();		//     A2
	PORT_ChangePCS0Input();		//     A3
	PORT_ChangeP913Input(0);	//     A4
	PORT_ChangeP912Input(0);	//     A5
	PORT_ChangeP911Input(0);	//     A6
	PORT_ChangeP910Input(0);	//     A7
	
	
	g_btAddressBus_Bit7_0 = ((A7 & 0x01) << 7) + 
				((A6 & 0x01) << 6) +
				((A5 & 0x01) << 5) +
				((A4 & 0x01) << 4) +
				((A3 & 0x01) << 3) +
				((A2 & 0x01) << 2) +
				((A1 & 0x01) << 1) +
				((A0 & 0x01));
	
	return g_btAddressBus_Bit7_0;
}

void Set_Data_Bus_bit7_0(BYTE btLowByte_Value)
/*
 * DataBus[7..0]
 * -----+-----+-----+-----+-----+-----+-----+-----|
 *   7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 * -----+-----+-----+-----+-----+-----+-----+-----|
 *  P77 | P76 | P75 | P74 | P73 | P72 | P71 | P70 |
 * -----+-----+-----+-----+-----+-----+-----+-----+
 */
{
	unsigned char sUART2_Send_Mesg[512] = "Data bus value entered error.\n";
	RW = HIGH;	// Enable sending direction for chip 74HC245
	if (btLowByte_Value > 0xFF)
	{
		UARTD2_SendData(sUART2_Send_Mesg, strlen("Data bus value entered error.\n")*sizeof(char));
		return;
	}
	else
	{
		PORT_ChangeP70Output(0);	// For	DB00
		PORT_ChangeP71Output(0);	//	DB01
		PORT_ChangeP72Output(0);	//	DB02
		PORT_ChangeP73Output(0);	//	DB03
		PORT_ChangeP74Output(0);	//	DB04
		PORT_ChangeP75Output(0);	//	DB05
		PORT_ChangeP76Output(0);	//	DB06
		PORT_ChangeP77Output(0);	//	DB07
		
		// LSB : bit0 : P70
		DB00 = (btLowByte_Value & 0x01);
		
		DB01 = (btLowByte_Value & 0x02) >> 1;
		DB02 = (btLowByte_Value & 0x04) >> 2;
		DB03 = (btLowByte_Value & 0x08) >> 3;
		DB04 = (btLowByte_Value & 0x10) >> 4;
		DB05 = (btLowByte_Value & 0x20) >> 5;
		DB06 = (btLowByte_Value & 0x40) >> 6;
		
		// MSB : bit7 : P77
		DB07 = (btLowByte_Value & 0x80) >> 7;
	}
	return;
}

void Set_Data_Bus_bit15_8(BYTE btMiddleByte_Value)
/*
 * DataBus[15..8]
 * ------+------+------+------+------+------+-----+-----+
 *   15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |
 * ------+------+------+------+------+------+-----+-----+
 *  P715 | P714 | P713 | P712 | P711 | P710 | P79 | P78 |
 * ------+------+------+------+------+------+-----+-----+
 */
{
	unsigned char sUART2_Send_Mesg[512] = "Data bus value entered error.\n";
	RW = HIGH;
	if (btMiddleByte_Value > 0xFF)
	{
		UARTD2_SendData(sUART2_Send_Mesg, strlen("Data bus value entered error.\n")*sizeof(char));
		return;
	}
	else
	{
		PORT_ChangeP78Output(0);
		PORT_ChangeP79Output(0);
		PORT_ChangeP710Output(0);
		PORT_ChangeP711Output(0);
		PORT_ChangeP712Output(0);
		PORT_ChangeP713Output(0);
		PORT_ChangeP714Output(0);
		PORT_ChangeP715Output(0);
		
		// LSB : bit8 : P78
		DB08 = (btMiddleByte_Value & 0x01);
		
		DB09 = (btMiddleByte_Value & 0x02) >> 1;
		DB10 = (btMiddleByte_Value & 0x04) >> 2;
		DB11 = (btMiddleByte_Value & 0x08) >> 3;
		DB12 = (btMiddleByte_Value & 0x10) >> 4;
		DB13 = (btMiddleByte_Value & 0x20) >> 5;
		DB14 = (btMiddleByte_Value & 0x40) >> 6;
		
		// MSB : bit15 : P715
		DB15 = (btMiddleByte_Value & 0x80) >> 7;
	}
	return;
}

void Set_Data_Bus_bit23_16(BYTE btHighByte_Value)
/*
 * DataBus[23..16]
 * ------+------+------+------+-------+-------+-------+-------+
 *   23  |  22  |  21  |  20  |  19   |  18   |  17   |  16   |
 * ------+------+------+------+-------+-------+-------+-------+
 *  PDL6 | PDL7 | PDL8 | PDL9 | PDL10 | PDL11 | PDL12 | PDL13 |
 * ------+------+------+------+-------+-------+-------+-------+
 */
{
	unsigned char sUART2_Send_Mesg[512] = "Data bus value entered error.\n";
	RW = HIGH;
	if (btHighByte_Value > 0xFF)
	{
		UARTD2_SendData(sUART2_Send_Mesg, strlen("Data bus value entered error.\n")*sizeof(char));
		return;
	}
	else
	{
		/*
		 * PDL
		 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
		 *   15  |  14  |  13   |  12   |  11   |  10   |  9   |  8   |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
		 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
		 *       |                                                    |                    |      |      |      |      |      |
		 *       |      +-----------------------------------------------------------+      |      |      |      |      |      |
		 *       |      | PDL13 | PDL12 | PDL11 | PDL10 | PDL9 | PDL8 | PDL7 | PDL6 | PDL5 | PDL4 | PDL3 | PDL2 | PDL1 | PDL0 |
		 *       |      +-----------------------------------------------------------+      |      |      |      |      |      |
		 *       |                                                    |                    |      |      |      |      |      |
		 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
		 *                            PDLH                            |                            PDLL                       |
		 * ------+------+-------+-------+-------+-------+------+------+------+------+------+------+------+------+------+------+
		 */
		 
		PORT_ChangePDL13Output(0);
		PORT_ChangePDL12Output(0);
		PORT_ChangePDL11Output(0);
		PORT_ChangePDL10Output(0);
		PORT_ChangePDL9Output(0);
		PORT_ChangePDL8Output(0);
		PORT_ChangePDL7Output(0);
		PORT_ChangePDL6Output(0);
		 
		// LSB : bit16 : PDL13
		DB16 = (btHighByte_Value & 0x01);		// PDL13
		
		DB17 = (btHighByte_Value & 0x02) >> 1;	// PDL12
		DB18 = (btHighByte_Value & 0x04) >> 2;	// PDL11
		DB19 = (btHighByte_Value & 0x08) >> 3;	// PDL10
		DB20 = (btHighByte_Value & 0x10) >> 4;	// PDL9
		DB21 = (btHighByte_Value & 0x20) >> 5;	// PDL8
		DB22 = (btHighByte_Value & 0x40) >> 6;	// PDL7
		
		// MSB : bit23 : PDL6
		DB23 = (btHighByte_Value & 0x80) >> 7;	// PDL6
	}
	return;
}



DWORD Read_Digital_In_Bus_Value()
{
	DWORD dwDigitalInValue = 0x00000000;
	
	g_dwDigitalIN_Bus_bit23_0 = (((DWORD)(PDL & 0x003F)) << 18) + 
				    (((DWORD)(((PCT.6 & 0x01) << 1) + (PCT.4 & 0x01))) << 16) + 
				    (((DWORD)(P9L & 0x07)) << 13) + 
				    ((DWORD)(((P5.1 & 0x01) << 1) + (P5.0 & 0x01)) << 11) + 
				    ((DWORD)((P3L & 0xE0)) << 8) + 
				    (((DWORD)(((P0.6 & 0x01) << 5) + (P0 & 0x1F))) << 2) +
				    ((DWORD)((P1 & 0x03)));
				    
	dwDigitalInValue = g_dwDigitalIN_Bus_bit23_0;
	
	return dwDigitalInValue;
}


void Set_DataBus_to_Initial_Default_After_Power_ON()
{
	/*
	 * Considering the connection (ON) / disconnection (OFF) state transition, all switches and
	 * relais should be disconnected (OFF), in case of emergency, after Power ON.
	 * So, we should set the data bus[23..0] to default : 0xFF, HIGH level enforces all switches
	 * and relaise disconnected.
	 *
	 * Remarked by XU ZAN @2012-07-27
	 */
	 
	RW = HIGH;
	
	/*
	 * DB[7..0]
	 */
	P7L = 0xFF;
	/*
	 * DB[15..8]
	 */
	P7H = 0xFF;
	/*
	 * DB[23..16]
	 */
	PDLH.5 = HIGH;
	PDLH.4 = HIGH;
	PDLH.3 = HIGH;
	PDLH.2 = HIGH;
	PDLH.1 = HIGH;
	PDLH.0 = HIGH;
	PDLL.7 = HIGH;
	PDLL.6 = HIGH;	
}

void Set_AddressBus_to_Initial_Default_After_Power_ON()
{
	/*
	 * Set Address Bus[7..0] to default : 0xFF
	 * meanwhile set control pin CS0 to HIGH. (CS0 HIGH means disable.)
	 */
	P9H.2 = HIGH;	// P910
	P9H.3 = HIGH;	// P911
	P9H.4 = HIGH;	// P912
	P9H.5 = HIGH;	// P913
	PCS.0 = HIGH;	// PCS0
	PCS.1 = HIGH;	// PCS1
	PCM.0 = HIGH;	// PCM0
	PCM.1 = HIGH;	// PCM1
	
	P9H.1 = HIGH;	// CS0 pin = P99
}

void Set_ControlBus_to_Initial_Default_After_Power_ON()
{
	P9H.0 = HIGH;	// CS1 pin = P98
	P9L.7 = HIGH;	// CS2 pin = P97
	P9L.6 = HIGH;	// CS3 pin = P96
	P9L.5 = HIGH;	// CS4 pin = P95
	P9L.4 = HIGH;	// CS5 pin = P94
	
	RW = HIGH;
	ENAD = LOW;
	CON = LOW;
	BUSY = LOW;
	ENDA = HIGH;
	
	NSLP = HIGH;
	ERR = HIGH;
	EN = LOW;
	STB = HIGH;
}

void Set_Initial_Default_After_Power_ON()
{
	Set_DataBus_to_Initial_Default_After_Power_ON();
	Set_AddressBus_to_Initial_Default_After_Power_ON();
	Set_ControlBus_to_Initial_Default_After_Power_ON();
}


void Control_Port_Mutx_From_Switch_to_ADC(E_CTRL_PIN_LEVEL eCtrl_Pin_CS0)
{
	if (eCtrl_Pin_CS0 == CONTROL_PIN_LOW)
	/* 
	 * CS0 = LOW is active.
	 */
	{
		ENAD = LOW;	// ENAD = LOW is disable
		ENDA = HIGH;	// ENDA = HIGH is disable
	}
	return;
}

void Control_Port_Mutx_From_ADC_to_Switch(E_CTRL_PIN_LEVEL eCtrl_Pin_ENAD)
{
	if ( (eCtrl_Pin_ENAD == CONTROL_PIN_HIGH) ||
	     (ENDA == CONTROL_PIN_LOW) )
	{
		CS0 = HIGH;	// disable
	}
	return;
}

void Control_Port_Mutx_From_ADC_to_DAC(E_CTRL_PIN_LEVEL eCtrl_Pin_ENAD)
{
	if (eCtrl_Pin_ENAD == CONTROL_PIN_HIGH)
	{
		ENDA = HIGH;	// disable
	}
	return;
}

void Control_Port_Mutx_From_DAC_to_ADC(E_CTRL_PIN_LEVEL eCtrl_Pin_ENDA)
{
	if (eCtrl_Pin_ENDA == CONTROL_PIN_LOW)
	{
		ENAD = LOW;
	}
	return;
}


void Write_DataBus_Single_CHn(     DWORD  dwCHn,  /* 0 <= CHn <= 23   must */
			      enum LEVEL  eSpecificLevelValue)
{
	RW = HIGH;	// firstly must enable sending direction.
	switch (dwCHn)
	{
	case 0:
		DB00 = eSpecificLevelValue;
		break;
	case 1:
		DB01 = eSpecificLevelValue;
		break;
	case 2:
		DB02 = eSpecificLevelValue;
		break;
	case 3:
		DB03 = eSpecificLevelValue;
		break;
	case 4:
		DB04 = eSpecificLevelValue;
		break;
	case 5:
		DB05 = eSpecificLevelValue;
		break;
	case 6:
		DB06 = eSpecificLevelValue;
		break;
	case 7:
		DB07 = eSpecificLevelValue;
		break;
	case 8:
		DB08 = eSpecificLevelValue;
		break;
	case 9:
		DB09 = eSpecificLevelValue;
		break;
	case 10:
		DB10 = eSpecificLevelValue;
		break;
	case 11:
		DB11 = eSpecificLevelValue;
		break;
	case 12:
		DB12 = eSpecificLevelValue;
		break;
	case 13:
		DB13 = eSpecificLevelValue;
		break;
	case 14:
		DB14 = eSpecificLevelValue;
		break;
	case 15:
		DB15 = eSpecificLevelValue;
		break;
	case 16:
		DB16 = eSpecificLevelValue;
		break;
	case 17:
		DB17 = eSpecificLevelValue;
		break;
	case 18:
		DB18 = eSpecificLevelValue;
		break;
	case 19:
		DB19 = eSpecificLevelValue;
		break;
	case 20:
		DB20 = eSpecificLevelValue;
		break;
	case 21:
		DB21 = eSpecificLevelValue;
		break;
	case 22:
		DB22 = eSpecificLevelValue;
		break;
	case 23:
		DB23 = eSpecificLevelValue;
		break;
	default :
		break;
	}
	return;
}

