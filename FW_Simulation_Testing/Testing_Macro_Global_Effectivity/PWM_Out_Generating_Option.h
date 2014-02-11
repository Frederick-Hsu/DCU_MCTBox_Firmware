/********************************************************************************************
 * File name	: PWM_Out_Generating_Option.h
 * Description	: The choice / option on how to generate the PWM_Out signal.
 * Creator		: XU ZAN	[Hse-OT]
 * Creation date: Sat.	Aug. 03, 2013
 * Copyright(C)		2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ********************************************************************************************/

#ifndef PWM_OUT_GENERATING_OPTION_H
#define PWM_OUT_GENERATING_OPTION_H

	/* The choice/option on generating the PWM_Out signal :
	 * Option 1: Through 2 timers alternative High/Low switching ---> to simulate the PWM_H/L
	 * Option 2: Just use 1 timer, Control the transformation between PWM_H time and PWM_L time 
	 */
	#define PWM_OPTION_2TIMERS_ALTERNATE_SWTCHING		1
	#define PWM_OPTION_1TIMER_HIGH_LOW_TRANSFORMING		2
	#define PWM_OUT_GENRATE_OPTION1		PWM_OPTION_2TIMERS_ALTERNATE_SWTCHING
	#define PWM_OUT_GENRATE_OPTION2		PWM_OPTION_1TIMER_HING_LOW_TRANSFORMING
	
	#define PWM_OUT_GENERATE_OPTION		PWM_OUT_GENRATE_OPTION2
	// #define PWM_OUT_GENERATE_OPTION		PWM_OPTION_1TIMER_HIGH_LOW_TRANSFORMING

#endif	/*  PWM_OUT_GENERATING_OPTION_H  */

