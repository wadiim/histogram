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

int max_val(int arr[], size_t len)
{
	int m = arr[0];

	size_t i;
	for (i = 1; i < len; ++i)
	{
		if (arr[i] > m)
		{
			m = arr[i];
		}
	}

	return m;
}

int min_val(int arr[], size_t len)
{
	int m = arr[0];

	size_t i;
	for (i = 0; i < len; ++i)
	{
		if (arr[i] < m)
		{
			m = arr[i];
		}
	}

	return m;
}

bool starts_with(const char *prefix, const char *str)
{
	size_t prefix_len = strlen(prefix);
	size_t str_len = strlen(str);

	return (str_len < prefix_len) ?
		false : memcmp(prefix, str, prefix_len) == 0;
}
