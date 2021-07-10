#include "histogram.h"
#include "utils.h"

#include <stdlib.h>

char* generate_horizontal_histogram(int values[], size_t values_len, char c)
{
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
	size_t bottom_area = (bottom < 0) ? (abs(bottom) + 1)*(values_len) : 0;
	size_t str_len = sum_positive(values, values_len) +
	       bottom_area + values_len - 1;
	char *str = malloc((str_len + 1)*sizeof(char));

	size_t i, pos = 0;
	int j;
	for (i = 0; i < values_len; ++i)
	{
		if (values[i] < 0)
		{
			for (j = bottom; j < 0; ++j)
			{
				if (j < values[i])
				{
					str[pos++] = ' ';
				}
				else
				{
					str[pos++] = c;
				}
			}
			str[pos++] = '|';
		}
		else
		{
			if (bottom < 0)
			{
				for (j = bottom; j < 0; ++j)
				{
					str[pos++] = ' ';
				}
				str[pos++] = '|';
			}
			for (j = 0; j < values[i]; ++j)
			{
				str[pos++] = c;
			}
		}
		str[pos++] = '\n';
	}
	str[str_len] = '\0';

	return str;
}

char* generate_vertical_histogram(int values[], size_t values_len, char c)
{
	int top = max_val(values, values_len);
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
	size_t str_len = (top - bottom + ((minv < 0) ? 1 : 0))
		*(values_len + 1) - 1;
	char *str = malloc((str_len + 1)*sizeof(char));

	int i;
	size_t j, pos = 0;
	for (i = top; i >= ((bottom < 0) ? bottom : 1); --i)
	{
		for (j = 0; j < values_len; ++j)
		{
			if (i == 0)
			{
				str[pos++] = '-';
			}
			else if (i > 0 && values[j] > 0 && values[j] >= i
				|| i < 0 && values[j] < 0 && values[j] <= i)
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
