/* pick.c -- program to make a selection for the shell.
 * (Idea based on pick program from The UNIX programming
 * environment, Kernighan&Pike)
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Thu Apr  6 11:09:55 AM EEST 2023
 * Copyright: (c) 2023 Luis Colorado.  All rights reserved.
 * License: BSD.
 */

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD          (0)
#define SUB          ('y' ^ 'n')

char **pick(
        char **argv_r,
        int    defop,
        int    add_sub)
{
    char **saved  = argv_r,
         **argv_l = argv_r;

    for (; *argv_r; argv_r++) {
        char line[64],
            *aux;
        do {
            fprintf(stderr, "%s? ", *argv_r);
            if (!fgets(line, sizeof line, stdin))
                return NULL;
            aux = strtok(line, " \t\n");
        } while (aux && (aux = strchr("yYnN", *aux)) == NULL);

        int op = (aux ? tolower(*aux) : defop) ^ add_sub;

        if (op == 'y') { /* add */
            if (argv_l != argv_r)
                *argv_l = *argv_r;
            argv_l++;
        }
    }
    /* copy last null */
    if (argv_l != argv_r) {
        *argv_l = *argv_r;
    }
    return saved;
}

int main(int argc, char **argv)
{
    int opt,
        def     = 'y',
        add_sub = 0;

    while ((opt = getopt(argc, argv, "asny")) != EOF) {
        switch (opt) {
        case 'a': add_sub = ADD; break;
        case 's': add_sub = SUB; break;
        case 'n': def     = 'n'; break; /* 'default' is no */
        case 'Y': def     = 'y'; break; /* 'default' is yes */
        }
    }
    argc -= optind; argv += optind;

    argv = pick(argv, def, add_sub);

    for (;*argv; argv++)
        printf("%s\n", *argv);

    return 0;
}
