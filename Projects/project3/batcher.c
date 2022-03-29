#include "stats.h" /*allows us to use cmp() and swap() and move()*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void comparator(Stats *stats, uint32_t A[], uint32_t x, uint32_t y) {

    if (cmp(stats, A[x], A[y]) == 1) { /*compares vars and adds 1 to number of compares*/
        swap(stats, &A[x],
            &A[y]); /*swaps vars and and 3 to number of moves, because must store in temp var before moving*/
    }
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t length) {
    if (length == 0) {
        return;
    }
    uint32_t n = length;
    uint32_t count = 0;
    for (uint32_t number = n; number >= 1;
         number /= 2) { /*converts the length variable into bit length*/
        count++;
    }

    uint32_t p = 1 << (count - 1); /*bitshift left amount of even indices in array*/

    while (p > 0) {
        uint32_t q = 1 << (count - 1);
        uint32_t r = 0;
        uint32_t d = p;

        while (d > 0) {
            for (uint32_t i = 0; i < (n - d); i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q = q >> 1;
            r = p;
        }
        p = p >> 1;
    }
}
