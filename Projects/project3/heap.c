#include "stats.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint32_t max_child(
    Stats *stats, uint32_t A[], uint32_t first, uint32_t last) { /*finds largest child and returns*/
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && (cmp(stats, A[right - 1], A[left - 1]) == 1)) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t A[], uint32_t first, uint32_t last) { /*sorts array*/
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);
    uint32_t range = last / 2;

    while ((mother <= range) && (found == false)) {
        if (cmp(stats, A[great - 1], A[mother - 1]) == 1) { /*swap mother and greater child*/
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = true;
        }
    }
}
void build_heap(Stats *stats, uint32_t A[], uint32_t first, uint32_t last) { /*creates max heap*/
    uint32_t range = last / 2;
    for (uint32_t father = range; father > first - 1; father--) {
        fix_heap(stats, A, father, last);
    }
}

void heap_sort(Stats *stats, uint32_t A[], uint32_t length) {
    uint32_t first = 1;
    uint32_t last = length;
    build_heap(stats, A, first, last);
    for (uint32_t leaf = last; leaf > first; leaf--) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf - 1);
    }
}
