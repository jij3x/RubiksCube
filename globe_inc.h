#ifndef GLOBE_INC_H_
#define GLOBE_INC_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define SWAP(a,b) do { a^=b; b^=a; a^=b; } while(0)

#define MAX_LAYERS 1024

typedef enum {
    false = 0, true = 1
} bool_t;

typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef long int64_t;
typedef int int32_t;
typedef short int16_t;

#endif /* GLOBE_INC_H_ */
