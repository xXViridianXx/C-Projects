#include "text.h"
#include "ht.h"
#include "bf.h"
#include "bv.h"
#include "node.h"
#include "speck.h"
#include "salts.h"
#include "parser.h"
#include "metric.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

//regex statement
#define WORD "[a-zA-Z]+([-'][a-zA-Z]+)*" //with help from Ben Grant

uint32_t noiselimit;

//creating text struct
struct Text {
    //defining vars
    HashTable *ht;
    BloomFilter *bf;
    int32_t word_count;
};

//function to convert word to lower
//strlen found from manpages
//tolower gotten form audrey
char *lower(char *word) {
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        word[i] = tolower(word[i]);
    }
    return word;
}

//creates text, if null serves as noise text
Text *text_create(FILE *infile, Text *noise) {
    //init vars
    Text *text = malloc(sizeof(Text));
    text->ht = ht_create(pow(2, 19));
    text->bf = bf_create(pow(2, 21));
    text->word_count = 0;
    //check this if statement
    if (infile == NULL) {
        fprintf(stderr, "Nothing in file,");
        return (Text *) NULL;
    }

    if (text->ht && text->bf) { // checking if item is non-zero
        regex_t re;
        //compiing regex
        if (regcomp(&re, WORD, REG_EXTENDED)) {
            fprintf(stderr, "Failed to compile regex\n");
            //free if failed
            regfree(&re);
            free(text);
            text = NULL;
            return (Text *) NULL;
        }
        char *word = NULL;
        uint32_t limit = 0;
        //if noise NULL
        if (noise == NULL) {
            //parse through file and creae nodes
            while (limit < noiselimit && ((word = next_word(infile, &re)) != NULL)) {
                limit += 1;
                word = lower(word);
                text->word_count += 1;
                //insert into ht and bf
                ht_insert(text->ht, word);
                bf_insert(text->bf, word);
            }
            regfree(&re);
            return text;
        }
        //if noise !NULL
        while ((word = next_word(infile, &re)) != NULL) {
            word = lower(word);
            //continue if advised by Miles
            if (text_contains(noise, word)) {
                continue;
            }
            //increment word count
            text->word_count += 1;
            //insert into ht ad bf
            ht_insert(text->ht, word);
            bf_insert(text->bf, word);
        }
        regfree(&re);
        return text;
    }
    free(text); //if null free and return 0
    text = NULL;
    return (Text *) NULL;
}
//deletes text
void text_delete(Text **text) {
    //deletes ht and bf
    ht_delete(&(*text)->ht);
    bf_delete(&(*text)->bf);
    free(*text);
    *text = NULL;
}
//calculates distance based on metric
double text_dist(Text *text1, Text *text2, Metric metric) {
    double distance = 0;
    //create iterator to iter through both texts
    HashTableIterator *hti1 = hti_create(text1->ht);
    HashTableIterator *hti2 = hti_create(text2->ht);
    Node *n = NULL;
    Metric m = metric;
    while ((n = ht_iter(hti1)) != NULL) {
        //depending on metric, do calculation and add to distance
        if (m == MANHATTAN) {
            distance
                += fabs(text_frequency(text1, n->word) - (double) text_frequency(text2, n->word));
        }
        if (m == EUCLIDEAN) {
            distance
                += pow(text_frequency(text1, n->word) - (double) text_frequency(text2, n->word), 2);
        }
        if (m == COSINE) {
            distance += (text_frequency(text1, n->word) * (double) text_frequency(text2, n->word));
        }
    }
    while ((n = ht_iter(hti2)) != NULL) {
        //next calculate distance for words not in text1
        if (text_contains(text1, n->word) == true) {
            continue;
        }
        //do calcs based on metric
        if (m == MANHATTAN) {
            distance
                += fabs(text_frequency(text2, n->word) - (double) text_frequency(text1, n->word));
        }
        if (m == EUCLIDEAN) {
            distance
                += pow(text_frequency(text1, n->word) - (double) text_frequency(text2, n->word), 2);
        }
        if (m == COSINE) {
            distance += (text_frequency(text1, n->word) * (double) text_frequency(text2, n->word));
        }
    }
    //do final steps in respective calculations
    if (m == EUCLIDEAN) {
        distance = sqrt(distance);
    }
    if (m == COSINE) {
        distance = 1 - distance;
    }
    //delete hti's
    hti_delete(&hti1);
    hti_delete(&hti2);

    return distance;
}
//return normalized frequency
double text_frequency(Text *text, char *word) {
    //divide by total word count
    if (text_contains(text, word)) {
        return (ht_lookup(text->ht, word))->count / (double) text->word_count;
    }
    return 0;
}

bool text_contains(Text *text, char *word) {
    //check bf first, if false, not in
    //if true, check ht for false positive
    if (bf_probe(text->bf, word) && ht_lookup(text->ht, word) != NULL) {
        return true;
    }
    return false;
}

//debugging for text
void text_print(Text *text) {
    //create iter to loop and print things in text
    HashTableIterator *hti = hti_create(text->ht);
    Node *n = NULL;
    while ((n = ht_iter(hti)) != NULL) {
        printf("(%s, %d)\n", n->word, n->count);
    }
    hti_delete(&hti);
}

/*int main(void) {
	FILE *file1 = fopen("kim.txt", "r");
	FILE *file2 = fopen("alice.txt", "r");
	FILE *noise_file = fopen("noise.txt", "r");
	Text *noise = text_create(noise_file, NULL);
	Text *text1 = text_create(file1, noise);
	Text *text2 = text_create(file2, noise);
	//only checks for lower case
	//char *word  = "pangenesis";
	//printf("total words: %d\n", text->word_count);
	//printf("%s noramlized frequency: %.8f\n", word, text_frequency(text, word));
	//Metric metric = MANHATTAN;
	//double distance = text_dist(text1, text2, metric);
	//printf("total distance = %f\n", distance);
	

	
	printf("\n");
	text_print(text1);
	printf("\n");
	text_print(text2);
	//printf("noise\n");
	//text_print(noise);
	
	
	text_delete(&text1);
	text_delete(&text2);
	text_delete(&noise);
	fclose(file1);
	fclose(file2);
	fclose(noise_file);
	return 0;
}*/
