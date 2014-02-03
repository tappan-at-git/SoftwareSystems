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
void get_card_name(char *prompt, char *card_name_byref) 
{
  puts(prompt);
  scanf("%2s", card_name_byref);
}

/* Converts card names (1-10, [J]ack, [Q]ueen, [K]ing, [A]ce) to a value (1-11).
 * Raises the invalid flag if the card is confusing or out of range.
 */
int get_card_val(char *card_name,int *val_byref)
{
    int invalid = 0;
    switch(card_name[0]) {
    case 'K':
    case 'Q':
    case 'J':
      *val_byref = 10;
      invalid = 0;
      /* exits switch, promptly returns */
      break;
    case 'A':
      *val_byref = 11;
      invalid = 0;
      /* exits switch, promptly returns */
      break;
    default:
      *val_byref = atoi(card_name);
      if ((*val_byref < 1) || (*val_byref > 10)) {
        invalid = 1;
      }
    }

    return invalid;
}

/* if val is in a low range, count++. If val is in a high range, count++
 */
void eval_val(int val, int *count_byref)
{
  if ((val >= 2) && (val < 7)) {
    (*count_byref)++;
  } else if (val == 10) {
    (*count_byref)--;
  }
}

/* Keeps track of a game of blackjack for the user. 
 * The count goes up whenever a low card is drawn and goes down with high cards.
 * Has a bad attitude.
 */
int main()
{
  char card_name[3];
  int val;
  int invalid;
  int count = 0;

  int running = 1;

  while(running) {
    get_card_name("Enter the card's name, human: ", card_name);

    /* check for escape sequence */
    if ((card_name[0] == 'X') || (card_name[0] == 'x')) {
      running = 0;  /* end loop */
      continue;
    }
    
    /* convert str card_name -> int val and raise flag if card_name is invalid. */
    invalid = get_card_val(card_name, &val);
    /* prompt for new card if the user screwed up */
    if (invalid) {
      puts("You don't actually know the rules to this game, do you?");
      continue;  /* retry */
    }

    eval_val(val, &count); /* in-/decrement count depending on val */
    printf("Current count: %i\n", count);
  } /* loop */

  return 0;
}
