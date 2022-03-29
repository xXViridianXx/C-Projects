#include "bv.h"
#include "stdlib.h"
#include "stdio.h"

//struct for bitvector
struct BitVector {
    uint32_t length;
    int8_t *vector;
};
//creates bitvector
BitVector *bv_create(uint32_t length) {
    //allocates memory
    BitVector *bv = malloc(sizeof(BitVector));
    bv->length = length;
    //allocates minimum bytes required for length inputted
    //formula taken from Eugene's section
    bv->vector = calloc((length + 7) / 8, sizeof(int8_t));
    if (bv->vector) { // checking if item is non-zero
        return bv;
    }
    free(bv); //if null free and return 0
    bv = NULL;
    return (BitVector *) 0;
}
//deletes bv
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    (*bv)->vector = NULL;
    free(*bv);
    *bv = NULL;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > (bv->length - 1)) {
        return false;
    }
    bv->vector[i / 8] |= (1 << (i % 8));
    return true;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length - 1) {
        return false;
    }
    bv->vector[i / 8] &= ~(1 << i % 8);
    return true;
}

bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length - 1) {
        return false;
    }
    return 1 & (bv->vector[i / 8] >> (i % 8));
}

void bv_print(BitVector *bv) {
    //	printf("bvlength =  %d\n", bv_length(bv));
    for (int i = bv_length(bv) - 1; i > -1; i--) {
        //printf("i = %d\n", i);
        printf("%d", bv_get_bit(bv, i));
        if (i % 8 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}

/*int main(void) {
	
	BitVector *bv = bv_create(8);
	bv_set_bit(bv, 7);
	bv_set_bit(bv, 2);
	bv_print(bv);
	//printf("bit at 7 = %d\n", bv_get_bit(bv, 6));


	//bv_print(bv);
	//bv_clr_bit(bv, 6);
	//bv_print(bv);
	bv_delete(&bv);
	return 0;
}*/
