#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include <stddef.h>

/*
 * Generates horizontal histogram whose bar heights correspond to the values
 * in the 'values' array. Bars are made of the 'c' character. Returns
 * dynamically allocated string that should be freed by the caller.
 */
char* generate_horizontal_histogram(int values[], size_t values_len, char c);

/*
 * Generates vertical histogram whose bar heights correspond to the values
 * in the 'values' array. Bars are made of the 'c' character. Returns
 * dynamically allocated string that should be freed by the caller.
 */
char* generate_vertical_histogram(int values[], size_t values_len, char c);

/*
 * Generates horizontal histogram using Unicode Box Drawing Characters.
 */
char* generate_pretty_horizontal_histogram(int values[], size_t values_len);

/*
 * Generates vertical histogram using Unicode Box Drawing Characters.
 */
char* generate_pretty_vertical_histogram(int values[], size_t values_len);

#endif /* HISTOGRAM_H_ */
