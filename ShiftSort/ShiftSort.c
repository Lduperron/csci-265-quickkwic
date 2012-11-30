/********** ShiftSort module---implementation **********/

#include <stdio.h>
#include <stdlib.h>
#include "kwic.h"
#include "LineStorage.h"
#include "ShiftSort.h"

/***** local constants *****/

/***** local types *****/

typedef struct {
	int lineNum;
	int shiftNum;
} LineList,*LineListPtr;

/***** local variables *****/

static LineListPtr lineList;
static int lineCount;
static int currentSize;
static int currentPower;

/***** state invariant *****

1. lineCount >= 0

2. if lineCount == 0 then
	lineList == NULL

3. if lineCount > 0
	lineList is a pointer to an array of lineCount lineLists's

4. forall i in [0..lineCount-1]
	lineList[i].lineNum in [0..LSNumLines()-1]
	lineList[i].shiftNum in [0..LSNumWords(lineList[i].lineNum)-1]

*/

/***** local functions *****/

/* return word wordNum from the shifted LineStorage line specified by linePtr */
static const char* SSNativeGetWord(LineListPtr linePtr,int wordNum)
{
	int shiftedWordNum;

	shiftedWordNum = (wordNum+linePtr->shiftNum) %
		LSNumWords(linePtr->lineNum);
	return LSGetWord(linePtr->lineNum,shiftedWordNum);
}

/*
* if line0 < line1
*       return a negative value
* else if line0 == line1
*       return 0
* else
*       return a positive value
*/
static int lineCompare(LineListPtr line0,LineListPtr line1)
{
	int i,j,minNumWords,minLine;
	int line0num = LSNumWords(line0->lineNum);
	int line1num = LSNumWords(line1->lineNum);
        
        
	if (line0num < line1num)
		minNumWords = line0num;
	else
		minNumWords = line1num;
	for (i = 0; i < minNumWords; i++) {
		j = strcmp(SSNativeGetWord(line0,i),SSNativeGetWord(line1,i));
		if (j != 0)
			return j;
	}
        /* assert: line0 and line1 are equal for the first min words */
	return line0num - line1num;
}

static int ipow(int base, int exp)
{
	int result = 1;
	while (exp) {
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}
	return result;
}
/*****exported functions*****/


void SSInit(void)
{
	lineList = NULL;
	lineCount = 0;
	currentPower = 0;
	currentSize = 0;
}

void SSReset(void)
{
	if (lineList != NULL)
		free(lineList);
	lineList = NULL;
	lineCount = 0;
	currentPower = 0;
	currentSize = 0;
}

KWStatus SSShiftSort(void)
{
	int i,j,k;

	lineCount = 0;
        /* fill lineList */
	k = 0;
	for (i = 0; i < LSNumLines(); i++) {
		for (j = 0; j < LSNumWords(i); j++) {
               /* exclude lines that start with a noise word */
			if (!WTIsMember(LSGetWord(i,j))) {
				lineCount++;
				if(lineCount >= currentSize) {
					currentPower++;
					lineList = realloc(lineList, ipow(2,currentPower)*sizeof(LineList)); // Two variables with the difference in name being a capital.  why =<
					currentSize = ipow(2,currentPower);
				}                  
				lineList[k].lineNum = i;
				lineList[k].shiftNum = j;
				k++;
			}
		}
	}

        /* sort the shifted lines */
	qsort(lineList,lineCount,sizeof(LineList),
		(int (*)(const void *, const void *))lineCompare);
		return KWSUCCESS;
}

const char* SSGetWord(int lineNum,int wordNum)
{
	if (lineNum < 0 || lineNum >= lineCount || wordNum < 0 ||
		wordNum >= LSNumWords(lineList[lineNum].lineNum))
		return NULL;
	else
		return SSNativeGetWord(&lineList[lineNum],wordNum);
}

int SSNumWords(int lineNum)
{
	if (lineNum < 0 || lineNum >= lineCount)
		return(KWRANGEERROR);
	else
		return(LSNumWords(lineList[lineNum].lineNum));
}

int SSGetShiftNum(int lineNum)
{
	if (lineNum < 0 || lineNum >= lineCount)
		return(KWRANGEERROR);
	else
		return(lineList[lineNum].shiftNum);
}

int SSNumLines(void)
{
	return(lineCount);
}

void SSPrintState(void)
{
	int i;

	for (i = 0; i < lineCount; i++)
		printf("lineList[%d]: lineNum=%d shiftNum=%d\n",
			i,lineList[i].lineNum,lineList[i].shiftNum);
}
