#include <vector/vector.h>

#include <stdlib.h>

#include <std/assert/assert.h>

void vector_free(vector_t* vec)
{
    assert(vec);
    assert(vec->data);

    free(vec->data);
    vec->data = 0;
}