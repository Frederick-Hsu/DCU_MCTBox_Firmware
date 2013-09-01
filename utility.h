/*************************************************************************************
 * Program name	: utility.h
 * Description	: Defines some utility functions.
 * Author	: XU ZAN
 * Date		: Tue.	Aug. 7, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 *************************************************************************************/
 
#ifndef _UTILITY_H
#define _UTILITY_H

	/*===============================================================*/
	// Includes : 
	#include <string.h>
	#include <ctype.h>
	#include <stdlib.h>
	
	
	/*===============================================================*/
	// Macros : 
	#define EXPERIMENT_DEBUG_PURPOSE
	
	
	
	/*===============================================================*/
	// Global variables :
	
	/*===============================================================*/
	// Types :
	typedef struct Attribute
	{
		char *sAttributeName;
		char *sAttributeValue;
	}
	Attribute, *PAttribute;

	typedef struct AttributeList
	{
		struct Attribute Attr;
		struct AttributeList *pNextNode;
	}
	AttributeList, *PAttributeList;
	
	/*===============================================================*/
	// Function prototypes :
	long power(int iBase, int iPow);
	

	void ToUpperString(char *String);

	void ToLowerString(char *String);

	int Convert_Str_To_Int(char *sDigitString, long *lDestVal);

	int Convert_Hex_Char_To_Int(char cHexChar);

	int Fetch_SegmentAttributesGroup_From_SeparatorStr(char 		StrWithSeparator[], 
							   const char 		cSeparator[], 
							   struct Attribute 	stAttrGroup[]);

	struct Attribute Parsing_Attribute_Segment(char sAttributeSegment[]);

#if defined (EXPERIMENT_DEBUG_PURPOSE)
	void Experiment_FetchSegmentAttributeGroup(char 		StrWithSeparator[], 
						   const char 		cSeparator[], 
						   PAttributeList 	pAttrList);

	void ReleaseAttributeList(PAttributeList pList);
#endif



#endif	/*    _UTILITY_H    */

