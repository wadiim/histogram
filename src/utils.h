#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>
#include <stddef.h>

/*
 * Sums the elements in the array 'arr'.
 */
int sum(int arr[], size_t len);

/*
 * Sums the positive elements in the array 'arr'.
 */
int sum_positive(int arr[], size_t len);

/*
 * Returns the maximum value in the array 'arr'. Assumes that 'arr' has at
 * least one element.
 */
int max_val(int arr[], size_t len);

/*
 * Returns the minimum value in the array 'arr'. Assumes that 'arr' has at
 * least one element.
 */
int min_val(int arr[], size_t len);

/*
 * Test if the string 'str' starts with the character sequence 'prefix'.
 */
bool starts_with(const char *prefix, const char *str);

#endif /* UTILS_H_ */
