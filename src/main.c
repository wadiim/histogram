#include "histogram.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *hist;
	int buffsize = 32;
	int input_len = 0;
	int *input = malloc(buffsize*sizeof(int));
	if (input == NULL)
	{
		perror(argv[0]);
		exit(errno);
	}

	/* Get input. */
	while (scanf("%d", &input[input_len]) == 1)
	{
		++input_len;
		if (input_len >= buffsize)
		{
			input = realloc(input, (buffsize*2)*sizeof(int));
			if (input == NULL)
			{
				perror(argv[0]);
				exit(errno);
			}
			buffsize *= 2;
		}
	}

	/* Free up unused memory. */
	input = realloc(input, input_len*sizeof(int));
	if (input == NULL && input_len > 0)
	{
		perror(argv[0]);
		exit(errno);
	}

	hist = generate_horizontal_histogram(input, input_len, '#');
	printf("%s\n", hist);

	free(hist);
	free(input);

	return 0;
}
