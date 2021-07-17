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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/*
 * Rotates a Box Drawing Character by 0 degrees.
 */
static char* rotate_pretty_char_0_degrees(char *pretty_char);

/*
 * Rotates a Box Drawing Character by 90 degrees clockwise.
 */
static char* rotate_pretty_char_90_degrees(char *pretty_char);

/*
 * Generates a character being an element of a pretty histogram based on
 * position specified by 'shift' and 'level' parameters and appends it to
 * the 'hist' String. 'shift' is the number of (possibly multibyte)
 * characters from the begining of the histogram. 'level' is the number of
 * (possibly multibyte and/or negative) characters from the 0-axis.
 * Returns the size in bytes of the generated character.
 */
static size_t generate_pretty_element(String *hist, size_t shift, int level,
	int values[], size_t values_len, enum Layout layout);

char* generate_horizontal_histogram(int values[], size_t values_len, char c)
{
	int bottom = MIN(min_val(values, values_len), 0);
	size_t i;
	int j;
	String str;
	String_init(&str);

	for (i = 0; i < values_len; ++i)
	{
		for (j = bottom; j <= MAX(values[i], 0); ++j)
		{
			if (j == 0 && bottom < 0)
			{
				String_append_char(&str, '|');
			}
			else if ((j > 0 && values[i] > 0 && values[i] >= j)
				|| (j < 0 && values[i] < 0 && values[i] <= j))
			{
				String_append_char(&str, c);
			}
			else if (j != 0)
			{
				String_append_char(&str, ' ');
			}
		}
		if (i != values_len - 1)
		{
			String_append_char(&str, '\n');
		}
	}

	return str.data;
}

char* generate_vertical_histogram(int values[], size_t values_len, char c)
{
	int top = MAX(max_val(values, values_len), 0);
	int bottom = MIN(min_val(values, values_len), 0);
	int i;
	size_t j;
	String str;
	String_init(&str);

	for (i = top; i >= ((bottom < 0) ? bottom : 1); --i)
	{
		for (j = 0; j < values_len; ++j)
		{
			if (i == 0)
			{
				String_append_char(&str, '-');
			}
			else if ((i > 0 && values[j] > 0 && values[j] >= i)
				|| (i < 0 && values[j] < 0 && values[j] <= i))
			{
				String_append_char(&str, c);
			}
			else
			{
				String_append_char(&str, ' ');
			}
		}
		if (i > ((bottom < 0) ? bottom : 1))
		{
			String_append_char(&str, '\n');
		}
	}

	return str.data;
}

char* generate_pretty_horizontal_histogram(int values[], size_t values_len)
{
	int bottom = MIN(min_val(values, values_len), 0);
	size_t i;
	int j;
	String str;
	String_init(&str);

	for (i = 0; i <= values_len*2; ++i)
	{
		for (j = bottom; ; ++j)
		{
			if (generate_pretty_element(&str, i, j, values,
				values_len, HORIZONTAL_LAYOUT) == 0)
			{
				break;
			}
		}
		if (i < values_len*2)
		{
			String_append_char(&str, '\n');
		}
	}

	return str.data;
}

char* generate_pretty_vertical_histogram(int values[], size_t values_len)
{
	int top = MAX(max_val(values, values_len), 0);
	int bottom = MIN(min_val(values, values_len), 0);
	int i;
	size_t j;
	String str;
	String_init(&str);

	for (i = top; i >= bottom; --i)
	{
		for (j = 0; j <= 2*values_len; ++j)
		{
			generate_pretty_element(&str, j, i, values,
				values_len, VERTICAL_LAYOUT);
		}
		if (i > bottom)
		{
			String_append_char(&str, '\n');
		}
	}

	return str.data;
}

static char* rotate_pretty_char_0_degrees(char *pretty_char)
{
	return pretty_char;
}

static char* rotate_pretty_char_90_degrees(char *pretty_char)
{
	char *ret;

	if (strcmp(pretty_char, HORIZONTAL) == 0)
	{
		ret = VERTICAL;
	}
	else if (strcmp(pretty_char, VERTICAL) == 0)
	{
		ret = HORIZONTAL;
	}
	else if (strcmp(pretty_char, DOWN_AND_RIGHT) == 0)
	{
		ret = DOWN_AND_LEFT;
	}
	else if (strcmp(pretty_char, DOWN_AND_LEFT) == 0)
	{
		ret = UP_AND_LEFT;
	}
	else if (strcmp(pretty_char, UP_AND_RIGHT) == 0)
	{
		ret = DOWN_AND_RIGHT;
	}
	else if (strcmp(pretty_char, UP_AND_LEFT) == 0)
	{
		ret = UP_AND_RIGHT;
	}
	else if (strcmp(pretty_char, VERTICAL_AND_RIGHT) == 0)
	{
		ret = DOWN_AND_HORIZONTAL;
	}
	else if (strcmp(pretty_char, VERTICAL_AND_LEFT) == 0)
	{
		ret = UP_AND_HORIZONTAL;
	}
	else if (strcmp(pretty_char, DOWN_AND_HORIZONTAL) == 0)
	{
		ret = VERTICAL_AND_LEFT;
	}
	else if (strcmp(pretty_char, UP_AND_HORIZONTAL) == 0)
	{
		ret = VERTICAL_AND_RIGHT;
	}
	else
	{
		ret = pretty_char;
	}

	return ret;
}

static size_t generate_pretty_element(String *hist, size_t shift, int level,
	int values[], size_t values_len, enum Layout layout)
{
	char *elem = "";
	char* (*rotate)(char*);

	if (layout == VERTICAL_LAYOUT)
	{
		rotate = rotate_pretty_char_0_degrees;
	}
	else
	{
		rotate = rotate_pretty_char_90_degrees;
	}

	if (shift % 2 != 0)
	{
		size_t curr_idx = (shift - 1)/2;

		if (layout == HORIZONTAL_LAYOUT
			&& level > MAX(values[curr_idx], 0))
		{
			elem = "";
		}
		else if (level == 0 || level == values[curr_idx])
		{
			elem = rotate(HORIZONTAL);
		}
		else
		{
			elem = " ";
		}
	}
	else
	{
		size_t next_idx = shift/2;
		size_t prev_idx = (next_idx > 0) ? next_idx - 1 : 0;

		if (layout == HORIZONTAL_LAYOUT && level > MAX(
			MAX(values[prev_idx], values[next_idx]), 0))
		{
			elem = "";
		}
		else if (level == 0)
		{
			if (prev_idx == 0 && next_idx == 0)
			{
				if (values[next_idx] == 0)
				{
					elem = rotate(HORIZONTAL);
				}
				else if (values[next_idx] > 0)
				{
					elem = rotate(UP_AND_RIGHT);
				}
				else
				{
					elem = rotate(DOWN_AND_RIGHT);
				}
			}
			else if (next_idx == values_len)
			{
				if (values[prev_idx] == 0)
				{
					elem = rotate(HORIZONTAL);
				}
				else if (values[prev_idx] > 0)
				{
					elem = rotate(UP_AND_LEFT);
				}
				else
				{
					elem = rotate(DOWN_AND_LEFT);
				}
			}
			else
			{
				if (values[prev_idx] == 0
					&& values[next_idx] == 0)
				{
					elem = rotate(HORIZONTAL);
				}
				else if (values[prev_idx] <= 0
					&& values[next_idx] <= 0)
				{
					elem = rotate(DOWN_AND_HORIZONTAL);
				}
				else if (values[prev_idx] >= 0
					&& values[next_idx] >= 0)
				{
					elem = rotate(UP_AND_HORIZONTAL);
				}
				else
				{
					elem = VERTICAL_AND_HORIZONTAL;
				}
			}
		}
		else if (level < 0)
		{
			if (prev_idx == 0 && next_idx == 0)
			{
				if (level < values[next_idx])
				{
					elem = " ";
				}
				else if (level == values[next_idx])
				{
					elem = rotate(UP_AND_RIGHT);
				}
				else
				{
					elem = rotate(VERTICAL);
				}
			}
			else if (next_idx == values_len)
			{
				if (level < values[prev_idx])
				{
					elem = " ";
				}
				else if (level == values[prev_idx])
				{
					elem = rotate(UP_AND_LEFT);
				}
				else
				{
					elem = rotate(VERTICAL);
				}
			}
			else
			{
				if (level < values[prev_idx]
					&& level < values[next_idx])
				{
					elem = " ";
				}
				else if (level != values[prev_idx]
					&& level != values[next_idx])
				{
					elem = rotate(VERTICAL);
				}
				else if (level == values[prev_idx]
					&& level == values[next_idx])
				{
					elem = rotate(UP_AND_HORIZONTAL);
				}
				else if (level == values[prev_idx]
					&& level < values[next_idx])
				{
					elem = rotate(UP_AND_LEFT);
				}
				else if (level == values[prev_idx]
					&& level > values[next_idx])
				{
					elem = rotate(VERTICAL_AND_LEFT);
				}
				else if (level < values[prev_idx]
					&& level == values[next_idx])
				{
					elem = rotate(UP_AND_RIGHT);
				}
				else
				{
					elem = rotate(VERTICAL_AND_RIGHT);
				}
			}
		}
		else
		{
			if (prev_idx == 0 && next_idx == 0)
			{
				if (level > values[next_idx])
				{
					elem = " ";
				}
				else if (level == values[next_idx])
				{
					elem = rotate(DOWN_AND_RIGHT);
				}
				else
				{
					elem = rotate(VERTICAL);
				}
			}
			else if (next_idx == values_len)
			{
				if (level > values[prev_idx])
				{
					elem = " ";
				}
				else if (level == values[prev_idx])
				{
					elem = rotate(DOWN_AND_LEFT);
				}
				else
				{
					elem = rotate(VERTICAL);
				}
			}
			else
			{
				if (level > values[prev_idx]
					&& level > values[next_idx])
				{
					elem = " ";
				}
				else if (level != values[prev_idx]
					&& level != values[next_idx])
				{
					elem = rotate(VERTICAL);
				}
				else if (level == values[prev_idx]
					&& level == values[next_idx])
				{
					elem = rotate(DOWN_AND_HORIZONTAL);
				}
				else if (level == values[prev_idx]
					&& level < values[next_idx])
				{
					elem = rotate(VERTICAL_AND_LEFT);
				}
				else if (level == values[prev_idx]
					&& level > values[next_idx])
				{
					elem = rotate(DOWN_AND_LEFT);
				}
				else if (level < values[prev_idx]
					&& level == values[next_idx])
				{
					elem = rotate(VERTICAL_AND_RIGHT);
				}
				else
				{
					elem = rotate(DOWN_AND_RIGHT);
				}
			}
		}
	}

	String_append(hist, elem);
	return strlen(elem);
}
