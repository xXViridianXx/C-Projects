#include "bf.h"
#include "bv.h"
#include "ht.h"
#include "metric.h"
#include "node.h"
#include "parser.h"
#include "pq.h"
#include "pqe.h"
#include "salts.h"
#include "speck.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "d:n:k:l:emch"

void usage(char *exec) { /*usage error function which I reused from last assignment*/
    fprintf(stderr,
        "\n"
        "Synopsis:\n"
        "----takes in written text from stdin and determines who the most "
        "likely author is based on a given database"
        "\n"
        "USAGE\n %s./encode [-hv] -i infile -o output\n"
        "\n" /*all command line options listed*/
        "OPTIONS\n"
        "    -h        help and usage\n"
        "    -d        use database\n"
        "    -n        path to noise file\n"
        "    -k        specifies number of matches\n"
        "    -l        specifies number of noise words to filter out\n"
        "    -e        euclidean distance\n"
        "    -m        mahattan distance\n"
        "    -c        cosine distance\n"
        "\n",
        exec);
    return;
}

int main(int argc, char **argv) {
    //default args defined
    int opt;
    FILE *data = fopen("lib.db", "r");
    FILE *noise = fopen("noise.txt", "r");

    uint32_t matches = 5;
    noiselimit = 100;

    Metric m = EUCLIDEAN;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //simple get opt
        switch (opt) {
        case 'd':
            fclose(data);
            data = fopen(optarg, "r");
            if (data == NULL) {
                fprintf(stderr, "ERROR: Nothing in File:\n"); //checking if file empty
                fprintf(stderr, "Try a Different File\n");
                return EXIT_FAILURE;
            }
            break;
        case 'n':
            fclose(noise);
            noise = fopen(optarg, "r");
            if (noise == NULL) {
                fprintf(stderr, "ERROR: Nothing in File:\n"); //checking if file empty
                fprintf(stderr, "Try a Different File\n");
                return EXIT_FAILURE;
            }
            break;
        case 'k': matches = strtoul(optarg, NULL, 10); break;
        case 'l': noiselimit = strtoul(optarg, NULL, 10); break;
        case 'e': m = EUCLIDEAN; break;
        case 'm': m = MANHATTAN; break;
        case 'c': m = COSINE; break;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
            break;
        }
    }
    //infile set to stdin
    FILE *infile = stdin;

    if (infile == NULL) {

        fprintf(stderr, "File Error: try again\n");
        return EXIT_FAILURE;
    }
    if (noiselimit == 0) {
        noiselimit = 10000;
    }
    //creates noise text
    Text *noise_text = text_create(noise, NULL);
    //creates anon text based on noise
    //if typing into stdin, use crtl+d to mark EOF
    Text *anon = text_create(infile, noise_text);
    int n = 0;
    //get author path pairs
    fscanf(data, "%d\n", &n);
    //create pq of number of pairs
    PriorityQueue *pq = pq_create(n);
    //printf("n = %d\n", n);

    char author[256];
    char *new;
    char path[256];
    FILE *fpath;
    //gets author and path pair
    for (int i = 0; i < n && !feof(data); i++) {
        fgets(author, sizeof(author), data);
        //sets /n to empty char
        if ((new = strchr(author, '\n')) != NULL) {
            *new = '\0';
        }
        fgets(path, sizeof(path), data);
        if ((new = strchr(path, '\n')) != NULL) {
            *new = '\0';
        }
        //creates text for database texts
        fpath = fopen(path, "r");
        Text *text = text_create(fpath, noise_text);
        //get distance and enqueue
        double d = text_dist(anon, text, m);
        /*if(strcmp(author, "H. G. Wells") == 0) {
		printf("%.8f\n", d);
	}*/
        enqueue(pq, author, d);
        text_delete(&text);
        fclose(fpath);
    }
    char *auth;
    double dist;
    //print what user will see
    //dequeue highest priority
    printf("\nTop %d, metric: %s, noise limit: %d\n", matches, metric_names[m], noiselimit);
    for (uint32_t i = 0; i < matches; i++) {
        dequeue(pq, &auth, &dist);
        printf("%d) %s [%.15f]\n", i + 1, auth, dist);
    }
    //delete things to delete
    text_delete(&(noise_text));
    text_delete(&(anon));

    fclose(noise);
    fclose(infile);
    fclose(data);
    pq_delete(&pq);

    return 0;
}
