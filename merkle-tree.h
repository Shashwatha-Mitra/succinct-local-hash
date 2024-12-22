#ifndef SUCCINCT_HASH
#define SUCCINCT_HASH

#include <openssl/sha.h>

typedef struct opening {
	int location;
	int length_of_op;
	unsigned char *data_at_location;
	unsigned char **op;
} opening_t;

// Compute the hash at block level
void compute_hash(const char *data, unsigned char hash[]);

// Compute the merkle root for the data
unsigned char* merkle_hash(const char *data, int block_size);

// Utility function to print the hash
void print_hash(const unsigned char hash[]);

// Compute the opening for the hash
opening_t* compute_opening (const char *data, int block_size, int location);

// Verify the opening
int verify_opening(const unsigned char root[], const char *data, int block_size, int location, opening_t *opening);

// Helper function to hash down two hashes
unsigned char *make_pair_and_hash(const unsigned char *data1, const unsigned char *data2);

// Helper function to print openings
void print_opening(opening_t *op);
#endif // SUCCINCT_HASH
