/* Eric Tappan
 * SoftSys Quiz 2
 * Comments are post-quiz markups.
 * Including // newlines
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* interleave(char str_a[], char str_b[]) {
    char* str_out = malloc(sizeof(char)*(strlen(str_a)+strlen(str_b))); // +1
    // surprisingly, this worked despite *not leaving space for the null 
    // terminator.* 
    int i = 0;
    while (str_a[i] && str_b[i]) {
        // I like your version better for not needing the *2's. These are ugly.
        str_out[i*2] = str_a[i];
        str_out[i*2+1] = str_b[i];
        i+=1;
    }
    if (str_a[i]) {
        strncat(str_out,str_a+i,strlen(str_a)-i);
    }
    else if (str_b[i]) {
        strncat(str_out,str_b+i,strlen(str_b)-i);
    }
    
    return str_out;
}

int main() {
    puts(interleave("aa","bb"));
    puts(interleave("clips","aloe"));
    puts(interleave("bat","country"));
    return 0;
}