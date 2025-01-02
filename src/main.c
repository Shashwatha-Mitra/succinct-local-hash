#include "merkle-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	const char *data = "Hello";
	int length_of_data = strlen(data);
	unsigned char *hash;
	int block_size = 2;
	int no_of_blocks = (length_of_data + block_size - 1)/block_size;

	for (int i=0; i<no_of_blocks; i++) {
		int location = i;
		hash = merkle_hash(data, block_size);
		if (!hash) {
			printf ("\nHashing failed! Check again!\n");
			continue;
		}
		printf ("Hash root: ");
		print_hash(hash);
		opening_t *op;
		op = compute_opening(data, block_size, location);
		if (!op) {
			printf ("\nOpening computation failed! Check again!\n");
			continue;
		}
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
  return 0;
}
