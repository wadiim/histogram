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

#define MAX(a,b) (((a)>(b))?(a):(b))

char* generate_horizontal_histogram(int values[], size_t values_len, char c)
{
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
	size_t i;
	int j;
	String str;
	String_init(&str);

	for (i = 0; i < values_len; ++i)
	{
		for (j = bottom; j <= ((values[i] > 0) ? values[i] : 0); ++j)
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
	int top = max_val(values, values_len);
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
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
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
	size_t i;
	int j;
	String str;
	String_init(&str);

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
					String_append(&str, VERTICAL);
				}
				else
				{
					String_append_char(&str, ' ');
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
							String_append(&str,
								VERTICAL);
						}
						else if (values[next_idx] >= 0)
						{
							String_append(&str,
								DOWN_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								DOWN_AND_LEFT);
						}
					}
					else if (next_idx == values_len)
					{
						if (values[prev_idx] == 0)
						{
							String_append(&str,
								VERTICAL);
						}
						else if (values[prev_idx] > 0)
						{
							String_append(&str,
								UP_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								UP_AND_LEFT);
						}
					}
					else
					{
						if (values[prev_idx] == 0
							&& values[next_idx] == 0)
						{
							String_append(&str,
								VERTICAL);
						}
						else if (values[prev_idx] <= 0
							&& values[next_idx] <= 0)
						{
							String_append(&str,
								VERTICAL_AND_LEFT);
						}
						else if (values[prev_idx] >= 0
							&& values[next_idx] >= 0)
						{
							String_append(&str,
								VERTICAL_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								VERTICAL_AND_HORIZONTAL);
						}
					}
				}
				else if (j < 0)
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (j < values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (j == values[next_idx])
						{
							String_append(&str,
								DOWN_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								HORIZONTAL);
						}
					}
					else if (next_idx == values_len)
					{
						if (j < values[prev_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (j == values[prev_idx])
						{
							String_append(&str,
								UP_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								HORIZONTAL);
						}
					}
					else
					{
						if (j < values[prev_idx]
							&& j < values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (j != values[prev_idx]
							&& j != values[next_idx])
						{
							String_append(&str,
								HORIZONTAL);
						}
						else if (j == values[prev_idx]
							&& j == values[next_idx])
						{
							String_append(&str,
								VERTICAL_AND_RIGHT);
						}
						else if (j == values[prev_idx]
							&& j < values[next_idx])
						{
							String_append(&str,
								UP_AND_RIGHT);
						}
						else if (j == values[prev_idx]
							&& j > values[next_idx])
						{
							String_append(&str,
								UP_AND_HORIZONTAL);
						}
						else if (j < values[prev_idx]
							&& j == values[next_idx])
						{
							String_append(&str,
								DOWN_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								DOWN_AND_HORIZONTAL);
						}
					}
				}
				else
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (j > values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (j == values[next_idx])
						{
							String_append(&str,
								DOWN_AND_LEFT);
						}
						else
						{
							String_append(&str,
								HORIZONTAL);
						}
					}
					else if (next_idx == values_len)
					{
						if (j > values[prev_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (j == values[prev_idx])
						{
							String_append(&str,
								UP_AND_LEFT);
						}
						else
						{
							String_append(&str,
								HORIZONTAL);
						}
					}
					else
					{
						if (j > values[prev_idx]
							&& j > values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (j != values[prev_idx]
							&& j != values[next_idx])
						{
							String_append(&str,
								HORIZONTAL);
						}
						else if (j == values[prev_idx]
							&& j == values[next_idx])
						{
							String_append(&str,
								VERTICAL_AND_LEFT);
						}
						else if (j == values[prev_idx]
							&& j < values[next_idx])
						{
							String_append(&str,
								UP_AND_HORIZONTAL);
						}
						else if (j == values[prev_idx]
							&& j > values[next_idx])
						{
							String_append(&str,
								UP_AND_LEFT);
						}
						else if (j < values[prev_idx]
							&& j == values[next_idx])
						{
							String_append(&str,
								DOWN_AND_HORIZONTAL);
						}
						else
						{
							String_append(&str,
								DOWN_AND_LEFT);
						}
					}
				}
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
	int top = max_val(values, values_len);
	int minv = min_val(values, values_len);
	int bottom = (minv < 0) ? minv : 0;
	int i;
	size_t j;
	String str;
	String_init(&str);

	for (i = top; i >= bottom; --i)
	{
		for (j = 0; j <= 2*values_len; ++j)
		{
			if (j % 2 != 0)
			{
				size_t curr_idx = (j - 1)/2;

				if (i == 0 || i == values[curr_idx])
				{
					String_append(&str, HORIZONTAL);
				}
				else
				{
					String_append_char(&str, ' ');
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
							String_append(&str,
								HORIZONTAL);
						}
						else if (values[next_idx] > 0)
						{
							String_append(&str,
								UP_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								DOWN_AND_RIGHT);
						}
					}
					else if (next_idx == values_len)
					{
						if (values[prev_idx] == 0)
						{
							String_append(&str,
								HORIZONTAL);
						}
						else if (values[prev_idx] > 0)
						{
							String_append(&str,
								UP_AND_LEFT);
						}
						else
						{
							String_append(&str,
								DOWN_AND_LEFT);
						}
					}
					else
					{
						if (values[prev_idx] == 0
							&& values[next_idx] == 0)
						{
							String_append(&str,
								HORIZONTAL);
						}
						else if (values[prev_idx] <= 0
							&& values[next_idx] <= 0)
						{
							String_append(&str,
								DOWN_AND_HORIZONTAL);
						}
						else if (values[prev_idx] >= 0
							&& values[next_idx] >= 0)
						{
							String_append(&str,
								UP_AND_HORIZONTAL);
						}
						else
						{
							String_append(&str,
								VERTICAL_AND_HORIZONTAL);
						}
					}
				}
				else if (i < 0)
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (i < values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (i == values[next_idx])
						{
							String_append(&str,
								DOWN_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								VERTICAL);
						}
					}
					else if (next_idx == values_len)
					{
						if (i < values[prev_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (i == values[prev_idx])
						{
							String_append(&str,
								UP_AND_LEFT);
						}
						else
						{
							String_append(&str,
								VERTICAL);
						}
					}
					else
					{
						if (i < values[prev_idx]
							&& i < values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (i != values[prev_idx]
							&& i != values[next_idx])
						{
							String_append(&str,
								VERTICAL);
						}
						else if (i == values[prev_idx]
							&& i == values[next_idx])
						{
							String_append(&str,
								UP_AND_HORIZONTAL);
						}
						else if (i == values[prev_idx]
							&& i < values[next_idx])
						{
							String_append(&str,
								UP_AND_LEFT);
						}
						else if (i == values[prev_idx]
							&& i > values[next_idx])
						{
							String_append(&str,
								VERTICAL_AND_LEFT);
						}
						else if (i < values[prev_idx]
							&& i == values[next_idx])
						{
							String_append(&str,
								UP_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								VERTICAL_AND_RIGHT);
						}
					}
				}
				else
				{
					if (prev_idx == 0 && next_idx == 0)
					{
						if (i > values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (i == values[next_idx])
						{
							String_append(&str,
								DOWN_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								VERTICAL);
						}
					}
					else if (next_idx == values_len)
					{
						if (i > values[prev_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (i == values[prev_idx])
						{
							String_append(&str,
								DOWN_AND_LEFT);
						}
						else
						{
							String_append(&str,
								VERTICAL);
						}
					}
					else
					{
						if (i > values[prev_idx]
							&& i > values[next_idx])
						{
							String_append_char(&str, ' ');
						}
						else if (i != values[prev_idx]
							&& i != values[next_idx])
						{
							String_append(&str,
								VERTICAL);
						}
						else if (i == values[prev_idx]
							&& i == values[next_idx])
						{
							String_append(&str,
								DOWN_AND_HORIZONTAL);
						}
						else if (i == values[prev_idx]
							&& i < values[next_idx])
						{
							String_append(&str,
								VERTICAL_AND_LEFT);
						}
						else if (i == values[prev_idx]
							&& i > values[next_idx])
						{
							String_append(&str,
								DOWN_AND_LEFT);
						}
						else if (i < values[prev_idx]
							&& i == values[next_idx])
						{
							String_append(&str,
								VERTICAL_AND_RIGHT);
						}
						else
						{
							String_append(&str,
								DOWN_AND_RIGHT);
						}
					}
				}
			}
		}
		if (i > bottom)
		{
			String_append_char(&str, '\n');
		}
	}

	return str.data;
}
