#include <network/network.h>

#include <assert.h>

void network_genedges(network_network_t* network)
{
    int i, j, k;
    network_layer_t *layer, *nextlayer;
    network_node_t *node, *nextnode;

    network_node_t *nodesdata, *nextnodesdata;
    network_layer_t *layersdata;
    network_edge_t edge, *newedge;

    assert(network);
    assert(network->layers.size >= 2);

    layersdata = (network_layer_t*) network->layers.data;

    for(i=0; i<network->layers.size-1; i++)
    {
        layer = &layersdata[i];
        nextlayer = &layersdata[i+1];

        nodesdata = (network_node_t*) layer->nodes.data;
        nextnodesdata = (network_node_t*) nextlayer->nodes.data;
        for(j=0; j<layer->nodes.size; j++)
        {
            node = &nodesdata[j];
            for(k=0; k<nextlayer->nodes.size; k++)
            {
                nextnode = &nextnodesdata[k];
                network_edgeinitialize(&edge);
                edge.nodes[0] = node;
                edge.nodes[1] = nextnode;
                newedge = list_push(&network->edges, &edge);

                list_push(&node->edges[1], &newedge);
                list_push(&node->edges[0], &newedge);
            }
        }
    }
}
