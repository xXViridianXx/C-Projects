#include "ht.h"
#include <string.h>
#include <stdlib.h>
#include "node.h"
#include <stdio.h>
#include "speck.h"
#include "salts.h"

//creating hashtable struct
struct HashTable {
    //defining vars
    uint64_t salt[2];
    uint32_t size;
    Node **slots;
    uint32_t current_size;
};
//creating iterator struct
struct HashTableIterator {
    //defining vars
    HashTable *table;
    uint32_t slot;
};
//creating hashtable
HashTable *ht_create(uint32_t size) {
    //allocating size of hastable
    HashTable *hashtable = malloc(sizeof(HashTable));
    //setting vars
    hashtable->size = size;
    hashtable->current_size = 0;
    //setting salts
    hashtable->salt[0] = SALT_HASHTABLE_LO;
    hashtable->salt[1] = SALT_HASHTABLE_HI;
    //allocating memory to node array
    hashtable->slots = (Node **) calloc(hashtable->size, sizeof(Node)); //allocate array of nodes
    return hashtable;
}
//deleting hastable
void ht_delete(HashTable **ht) {
    //deleting every node in hastable
    for (uint32_t i = 0; i < ht_size(*ht); i++) {
        node_delete(&((*ht)->slots[i]));
    }
    //delete node array and hashtable
    free((*ht)->slots);
    (*ht)->slots = NULL;
    free(*ht);
    *ht = NULL;
}
//return size
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *word) {
    //function taken from lecture slides
    //getting index with regards to hastable size
    uint32_t index = hash(ht->salt, word) % ht_size(ht);
    uint32_t count = 0;

    while ((count < ht_size(ht)) && (ht->slots[index] != NULL)) {
        //look through node array
        Node *data = ht->slots[index];
        //if looked up string same, return it
        if (data && strcmp(data->word, word) == 0) {
            return data;
        }
        //increment index by 1;
        index = (index + 1) % ht_size(ht);
        count += 1;
    }
    //if not found, return null node
    return (Node *) NULL;
}

Node *ht_insert(HashTable *ht, char *word) {
    //taken from lecture slides
    uint32_t index = hash(ht->salt, word) % ht_size(ht);
    uint32_t count = 0;
    Node *found = NULL;
    //if alraedy in hastable, increase count
    if ((found = ht_lookup(ht, word))) {
        found->count += 1;
        return found;
    }
    //while count less than size
    while (count < ht_size(ht)) {
        //if location null, insert
        if (ht->slots[index] == NULL) {
            ht->slots[index] = node_create(word);
            (ht->slots[index])->count += 1;
            ht->current_size += 1;
            return ht->slots[index];
        }
        //if not null, increment index
        index = (index + 1) % ht_size(ht);
        count += 1;
    }
    return (Node *) NULL;
}

void ht_print(HashTable *ht) {
    uint32_t count = 0;
    //prints hastable
    while (count < ht_size(ht)) {
        node_print(ht->slots[count]);
        count += 1;
    }
}
//creates hashtable iterator
HashTableIterator *hti_create(HashTable *ht) {
    //allocating memory
    HashTableIterator *hti = malloc(sizeof(HashTableIterator));
    //setting vars
    hti->table = ht;
    hti->slot = 0;
    return hti;
}

void hti_delete(HashTableIterator **hti) {
    //only freeing hastable iterator
    free(*hti);
    *hti = NULL;
}

Node *ht_iter(HashTableIterator *hti) {
    static uint32_t count = -1;
    static uint32_t found = 0;
    //everytime iter called, count incremented
    count += 1;
    //stop iterating once size reached
    if (found == (hti->table)->current_size) {
        count = 0;
        found = 0;
        return (Node *) NULL;
    }
    //if null, skip over
    while ((hti->table)->slots[count] == NULL) {
        count += 1;
    }
    //else increment found and return node
    found += 1;
    return (hti->table)->slots[count];
}

/*int main (void) { 
	HashTable *ht = ht_create(6);
	HashTableIterator *hti = hti_create(ht);
	char *pika = "pika";
	char *gar = "chomp";
	char *swampart = "swamp";
	char *regi = "ice";
	//char *onix = "onix";
	//char *ralts = "ralts";
	ht_insert(ht, pika);
	ht_insert(ht, gar);
	ht_insert(ht, regi);
	ht_insert(ht, swampart);
	//ht_insert(ht, ralts);
	//ht_insert(ht, onix);
	
	//Node *found  =  ht_lookup(ht, gar);
	
	ht_print(ht);
	
	printf("\n");
	Node *n = NULL;
	uint32_t found = 0;
	printf("current size = %d\n", ht->current_size);
	while ((n = ht_iter(hti)) != NULL) {
		found +=1;
		printf("%s\n", n->word);
	}

	//printf("\nfound node\n");
	//node_print(found);
	
	//Node *found1  =  ht_lookup(ht, ralts);
	
	//printf("\nsearching?\n");
	//node_print(found1);

	

	hti_delete(&hti);
	ht_delete(&ht);

	

	return 0;
}*/
