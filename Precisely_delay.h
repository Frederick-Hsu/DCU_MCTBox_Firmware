/****************************************************************************************************
 * File name	: Precisely_delay.h
 * Description	: Simulate to implement the precise delay by virtual of the CPU clock tick. 
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Tue.	June 25, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ****************************************************************************************************/


#ifndef PRECISELY_DELAY_H
#define PRECISELY_DELAY_H

	const long lCPU_CLOCK_InHz = 32000000L;	// Clock = 32MHz
	
	void Calculate_CPU_Cycle_Counts(long lDelayInMs);
	
	void Start_Precise_Delay(void);

#endif	/*  PRECISELY_DELAY_H  */

