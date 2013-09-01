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

int Convert_Str_To_Int(char *sDigitString, long *lDestVal)
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
	size_t i = 0;
	int iLeapStep = 0;

	long lTempValue = 0;

	char sPrefix_Sign[2] = ""; 
	char sSuffix_Sign = '0';
	char cTempChar[2] = "";

	if ( (strncmp(sDigitString, "0x", 2) == 0) || 
	     (strncmp(sDigitString, "0X", 2) == 0) ||
	     (strncmp(sDigitString+strlen(sDigitString)-1, "h", 1) == 0) ||
	     (strncmp(sDigitString+strlen(sDigitString)-1, "H", 1) == 0)		// Explicit Hex format condition
	   )
	{
		for (i=strlen(sDigitString); i>0; i--)
		{
			strncpy(cTempChar, sDigitString+i-1, 1);
			if ( !((cTempChar[0] == 'H') || (cTempChar[0] == 'h')) )
			{
				if ( (cTempChar[0] == 'X') || (cTempChar[0] == 'x') )	// For this condition, it tells the Hex sign "0x" or "0X" have been found, it should break out.
				{
					break;
				}
				if ( ((cTempChar[0] >= '0') && (cTempChar[0] <= '9')) ||
				     ((cTempChar[0] >= 'A') && (cTempChar[0] <= 'F')) ||
				     ((cTempChar[0] >= 'a') && (cTempChar[0] <= 'f')) )
				{
					// lTempValue += atoi(cTempChar) * power(16, (i-strlen(sDigitString))*(-1));
					lTempValue += Convert_Hex_Char_To_Int(cTempChar[0])* power(16, ((i-strlen(sDigitString))*(-1)-iLeapStep)); 
				}
				else
				{
					return -1;
				}
			}
			else if ((cTempChar[0] == 'H') || (cTempChar[0] == 'h'))
			{
				iLeapStep++;
			}
		}
	}
	else	// Decimal format condition
	{
		for (i=strlen(sDigitString); i>0; i--)
		{
			strncpy(cTempChar, sDigitString+i-1, 1);
			if ( ((cTempChar[0] >= '0') && (cTempChar[0] <= '9')) )
			{
				lTempValue += Convert_Hex_Char_To_Int(cTempChar[0]) * power(10, (i-strlen(sDigitString))*(-1));
			}
			else
			{
				return -1;
			}
		}
	}

	*lDestVal = lTempValue;
	return iError;
}

int Convert_Hex_Char_To_Int(char cHexChar)
{
	unsigned int iConvertedValue = 0;
	switch (cHexChar)
	{
	case 'A' :
	case 'a' :
		iConvertedValue = 10;
		break;
	case 'B' :
	case 'b' :
		iConvertedValue = 11;
		break;
	case 'C' :
	case 'c' :
		iConvertedValue = 12;
		break;
	case 'D' :
	case 'd' :
		iConvertedValue = 13;
		break;
	case 'E' :
	case 'e' :
		iConvertedValue = 14;
		break;
	case 'F' :
	case 'f' :
		iConvertedValue = 15;
		break;
	case '0' :
	case '1' :
	case '2' :
	case '3' :
	case '4' :
	case '5' :
	case '6' :
	case '7' :
	case '8' :
	case '9' :
		iConvertedValue = atoi(&cHexChar);
		break;
	default :
		iConvertedValue = 0;
		break;
	}
	return iConvertedValue;
}

int Fetch_SegmentAttributesGroup_From_SeparatorStr(char 		StrWithSeparator[], 
						   const char 		cSeparator[], 
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
		myAttribute.sAttributeName = (char *)malloc(strlen(sAttributeSegment)*sizeof(char));
		sprintf(myAttribute.sAttributeName, "%s", sAttributeSegment);
		myAttribute.sAttributeValue = (char *)malloc(1*sizeof(char));
		sprintf(myAttribute.sAttributeValue, "%s", "");
	}
	else
	{
		strncpy(sFrontAttributeName, sAttributeSegment, uiPosOfSeparator_Space);
		strncpy(sRearAttributeValue, 
			sAttributeSegment+uiPosOfSeparator_Space+1, 
			strlen(sAttributeSegment)-uiPosOfSeparator_Space-1);
		
		myAttribute.sAttributeName = (char *)malloc(strlen(sFrontAttributeName)*sizeof(char));
		sprintf(myAttribute.sAttributeName, "%s", sFrontAttributeName);
		myAttribute.sAttributeValue = (char *)malloc(strlen(sRearAttributeValue)*sizeof(char));
		sprintf(myAttribute.sAttributeValue, "%s", sRearAttributeValue);
	}
	return myAttribute;
}

#if defined (EXPERIMENT_DEBUG_PURPOSE)
	void Experiment_FetchSegmentAttributeGroup(char 		StrWithSeparator[], 
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
			free(pList);
			pList = pTemp;
		}
		while (pList != NULL);
		return;
	}
#endif

