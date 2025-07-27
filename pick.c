/* pick.c -- program to make a selection for the shell.
 * (Idea based on pick program from The UNIX programming
 * environment, Kernighan&Pike)
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Thu Apr  6 11:09:55 AM EEST 2023
 * Copyright: (c) 2023 Luis Colorado.  All rights reserved.
 * License: BSD.
 */

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F(_fmt)  "%s:%d:%s: "_fmt, __FILE__, __LINE__, __func__

void pick(char *arg, FILE *io)
{
	char               line[1024];
	char              *s;
	static const char *ynq = "ynq";

	do {
		fprintf(io, "%s? ", arg);
		fflush(io);

		if (!fgets(line, sizeof line, io)) {
			fprintf(stderr,
				F("EOF on interactive input descriptor\n"));
			exit(EXIT_FAILURE);
		}
		s = strtok(line, " \t\n");
		if (s && !strchr(ynq, tolower(s[0]))) {
			fprintf(io,
				F("Please, answer only [%s]\n"),
				ynq);
		}
	} while (!s || !strchr(ynq, tolower(s[0])));
	/* s && strchr(ynq, tolower(s[0])) */

	switch (s[0]) {
	case 'y': puts(arg);          break;
	case 'n':                     break;
	case 'q': exit(EXIT_SUCCESS); break;
	}
} /* pick */

int main(int argc, char **argv)
{
	int opt;
	char *sel_input = "/dev/tty",
	     *dat_input = NULL;

	while ((opt = getopt(argc, argv, "t:i:")) != EOF) {
		switch (opt) {
		case 't': sel_input = optarg; break;
		case 'i': dat_input = optarg; break;
		}
	}
	argc -= optind;
	argv += optind;

	FILE *ans_in = fopen(sel_input, "r+");
	if (!ans_in) {
		fprintf(stderr, F("%s: %s\n"),
			sel_input, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (argc) {
		/* choices are program parameters */
		for (int i = 0; i < argc; i++) {
			pick(argv[i], ans_in);
		}
	} else {
		char line[1024];
		/* choices are read from input file */
		FILE *dat_in = stdin;
		if (dat_input) {
			dat_in = fopen(dat_input, "r");
			if (!dat_in) {
				fprintf(stderr, F("%s: %s\n"),
					dat_input,
					strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
		while (fgets(line, sizeof line, dat_in)) {
			char *choice = strtok(line, "\n");
			pick(choice, ans_in);
		}
	}

    return 0;
}
