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
	bool help;
	int layout;
	bool pretty;
} Options;

/*
 * Parses command-line options and arguments. Returns dynamically allocated
 * structure that should be freed by the caller.
 */
static Options *parse_args(int argc, char **argv);

/*
 * Reads integers from the standard input and stores them in the array 'input'.
 * The 'input' array is assumed to be NULL and should be freed by the caller.
 * Returns zero on success and non-zero on failure.
 */
static int get_input(int *input[], size_t *input_len);

static void show_usage(char *program_name);

int main(int argc, char **argv)
{
	char *hist;
	Options *opts;

	int *input = NULL;
	size_t input_len = 0;

	opts = parse_args(argc, argv);

	if (opts->help == true)
	{
		free(opts);
		exit(EXIT_SUCCESS);
	}

	if (get_input(&input, &input_len) != 0)
	{
		perror(argv[0]);
		exit(errno);
	}

	if (opts->layout == HORIZONTAL)
	{
		if (opts->pretty == true)
		{
			hist = generate_pretty_horizontal_histogram(input,
					input_len);
		}
		else
		{
			hist = generate_horizontal_histogram(input,
					input_len, opts->c);
		}
	}
	else
	{
		if (opts->pretty == true)
		{
			hist = generate_pretty_vertical_histogram(input,
					input_len);
		}
		else
		{
			hist = generate_vertical_histogram(input,
					input_len, opts->c);
		}
	}
	printf("%s\n", hist);

	free(hist);
	free(opts);
	free(input);

	return EXIT_SUCCESS;
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
	opts->help = false;
	opts->layout = HORIZONTAL;
	opts->pretty = false;

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
			ptrdiff_t opt_len = arg - argv[i];
			opt = malloc((opt_len + 1)*sizeof(char));
			memcpy(opt, argv[i], opt_len);
			opt[opt_len] = '\0';
			++arg;
		}
		else
		{
			opt = malloc((strlen(argv[i]) + 1)*sizeof(char));
			strcpy(opt, argv[i]);
			if (i + 1 < argc &&
				starts_with("--", argv[i + 1]) == false)
			{
				arg = argv[i + 1];
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
		else if (strcmp(opt, "--pretty") == 0)
		{
			opts->pretty = true;
		}
		else if (strcmp(opt, "--help") == 0)
		{
			show_usage(argv[0]);
			opts->help = true;
		}
		else
		{
			fprintf(stderr, "%s: Invalid option '%s'\n",
					argv[0], opt);
			exit(EXIT_FAILURE);
		}

		free(opt);
	}

	return opts;
}

static int get_input(int *input[], size_t *input_len)
{
	size_t buff_size = 32;

	*input_len = 0;
	*input = malloc(buff_size*sizeof(int));
	if (input == NULL)
	{
		return -1;
	}

	while (scanf("%d", &(*input)[*input_len]) == 1)
	{
		++(*input_len);
		if (*input_len >= buff_size)
		{
			*input = realloc(*input, buff_size*2*sizeof(int));
			if (*input == NULL)
			{
				return -1;
			}
			buff_size *= 2;
		}
	}

	*input = realloc(*input, (*input_len)*sizeof(int));
	if (*input == NULL && *input_len > 0)
	{
		return -1;
	}

	return 0;
}

static void show_usage(char *program_name)
{
	printf("\
Usage: %s [OPTIONS]\n\n\
Generate a histogram from integers given on stdin.\n\n\
Options:\n\
", program_name);
	printf("\
  --char <char>     Specify the ASCII character used for printing the \
histogram.\n\
                    This option has no effect when the '--pretty' \
option is given.\n\
");
	printf("\
  --help            Show help message and exit.\n\
");
	printf("\
  --layout <layout> Specify the layout of the bars in the histogram.\n\
                    <layout> can be 'horizontal' or 'vertical'.\n\
");
	printf("\
  --pretty          Pretty-print the histogram using Unicode Box \
Drawing Characters.\n\
                    This option requires a terminal with UTF-8 \
encoding to work properly.\n\
");
}
