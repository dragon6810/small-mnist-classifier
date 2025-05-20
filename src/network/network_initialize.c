#include <network/network.h>

#include <std/assert/assert.h>
#include <string.h>

void network_initialize(network_network_t* network)
{
    assert(network);

    memset(network, 0, sizeof(network_network_t));
    LIST_INITIALIZE(network->layers);
    LIST_INITIALIZE(network->edges);
}
