/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define NUM_TRACKS 5   // (4) Damn your ;!

char tracks[][80] = {
    "So What",
    "Freddie Freeloader",
    "Blue in Green",
    "All Blues",
    "Flamenco Sketches"
};


// Finds all tracks that contain the given string.
//
// Prints track number and title.
void find_track(char *search_for)
{
    int i;
    for (i=0; i<NUM_TRACKS; i++) {
	if (strstr(tracks[i], search_for)) {
	    printf("Track %i: '%s'\n", i, tracks[i]);
	}
    }
}


// Finds all tracks that match the given pattern.
//
// Prints track number and title.
void find_track_regex(char *pattern)
{
    int i, ret;
    regex_t regex;
    int msglen = 30;
    char *msgbuf = malloc(sizeof(char)*msglen);       // (2) No size allocated 
                                                  // originally.
    // int regcomp returns 0 on success and an error code otherwise.
    ret = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);  // (1) Should be 
                                                               // bitwise OR, |.
                                                               // Was ||.
    if (ret) {
        fprintf(stderr, "Could not compile regex\n");   // Could be more useful.
        exit(1);        // exits on error
    }

    for (i=0; i<NUM_TRACKS; i++) {
        // int regexec returns 0 on match, REG_NOMATCH, or an error value
        ret = regexec(&regex, tracks[i], 0, NULL, 0);
	if (!ret) {
	    printf("Track %i: '%s'\n", i, tracks[i]);
	} else if (ret == REG_NOMATCH) {
	    continue;
	} else {
           regerror(ret, &regex, msgbuf, msglen); // (3) sizeof doesn't 
                                                        // work that way!
           fprintf(stderr, "Regex match failed: %s\n", msgbuf);
	   exit(1);     // exits on error
	}
    }

    /* I'm not sure this is necessary, but it's possible that if you
       let regex go out of scope without running regfree, it leaks
       (that is, leaves some allocated memory unfreed). */
    regfree(&regex);    // (5) Necessary or not, use a pointer! 
    free(msgbuf);
}


int main (int argc, char *argv[])
{
    // (6) 'F' is a character, not a string. Good luck with that one.
    char *target = "F";         // Finds "Freddie Freeloader" and "Flamenco 
                                // Sketches"
    char *pattern = "Fr.*Fr.*"; // Looks for "Fr"+0 or more other 
                                // characters+"Fr"+ 0 or more other character.
                                // Finds "Freddie Freeloader".

    find_track(target);         // No return asked for or specified.
    find_track_regex(pattern);  // No return asked for or specified.

    return 0;
}
