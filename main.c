#include "merkle-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
	const char *data = "Hello-wo";
	int length_of_data = strlen(data);
	unsigned char *hash;
	int block_size = 1;

	for (int i=0; i<length_of_data; i++) {
		int location = i;
		hash = merkle_hash(data, block_size);
		printf ("Hash root: ");
		print_hash(hash);
		opening_t *op;
		op = compute_opening(data, block_size, location);
		printf ("\n");
		print_opening(op);
		if (verify_opening(hash, data, block_size, location, op)) {
			printf ("\nSuccess!!\n");
		} else {
			printf ("\nDebug your code!\n");
			exit(-1);
		}
		printf ("----------------------------------------------------------------------------\n");
	}
}
