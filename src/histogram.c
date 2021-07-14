#include "histogram.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

/* Unicode Box Drawing Characters in UTF-8 encoding */
#define HORIZONTAL "\xE2\x94\x80"
#define VERTICAL "\xE2\x94\x82"
#define DOWN_AND_RIGHT "\xE2\x94\x8C"
#define DOWN_AND_LEFT "\xE2\x94\x90"
#define UP_AND_RIGHT "\xE2\x94\x94"
#define UP_AND_LEFT "\xE2\x94\x98"
#define VERTICAL_AND_RIGHT "\xE2\x94\x9C"
#define VERTICAL_AND_LEFT "\xE2\x94\xA4"
#define DOWN_AND_HORIZONTAL "\xE2\x94\xAC"
#define UP_AND_HORIZONTAL "\xE2\x94\xB4"
#define VERTICAL_AND_HORIZONTAL "\xE2\x94\xBC"

/* The lenght of a single Box Drawing Character */
#define BOX_CHAR_LEN 3

#define MAX(a,b) (((a)>(b))?(a):(b))

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
		for (j = bottom; j <= ((values[i] > 0) ? values[i] : 0); ++j)
		{
			if (j == 0 && bottom < 0)
			{
				str[pos++] = '|';
			}
			else if (j > 0 && values[i] > 0 && values[i] >= j
				|| j < 0 && values[i] < 0 && values[i] <= j)
			{
				str[pos++] = c;
			}
			else if (j != 0)
			{
				str[pos++] = ' ';
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

char* generate_pretty_horizontal_histogram(int values[], size_t values_len)
{
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
	size_t bottom_area = (bottom < 0) ? (abs(bottom) + 1)*(values_len) : 0;
	size_t str_len = 27*(sum_positive(values, values_len) +
	       bottom_area + values_len) - 1;
	char *str = malloc((str_len + 1)*sizeof(char));

	size_t i, pos = 0;
	int j;
	for (i = 0; i <= values_len*2; ++i)
	{
		if (i % 2 != 0)
		{
			size_t curr_idx = (i - 1)/2;
			for (j = bottom; j <= ((values[curr_idx] > 0) ?
						values[curr_idx] : 0); ++j)
			{
				if (j == 0 || j == values[curr_idx])
				{
					memcpy(str + pos, VERTICAL,
							BOX_CHAR_LEN);
					pos += BOX_CHAR_LEN;
				}
				else
				{
					str[pos++] = ' ';
				}
			}
		}
		else
		{
			size_t next_idx = i/2;
			size_t prev_idx = (next_idx > 0) ? next_idx - 1 : 0;

			for (j = bottom; j <= MAX(MAX(values[prev_idx],
				values[next_idx]), 0); ++j)
			{
				if (j == 0)
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (values[next_idx] == 0)
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
						}
						else if (values[next_idx] >= 0)
						{
							memcpy(str + pos,
								DOWN_AND_RIGHT,
								BOX_CHAR_LEN);
						}
						else
						{
							memcpy(str + pos,
								DOWN_AND_LEFT,
								BOX_CHAR_LEN);
						}
						pos += BOX_CHAR_LEN;
					}
					else if (next_idx == values_len)
					{
						if (values[prev_idx] == 0)
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
						}
						else if (values[prev_idx] > 0)
						{
							memcpy(str + pos,
								UP_AND_RIGHT,
								BOX_CHAR_LEN);
						}
						else
						{
							memcpy(str + pos,
								UP_AND_LEFT,
								BOX_CHAR_LEN);
						}
						pos += BOX_CHAR_LEN;
					}
					else
					{
						if (values[prev_idx] == 0
							&& values[next_idx] == 0)
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
						}
						else if (values[prev_idx] <= 0
							&& values[next_idx] <= 0)
						{
							memcpy(str + pos,
								VERTICAL_AND_LEFT,
								BOX_CHAR_LEN);
						}
						else if (values[prev_idx] >= 0
							&& values[next_idx] >= 0)
						{
							memcpy(str + pos,
								VERTICAL_AND_RIGHT,
								BOX_CHAR_LEN);
						}
						else
						{
							memcpy(str + pos,
								VERTICAL_AND_HORIZONTAL,
								BOX_CHAR_LEN);
						}
						pos += BOX_CHAR_LEN;
					}
				}
				else if (j < 0)
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (j < values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (j == values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else if (next_idx == values_len)
					{
						if (j < values[prev_idx])
						{
							str[pos++] = ' ';
						}
						else if (j == values[prev_idx])
						{
							memcpy(str + pos,
								UP_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else
					{
						if (j < values[prev_idx]
							&& j < values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (j != values[prev_idx]
							&& j != values[next_idx])
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j == values[prev_idx]
							&& j == values[next_idx])
						{
							memcpy(str + pos,
								VERTICAL_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j == values[prev_idx]
							&& j < values[next_idx])
						{
							memcpy(str + pos,
								UP_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j == values[prev_idx]
							&& j > values[next_idx])
						{
							memcpy(str + pos,
								UP_AND_HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j < values[prev_idx]
							&& j == values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								DOWN_AND_HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
				}
				else
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (j > values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (j == values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else if (next_idx == values_len)
					{
						if (j > values[prev_idx])
						{
							str[pos++] = ' ';
						}
						else if (j == values[prev_idx])
						{
							memcpy(str + pos,
								UP_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else
					{
						if (j > values[prev_idx]
							&& j > values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (j != values[prev_idx]
							&& j != values[next_idx])
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j == values[prev_idx]
							&& j == values[next_idx])
						{
							memcpy(str + pos,
								VERTICAL_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j == values[prev_idx]
							&& j < values[next_idx])
						{
							memcpy(str + pos,
								UP_AND_HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j == values[prev_idx]
							&& j > values[next_idx])
						{
							memcpy(str + pos,
								UP_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (j < values[prev_idx]
							&& j == values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								DOWN_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
				}
			}
		}
		str[pos++] = '\n';
	}
	str[pos] = '\0';

	return str;
}

char* generate_pretty_vertical_histogram(int values[], size_t values_len)
{
	int top = max_val(values, values_len);
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
	size_t str_len = 27*(top - bottom + ((minv < 0) ? 1 : 0))
		*(values_len + 1) - 1;
	char *str = malloc((str_len + 1)*sizeof(char));

	int i;
	size_t j, pos = 0;
	for (i = top; i >= ((bottom < 0) ? bottom : 0); --i)
	{
		for (j = 0; j <= 2*values_len; ++j)
		{
			if (j % 2 != 0)
			{
				size_t curr_idx = (j - 1)/2;

				if (i == 0 || i == values[curr_idx])
				{
					memcpy(str + pos, HORIZONTAL,
						BOX_CHAR_LEN);
					pos += BOX_CHAR_LEN;
				}
				else
				{
					str[pos++] = ' ';
				}
			}
			else
			{
				size_t next_idx = j/2;
				size_t prev_idx = (next_idx > 0) ?
					next_idx - 1 : 0;

				if (i == 0)
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (values[next_idx] == 0)
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
						}
						else if (values[next_idx] > 0)
						{
							memcpy(str + pos,
								UP_AND_RIGHT,
								BOX_CHAR_LEN);
						}
						else
						{
							memcpy(str + pos,
								DOWN_AND_RIGHT,
								BOX_CHAR_LEN);
						}
						pos += BOX_CHAR_LEN;
					}
					else if (next_idx == values_len)
					{
						if (values[prev_idx] == 0)
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
						}
						else if (values[prev_idx] > 0)
						{
							memcpy(str + pos,
								UP_AND_LEFT,
								BOX_CHAR_LEN);
						}
						else
						{
							memcpy(str + pos,
								DOWN_AND_LEFT,
								BOX_CHAR_LEN);
						}
						pos += BOX_CHAR_LEN;
					}
					else
					{
						if (values[prev_idx] == 0
							&& values[next_idx] == 0)
						{
							memcpy(str + pos,
								HORIZONTAL,
								BOX_CHAR_LEN);
						}
						else if (values[prev_idx] <= 0
							&& values[next_idx] <= 0)
						{
							memcpy(str + pos,
								DOWN_AND_HORIZONTAL,
								BOX_CHAR_LEN);
						}
						else if (values[prev_idx] >= 0
							&& values[next_idx] >= 0)
						{
							memcpy(str + pos,
								UP_AND_HORIZONTAL,
								BOX_CHAR_LEN);
						}
						else
						{
							memcpy(str + pos,
								VERTICAL_AND_HORIZONTAL,
								BOX_CHAR_LEN);
						}
						pos += BOX_CHAR_LEN;
					}
				}
				else if (i < 0)
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (i < values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (i == values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else if (next_idx == values_len)
					{
						if (i < values[prev_idx])
						{
							str[pos++] = ' ';
						}
						else if (i == values[prev_idx])
						{
							memcpy(str + pos,
								UP_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else
					{
						if (i < values[prev_idx]
							&& i < values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (i != values[prev_idx]
							&& i != values[next_idx])
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i == values[prev_idx]
							&& i == values[next_idx])
						{
							memcpy(str + pos,
								UP_AND_HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i == values[prev_idx]
							&& i < values[next_idx])
						{
							memcpy(str + pos,
								UP_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i == values[prev_idx]
							&& i > values[next_idx])
						{
							memcpy(str + pos,
								VERTICAL_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i < values[prev_idx]
							&& i == values[next_idx])
						{
							memcpy(str + pos,
								UP_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								VERTICAL_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
				}
				else
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (i > values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (i == values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else if (next_idx == values_len)
					{
						if (i > values[prev_idx])
						{
							str[pos++] = ' ';
						}
						else if (i == values[prev_idx])
						{
							memcpy(str + pos,
								DOWN_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
					else
					{
						if (i > values[prev_idx]
							&& i > values[next_idx])
						{
							str[pos++] = ' ';
						}
						else if (i != values[prev_idx]
							&& i != values[next_idx])
						{
							memcpy(str + pos,
								VERTICAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i == values[prev_idx]
							&& i == values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_HORIZONTAL,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i == values[prev_idx]
							&& i < values[next_idx])
						{
							memcpy(str + pos,
								VERTICAL_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i == values[prev_idx]
							&& i > values[next_idx])
						{
							memcpy(str + pos,
								DOWN_AND_LEFT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else if (i < values[prev_idx]
							&& i == values[next_idx])
						{
							memcpy(str + pos,
								VERTICAL_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
						else
						{
							memcpy(str + pos,
								DOWN_AND_RIGHT,
								BOX_CHAR_LEN);
							pos += BOX_CHAR_LEN;
						}
					}
				}
			}
		}
		str[pos++] = '\n';
	}

	str[str_len] = '\0';

	return str;
}
