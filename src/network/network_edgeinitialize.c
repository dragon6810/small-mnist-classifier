#include <network/network.h>

#include <assert/assert.h>
#include <string.h>

#include <random/random.h>


void network_edgeinitialize(network_edge_t* edge)
{
    assert(edge);

    memset(edge, 0, sizeof(network_edge_t));
}
