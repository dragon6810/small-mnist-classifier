#include <network/network.h>

#include <assert.h>
#include <string.h>

void network_addlayer(network_network_t* network, network_layer_t* layer)
{
    assert(network);
    assert(layer);
    
    list_push(&network->layers, layer);
}
