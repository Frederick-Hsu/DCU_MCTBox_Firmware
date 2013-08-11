/****************************************************************************************************
 * File name	: Precisely_delay.c
 * Description	: Simulate to implement the precise delay by virtual of the CPU clock tick.
 * Creator	: XU ZAN	[HSE-OT]
 * Creation date: Tue.	June 25, 2013
 * Copyright(C)	2010 --- 2013	Hella Shanghai Electronics Co., Ltd.
 * All rights reserved.
 *
 ****************************************************************************************************/

#include "Precisely_delay.h"

static long long llCPUCycleCnts = 0; 

void Calculate_CPU_Cycle_Counts(long lDelayInMs)
{
	llCPUCycleCnts = (lCPU_CLOCK_InHz * lDelayInMs) / 1000;	
	return;
}

void Start_Precise_Delay()
{
#if 0
	#pragma asm
		st.w r0, $_llCPUCycleCnts
		mov r1, 0
	START : nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		add r1, r1+14
		comp r1, r0
		jmp START
	#pragma endasm
#endif
}
/*
 * END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */

