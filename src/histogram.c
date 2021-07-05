#include "histogram.h"
#include "utils.h"

#include <stdlib.h>

char* generate_horizontal_histogram(int values[], size_t values_len, char c)
{
	size_t str_len = sum(values, values_len) + values_len - 1;
	char *str = malloc((str_len + 1)*sizeof(char));

	size_t i, pos = 0;
	int j;
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

char* generate_vertical_histogram(int values[], size_t values_len, char c)
{
	int height = max_val(values, values_len);
	size_t str_len = height*(values_len + 1) - 1;
	char *str = malloc((str_len + 1)*sizeof(char));

	int i;
	size_t j, pos = 0;
	for (i = height; i > 0; --i)
	{
		for (j = 0; j < values_len; ++j)
		{
			if (values[j] >= i)
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
