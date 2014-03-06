/*

    Copyright 2007 Allen B. Downey

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    The GNU General Public License is available from
    http://www.gnu.org/licenses/.

*/

// generate a random float using the algorithm described
// at allendowney.com/research/rand
float my_random_float()
{
  int x, exp, mant;
  float f;

  // this union is for assembling the float.
  union {
    float f;
    int i;
  } b;

  // generate 31 random bits (assuming that RAND_MAX is 2^31 - 1
  x = random();

  // use bit-sca-forward to find the first set bit and
  // compute the exponent
  asm ("bsfl %1, %0"
       :"=r"(exp)
       :"r"(x)
      );
  exp = 126 - exp;

  // use the other 23 bits for the mantissa (for small numbers
  // this means we are re-using some bits)
  mant = x >> 8;
  b.i = (exp << 23) | mant;

  return b.f;
}

// alternative implementation of my algorithm that doesn't use
// embedded assembly
float my_random_float2()
{
  int x;
  int mant;
  int exp = 126;
  int mask = 1;

  union {
    float f;
    int i;
  } b;

  // generate random bits until we see the first set bit
  while (1) {
    x = random();
    if (x == 0) {
      exp -= 31;
    } else {
      break;
    }
  }

  // find the location of the first set bit and compute the exponent
  while (x & mask) {
    mask <<= 1;
    exp--;
  }

  // use the remaining bit as the mantissa
  mant = x >> 8;
  b.i = (exp << 23) | mant;

  return b.f;
}

// compute a random double using my Allen Downey's algorithm on the range [0,1]
double my_random_double()
{
  // TODO: fill this in
  /* A double has the form:
   * sign[1]:exponent[11]:fraction[52]
   * sign should be 0
   * The exponent is biased by 1023 to get values on the range [-1022,1022]
   * Exponent values of 0 and 2047 have special meanings (subnormals, etc.)
   * To be on the range [0,1] we need exponents on the range [0,1023].
   */
  long long x;
  long long mant;
  long long exp = 1022; // Why does this need to be one less than the bias?
  long long mask = 1;

  union {
    double d;
    long long ll;
  } b;
  
  // generate random bits until we see the first set bit
  while (1) {
    x = (((long long) random()) << 32) | (long long) random();
    x = x;
    if (x == 0) {
      exp -= 62;
    } else {
      break;
    }
  }
  
  printf("x: %lli\n",x);
  // find the location of the first set bit and compute the exponent
  while (x & mask) {
    mask <<= 1;
    exp--;
  }

  // use the remaining bit as the mantissa
  mant = x >> 11;
  b.ll = ((exp << 52) | mant);
  printf("returning: %lli\n",b.ll);
  return b.d;
}

// return a constant (this is a dummy function for time trials)
float dummy()
{
  float f = 0.5;

  return f;
}

// generate a random integer and convert to float (dummy function)
float dummy2()
{
  int x;
  float f;

  x = random();
  f = (float) x;

  return f;
}

// generate a random float using the standard algorithm
float random_float()
{
  int x;
  float f;

  x = random();
  f = (float) x / (float)RAND_MAX;

  return f;
}


// generate a random double using the standard algorithm
float random_double()
{
  int x;
  double f;

  x = random();
  f = (double) x / (double)RAND_MAX;

  return f;
}


