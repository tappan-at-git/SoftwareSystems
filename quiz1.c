#include <stdio.h>
#include <stdlib.h>

int is_digit(char c) {
  return (c >= '0') && (c <= '9');
}

int is_integer(char *s) {
  int i = 0;

  while (s[i] != '\0') {
    if (!is_digit(s[i])) {
      return 0;
    }
    i++;
  }
  return 1;
}

int main()
{
  char string[] = "123";
  int number;
  
  if (is_integer(string)) {
    number = atoi(string);
    printf("Your number is: %i\n", number);
  }

  return 0;
}
