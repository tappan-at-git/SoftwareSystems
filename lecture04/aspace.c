/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>

int global;

void printer()
{
  int print_local = 13;
  
  printf ("Address of print_local is %p\n", &print_local);
}

int main ()
{
    int local = 5;
    void *p = malloc(128);
    void *q = malloc(128);
    void *r = malloc(128);
    
    void *a = malloc(4);
    void *b = malloc(4);

    printf ("Address of main is %p\n", main);
    printf ("Address of global is %p\n", &global);
    printf ("Address of local is %p\n", &local);
    printf ("Address of p is %p\n", p); /*the heap grows upwards!*/
    printf ("Address of q is %p\n", q);
    printf ("Address of r is %p\n", r);
    printer(); /*the stack grows downwards!*/
    printf ("Address of printer is %p\n", printer);
    printf ("a is at %p, b is at %p: %i bytes difference\n", a,b,b-a); /*b is 16 bytes after a*/
    
    return 0;
}
