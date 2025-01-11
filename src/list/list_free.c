#include <list/list.h>

#include <assert/assert.h>
#include <stdlib.h>

void list_free(list_t* list)
{
    assert(list);
    assert(list->data);

    free(list->data);
}