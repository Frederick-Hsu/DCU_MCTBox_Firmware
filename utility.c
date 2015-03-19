/*************************************************************************************
 * Program name	: utility.c
 * Description	: Implements some utility functions.
 * Author	: XU ZAN
 * Date		: Tue.	Aug. 7, 2012
 * Copyright(C)		2010 --- 2012	Hella (Shanghai) Electronics Co., Ltd.
 * All rights reserved.
 *
 *************************************************************************************/


#include "utility.h"
#include <string.h>
#include <stdio.h>


long power(int iBase, int iPow)
{
	int i = 0;
	
	long lProduct = 1;
	
	for (i = 1; i <= iPow; i++)
	{
		lProduct = lProduct * iBase;
	}
	
	return lProduct;
}

void ToUpperString(char *String)
{
	size_t i = 0;
	char cTempChar = '0';
	size_t iLenOfString = strlen(String);

	for (i=0; i<iLenOfString; i++)
	{
		cTempChar = toupper(String[i]);
		String[i] = cTempChar;
	}
	return;
}

void ToLowerString(char *String)
{
	int i = 0;
	char cTempChar = '0';
	int iLenOfString = strlen(String);

	for (i=0; i<iLenOfString; i++)
	{
		cTempChar = tolower(String[i]);
		String[i] = cTempChar;
	}
	return;
}

int Convert_Str_To_Int(char *sHexStr, long *lDestVal)
/************************************************************************
 * Function name: Convert_Str_To_Int
 *
 * Purpose	: Convert the hex format or decimal format string into
 *				  integer value.
 *
 * IN argument	: sDigitString
 * OUT argument	: lDestVal
 * Returns	: error number
 *
 * Caution	: This function can only support the hex and decimal format
 *		: IN argument, other formats (e.g. bin or octal) are not 
 *		: supported. 
 *		: and what's more important, the hex format argument must
 *		: be signed explicitly. for example : 0x5D, 69H, 0XFE, 80h,
 *		: otherwise, it could be treated as a decimal format or 
 *		: directly an error.
 ************************************************************************/
{
	int iError = 0;

	long lResult = 0x000000000L;
	int iLen = 0, i = 0;
	char sTargetHexStr[16] = {0};
	
	if (strstr(sHexStr, "0x") || strstr(sHexStr, "0X"))		// For the hex format like : 0x5D
	{
		if ( (strstr(sHexStr, "H") == NULL) && (strstr(sHexStr, "h") == NULL) )
		{
			strcpy(sTargetHexStr, sHexStr+2);	// Should ignore the "0x" hex sign.
			iLen = strlen(sTargetHexStr);	
			for (i=iLen; i>0; i--)
			{
				if ( ((sTargetHexStr[i-1]>='0') && (sTargetHexStr[i-1]<='9')) || 
				     ((sTargetHexStr[i-1]>='A') && (sTargetHexStr[i-1]<='F')) ||
				     ((sTargetHexStr[i-1]>='a') && (sTargetHexStr[i-1]<='f')) )		// To check each character 
				{
					lResult += ((Convert_Hex_Char_To_Int(sTargetHexStr[i-1]))<<((iLen-i)<<2));
				}
				else
				{
					return -2;
				}
			}
		}
		else
		{	// To avoid the redundant hex format like : 0x5DH
			return -1;
		}
	}
	else if ( strstr(sHexStr, "H") || strstr(sHexStr, "h") )	// For the hex format like : 5DH
	{
		if ((strstr(sHexStr, "0x")==NULL) && (strstr(sHexStr, "0X")==NULL))
		{
			strncpy(sTargetHexStr, sHexStr, strlen(sHexStr)-1);	// Ignore the 'H' hex sign.
			iLen = strlen(sTargetHexStr);
			for (i=iLen; i>0; i--)
			{
				if ( ((sHexStr[i-1]>='0') && (sHexStr[i-1]<='9')) ||
					 ((sHexStr[i-1]>='A') && (sHexStr[i-1]<='F')) ||
					 ((sHexStr[i-1]>='a') && (sHexStr[i-1]<='f')) )
				{
					lResult += ((Convert_Hex_Char_To_Int(sTargetHexStr[i-1]))<<((iLen-i)<<2));
				}
				else
				{
					return -2;
				}
			}
		}
		else
		{
			return -1;
		}
	}
	else	// To process the decimal string
	{
		lResult = atoi(sHexStr);
	}
	*lDestVal = lResult;
	
	return iError;
}

int Convert_Hex_Char_To_Int(char cHexChar)
{
	unsigned int iConvertedValue = 0;
	switch (cHexChar)
	{
	case 'A' :
	case 'a' : iConvertedValue = 10; break;
	case 'B' :
	case 'b' : iConvertedValue = 11; break;
	case 'C' :
	case 'c' : iConvertedValue = 12; break;
	case 'D' :
	case 'd' : iConvertedValue = 13; break;
	case 'E' :
	case 'e' : iConvertedValue = 14; break;
	case 'F' :
	case 'f' : iConvertedValue = 15; break;
	case '0' : iConvertedValue =  0; break;
	case '1' : iConvertedValue =  1; break;
	case '2' : iConvertedValue =  2; break;
	case '3' : iConvertedValue =  3; break;
	case '4' : iConvertedValue =  4; break;
	case '5' : iConvertedValue =  5; break;
	case '6' : iConvertedValue =  6; break;
	case '7' : iConvertedValue =  7; break;
	case '8' : iConvertedValue =  8; break;
	case '9' : iConvertedValue =  9; break;
	default  : iConvertedValue =  0; break;
	}
	return iConvertedValue;
}

int Fetch_SegmentAttributesGroup_From_SeparatorStr(char 				StrWithSeparator[], 
						   						   const char 			cSeparator[], 
												   struct Attribute 	stAttrGroup[])
{
	int iError = 0, iCnt = 0;
	unsigned int uiLen = strlen(StrWithSeparator),
		     uiPosOfSeparator = strcspn(StrWithSeparator, cSeparator);
	char sTemp1AttrSegment[64] = {0}, sTempRestStr[128] = {0}, sTempSwap[128] = {0};

	sprintf(sTempRestStr, "%s", StrWithSeparator);
	do
	{
		uiPosOfSeparator = strcspn(sTempRestStr, cSeparator);
		memset(sTemp1AttrSegment, 0, sizeof(sTemp1AttrSegment));
		strncpy(sTemp1AttrSegment, sTempRestStr, uiPosOfSeparator);

		stAttrGroup[iCnt] = Parsing_Attribute_Segment(sTemp1AttrSegment);
		iCnt++;
		
		strncpy(sTempSwap, sTempRestStr+uiPosOfSeparator+1, strlen(sTempRestStr)-uiPosOfSeparator-1);
		memset(sTempRestStr, 0, sizeof(sTempRestStr));
		strcpy(sTempRestStr, sTempSwap);
		memset(sTempSwap, 0, sizeof(sTempSwap));
	}
	while (strstr(sTempRestStr, cSeparator) != NULL);
	memset(sTemp1AttrSegment, 0, sizeof(sTemp1AttrSegment));
	strcpy(sTemp1AttrSegment, sTempRestStr);
	stAttrGroup[iCnt] = Parsing_Attribute_Segment(sTemp1AttrSegment);

	return iError;
}


struct Attribute Parsing_Attribute_Segment(char sAttributeSegment[])
{
	struct Attribute myAttribute;
	unsigned int uiLen = strlen(sAttributeSegment),
		     uiPosOfSeparator_Space = strcspn(sAttributeSegment, " ");
	char sFrontAttributeName[32] = {0}, sRearAttributeValue[32] = {0};

	if (uiPosOfSeparator_Space == uiLen)	// In this condition, just AttributeName, no AttributeValue
	{
		#if !defined (ATTRIBUTE_DEFINITION_DIFF)
			myAttribute.sAttributeName = (char *)malloc(strlen(sAttributeSegment)*sizeof(char));
			sprintf(myAttribute.sAttributeName, "%s", sAttributeSegment);
			myAttribute.sAttributeValue = (char *)malloc(1*sizeof(char));
			sprintf(myAttribute.sAttributeValue, "%s", "");
		#else
			sprintf(myAttribute.sAttributeName, "%s", sAttributeSegment);
			sprintf(myAttribute.sAttributeValue, "%s", "");
		#endif	/*  ATTRIBUTE_DEFINITION_DIFF  */
	}
	else
	{
		strncpy(sFrontAttributeName, sAttributeSegment, uiPosOfSeparator_Space);
		strncpy(sRearAttributeValue, 
			sAttributeSegment+uiPosOfSeparator_Space+1, 
			strlen(sAttributeSegment)-uiPosOfSeparator_Space-1);
		
		#if !defined (ATTRIBUTE_DEFINITION_DIFF)
			myAttribute.sAttributeName = (char *)malloc(strlen(sFrontAttributeName)*sizeof(char));
			sprintf(myAttribute.sAttributeName, "%s", sFrontAttributeName);
			myAttribute.sAttributeValue = (char *)malloc(strlen(sRearAttributeValue)*sizeof(char));
			sprintf(myAttribute.sAttributeValue, "%s", sRearAttributeValue);
		#else
			sprintf(myAttribute.sAttributeName, "%s", sFrontAttributeName);
			sprintf(myAttribute.sAttributeValue, "%s", sRearAttributeValue);
		#endif	/*  ATTRIBUTE_DEFINITION_DIFF  */
	}
	return myAttribute;
}

#if defined (EXPERIMENT_DEBUG_PURPOSE)
	void Experiment_FetchSegmentAttributeGroup(char 			StrWithSeparator[],
											   const char 		cSeparator[],
											   PAttributeList 	pAttrList)
	{
		unsigned int uiLen = strlen(StrWithSeparator),
			     uiPosOfSeparator = strcspn(StrWithSeparator, cSeparator);
		char sTemp1AttributeSegment[64] = {0}, sTempRestStr[128] = {0}, sTempSwap[128] = {0};
		sprintf(sTempRestStr, "%s", StrWithSeparator);

		// pAttrList = (PAttributeList)malloc(sizeof(AttributeList));
		do
		{
			uiPosOfSeparator = strcspn(sTempRestStr, cSeparator);
			memset(sTemp1AttributeSegment, 0, sizeof(sTemp1AttributeSegment));
			strncpy(sTemp1AttributeSegment, sTempRestStr, uiPosOfSeparator);

			pAttrList->Attr = Parsing_Attribute_Segment(sTemp1AttributeSegment);
			pAttrList->pNextNode = (PAttributeList)malloc(sizeof(AttributeList));
			pAttrList = pAttrList->pNextNode;

			strncpy(sTempSwap, sTempRestStr+uiPosOfSeparator+1, strlen(sTempRestStr)-uiPosOfSeparator-1);
			memset(sTempRestStr, 0, sizeof(sTempRestStr));
			strcpy(sTempRestStr, sTempSwap);
			memset(sTempSwap, 0, sizeof(sTempSwap));
		}
		while (strstr(sTempRestStr, cSeparator) != NULL);
		memset(sTemp1AttributeSegment, 0, sizeof(sTemp1AttributeSegment));
		strcpy(sTemp1AttributeSegment, sTempRestStr);
		pAttrList->Attr = Parsing_Attribute_Segment(sTemp1AttributeSegment);
		pAttrList->pNextNode = NULL;	// Remember to set the last node to be NULL
		return;
	}

	void ReleaseAttributeList(PAttributeList pList)
	{
		void *pTemp = NULL, *pHead = pList;
		do
		{
			pTemp = (PAttributeList)pList->pNextNode;
			// free(pList->Attr.sAttributeName);
			// free(pList->Attr.sAttributeValue);
			free(pList);
			pList = pTemp;
		}
		while (pList != NULL);
		return;
	}
#endif

