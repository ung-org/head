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

#define _XOPEN_SOURCE 700
#include <errno.h>
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
		fprintf(stderr, "FIXME: Couldn't open %s: %s\n", path ? path : "stdin", strerror(errno));
		return 1;
	}

	while (!feof(f) && nlines > 0) {
		char *line = NULL;
		size_t n = 0;

		getline(&line, &n, f);
		fputs(line, stdout);
		nlines--;
	}

	if (f != stdin) {
		fclose(f);
	}

	return 0;
}

int main(int argc, char **argv)
{
	int lines = 10;
	int showname = 0;
	char *end;

	int c;
	while ((c = getopt(argc, argv, ":n:")) != -1) {
		switch (c) {
		case 'n':
			lines = strtol(optarg, &end, 10);
			if (end != NULL && strlen(end) > 0) {
				fprintf(stderr, "head: Invalid number %s\n", optarg);
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
