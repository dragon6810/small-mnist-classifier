#include <network/network.h>

#include <std/assert/assert.h>
#include <string.h>

#include <std/random/random.h>


void network_edgeinitialize(network_edge_t* edge)
{
    assert(edge);

    memset(edge, 0, sizeof(network_edge_t));
}
