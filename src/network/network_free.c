#include <network/network.h>

#include <std/assert/assert.h>

static void network_free_freenode(network_node_t* node)
{
    int i;

    assert(node);

    for(i=0; i<2; i++)
    {
        if(!node->edges[i].data)
            continue;
        
        LIST_FREE(node->edges[i]);
    }
}

static void network_free_freelayer(network_layer_t* layer)
{
    int i;

    network_node_t *nodesdata;

    assert(layer);

    nodesdata = (network_node_t*) layer->nodes.data;
    for(i=0; i<layer->nodes.size; i++)
        network_free_freenode(&nodesdata[i]);

    LIST_FREE(layer->nodes);
}

void network_free(network_network_t* network)
{
    int i;

    network_layer_t *layersdata;

    assert(network);

    LIST_FREE(network->edges);

    layersdata = (network_layer_t*) network->layers.data;
    for(i=0; i<network->layers.size; i++)
        network_free_freelayer(&layersdata[i]);
    LIST_FREE(network->layers);
}