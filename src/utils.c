#include "utils.h"

#include <stdlib.h>
#include <string.h>

int sum_positive(int arr[], size_t len)
{
	int sum = 0;

	size_t i;
	for (i = 0; i < len; ++i)
	{
		if (arr[i] > 0)
		{
			sum += arr[i];
		}
	}

	return sum;
}

void minmax(int *minptr, int *maxptr, int arr[], size_t len)
{
	size_t i;

	if (minptr != NULL)
	{
		*minptr = arr[0];
	}
	if (maxptr != NULL)
	{
		*maxptr = arr[0];
	}

	for (i = 1; i < len; ++i)
	{
		if (minptr != NULL && arr[i] < *minptr)
		{
			*minptr = arr[i];
		}
		else if (maxptr != NULL && arr[i] > *maxptr)
		{
			*maxptr = arr[i];
		}
	}
}

int min_val(int arr[], size_t len)
{
	int minv;
	minmax(&minv, NULL, arr, len);
	return minv;
}

int max_val(int arr[], size_t len)
{
	int maxv;
	minmax(NULL, &maxv, arr, len);
	return maxv;
}

bool starts_with(const char *prefix, const char *str)
{
	size_t prefix_len = strlen(prefix);
	size_t str_len = strlen(str);

	return (str_len < prefix_len) ?
		false : memcmp(prefix, str, prefix_len) == 0;
}
