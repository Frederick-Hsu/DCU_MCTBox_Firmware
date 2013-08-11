#include "Testing_Instances.h"
#include "PWM_Out.h"
#include <stdio.h>


	void Testing_Macro_Global_Effectivity(void)
	{
		Set_Config_PWM_Out_Param(1);
	}

	void Testing_Macro_Effectivity_In_Cfile(void)
	{
		printf("Testing whether the macro is effective, while macro is defined in C file.\n");
		return;
	}

