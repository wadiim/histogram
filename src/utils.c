#include "utils.h"

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

