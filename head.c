/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2017, Jakob Kaivo <jkk@ung.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
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
