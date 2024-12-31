#include <network/network.h>

#include <stdio.h>
#include <assert.h>

void network_learn(network_network_t* network, unsigned long int want)
{
    int i;

    network_layer_t *output;
    float cost;
    float target, val;

    assert(network);
    assert(network->layers.size >= 2);

    return;

    cost = 0.0;
    output = ((network_layer_t*) network->layers.data) + network->layers.size - 1;
    for(i=0; i<output->nodes.size; i++)
    {
        val = ((network_node_t*)output->nodes.data)[i].val;
        target = i == want ? 1.0 : 0.0;
        cost += (target - val) * (target - val);
    }
}
