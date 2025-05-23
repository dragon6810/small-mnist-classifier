#include <network/network.h>

#include <stdio.h>
#include <std/assert/assert.h>

const float learnrate = -0.01;

void network_learn(network_network_t* network)
{
    int i, j;

    network_layer_t *layersdata;
    network_node_t *nodesdata;
    network_edge_t *edgesdata;

    layersdata = (network_layer_t*) network->layers.data;
    edgesdata = (network_edge_t*) network->edges.data;

    for(i=0; i<network->edges.size; i++)
    {
        edgesdata[i].weight += edgesdata[i].wantnudge * learnrate;
        edgesdata[i].wantnudge = 0.0;
    }

    for(i=0; i<network->layers.size; i++)
    {
        nodesdata = (network_node_t*) layersdata[i].nodes.data;
        for(j=0; j<layersdata[i].nodes.size; j++)
        {
            nodesdata[j].bias += nodesdata[j].wantnudge * learnrate;
            nodesdata[j].wantnudge = 0.0;
        }
    }
}
