#include <vector/vector.h>

#include <assert.h>
#include <stdlib.h>

void vector_free(vector_t* vec)
{
    assert(vec);
    assert(vec->data);

    free(vec->data);
    vec->data = 0;
}