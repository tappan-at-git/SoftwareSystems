/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0
Modified by Eric Tappan, 02/2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define NUM_TRACKS 5
#define MAX_MATCHES 1 // we only care about a single regex hit per track name

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
void find_track(char search_for[])
{
    int i;
    for (i=0; i<NUM_TRACKS; i++) {
	if (strstr(tracks[i], search_for)) {
	    printf("Track %i: '%s'\n", i, tracks[i]);
	}
    }
}

// Prints an appropriate error message for regex.h library functions, then 
// exit(1).
void handle_regerror(int errcode, regex_t* exp, int strlen)
{
    char *err_str = malloc(sizeof(char)*strlen);
    regerror(errcode, exp, err_str, 20);        // turns a regex.h errorcode 
                                                // into a human-readable string.
    printf("regcomp failed (error code %s)\n", err_str);
    exit(1);
}

// Finds all tracks that match the given pattern.
//
// Prints track number and title.
// Exits with code 1 for bad regex.h functions
void find_track_regex(char* pattern)
{
    regmatch_t matches[MAX_MATCHES];
    
    int errcode; //regex compilation error code
    char *err_str = malloc(sizeof(char)*20);
    regex_t exp; // compiled regular expression matching pattern
    
    // compile with POSIX Extended syntax. Ignore case during match.
    errcode = regcomp(&exp, pattern, REG_EXTENDED|REG_ICASE);
    if (errcode) {
        handle_regerror(errcode, &exp, 20);     // exit(1)
    }
    
    int i;
    int no_match=1;
    for (i = 0; i<NUM_TRACKS; i++) {
        errcode = regexec(&exp,tracks[i],MAX_MATCHES, matches, 0);
        if (errcode == REG_NOMATCH) {
            continue;                   // No match, but no real error: try 
                                        // again.
        }
        // regexec returns 0 on a match, which is all we care about
        else if (!errcode) {
            printf("Track %i: '%s'\n", i, tracks[i]);
            no_match=0;           // found one match, but keep looking for more.
        } else {
            handle_regerror(errcode, &exp, 20); // exit(1)
        }
    }
    
    if (no_match) {
        printf("No Track found for '%s'\n", pattern);   // :c
    }
    
    regfree(&exp);      // be free, little one!
}

// Truncates the string at the first newline, if there is one. 
void rstrip(char s[])
{
    char *ptr = strchr(s, '\n');
    if (ptr) {
	*ptr = '\0';
    }
}

int main (int argc, char *argv[])
{
    char search_for[80];

    /* take input from the user and search */
    printf("Search for: ");
    fgets(search_for, 80, stdin);
    rstrip(search_for);

    //find_track(search_for);
    find_track_regex(search_for);

    return 0;
}
