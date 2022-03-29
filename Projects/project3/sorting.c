#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h" /*imports all the modules neccessary for the assignment*/
#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "ahbiqr:n:p:H"
typedef enum {
    HEAPSORT,
    BATCHERSORT,
    INSERTIONSORT,
    QUICKSORT
} Sorts; /*assigns names to integers; Heap = 0, Batch = 1, etc.*/
typedef enum { SEED, ELEMENTS, SIZE, HELP } Filters; /* I used Eugene's section for this*/

void usage(char *exec) { /*usage error function which I reused from last assignment*/
    fprintf(stderr,
        "\n"
        "Synopsis:\n"
        "----Builds a randomized array and sorts it based on user input"
        "\n"
        "USAGE\n"
        "----%s [-ahbiqr:n:p:H] [-q -n 1000 -p 0]\n"
        "\n" /*all command line options listed*/
        "OPTIONS\n"
        "    -H            display program help and usage\n"
        "    -h        heap sort\n"
        "    -b        batcher sort\n"
        "    -i        insertion sort\n"
        "    -q        quick sort\n"
        "    -r sets seed for randomizer\n"
        "    -p how many elements to print out\n"
        "    -n sets size of array\n"
        "\n",
        exec);
    return;
}

void print_sorted(
    uint32_t A[], uint32_t size, uint32_t elements) { /*prints the array in desired format*/
    uint32_t count = 0;
    uint32_t num_count = 0;
    if (elements > size) { /*if elements printed greater than size, print everything in array*/
        for (uint32_t i = 0; i < size; i++) {
            printf("%13" PRIu32, A[i]);
            count++;
            num_count++;
            if ((count == 5)
                || (num_count
                    == size)) { /*creates 5 columns, each time count == 5, make a new line and resent count*/
                printf("\n");
                count = 0;
            }
            /*num_count counts number of elements, and if last element and not multiple of 5, then create a new line*/
            else if ((count != 5)
                     && (num_count == size)) { /*does same format even if count not multiple of 5*/
                printf("\n");
            }
        }
    } else { /*if elements less than or equal to size, elements is upper bound of loop rather than size*/
        for (uint32_t i = 0; i < elements; i++) {
            printf("%13" PRIu32, A[i]);
            count++;
            num_count++;
            if (count == 5 || (num_count == elements)) {
                printf("\n");
                count = 0;
            } else if (
                (count != 5)
                && (num_count
                    == size)) { /*does same thing as top if statement; could have put it in function*/
                printf("\n");
            }
        }
    }
}

uint32_t *alloc(uint32_t *A, uint32_t seed,
    uint32_t
        size) { /*After doing malloc() once, this fills the memoery with array of pseudo-random numbers*/

    uint32_t bit_mask = 0x3FFFFFFF; /*bit mask*/
    srandom(seed); /*takes in user inputted or default seed*/
    for (uint32_t i = 0; i < size; i++) {
        A[i] = (random() & bit_mask); /*bitwise and to bitmask random number to 30 bits*/
    }
    return A; /*returns randmoized array*/
}

void algorithims(/*function that does sorting based on user input*/
    uint32_t A[], Set s, char *sorts[], uint32_t size, uint32_t elements, uint32_t seed) {
    Stats my_stats; /*uses stats module to set moves and compares*/
    my_stats.moves = 0;
    my_stats.compares = 0;
    /*Eugene did with switch cases, but gave me bad prints so used if statements*/
    for (Sorts o = HEAPSORT; o <= QUICKSORT;
         o++) { /*uses the Sorts enum to lop through the set until quicksort*/
        if (member_set(o, s)) { /*checks if sort user selected in set*/
            if (o == HEAPSORT) { /* if member and HEAPSORT, then run */
                heap_sort(&my_stats, A, size); /* runs heapsort*/
                printf("%s, %u elements, %lu moves, %lu compares\n", sorts[0], size, my_stats.moves,
                    my_stats.compares);
                print_sorted(A, size, elements); /*call function and prints array*/
                reset(&my_stats); /*resets move and compare*/
                free(A); /*deallocates memory*/
                A = alloc(A, seed, size); /*randomizes array again*/
            }
            if (o == BATCHERSORT) {
                batcher_sort(&my_stats, A, size);
                printf("%s, %u elements, %lu moves, %lu compares\n", sorts[1], size, my_stats.moves,
                    my_stats.compares);
                print_sorted(A, size, elements);
                reset(&my_stats);
                free(A);
                A = alloc(A, seed, size);
            }
            if (o == INSERTIONSORT) {
                insertion_sort(&my_stats, A, size);
                printf("%s, %u elements, %lu moves, %lu compares\n", sorts[2], size, my_stats.moves,
                    my_stats.compares);
                print_sorted(A, size, elements);
                reset(&my_stats);
                free(A);
                A = alloc(A, seed, size);
            }
            if (o == QUICKSORT) {
                quick_sort(&my_stats, A, size);
                printf("%s, %u elements, %lu moves, %lu compares\n", sorts[3], size, my_stats.moves,
                    my_stats.compares);
                print_sorted(A, size, elements);
                reset(&my_stats);
                free(A);
                A = alloc(A, seed, size);
            }
        }
    }
}

int main(int argc, char **argv) {
    Set s = empty_set(); /*creates empty set*/
    Set f = empty_set();
    int opt;
    uint32_t seed = 13371453;
    uint32_t elements = 100;
    uint32_t size = 100;
    char *sorts[4] = { "Heap Sort", "Batcher Sort", "Insertion Sort",
        "Quick Sort" }; /*default values and arrays so I can use in multiple places*/

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            s = insert_set(HEAPSORT, s); /*adds to set if user inputed command line option*/
            s = insert_set(BATCHERSORT, s);
            s = insert_set(INSERTIONSORT, s);
            s = insert_set(QUICKSORT, s);
            break;
        case 'h': s = insert_set(HEAPSORT, s); break;
        case 'b': s = insert_set(BATCHERSORT, s); break;
        case 'i': s = insert_set(INSERTIONSORT, s); break;
        case 'q': s = insert_set(QUICKSORT, s); break;
        case 'r':
            f = insert_set(
                SEED, f); /*add to other set only for filters done because of error checking*/
            seed = (uint32_t) strtod(optarg, NULL);
            break;
        case 'p':
            f = insert_set(ELEMENTS, f);
            elements = (uint32_t) strtod(optarg, NULL);
            break;
        case 'n':
            f = insert_set(SIZE, f);
            size = (uint32_t) strtod(optarg, NULL);
            break;
        case 'H':
            f = insert_set(SIZE, f);
            usage(argv[0]);
            return 1;
            break;
        default: /*give error and usage if wrong input*/
            usage(argv[0]);
            return EXIT_FAILURE;
            break;
        }
    }

    uint32_t checker = 0;
    for (int count = 0; count < 4; count++) {
        checker = checker || member_set(count, s);
    }
    if (checker == 0) { /*checks if user inputted a sorting method; reused from my assignment 2*/
        fprintf(stderr, "Error: Missing Sorting Algorithims\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    for (Filters i = SEED; i <= SIZE;
         i++) { /*checks for filters and informs user if didn't no input for them*/
        if (!member_set(i, f)) {
            if (i == SEED) {
                printf("No Seed Inputted; Default Used\n");
            }
            if (i == ELEMENTS) {
                printf("Number Of Elements To Print Not Specified; Default Used\n");
            }
            if (i == SIZE) {
                printf("No Size Inputted; Default Used\n");
            }
        }
    }

    uint32_t *A = (uint32_t *) malloc(
        size
        * sizeof(
            uint32_t)); /*allocates size * 4 bytes to create "size" amount of elements since each uint32 is 4 bytes*/
    algorithims(
        alloc(A, seed, size), s, sorts, size, elements, seed); /*calls  algorithims to run sort*/

    return 0;
}
