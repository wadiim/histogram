#include "histogram.h"
#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Layout { HORIZONTAL, VERTICAL };

int main(int argc, char **argv)
{
	int i;
	char *hist;

	int layout = HORIZONTAL;
	char* opt = NULL;
	char* arg = NULL;

	int buffsize = 32;
	int input_len = 0;
	int *input = malloc(buffsize*sizeof(int));
	if (input == NULL)
	{
		perror(argv[0]);
		exit(errno);
	}

	/* Parse arguments. */
	for (i = 1; i < argc; ++i)
	{
		arg = NULL;
		if (strcmp(argv[i], "--") == 0)
		{
			break;
		}
		else if (starts_with("--", argv[i]) == false)
		{
			continue;
		}
		arg = strchr(argv[i], '=');
		if (arg != NULL)
		{
			opt = malloc((arg-argv[i]+1)*sizeof(char));
			memcpy(opt, argv[i], arg-argv[i]);
			opt[arg-argv[i]] = '\0';
			++arg;
		}
		else
		{
			opt = malloc((strlen(argv[i])+1)*sizeof(char));
			strcpy(opt, argv[i]);
			if (i < argc-1 && starts_with("--", argv[i+1]) == false)
			{
				arg = argv[i+1];
			}
		}

		if (strcmp(opt, "--layout") == 0)
		{
			if (arg == NULL)
			{
				fprintf(stderr, "%s: Argument missing after: "\
						"'%s'\n", argv[0], opt);
				exit(EXIT_FAILURE);
			}
			else if (strcmp(arg, "vertical") == 0)
			{
				layout = VERTICAL;
			}
			else if (strcmp(arg, "horizontal") == 0)
			{
				layout = HORIZONTAL;
			}
			else
			{
				fprintf(stderr, "%s: Invalid argument '%s' "\
						"for '%s'\nValid arguments "\
						"are: 'horizontal', "\
						"'vertical'\n", argv[0],
						arg, opt);
				exit(EXIT_FAILURE);
			}
		}

		free(opt);
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

	if (layout == HORIZONTAL)
	{
		hist = generate_horizontal_histogram(input, input_len, '#');
	}
	else
	{
		hist = generate_vertical_histogram(input, input_len, '#');
	}
	printf("%s\n", hist);

	free(hist);
	free(input);

	return 0;
}
