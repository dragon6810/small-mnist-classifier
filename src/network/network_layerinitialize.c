#include <network/network.h>

#include <std/assert/assert.h>
#include <string.h>

void network_layerinitialize(network_layer_t* layer, int nnodes, float (*sigma)(network_layer_t*, float), float (*sigmaslope)(network_layer_t*, float))
{
    int i;

    network_node_t *nodesdata;

    assert(layer);

    memset(layer, 0, sizeof(network_layer_t));
    LIST_INITIALIZE(layer->nodes);
    LIST_RESIZE(layer->nodes, nnodes);
    nodesdata = (network_node_t*) layer->nodes.data;

    for(i=0; i<nnodes; i++)
        network_nodeinitialize(&nodesdata[i]);

    layer->sigma = sigma;
    layer->sigmaslope = sigmaslope;
}
