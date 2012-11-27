/********** Output module---implementation **********/

#include <stdio.h>
#include <string.h>
#include "kwic.h"
#include "Output.h"
#include "ShiftSort.h"

/***** local constants *****/

/***** local types *****/

/***** exported functions *****/

void OUInit(void)
{
	/* intentionally empty */
}

void OUPrint(void)
{
	int i,j, h;
   char temporigstring[80];
   char tempnewstring[80];

	for (i = 0; i < SSNumLines(); i++) 
	{
		int numWords = SSNumWords(i);
		strcpy(temporigstring, "");
		strcpy(tempnewstring, "");
  
		for(h = 0; h < (numWords-SSGetShiftNum(i)); h++)
		{
			strcat(temporigstring, SSGetWord(i,h));
			strcat(temporigstring, " ");
		}
	 
		for(h = (numWords-SSGetShiftNum(i)); h < (numWords); h++)
		{
			strcat(tempnewstring, SSGetWord(i,h));
			if (h != SSNumWords(i)-1)
			{
				strcat(tempnewstring, " ");
			}
		}
	 
		printf("%40s %-40s\n", tempnewstring ,  temporigstring);
	}
}
