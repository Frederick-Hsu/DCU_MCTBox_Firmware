/***********************************************************************************************************************
 * Project	: Renesas_MCU_uPD70F3376_Switch_Matrix_Controller
 *		: Firmware simulation testing project.
 *
 * File name	: FW_Simulation_Testing_Main.c
 * Description	: Simulate to test the firmware of Renesas_MCU_uPD70F3376_Switch_Matrix_Controller.
 *		: This file is the portal of firmware simulation testing project.
 * Creator	: XU ZAN [HSE-OT]
 * Creation date: Sat.	March 2, 2013
 * Copyright(C)         2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
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
                                          // "$ACTIon:DAC:VOLT -12.5!";
                                          // "$ACTIon:DIN 2:STATe?!";
                                          // "$ACTIon:DIN 5:STATe?;DIN 8:STATe?;DIN 7:STATe?;DIN 24:STATe?!";
                                          // "$ACTIon:DOUT 0x05:16 HIGH!";
                                          // "$ACTIon:DOUT 0x08:21 LOW;DOUT 0x03:19 LOW;DOUT 0x10:04 HIGH!";

                                          /*==========================================================================================*/

                                          // "$SWITch 0x01:12 ON!";
                                          // "$DOUT 0x01:21 HIGH;DOUT 0x01:19 HIGH;DOUT 0x01:04 HIGH;DOUT 0x01:07 HIGH;DOUT 0x01:14 HIGH;DOUT 0x01:21 HIGH!";
                                          // "$DOUT 0x01:XXXXXXX1X0X10XX111X000X1!";
										  "$PWMO CONF:OUT1:FREQ 10:DUTY 40%:VOLT 5!";
										  // "$PWMO START:OUT1:PRIM:CHNL 1!";
										  // "$PWMO START:OUT2:SECN:BDID 0x01:CHNL 21!";
										  // "$PWMO STOP:OUT1!";
                                          // "$ADC 2:VOLT?!";
                                          // "$SYSTem:ERR?!";
        int iResult = 0;

	iResult = Parse_UART2_Received_Message(sSimulationTestingCmdMesg);

	printf("Current result : %d\n", iResult);


/*******************************/
	return 0;
}

