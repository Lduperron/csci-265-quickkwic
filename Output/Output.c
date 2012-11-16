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

   //   printf(">>>>>%s\n", temporigstring);
   //   printf(">>>>>%s\n", tempnewstring);
      
      printf("%40s %-40s\n", tempnewstring ,  temporigstring);
   /*
		for (j = 0; j < numWords; j++) {
			int shiftNum = SSGetShiftNum(i);
         
         printf("%d", SSGetShiftNum(i));
         
        // strcpy(tempstring, SSGetWord(i,j));
			printf("%s",SSGetWord(i,j));

			// no comma if line is unshifted
			//  otherwise comma follows last word on original line
			 
			if (shiftNum != 0 && j == numWords-shiftNum-1)
				printf(",");

			// space follows every word but the last 
			if (j != SSNumWords(i)-1)
				printf(" ");
		}
		printf("\n");
      
      */
	}
}
