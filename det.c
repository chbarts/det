#include <math.h>
#include <fenv.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <getopt.h>
#include "determinant.h"

#include <errno.h>

extern int optind;

#define CLEAR() do { if (feclearexcept(FE_ALL_EXCEPT) != 0) { perror("feclearexcept()"); } errno = 0; } while (0)

static void help(char name[])
{
    printf("%s [options...] [[a b] [c d]] or [[a b c] [d e f] [g h i]]\n",
           name);
    puts("Note: Do not use brackets, only put the numbers");
    puts("Options:");
    puts("--2x2 | -2              Do a 2x2 determinant");
    puts("--3x3 | -3              Do a 3x3 determinant");
    puts("--help | -h             Print this help");
    puts("--version | -v          Print version information");
}

static void version(void)
{
    puts("det version 0.1");
    puts("Copyright 2016, Chris Barts");
    puts("Licensed under the GNU General Public License, version 3.0 or, at your option, any later version.");
    puts("This software has NO WARRANTY, even for USABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
}

int main(int argc, char *argv[])
{
    int i, j, c, lind, nargs = 0;
    char *pnam, *ptr;
    ld args[9];
    struct option longopts[] = {
        {"2x2", 0, 0, 0},
        {"3x3", 0, 0, 0},
        {"help", 0, 0, 0},
        {"version", 0, 0, 0},
        {0, 0, 0, 0}
    };

    pnam = argv[0];

    if (argc == 1) {
        help(pnam);
        exit(EXIT_SUCCESS);
    }

    while ((c = getopt_long(argc, argv, "23hv", longopts, &lind)) != -1) {
        switch (c) {
        case 0:
            switch (lind) {
            case 0:
                nargs = 4;
                break;
            case 1:
                nargs = 9;
                break;
            case 2:
                help(pnam);
                exit(EXIT_SUCCESS);
                break;
            case 3:
                version();
                exit(EXIT_SUCCESS);
                break;
            default:
                help(pnam);
                exit(EXIT_FAILURE);
                break;
            }

            break;
        case '2':
            nargs = 4;
            break;
        case '3':
            nargs = 9;
            break;
        case 'h':
            help(pnam);
            exit(EXIT_SUCCESS);
            break;
        case 'v':
            version();
            exit(EXIT_SUCCESS);
            break;
        default:
            help(pnam);
            exit(EXIT_FAILURE);
            break;

        }
    }

    if (0 == nargs) {
        fprintf(stderr, "%s: must specify matrix size\n", pnam);
        help(pnam);
        exit(EXIT_FAILURE);
    }

    if ((optind + nargs - 1) >= argc) {
        fprintf(stderr,
                "%s: insufficient number of parameters for options given\n",
                pnam);
        help(pnam);
        exit(EXIT_FAILURE);
    }

    for (i = optind, j = 0; (i < argc) && (j < nargs); i++) {
        args[j] = strtold(argv[i], &ptr);
        if ((argv[i] == ptr) && (0.0 == args[j])) {
            fprintf(stderr, "%s: %s is invalid\n", pnam, argv[i]);
            continue;
        }

        if (ERANGE == errno) {
            if (args[j] == HUGE_VALL) {
                fprintf(stderr, "%s: %s caused overflow\n", pnam, argv[i]);
            } else if (args[j] == -HUGE_VALL) {
                fprintf(stderr, "%s: %s caused negative overflow\n",
                        pnam, argv[i]);
            } else {
                fprintf(stderr, "%s: %s caused underflow\n", pnam,
                        argv[i]);
            }

            CLEAR();
            continue;
        }

        j++;
    }

    if (j != nargs) {
        fprintf(stderr,
                "%s: not enough valid arguments (wanted %d, got %d)\n",
                pnam, nargs, j);
        help(pnam);
        exit(EXIT_FAILURE);
    }

    if (4 == nargs) {
        printf("%.*Lg\n", LDBL_DIG,
               det2x2(args[0], args[1], args[2], args[3]));
    } else {
        printf("%.*Lg\n", LDBL_DIG, det3x3(args[0], args[1], args[2],
                                           args[3], args[4], args[5],
                                           args[6], args[7], args[8]));
    }

    return 0;
}
