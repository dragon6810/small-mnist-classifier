#include <network/network.h>

void network_cleanupbackprop(network_network_t* network)
{
    int i, j;

    network_layer_t *layersdata;
    network_node_t *nodesdata;

    layersdata = (network_layer_t*) network->layers.data;

    for(i=0; i<network->layers.size; i++)
    {
        nodesdata = (network_node_t*) layersdata[i].nodes.data;
        for(j=0; j<layersdata[i].nodes.size; j++)
            nodesdata[j].inboundwslope = nodesdata[j].inboundbslope = 0.0;
    }
}