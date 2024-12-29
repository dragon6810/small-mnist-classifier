#include <network/network.h>

#include <assert.h>
#include <string.h>

void network_nodeinitialize(network_node_t* node)
{
    int i;

    assert(node);

    memset(node, 0, sizeof(network_node_t));
    for(i=0; i<2; i++)
        list_initialize(&node->edges[i], sizeof(network_edge_t*));
}
