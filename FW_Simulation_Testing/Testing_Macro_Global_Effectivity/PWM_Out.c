/********************************************************************************************
 * File name	: PWM_Out.c
 * Description	: PWM module, implements the PWM Out functions.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Fri.	June 07, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 * 
 ********************************************************************************************/

#include <stdio.h>
#include "PWM_Out.h"
#include "PWM_Out_Generating_Option.h"

#if (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION1)
	void Set_Config_PWM_Out_Param(int stPwmParam)
	{
		printf("This is Option 1.\n");
		return;
	}
#elif (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION2)
	void Set_Config_PWM_Out_Param(int stPwmParam)
	{
		printf("This is Option 2.\n");
		return;
	}
#endif