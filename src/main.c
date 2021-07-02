#include "histogram.h"
#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Layout
{
	HORIZONTAL,
	VERTICAL
};

typedef struct
{
	char c;
	int layout;
} Options;

/*
 * Parses command-line options and arguments. Returns dynamically allocated
 * structure that should be freed by the caller.
 */
static Options *parse_args(int argc, char **argv);

int main(int argc, char **argv)
{
	char *hist;
	Options *opts;
	int buffsize = 32;
	int input_len = 0;
	int *input = malloc(buffsize*sizeof(int));
	if (input == NULL)
	{
		perror(argv[0]);
		exit(errno);
	}

	opts = parse_args(argc, argv);

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

	if (opts->layout == HORIZONTAL)
	{
		hist = generate_horizontal_histogram(input, input_len,
				opts->c);
	}
	else
	{
		hist = generate_vertical_histogram(input, input_len,
				opts->c);
	}
	printf("%s\n", hist);

	free(hist);
	free(opts);
	free(input);

	return 0;
}

static Options *parse_args(int argc, char **argv)
{
	int i;
	char* opt = NULL;
	char* arg = NULL;

	Options *opts = malloc(sizeof(Options));
	if (opts == NULL)
	{
		perror(argv[0]);
		exit(errno);
	}
	opts->c = '#';
	opts->layout = HORIZONTAL;

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
			int opt_len = arg - argv[i];
			opt = malloc((opt_len+1)*sizeof(char));
			memcpy(opt, argv[i], opt_len);
			opt[opt_len] = '\0';
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
				opts->layout = VERTICAL;
			}
			else if (strcmp(arg, "horizontal") == 0)
			{
				opts->layout = HORIZONTAL;
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
		else if (strcmp(opt, "--char") == 0)
		{
			if (arg == NULL)
			{
				fprintf(stderr, "%s: Argument missing after: "\
						"'%s'\n", argv[0], opt);
				exit(EXIT_FAILURE);
			}
			else if (strlen(arg) == 1)
			{
				opts->c = arg[0];
			}
			else
			{
				fprintf(stderr, "%s: Invalid argument '%s' "\
						"for '%s'\nShould be a "\
						"single character\n",
						argv[0], arg, opt);
				exit(EXIT_FAILURE);
			}
		}

		free(opt);
	}

	return opts;
}
