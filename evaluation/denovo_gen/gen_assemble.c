#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include "build.h"
#include "probe.h"
#include "packingDNAseq.h"
#include "kmer_hash.h"

int main(int argc, char *argv[]){

	/** Declarations **/
	double inputTime=0.0, constrTime=0.0, traversalTime=0.0;
	hash_table_t *tbl;
	start_kmer_t *start_list = NULL;
	char *input_UFX_name = argv[1];
	memory_heap_t mem_heap;

	/** Graph construction **/
	constrTime -= gettime();

    build(&tbl, &start_list, &mem_heap, input_UFX_name);

	printf("Built\n");
	constrTime += gettime();

	/** Graph traversal **/
	traversalTime -= gettime();

    FILE *out = fopen("serial.out", "w");
    probe(tbl, start_list, out);
    fclose(out);

    printf("Probed\n");
	traversalTime += gettime();

	/* Clean up */
    dealloc_heap(&mem_heap);

	/** Print timing and output info **/
	/***** DO NOT CHANGE THIS PART ****/
	printf("%s: Input set: %s\n", argv[0], argv[1]);
	printf("Input reading time: %f seconds\n", inputTime);
	printf("Graph construction time: %f seconds\n", constrTime);
	printf("Graph traversal time: %f seconds\n", traversalTime);
	return 0;
}
