#include "utils.h"

#include <string.h>

int sum(int arr[], int len)
{
	int sum = 0;

	int i;
	for (i = 0; i < len; ++i)
	{
		sum += arr[i];
	}

	return sum;
}

int maxval(int arr[], int len)
{
	int m = arr[0];

	int i;
	for (i = 1; i < len; ++i)
	{
		if (arr[i] > m)
		{
			m = arr[i];
		}
	}

	return m;
}

bool starts_with(const char *prefix, const char *str)
{
	int prefix_len = strlen(prefix);
	int str_len = strlen(str);

	return (str_len < prefix_len) ?
		false : memcmp(prefix, str, prefix_len) == 0;
}
