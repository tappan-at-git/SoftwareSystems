/* Eric Tappan
 * 1/29/2014
 * HW 1
 *
 * Engages with the user in a dubiously-legal card-counting excercise.
 * Accepts card names from the user and replies with a running scount based on
 * their values.

*/
#include <stdio.h>
#include <stdlib.h>

/* Prompts the user for input (a card name).
 * User input is truncated to the first two characters.
 * prompt: string prompt to display
 * card_name: buffer where the result is stored
 */
void get_card_name(char *prompt, char *card_name) 
{
  puts(prompt);
  scanf("%2s", card_name);
}

// converts card names (1-10, [J]ack, [Q]ueen, [K]ing, [A]ce) to a value (1-11).
// Raises the invalid flag if the card is confusing or out of range.
int get_card_val(char *card_name,int *val)
{
    int invalid = 0;
    switch(card_name[0]) {
    case 'K':
    case 'Q':
    case 'J':
      *val = 10;
      invalid = 0;
      break; // exits switch, promptly returns
    case 'A':
      *val = 11;
      invalid = 0;
      break; // exits switch, promptly returns
    default:
      *val = atoi(card_name);
      if ((*val < 1) || (*val > 10)) {
        invalid = 1;
      }
    }

    return invalid;
}

// if val is in a low range, count++. If val is in a high range, count++
void eval_val(int val, int *count)
{
  if ((val > 2) && (val < 7)) {  // for some reason we don't care about 2s
    (*count)++;
  } else if (val == 10) { // aces go home
    (*count)--;
  }
}

int main()
{
  char card_name[3];
  int val;
  int invalid;
  int count = 0;

  int running = 1;

  while(running) {
    get_card_name("Enter the card's name, human: ", card_name);

    // check for escape sequence
    if ((card_name[0] == 'X') || (card_name[0] == 'x')) {
      running = 0;  // end loop
      continue;
    }
    
    // convert str card_name -> int val and raise flag if card_name is invalid.
    invalid = get_card_val(card_name, &val);
    // prompt for new card if the user screwed up
    if (invalid) {
      puts("You don't actually know the rules to this game, do you?");
      continue;  // retry
    }

    eval_val(val, &count); // in-/decrement count depending on val
    printf("Current count: %i\n", count);
  } //loop

  return 0;
}
