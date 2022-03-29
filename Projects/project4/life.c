#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define OPTIONS "tsn:i:o:"
#define DELAY   50000

void usage(char *exec) { /*usage error function which I reused from last assignment*/
    fprintf(stderr,
        "\n"
        "Synopsis:\n"
        "----Builds a universe based on user input and simulates The Game of Life"
        "\n"
        "USAGE\n"
        "----%s [-tsn:i:o:] [-i lists/acorn.txt -n 100 -t -s]\n"
        "\n" /*all command line options listed*/
        "OPTIONS\n"
        "    -t        Make universe toroidal (donut shaped)\n"
        "    -s        Silence animation\n"
        "    -n        Number of generations\n"
        "    -i        Enter user infile\n"
        "    -o Enter user outfile\n"
        "\n",
        exec);
    return;
}

int main(int argc, char **argv) {
    uint32_t generations = 100;
    int opt;
    bool toroidal = false;
    bool animate = true;
    FILE *input = stdin;
    FILE *output = stdout;
    uint32_t rows;
    uint32_t cols;
    bool file_selected = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': animate = false; break;
        case 'n': generations = strtoul(optarg, NULL, 10); break;
        case 'i':
            file_selected = true;
            input = fopen(optarg, "r");
            break;
        case 'o': output = fopen(optarg, "w"); break;

        default:
            usage(argv[0]);
            return EXIT_FAILURE;
            break;
        }
    }

    if (file_selected == false) {
        printf("Error: No File Inputted\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (input == NULL) {
        printf("Error: File Empty\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    fscanf(input, "%" SCNu32 " %" SCNu32 "\n", &rows, &cols);
    Universe *A = uv_create(rows, cols, toroidal);
    uv_populate(A, input);
    Universe *B = uv_create(rows, cols, toroidal);
    Universe *temp;
    if (animate == false) {
        for (uint32_t gen = 0; gen < generations; gen++) {
            for (uint32_t r = 0; r < uv_rows(A); r++) {
                for (uint32_t c = 0; c < uv_cols(A); c++) {

                    if ((uv_get_cell(A, r, c) == true)
                        && ((uv_census(A, r, c) == 2) || (uv_census(A, r, c) == 3))) {
                        uv_live_cell(B, r, c);
                    } else if ((uv_get_cell(A, r, c) == false) && (uv_census(A, r, c) == 3)) {
                        uv_live_cell(B, r, c);
                    } else {
                        uv_dead_cell(B, r, c);
                    }
                }
            }
            temp = B;
            B = A;
            A = temp;
        }
    }
    if (animate == true) {
        initscr();
        curs_set(FALSE);
        clear();
        for (uint32_t gen = 0; gen <= generations; gen++) {
            for (uint32_t r = 0; r < uv_rows(A); r++) {
                for (uint32_t c = 0; c < uv_cols(A); c++) {
                    if ((uv_get_cell(A, r, c) == true)
                        && ((uv_census(A, r, c) == 2) || (uv_census(A, r, c) == 3))) {
                        uv_live_cell(B, r, c);
                        mvprintw(r, c, "o");
                    } else if ((uv_get_cell(A, r, c) == false) && (uv_census(A, r, c) == 3)) {
                        uv_live_cell(B, r, c);
                        mvprintw(r, c, "o");
                    } else {
                        uv_dead_cell(B, r, c);
                        mvprintw(r, c, " ");
                    }
                    refresh();
                }
                usleep(DELAY);
            }
            temp = B;
            B = A;
            A = temp;
        }
        endwin();
    }
    uv_print(B, output);
    uv_delete(A);
    uv_delete(B);
    fclose(input);
    fclose(output);
    return 0;
}
