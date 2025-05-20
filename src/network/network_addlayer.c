#include <network/network.h>

#include <std/assert/assert.h>
#include <string.h>

void network_addlayer(network_network_t* network, network_layer_t* layer)
{
    assert(network);
    assert(layer);
    
    LIST_PUSH(network->layers, *layer);
}
