#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

void print_hash(unsigned char hash[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main() {
    // Input data
    const char *data = "Hello, World!";
    unsigned char hash[SHA256_DIGEST_LENGTH]; // SHA-256 outputs 32 bytes

    // Compute the SHA-256 hash
    SHA256((unsigned char *)&data[1], strlen(&data[1]), hash);

	// Print the data
	printf ("Data: %c\n", data[1]);

    // Print the hash
    printf("SHA-256 hash: ");
    print_hash(hash, SHA256_DIGEST_LENGTH);

    return 0;
}
