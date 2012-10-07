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

#pragma ioreg


/******************************************************************************/
// Macros :
#if !defined (ENABLE_OCDM_DEBUG)
	#define ENABLE_OCDM_DEBUG
	// #undef	ENABLE_OCDM_DEBUG
#endif


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
	/* First of all, do system initialization */
	SystemInit();
	
	#if defined (ENABLE_OCDM_DEBUG)
		Enable_OCDM();
	#else
		Disable_OCDM();
	#endif	
	
	UARTD2_Start();		// Open the COM port, communicating with PC.
	/*
	 * Start to print out the basic information.
	 * Added by XU ZAN @2012-07-27
	 */
	PrintOut_Project_Information();
	Display_SW_Version();
	
	P9H.1 = 1;
	PCT.0 = 1;
	P7L.0 = 1;
	PCM.1 = 1;
	/* Start user code. Do not edit comment generated here */
	while (1) 
	{
	#if 0
		P9H.1 = 1;
	
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		P9H.1 = 0;
	
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	#endif
	}
	/* End user code. Do not edit comment generated here */
}


/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


