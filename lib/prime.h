#ifndef PRIME_H
#define PRIME_H

#include <math.h>

/**
 * @brief Return whether a number is prime or not.
 * @param number The number to check.
 * @return :
 *   0 if the number is not prime. \n
 *   1 if the number is prime. \n
 *   -1 if undefined (i.e number < 2) \n
 * */
int is_prime(const int);

/**
 * @brief the next prime after x , or  x if x is prime.
 * */
int next_prime(const int);

#endif // PRIME_H