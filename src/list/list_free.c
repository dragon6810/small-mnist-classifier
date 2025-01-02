#include <list/list.h>

#include <assert.h>

void list_free(list_t* list)
{
    assert(list);
    assert(list->data);

    free(list->data);
}