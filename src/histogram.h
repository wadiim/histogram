#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

/*
 * Generates horizontal histogram whose bar heights correspond to the values
 * in the 'values' array. Bars are made of the 'c' character. Returns
 * dynamically allocated string that should be freed by the caller.
 */
char* generate_horizontal_histogram(int values[], int values_len, char c);

/*
 * Generates vertical histogram whose bar heights correspond to the values
 * in the 'values' array. Bars are made of the 'c' character. Returns
 * dynamically allocated string that should be freed by the caller.
 */
char* generate_vertical_histogram(int values[], int values_len, char c);

#endif /* HISTOGRAM_H_ */
