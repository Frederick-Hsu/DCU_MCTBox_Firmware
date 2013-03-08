/***********************************************************************************************************************
 * Project		: Renesas_MCU_uPD70F3376_Switch_Matrix_Controller
 *				: Firmware simulation testing project.
 *
 * File name	: FW_Simulation_Testing_Main.c
 * Description	: Simulate to test the firmware of Renesas_MCU_uPD70F3376_Switch_Matrix_Controller.
 *				: This file is the portal of firmware simulation testing project.
 * Creator		: XU ZAN [HSE-OT]
 * Creation date: Sat.	March 2, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ***********************************************************************************************************************/

#include <stdio.h>

#include "../protocol/Parse_UART2_Message.h"


/*******************************************************
 * The portal of program : main() function
 *******************************************************/
int main(int argc, char *argv[])
{
	char *sSimulationTestingCmdMesg = // "$ACTIon:SWITch 0x01:12 ON!";
									  // "$ACTIon:ADC 2:VOLT?!";
									  // "$ACTIon:ADC *:VOLT?!";
									  // "$ACTIon:ADC 1:VOLT?;ADC 3:VOLT?;ADC 7:VOLT?;ADC 5:VOLT?;ADC 6:VOLT?;ADC 4:VOLT?!";
									  // "$ACTIon:ADC 1:VOLT?;ADC 3:VOLT?!";
									  // "$ACTIon:ADC 5:CURRent 82.5?!";
									  // "$ACTIon:ADC 7:CURRent 128.35?;ADC 8:CURRent 65.80?;ADC 6:CURRent 4581.2380?!";
                                                                             "$ACTIon:DAC:VOLT -12.5!";
	int iResult = 0;

	iResult = Parse_UART2_Received_Message(sSimulationTestingCmdMesg);

	printf("Current result : %d\n", iResult);


/*******************************/
	return 0;
}

