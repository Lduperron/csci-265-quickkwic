/********** LineStorage module---implementation **********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kwic.h"
#include "LineStorage.h"

/***** local constants *****/

/***** local types *****/

/*Each line is a list of WordNodes.*/
typedef struct WordNode {
	char* word;
	struct WordNode* nextWordPtr;
} WordNode;
typedef WordNode* WordNodePtr;

/*The LineStorage module stores a list of LineNodes*/
typedef struct LineNode {
	WordNodePtr headWordPtr;
	WordNodePtr tailWordPtr;
	int wordCount;
} LineNode;
typedef LineNode* LineNodePtr;

/***** local variables *****/

static LineNodePtr tailLinePtr;
static int lineCount;
static LineNodePtr* lineNodeArray = NULL;
static int lineNodeArraySize = 0;
static int lineNodeArrayCapacity = 1;
static int currentPower = 0;

/***** state invariant *****

1. if lineCount == 0 then
	taillinePtr == NULL
   else
	tailLinePtr points to the last LineNode in the array of LineNodes.
	There are lineCount LineNodes in this list.

2. for every LineNode allocated by LineStorage
	if wordCount == 0 then
	    headWordPtr == NULL
	    tailWordPtr == NULL
	else
	    headWordPtr points to a null-terminated linked list of WordNodes.
	    tailWordPtr points to the last WordNode in this list.
	    There are wordCount WordNodes in this list.

3. For every WordNode allocated word is a null-terminated array of characters.

*/

/***** local functions *****/

static int ipow(int base, int exp)
{
	int result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}



 LineNodePtr LSNativeGetLine(int i)
{
	LineNodePtr tmpLinePtr;

	if (i < 0)
	{
		return NULL;
	}
   

	if(i >= lineNodeArraySize)
	{
		return NULL;
	}
   
   
	return lineNodeArray[i];
   
	//return tmpLinePtr;
}

/*
* if wordNodePtr != NULL && the list headed by wordNodePtr has >= i words then
*	return a pointer to the ith word in the list headed by wordNodePtr
* else
*	return NULL
* Assumed: wordNodePtr is either NULL or a pointer to a list of WordNodes
*/
WordNodePtr getWord(WordNodePtr wordNodePtr,int i)
{
	if (i < 0)
		return NULL;
	while (i-- > 0 && wordNodePtr != NULL) {
		wordNodePtr = wordNodePtr->nextWordPtr;
	}
	return wordNodePtr;
}

/***** exported functions *****/

void LSInit(void)
{
	tailLinePtr = NULL;
	lineCount = 0;
	lineNodeArray = (LineNodePtr*) malloc(sizeof(LineNodePtr));
}

void LSReset(void)
{
	LineNodePtr tmpLinePtr;
	WordNodePtr tmpWordPtr0,tmpWordPtr1;
	int i;
	for(i = 0; i < lineNodeArraySize; i++) {
		tmpWordPtr0 = lineNodeArray[i]->headWordPtr;
		while (tmpWordPtr0 != NULL) {
			tmpWordPtr1 = tmpWordPtr0->nextWordPtr;
			free(tmpWordPtr0->word);
			free(tmpWordPtr0);
			tmpWordPtr0 = tmpWordPtr1;
		}
      
		free(lineNodeArray[i]);
	}
   
	free(lineNodeArray); 
	lineNodeArray = (LineNodePtr*) malloc(sizeof(LineNodePtr));
	lineNodeArraySize = 0;
	lineNodeArrayCapacity = 1;
	currentPower = 0;
	lineCount = 0;
	tailLinePtr = NULL;
}

KWStatus LSAddLine(void)
{
	LineNodePtr newLinePtr;

	/* create and fill a LineNode */
	newLinePtr = malloc(sizeof(LineNode));
	if (newLinePtr == NULL)
		return KWMEMORYERROR;
	lineCount++;
	newLinePtr->headWordPtr = NULL;
	newLinePtr->tailWordPtr = NULL;
	newLinePtr->wordCount = 0;

	if(lineNodeArraySize < lineNodeArrayCapacity) {
		lineNodeArray[lineNodeArraySize] = newLinePtr;
		lineNodeArraySize++;
	}
	else
	{
		currentPower++;
		lineNodeArray = (LineNodePtr*) realloc (lineNodeArray , ipow(2,currentPower)*sizeof(LineNodePtr));
		lineNodeArrayCapacity = ipow(2,currentPower);
		lineNodeArray[lineNodeArraySize] = newLinePtr;
		lineNodeArraySize++;
	}
   
   
	tailLinePtr = lineNodeArray[lineNodeArraySize-1];
   
	return KWSUCCESS;
}

KWStatus LSAddWord(char* word)
{
	WordNodePtr newWordPtr;

	if (tailLinePtr == NULL)
		return KWRANGEERROR;

	/* create and fill a WordNode */
	newWordPtr = malloc(sizeof(WordNode));
	if (newWordPtr == NULL)
		return KWMEMORYERROR;
	newWordPtr->word = malloc(strlen(word)+1);
	if (newWordPtr->word == NULL) {
		free(newWordPtr);
		return KWMEMORYERROR;
	}
	tailLinePtr->wordCount++;
	strcpy(newWordPtr->word,word);
	newWordPtr->nextWordPtr = NULL;

	/* link in the new WordNode */
	if (tailLinePtr->tailWordPtr == NULL) { /* empty line */
		tailLinePtr->headWordPtr = newWordPtr;
	} else {
		tailLinePtr->tailWordPtr->nextWordPtr = newWordPtr;
	}
	tailLinePtr->tailWordPtr = newWordPtr;
	return KWSUCCESS;
}

const char* LSGetWord(int lineNum,int wordNum)
{
	LineNodePtr tmpLinePtr;
	WordNodePtr tmpWordPtr;

	if (lineNum >= lineCount)
		return NULL;

	/* find line LineNum */
	tmpLinePtr = LSNativeGetLine(lineNum);
	if (tmpLinePtr == NULL)
		return NULL;

	/* find word wordNum */
	tmpWordPtr = getWord(tmpLinePtr->headWordPtr,wordNum);
	if (tmpWordPtr == NULL)
		return NULL;
	return tmpWordPtr->word;
}

int LSNumWords(int lineNum)
{
	LineNodePtr tmpLinePtr;

	/* find line lineNum */
	tmpLinePtr = LSNativeGetLine(lineNum);

	if (tmpLinePtr == NULL) {
		return KWRANGEERROR;
	}
   
	/* count the words in line lineNum */
	
   return tmpLinePtr->wordCount;
   //return 4;
}

int LSNumLines(void)
{
	return lineCount;
}

void LSPrintState(void)
{
	int i = 0;
	LineNodePtr tmpLinePtr;
	WordNodePtr tmpWordPtr;

   
	printf("lineCount:%d\n",lineCount);
	for (i = 0; i < lineNodeArraySize; i++) {
		tmpLinePtr = lineNodeArray[i];
		printf("\twordCount:%d\n\t",tmpLinePtr->wordCount);
		for (tmpWordPtr = tmpLinePtr->headWordPtr; tmpWordPtr != NULL; tmpWordPtr = tmpWordPtr->nextWordPtr) {
			printf("!%s",tmpWordPtr->word);
		}
		printf("!\n");
	}
}
