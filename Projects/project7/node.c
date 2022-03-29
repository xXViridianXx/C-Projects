#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_create(char *word) {
    //alocating memory to node
    Node *node = malloc(sizeof(Node));
    node->word = strdup(word); //duplicating word to pointer
    node->count = 0; //init count
    return node;
}

void node_delete(Node **n) {
    if (*n != NULL) {
        //freeing word b/c of string dup
        free((*n)->word);
        (*n)->word = NULL;
        //freeing node
        free(*n);
        *n = NULL;
    }
}

void node_print(Node *n) { //for debugging
    //if node null, print null
    if (n == NULL) {
        printf("NULL\n");
    }
    //else print actual node
    if (n != NULL) {
        printf("(word = %s, count = %d)\n", n->word, n->count);
    }
}

/*int main(void) {
	char *word = "Pikachu";
	Node *node = node_create(word);
	node_print(node);
	char *word1 = "Garchomp";
	Node *node1 = node_create(word1);
	node_print(node1);
	node_delete(&node);
	node_delete(&node1);
	return 0;
}*/
