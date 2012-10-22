/*
*******************************************************************************
**
**  This device driver was created by Applilet2 for V850ES/Fx3
**  32-Bit Single-Chip Microcontrollers
**
**  Copyright(C) 2002, 2012 Renesas Electronics Corporation
**  All rights reserved by Renesas Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  Renesas Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	 main.c
**  Abstract :	This file implements main function.
**  APIlib :	Applilet2 for V850ES/Fx3 V2.43 [30 Mar 2011]
**
**  Device :	uPD70F3376
**
**  Compiler :	CA850
**
**  Creation date:	09/26/2012
**  
*******************************************************************************
*/

/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include "macrodriver.h"
#include "timer.h"
#include "serial.h"
#include "can.h"
#include "port.h"
#include "system.h"
/* Start user code for include definition. Do not edit comment generated here */
/* End user code for include definition. Do not edit comment generated here */
#include "user_define.h"

#include "JTAG_On_Chip_Debug_Mode.h"
#include "Project.h"

#include "bus.h"

#pragma ioreg


/******************************************************************************/
// Macros :
#if !defined (ENABLE_OCDM_DEBUG)
	#define ENABLE_OCDM_DEBUG
	// #undef	ENABLE_OCDM_DEBUG
#endif

#define ADC_DEBUG


/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global definition. Do not edit comment generated here */
/* End user code for global definition. Do not edit comment generated here */



void  main(void)
/*
**-----------------------------------------------------------------------------
**
**	Abstract:
**		This function implements main function.
**
**	Parameters:
**		None
**
**	Returns:
**		None
**
**-----------------------------------------------------------------------------
*/
{
	long i = 0, j = 0;
	int iDINtemp = 5;
	int iAdcVal = 0x00000000;
	
	/* First of all, do system initialization */
	SystemInit();
	
	#if defined (ENABLE_OCDM_DEBUG)
		Enable_OCDM();
	#else
		Disable_OCDM();
	#endif	
	
	UARTD2_Start();		// Open the COM port, communicating with PC.
	#if 0
	/*
	 * Start to print out the basic information.
	 * Added by XU ZAN @2012-07-27
	 */
	for (i=0; i<100000; i++)
	{
		NOP();
	}
	// PrintOut_Project_Information();
	
	for (i=0; i<5; i++)
	{
		Display_SW_Version();
		#if 1
		for (j=0; j<100000; j++)
		{
			NOP();
		}
		#endif
	}
	#endif
	
	
	
	A0 = 1;
	A1 = 0;
	A2 = 0;
	A3 = 0;
	A4 = 0;
	A5 = 0;
	A6 = 0;
	A7 = 0;
	

	RW = 1;
	CS0 = 0;
	
	#if 0
	Set_DataBus_from_DB07_to_DB00(0xDF);
	Set_DataBus_from_DB15_to_DB08(0xDF);
	Set_DataBus_from_DB23_to_DB16(0xE8);
	#endif
	
	Write_DataBus_Single_CHn(10, 1);
	Write_DataBus_Single_CHn(23, 1);
	Write_DataBus_Single_CHn(15, 1);
	Write_DataBus_Single_CHn(15, 0);
	
	#if 0
	DB00 = 0;
	DB01 = 0;
	DB02 = 0;
	DB03 = 0;
	DB04 = 0;
	DB05 = 0;
	DB06 = 0;
	DB07 = 0;
	DB08 = 0;
	DB09 = 0;
	DB10 = 1;
	DB11 = 1;
	PORT_ChangeP712Output(0);
	DB12 = 1;
	DB13 = 1;
	PORT_ChangeP714Output(1);
	DB14 = 0;
	PORT_ChangeP714Output(1);
	DB15 = 1;
	PORT_ChangeP714Output(0);
	DB16 = 1;
	DB17 = 1;
	DB18 = 1;
	DB19 = 1;
	DB20 = 1;
	DB21 = 1;
	DB22 = 1;
	PORT_ChangePDL6Output(1);
	DB23 = 0;	
	#endif
	
	

	
	/* Start user code. Do not edit comment generated here */
	while (1) 
	{
		
		#if !defined (ADC_DEBUG)
			PORT_ChangePCM3Input();
			CON = 1;
			
			A0 = 1;
			A1 = 0;
			A2 = 0;
			A3 = 0;
			A4 = 0;
			A5 = 0;
			A6 = 0;
			A7 = 0;
			
			ENAD = 1;
			
			CON = 0;
			NOP();
			NOP();
			NOP();
			NOP();
			CON = 1;
			
			do
			{
				NOP();
			}
			while (BUSY != 1);
			
			iAdcVal = Read_ADCInput_DataBus_from_DB15_to_DB00();
			
			ENAD = 0;
			
		#endif
		
		
		
		#if 0
		iDINtemp = DIN23;
		if (iDINtemp == 1)
		{
			DB00 = 1;
		}
		else if (iDINtemp == 0)
		{
			DB00 = 0;
		}
		#endif
		
	#if 0
		DOUT00 = 0;
		DOUT01 = 0;
		DOUT02 = 0;
		DOUT03 = 0;
		DOUT04 = 0;
		DOUT05 = 0;
		DOUT06 = 0;
		DOUT07 = 0;
		DOUT08 = 0;
		DOUT09 = 0;
		DOUT10 = 1;
		DOUT11 = 1;
		DOUT12 = 1;
		DOUT13 = 1;
		DOUT14 = 1;
		DOUT15 = 1;
		DOUT16 = 1;
		DOUT17 = 1;
		DOUT18 = 1;
		DOUT19 = 1;
		DOUT20 = 1;
		DOUT21 = 1;
		DOUT22 = 1;
		PORT_ChangePDL6Output(1);
		// DOUT23 = 1;
		
		for (i=0; i<10000000; i++)
		{
			NOP();
		}
		
		DB00 = 1;
		DB01 = 1;
		DB02 = 1;
		DB03 = 1;
		DB04 = 1;
		DB05 = 1;
		DB06 = 1;
		DB07 = 1;
		DB08 = 1;
		DB09 = 1;
		DB10 = 0;
		DB11 = 0;
		DB12 = 0;
		DB13 = 0;
		DB14 = 0;
		DB15 = 0;
		DB16 = 0;
		DB17 = 0;
		DB18 = 0;
		DB19 = 0;
		DB20 = 0;
		DB21 = 0;
		DB22 = 0;
		PORT_ChangePDL6Output(0);
		// DB23 = 0;
		for (i=0; i<10000000; i++)
		{
			NOP();
		}
	#endif
	}
	/* End user code. Do not edit comment generated here */
}


/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


