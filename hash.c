#include "main.h"
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdlib.h>

char *make_salt() {
    // Seed random number generator
    static int initialized = 0;
    if (!initialized) {
        RAND_poll();
        initialized = 1;
    }

    char *buffer = malloc(32 * sizeof(char));
    RAND_bytes((unsigned char*)buffer, 32 * sizeof(char));
    for (int i = 0; i < 32; i++) if (buffer[i] == 0) buffer[i] = 1;

    return buffer;
}


char *hash(const char *input, const char *salt) {
    char *all_input = NULL;
    asprintf(&all_input, "%.32s%s", salt, input);

    unsigned char *md = SHA512((const unsigned char*)all_input, strlen(input), NULL);

    char *hex = malloc(SHA512_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
        sprintf(hex + i * 2, "%02x", md[i]);

    char *output = NULL;
    asprintf(&output, "%.32s%s", salt, hex);
    free(all_input);
    free(hex);
    return output;
}


int hash_equals(const char *existing_hash, const char *text) {
    char *hashed_text = hash(text, existing_hash);
    int result = !strcmp(existing_hash, hashed_text);
    free(hashed_text);
    return result;
}
