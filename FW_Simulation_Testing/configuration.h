/***************************************************************************************
 * Project		: FW_Simulation_Testing
 *
 * File name	: configuration.h
 * Description	: Configure some macros and type-definitions in this file, to make the
 *				: simulation-debugging available & possible in Visual Studio environment
 *				: Because these program files are built in Renesas CubeSuite+ MCU
 *				: firmware compiling environment.
 * Creator		: XU ZAN [HSE-OT]
 * Creation date: Sat.	March 2, 2013
 * Copyright(C)		2010 --- 2013	Hella Shanghai Electronics CO., Ltd.
 * All rights reserved.
 *
 ***************************************************************************************/

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

	typedef unsigned int	UINT;
	typedef UINT			DWORD;
	typedef signed int		SINT;

	typedef unsigned short	USHORT;
	typedef signed short	SHORT;
	typedef USHORT			WORD;

	typedef unsigned char	UCHAR;
	typedef UCHAR			BYTE;
	typedef signed char		SCHAR;

	typedef unsigned char	BOOL;
	typedef unsigned short	MD_STATUS;

	#define MD_ON	1
	#define MD_OFF	0

	enum LEVEL
	{
		LOW		= 0,
		HIGH	= 1
	};

#endif	/*  _CONFIGURATION_H  */

