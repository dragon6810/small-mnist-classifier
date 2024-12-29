#include <network/network.h>

#include <assert.h>
#include <string.h>

void network_initialize(network_network_t* network)
{
    assert(network);

    memset(network, 0, sizeof(network_network_t));
    list_initialize(&network->layers, sizeof(network_layer_t)); 
    list_initialize(&network->edges, sizeof(network_edge_t));
}
