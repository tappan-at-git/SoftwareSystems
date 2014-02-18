/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

char *tracks[] = {
    "So What",
    "Freddie Freeloader",
    "Blue in Green",
    "All Blues",
    "Flamenco Sketches"
};

/* Returns a heap-allocated string that contains the strings 
   from the given array, joined up with no spaces between.
*/
char *strjoin(char *array[], int n)
{
    int num_char = 0;
    
    int i;
    for (i = 0; i<n; i++) {
        num_char += strlen(array[i]);
    }
        
    char *joined = malloc(sizeof(char)*num_char + 1);
    assert(joined);
    strcpy(joined,"\0"); 
    
    int end = 0;
    for (i = 0; i<n; i++) {
        strcpy(joined+end, array[i]);
        end += strlen(array[i]);
    }
    
    return joined;
}


int main (int argc, char *argv[])
{
    char *s = strjoin(tracks, 5);
    
    printf("%s\n", s);
    free(s);
    return 0;
}
