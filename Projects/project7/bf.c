#include "bf.h"
#include "bv.h"
#include "speck.h"
#include "salts.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//struct for bf
struct BloomFilter {
    //defining salts and filter
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};
//allocating and creating bf
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = malloc(sizeof(BloomFilter));
    //settign salts
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    //create bit vector
    bf->filter = bv_create(size);
    return bf;
}
//delete bf and bv
void bf_delete(BloomFilter **bf) {
    bv_delete(&((*bf)->filter));
    free(*bf);
    *bf = NULL;
}
//return size
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}
//insert something inside bf
void bf_insert(BloomFilter *bf, char *word) {
    //get 3 indexes using the salts
    uint32_t index1 = hash(bf->primary, word) % bf_size(bf);
    uint32_t index2 = hash(bf->secondary, word) % bf_size(bf);
    uint32_t index3 = hash(bf->tertiary, word) % bf_size(bf);
    //set the bit at those indexes
    bv_set_bit(bf->filter, index1);
    bv_set_bit(bf->filter, index2);
    bv_set_bit(bf->filter, index3);
}

bool bf_probe(BloomFilter *bf, char *word) {
    //hash again to get indexes
    uint32_t index1 = hash(bf->primary, word) % bf_size(bf);
    uint32_t index2 = hash(bf->secondary, word) % bf_size(bf);
    uint32_t index3 = hash(bf->tertiary, word) % bf_size(bf);
    //if all spots 1, retrn true; else false
    return (bv_get_bit(bf->filter, index1) & bv_get_bit(bf->filter, index2)
            & bv_get_bit(bf->filter, index3));
}
//print bf
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}

/*int main(void) {
	BloomFilter *bf = bf_create(10);
	printf("bloom filter size = %d\n", bf_size(bf));
	char *hello = "hello";
	char *bye = "bye";
	bf_insert(bf, hello);
	bf_print(bf);
	bool passed = bf_probe(bf, hello);
	if(passed == true) {
		printf("%s in bloomfilter\n", hello);
	}
	//assert(passed = true);
	passed = bf_probe(bf, bye);
	if(passed == false) {
		printf("%s not in bloomfilter\n", bye);
	}
	//assert(passed = true);
	bf_delete(&bf);
	return 0;
}*/
