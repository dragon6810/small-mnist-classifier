#include <vector/vector.h>

#include <assert.h>
#include <stdlib.h>

void vector_alloc(vector_t* vec, unsigned long int len)
{
    assert(vec);

    vec->len = len;
    vec->data = (float*) calloc(1, sizeof(float) * vec->len);
}