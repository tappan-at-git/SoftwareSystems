/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Returns a heap-allocated string with length n, with
// the given string in the middle, surrounded by fillchar.
//
// For example, if s is allen, n is 10 and fillchar is .
// the result is ..allen... 

char *center(char *s, int n, char fillchar)
{
    int len = strlen(s);        // Don't loop through the string more than you 
                                // need to.
    if (n < len) {
        fprintf(stderr, "could not center %s with %c in a length %i string: n \
must be longer than strlen(s)!", s, fillchar, n);
        exit(1);
    }
    char *result = malloc(sizeof(char)*n);
    char *buf = result;         // One pointer to return, and one to advance.
    
    int leftfill = (n-len)/2; // left fill may be 1 less than right fill to 
                              // deal with odd length strings.
    int i = 0;
    // the first leftfill characters in *result will be filler
    while (i<leftfill) {
        *(buf++) = fillchar;    // increment but look at initial pointer
        i++;
    }
    // the centered string, which we know to be length len
    while (i<(leftfill+len)) {
        *(buf++) = *(s++);
        i++;
    }
    // the remaining characters up to the nth are filler
    while (i<n) {
        *(buf++) = fillchar;
        i++;
    }
    return result;
}


int main (int argc, char *argv[])
{
    // ..............................
    // ...........Software...........
    char *s = center("Software", 30, '.');
    printf("%s\n", s);
    
    // -------------------------------
    // ------------Systems------------
    char *s2 = center("Systems", 31, '-');
    printf("%s\n", s2);

    // ********************************
    // **********Spring 2014***********
    char *s3 = center("Spring 2014", 32, '*');
    printf("%s\n", s3);

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // ^^^^^^^^^^^^^Exam 1^^^^^^^^^^^^^^
    char *s4 = center("Exam 1", 33, '^');
    printf("%s\n", s4);

    return 0;
}
