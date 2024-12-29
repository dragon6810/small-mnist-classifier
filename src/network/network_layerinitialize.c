#include <network/network.h>

#include <assert.h>
#include <string.h>

void network_layerinitialize(network_layer_t* layer, int nnodes)
{
    int i;

    network_node_t *nodesdata;

    assert(layer);

    memset(layer, 0, sizeof(network_layer_t));
    list_initialize(&layer->nodes, sizeof(network_node_t));
    list_resize(&layer->nodes, nnodes);
    nodesdata = (network_node_t*) layer->nodes.data;

    for(i=0; i<nnodes; i++)
        network_nodeinitialize(&nodesdata[i]);
}
