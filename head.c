/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2019, Jakob Kaivo <jkk@ung.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _POSIX_C_SOURCE 2
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static int head(const char *path, int nlines)
{
	FILE *f = stdin;
	if (path && strcmp(path, "-")) {
		f = fopen(path, "rb");
	}

	if (f == NULL) {
		fprintf(stderr, "head: %s: %s\n", path, strerror(errno));
		return 1;
	}

	int c;
	while ((c = fgetc(f)) != EOF && nlines > 0) {
		putchar(c);
		if (c == '\n') {
			nlines--;
		}
	}

	if (f != stdin) {
		fclose(f);
	}

	return 0;
}

static void fixobsolete(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			return;
		}

		if (!strcmp(argv[i], "--")) {
			return;
		}

		if (isdigit(argv[i][1])) {
			fprintf(stderr, "head: '-#' is obsolete; use '-n #'\n");
			char *opt = malloc(strlen(argv[i]) + 2);
			if (opt == NULL) {
				perror("strings");
				exit(1);
			}
			sprintf(opt, "-n %s", argv[i] + 1);
			argv[i] = opt;
		}
	}
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");

	int lines = 10;
	int showname = 0;
	char *end;

	int c;
	fixobsolete(argc, argv);
	while ((c = getopt(argc, argv, "n:")) != -1) {
		switch (c) {
		case 'n':
			lines = strtol(optarg, &end, 10);
			if (*end != '\0') {
				fprintf(stderr, "head: invalid number %s\n", optarg);
				return 1;
			}
			break;

		default:
			return 1;
		}
	}

	if (optind < argc - 1) {
		showname = 1;
	}

	int r = 0;
	do {
		if (showname) {
			if (showname == 1) {
				showname++;
			} else {
				putchar('\n');
			}
			printf("==> %s <==\n", argv[optind]);
		}
		r |= head(argv[optind++], lines);
	} while (optind < argc);

	return r;
}
