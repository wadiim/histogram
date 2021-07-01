#include "histogram.h"
#include "utils.h"

#include <stdlib.h>

char* generate_horizontal_histogram(int values[], int values_len, char c)
{
	int str_len = sum(values, values_len) + values_len - 1;
	char *str = malloc((str_len + 1)*sizeof(char));
	int pos = 0;

	int i, j;
	for (i = 0; i < values_len; ++i)
	{
		for (j = 0; j < values[i]; ++j)
		{
			str[pos++] = c;
		}
		str[pos++] = '\n';
	}
	str[str_len] = '\0';

	return str;
}

char* generate_vertical_histogram(int values[], int values_len, char c)
{
	int rows = maxval(values, values_len);
	int str_len = rows*(values_len + 1) - 1;
	char *str = malloc((str_len + 1)*sizeof(char));
	int pos = 0;

	int i;
	for (; rows > 0; --rows)
	{
		for (i = 0; i < values_len; ++i)
		{
			if (values[i] >= rows)
			{
				str[pos++] = c;
			}
			else
			{
				str[pos++] = ' ';
			}
		}
		str[pos++] = '\n';
	}

	str[str_len] = '\0';

	return str;
}
