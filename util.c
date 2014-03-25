#include "./globe_inc.h"

uint64_t djb2_hash(unsigned char *str) {
	uint64_t hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}
