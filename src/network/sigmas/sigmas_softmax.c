#include <network/sigmas/sigmas.h>

#include <assert.h>
#include <math.h>

float sigmas_softmax(network_layer_t* layer, float x)
{
    int i;

    network_node_t *nodesdata;
    float denom;

    assert(layer);

    nodesdata = (network_node_t*) layer->nodes.data;
    for(i=0, denom=0; i<layer->nodes.size; i++)
        denom += expf(nodesdata[i].val);

    return expf(x) / denom;
}