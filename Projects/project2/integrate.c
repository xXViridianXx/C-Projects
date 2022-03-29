#include "functions.h"
#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "-abcdefghijp:q:n:H"
/* pops up when user needs help*/
void usage(char *exec) {
    fprintf(stderr,
        "\n"
        "Synopsis:\n"
        "----integrates function using the Composite Simpson's 1/3 rule"
        "\n"
        "USAGE\n"
        "----%s [-abcdefghijn:p:q:H] [-a -p lower -q upper -n partitions]\n"
        "\n"
        "OPTIONS\n"
        "    -H            display program help and usage\n"
        "    -[a-j]        sets function to integrate based on letter\n"
        "    -p lower sets lower bound for function\n"
        "    -q upper sets upper bound for function\n"
        "    -n partitions sets number of partitions\n"
        "\n",
        exec);
    return;
}
/*pops up when user inputs wrong bound*/
void functions(char *e[]) {
    printf("	Equations:\n"
           "		-a: %s bounds: [-1, 1]\n"
           "		-b: %s bounds: [0, 1) U (1, inf)\n"
           "		-c: %s bounds: (-inf, inf)\n"
           "		-d: %s bounds: (-inf, inf)\n"
           "		-e: %s bounds: (-inf, inf)\n"
           "		-f: %s bounds: (1, inf)\n"
           "		-g: %s bounds: (-inf, inf)\n"
           "		-h: %s bounds: (-inf, 0) U (0, inf)\n"
           "		-i: %s bounds: (-inf, 2)\n"
           "		-j: %s bounds: (-inf, inf)\n",
        e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7], e[8], e[9]);
    return;
}

/* argc = # of args passed */
/* **argv = array of char pointers; for char strings as args*/
int main(int argc, char **argv) {
    double (*table[])(double)
        = { a, b, c, d, e, f, g, h, i, j }; /*Eugene's section game me this idea*/
    char *equations[10] = { "sqrt(1 - x^4)", "1/log(x)", "e^-x2", "sin(x^2)", "cos(x^2)",
        "log(log(x))", "sin(x)/ x", "e^-x/x", "e^e^x",
        "sqrt(sin(x)^2 + cos(x)^2)" }; /*so I can use values in multiple places*/
    bool input[10] = { false }; /*default values for error handling*/
    double up, low = 1e-14;
    uint32_t defaultPartitions = 100;
    uint32_t partitions = 0;
    int opt;
    /*keep looping searching for options, if can't find = -1; if -1 then break */
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        /*looking at each option*/
        switch (opt) {
        case 'a': input[0] = true; break;
        case 'b': input[1] = true; break;
        case 'c': input[2] = true; break;
        case 'd': input[3] = true; break;
        case 'e': input[4] = true; break;
        case 'f': input[5] = true; break;
        case 'g': input[6] = true; break;
        case 'h': input[7] = true; break;
        case 'i': input[8] = true; break;
        case 'j': input[9] = true; break;
        case 'p': low = (double) strtod(optarg, NULL); break;
        case 'q': up = (double) strtod(optarg, NULL); break;
        case 'n': partitions = (uint32_t) strtod(optarg, NULL); break;
        default: /*give error and usage if wrong input*/
            usage(argv[0]);
            return EXIT_FAILURE;
            break;
        }
    }

    bool checker = false;
    uint32_t step = 2;
    uint32_t start = 2;
    for (int count = 0; count < 10; count++) {
        checker = checker || input[count]; /*checks if user inputted equation*/
    }
    if (checker == false) {
        fprintf(stderr,
            "Error: Missing Equation -a-j\n"); /*if not then display error and usage with functions*/
        functions(equations);
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (low == 1e-14 || up == 1e-14) {
        printf("Error: Upper or Lower Bound Not Inputed;\n");
        usage(argv[0]); /*checks if user inputted bounds*/
        return EXIT_FAILURE;
    }
    if (partitions == 0) {
        printf(
            "Error: Partitions Not Inputed; Default Value Used.\n"); /*checks is partitions entered; use default if not*/
        partitions = defaultPartitions;
    }

    if (input[0] && (low < -1 || up > 1)) {
        printf("Invalid bound for %s\n", equations[0]); /*checks for bounds*/
        functions(equations);
        return 1;
    }
    if (input[1] && (low < 0 || up < 0 || (low == 1 || up == 1))) {
        printf("Invalid bound for %s\n", equations[1]);
        functions(equations);
        return 1;
    }
    if (input[5] && (low <= 1 || up <= 1)) {
        printf("Invalid bound for %s\n", equations[1]);
        functions(equations);
        return 1;
    }

    if (partitions % 2 == 1) {
        start = 1;
    }
    for (int count = 0; count < 10; count++) {
        if (input[count]) {
            printf("%s,%f,%f,%d\n", equations[count], low, up,
                partitions); /*prints on to console for user to see*/
            while (start <= partitions) {
                printf("%u,%.15lf\n", start, integrate(table[count], low, up, start));
                start += step;
            }
        }
    }
    return 0;
}
