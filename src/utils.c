#include "utils.h"

#include <stdlib.h>
#include <string.h>

#define STRING_INITIAL_CAPACITY 64

void String_init(String *str)
{
	str->size = 0;
	str->data = malloc(STRING_INITIAL_CAPACITY*sizeof(char));
	str->data[0] = '\0';
	str->capacity = STRING_INITIAL_CAPACITY;
}

void String_append(String *str, char *postfix)
{
	size_t postfix_len = strlen(postfix);

	while (str->size + postfix_len >= str->capacity)
	{
		str->data = realloc(str->data,
				2*str->capacity*sizeof(char));
		str->capacity *= 2;
	}

	memcpy(str->data + str->size, postfix, postfix_len);
	str->size += postfix_len;
	str->data[str->size] = '\0';
}

void String_append_char(String *str, char c)
{
	char tmp[2] = "\0";
	tmp[0] = c;
	String_append(str, tmp);
}

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
