#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

/*
 * Sums the elements in the array 'arr'.
 */
int sum(int arr[], int len);

/*
 * Returns the maximum value in the array 'arr'. Assumes that 'arr' has at
 * least one element.
 */
int maxval(int arr[], int len);

/*
 * Test if the string 'str' starts with the character sequence 'prefix'.
 */
bool starts_with(const char *prefix, const char *str);

#endif /* UTILS_H_ */
