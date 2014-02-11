/****************************************************************************************
 * Project		: Testing_Macro_Global_Effectivity
 * Description	: 测试宏的全局有效性问题，为Renesas_MCU_uPD70F3376_Switch_Matrix_Controller
 *				  项目编写测试用例。
 * Creator		: XU ZAN	[HSE-OT]
 * Creation date: Sat.	Aug. 03, 2013
 * Copyright(C)		2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ****************************************************************************************/

#include <stdio.h>
#include "Testing_Instances.h"
#include "PWM_Out_Generating_Option.h"

int main(int argc, char *argv[])
{
	printf("Start :\n");
#if (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION2)
	Testing_Macro_Global_Effectivity();
#elif (PWM_OUT_GENERATE_OPTION == PWM_OUT_GENRATE_OPTION1)
	Testing_Macro_Effectivity_In_Cfile();
#endif

/******************************/
	printf("\n");
	getchar();
	return 0;
}