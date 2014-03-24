#ifndef GLOBE_INC_H_
#define GLOBE_INC_H_

#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define ABS(x) ((x)<0?-(x):(x))
#define SWAP(a,b) do { a^=b; b^=a; a^=b; } while(0)

typedef enum {
	false = 0, true = 1
} bool_t;

#endif /* GLOBE_INC_H_ */
