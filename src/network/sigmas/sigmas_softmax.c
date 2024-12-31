#include <network/sigmas/sigmas.h>

#include <assert.h>
#include <math.h>

float sigmas_softmax(network_layer_t* layer, float x)
{
    int i;

    float denom;

    assert(layer);

    for(i=0, denom=0; i<layer->nodes.size; i++)
        denom += expf(((network_node_t*)layer->nodes.data)[i].val);

    return expf(x) / denom;
}