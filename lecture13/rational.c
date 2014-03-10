#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// a rational number is a fraction in the form (float) (int numer)/(int denom)
typedef struct {
    int numer;
    int denom;
} Rational;

// Returns a new Rational with the given numer and denom.
// If unable to allocate, prints an error message and exits.
Rational *make_rational(int numer, int denom) {
    Rational *rat = (Rational *) malloc(sizeof(Rational));
    if (rat == NULL) {
      printf("Malloc failed. Could not make %d/%d.",numer,denom);
      exit(1);
    }
    rat->numer = numer;
    rat->denom = denom;
    return rat;
}

// Computes a floating-point approximation of a Rational.
double rational_to_double(Rational *rational) {
    double numer = rational->numer;
    double denom = rational->denom;
    return (numer/denom);
}

// Multiplies two rational numbers; returns a new Rational. Does not simplify.
Rational *mult_rational(Rational *r1, Rational *r2) {
    int numer = (r1->numer)*(r2->numer);
    int denom = (r1->denom)*(r2->denom);
    Rational *new_rat = make_rational(numer,denom);
    return new_rat;
}

// Frees a Rational.
void free_rational(Rational *rational) {
    free(rational);
}

// Prints a rational in fraction form.
void print_rational(Rational *rational) {
    printf("%d / %d\n", rational->numer, rational->denom);
}

int main(void)
{
    Rational *rational = make_rational(3, 7);
    printf("3 / 7 = ");
    print_rational(rational);

    double d = rational_to_double(rational);
    printf("0.428571 = ");
    printf("%lf\n", d);

    Rational *square = mult_rational(rational, rational);
    printf("(3/7)^2 = 9/49 = ");
    print_rational(square);
	
    free_rational(rational);
    free_rational(square);

    return EXIT_SUCCESS;
}
