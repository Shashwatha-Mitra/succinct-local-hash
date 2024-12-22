#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merkle-tree.h"

void compute_hash (const char *data, unsigned char hash[]) {
	SHA256((unsigned char *)data, strlen(data), hash);
}

/*
	Compute Merkle Tree hash. 
	Steps:
		1. Compute the number of blocks required
		2. Hash data of each block
		3. Build merkle tree and return hash at the root
*/
unsigned char* merkle_hash (const char *data, int block_size) {
	// Compute the number of blocks required
	int length_of_data = strlen(data);
	int no_of_blocks = (length_of_data + block_size - 1)/block_size; // ceiling division
	
	unsigned char **hashes = (unsigned char **) calloc (no_of_blocks, sizeof(unsigned char *));

	// Compute the hash for each leaf node
	printf ("Building Hash blocks..\n");
	for (int i=0; i<no_of_blocks; i++) {
		hashes[i] = (unsigned char *) calloc (SHA256_DIGEST_LENGTH, sizeof(unsigned char));
		unsigned char block_data[block_size+1];
		strncpy (block_data, data+(i*block_size), block_size);
		block_data[block_size] = '\0';
		compute_hash(block_data, hashes[i]);
	}
	printf ("Done hash blocks..\n");

	// Build merkle tree
	printf ("Building Merkle Tree..\n");
	int num_leaves = no_of_blocks;
	while (num_leaves > 1) {
		for (int j=0; j<num_leaves/2; j++) {
			hashes[j] = make_pair_and_hash(hashes[2*j], hashes[2*j+1]);
		}
		for (int j=num_leaves/2; j<num_leaves; j++) {
			free(hashes[j]);
		}
		num_leaves = num_leaves/2;
	}
	printf ("Finished building Tree..\n");

	return hashes[0];
}

void print_hash(const unsigned char hash[]) {
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

unsigned char* make_pair_and_hash(const unsigned char *data1, const unsigned char *data2) {
	unsigned char *combined_hash = (unsigned char *) calloc (SHA256_DIGEST_LENGTH*2+1, sizeof(unsigned char));
	strncpy (combined_hash, data1, SHA256_DIGEST_LENGTH);
	strncpy (combined_hash + SHA256_DIGEST_LENGTH, data2, SHA256_DIGEST_LENGTH);
	combined_hash[SHA256_DIGEST_LENGTH*2] = '\0';
	
	unsigned char *final_hash = (unsigned char *) calloc (SHA256_DIGEST_LENGTH, sizeof (unsigned char)); 
	compute_hash (combined_hash, final_hash);
	return final_hash;
}

int log_base_2 (int num) {
	int result = 0;
	while (num > 0) {
		result++;
		num = num >> 1;
	}
	return result;
}

opening_t* compute_opening(const char *data, int block_size, int location) {
	int length_of_data = strlen(data);
	int no_of_blocks = (length_of_data + block_size - 1)/block_size;
	if (location > no_of_blocks) {
		return NULL;
	}
	
	opening_t *opening = (opening_t *) calloc (1, sizeof(opening_t));
	opening->location = location;
	opening->length_of_op = log_base_2(no_of_blocks);
	opening->data_at_location = (unsigned char *) calloc (block_size + 1, sizeof(unsigned char));
	strncpy(opening->data_at_location, data + (location * block_size), block_size);
	opening->data_at_location[block_size] = '\0';
	opening->op = (unsigned char **) calloc (opening->length_of_op, sizeof (unsigned char *));

	unsigned char **hashes;
	hashes = (unsigned char **) calloc (no_of_blocks, sizeof(unsigned char **));
	
	// Compute the hash for each leaf node
	printf ("Opening hash blocks..\n");
	for (int i=0; i<no_of_blocks; i++) {
		hashes[i] = (unsigned char *) calloc (SHA256_DIGEST_LENGTH, sizeof(unsigned char));
		unsigned char block_data[block_size+1];
		strncpy (block_data, data+(i*block_size), block_size);
		block_data[block_size] = '\0';
		compute_hash(block_data, hashes[i]);
	}
	printf ("Done!..\n");

	// Build the opening
	int op_idx = 0;
	int num_leaves = no_of_blocks;
	printf ("Building Opening..\n");
	while (num_leaves > 1) {
		int sibling_node = (location % 2) ? location - 1 : location + 1;
		opening->op[op_idx] = (unsigned char *) calloc (SHA256_DIGEST_LENGTH, sizeof(unsigned char));
		strncpy(opening->op[op_idx], hashes[sibling_node], SHA256_DIGEST_LENGTH);
		op_idx++;
		for (int j=0; j<num_leaves/2; j++) 
			hashes[j] = make_pair_and_hash(hashes[2*j], hashes[2*j+1]);
		for (int j=num_leaves/2; j<num_leaves; j++) 
			free(hashes[j]);
		num_leaves /= 2;
		location /= 2;
	}
	opening->op[op_idx] = (unsigned char *) calloc (SHA256_DIGEST_LENGTH, sizeof(unsigned char));
	strncpy(opening->op[op_idx],hashes[0], SHA256_DIGEST_LENGTH);
	free(hashes[0]);
	printf ("Done Building Opening..\n");
	return opening;
}

void print_opening(opening_t *opening) {
	printf ("-------------------------- Opening Info -------------------------\n");
	printf ("Index of opening : %d\n", opening->location);
	printf ("Data at Index    : %s\n", opening->data_at_location);	
	printf ("Hashes in opening: \n");
	for (int i=0; i<opening->length_of_op; i++) {
		print_hash(opening->op[i]);
	}
	printf ("-----------------------------------------------------------------\n");
}

int verify_opening(const unsigned char root[], const char *data, int block_size, int location, opening_t *opening) {
	// Check if location information is correct
	if (location != opening->location)
		return 0;
	
	// Check if data at location is correct
	unsigned char *block_data = (unsigned char *) calloc (block_size + 1, sizeof (unsigned char));
	strncpy (block_data, data + (location * block_size), block_size);
	block_data[block_size] = '\0';
	if (strncmp(opening->data_at_location, block_data, block_size))
		return 0;

	// Compute hashes
	unsigned char *hash = (unsigned char *) calloc (SHA256_DIGEST_LENGTH, sizeof(unsigned char));
	compute_hash(block_data, hash);
	
	unsigned char *running_hash = (unsigned char *) calloc (SHA256_DIGEST_LENGTH, sizeof(unsigned char));
	for (int i=0; i<opening->length_of_op-1; i++) {
		if (location % 2)
			running_hash = make_pair_and_hash (opening->op[i], hash);
		else
			running_hash = make_pair_and_hash (hash, opening->op[i]);
		strncpy(hash, running_hash, SHA256_DIGEST_LENGTH);
		location /= 2;
	}
	if (strncmp(running_hash, root, SHA256_DIGEST_LENGTH))
		return 0;
	return 1;
}
