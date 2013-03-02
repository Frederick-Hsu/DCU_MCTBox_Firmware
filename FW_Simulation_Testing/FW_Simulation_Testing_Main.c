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

#include "..\protocol\Parse_UART2_Message.h"


/*******************************************************
 * The portal of program : main() function
 *******************************************************/
int main(int argc, char *argv[])
{
	char *sSimulationTestingCmdMesg // = "$ACTIon:SWITch 0x01:12 ON!";
									// = "$ACTIon:ADC CH2:VOLT?!";
									// = "$ACTIon:ADC *:VOLT?!";
									// = "$ACTIon:ADC CH1:VOLT?;ADC CH3:VOLT?;ADC CH7:VOLT?!";
									   = "$ACTIon:ADC CH5:CURRent 82.5?!";
									// = "$ACTIon:ADC CH7:CURRent 128.35?;ADC CH8:CURRent 65.80?!";
	int iResult = 0;

	iResult = Parse_UART2_Received_Message(sSimulationTestingCmdMesg);

	printf("Current result : %d\n", iResult);


/*******************************/
	return 0;
}

