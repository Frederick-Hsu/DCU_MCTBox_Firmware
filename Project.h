/************************************************************************************************************************************
 * Project name		: Switch_Matrix_Controller
 *
 * Program name		: Project.h
 * Description		: Describe this project, her property and purpose.
 *			: Some relative information about this project.
 *
 * Development team	: Software group, Operations Testing department, Hella (Shanghai) Electronics Co., Ltd.
 *			: 
 *			: Development project team :
 *			: 	Team leader : HENRY XING
 *			:	Hardware engineers : HENRY XING,  NEIL WU
 *			:	Software engineers : XU ZAN, SHI GANG, MA LIANG
 *
 * Author		: XU ZAN
 * Creation date	: Thu.	July 19, 2012
 *
 * Copyright(C)		2010 --- 2012		Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 * 
 ************************************************************************************************************************************/
 
#ifndef _PROJECT_H
#define _PROJECT_H

	#define THIS_PROJECT	"Project : Switch Matrix Controller\n"
	#define DEVICE		"MCU : Renesas V850ES/FG3, type : uPD70F3376\n"
	#define PROPERTY_PURPOSE	"It realizes that :\n1. Controlling switch matrix;\n2. ADC measuring and DAC outputing;\n3. CAN bus analyzer and transciver;\n4. Digital IN and OUT.\n"
	#define DEVELOPMENT_TEAM	"Development team : \n\tTeam leader : HENRY XING \n\tHW : HENRY XING, NEIL WU \n\tSW : XU ZAN\n"
	#define COPYRIGHT		"Copyright(C)  2010---2012  Hella (Shanghai) Electronics Co., Ltd.\nAll rights reserved.\n"
	
	
	/*********************************************************************************************************/
	void PrintOut_Project_Information();
	void Display_SW_Version();


#endif	/*    _PROJECT_H    */

