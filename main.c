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
#include <stdio.h>
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
#include "ADC_and_DAC.h"


/*========================================================*/
#include "Just_test_and_debug.h"
/*========================================================*/

#pragma ioreg



/******************************************************************************/
// Macros :
#if !defined (ENABLE_OCDM_DEBUG)
	#define ENABLE_OCDM_DEBUG
	// #undef	ENABLE_OCDM_DEBUG
#endif

#define SIZEOF_HEAP 0x1000

#define ADC_DEBUG


/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global definition. Do not edit comment generated here */


/******************************************************************************/
// Global variables :
int __sysheap[SIZEOF_HEAP>>2];
size_t __sizeof_sysheap = SIZEOF_HEAP;
int *pgHeapMemoryAddr;

extern unsigned long _S_romp;

/*
 * Define a global variable for storing the COM received buffer : gRxBuf
 * and limit the max length of command : 256 chars
 *
 * Remarked by XUZAN@2013-02-21
 */
UCHAR gRxBuf[512] = {0};	// Initialize to be NULL for all array elements


/******************************************************************************/
// Global functions prototype :
int _rcopy(unsigned long *, long);

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
	
	/*
	 * Heap memory initialization at the beginnig of program.
	 * Added by XUZAN@2013-02-08
	 */
	int ret = 0;
	ret = _rcopy(&_S_romp, -1);
	pgHeapMemoryAddr = __sysheap;
	
	#if 0
	for (i=0; i<50000; i++)
	{
		NOP();
	}
	#endif
	/* First of all, do system initialization */
	SystemInit();
	
	
	#if !defined (ENABLE_OCDM_DEBUG)
		Enable_OCDM();
	#else
		Disable_OCDM();
	#endif	
	
	#if defined (TEST_DEBUG_PURPOSE)
		SetInitialState_TurnOffAllSwitches();
		
		SetInitialState_TurnOffAllSwitches();
	#endif	/* End   TEST_DEBUG_PURPOSE    */
	
	UARTD2_Start();		// Open the COM port, communicating with PC.
	/*
	 * Start to print out the basic information.
	 * Added by XU ZAN @2012-07-27
	 */
	for (i=0; i<5000000; i++)
	{
		NOP();
	}
	Display_SW_Version();

	
	// Test_Switch_Ctrl();
	// Enable_All_Switches();
	
	/*
	 * Register the UART receiving data action,
	 * preliminarily get ready to receive COM command.
	 */
	UARTD2_ReceiveData(gRxBuf, sizeof(gRxBuf));

	
	/* Start user code. Do not edit comment generated here */
	while (1) 
	{
		#if defined (TEST_DEBUG_PURPOSE)
			// Test_MCU_Chip_System();
			// Print_ADC_Values_8Channels();
		#endif	/* End   TEST_DEBUG_PURPOSE    */
		
		#if 0
		for (i=0; i<100; i++)
		{
			NOP();
		}
		#endif
		
	}
	/* End user code. Do not edit comment generated here */
}


/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */


