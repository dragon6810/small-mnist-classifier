#include <network/network.h>

#include <assert.h>

float network_run_relu(float x)
{
    if(x < 0)
        return 0;

    return x;
}

void network_run_runnode(network_node_t* node)
{
    int i;

    network_edge_t **edgesdata;

    assert(node);

    node->val = node->bias;
    edgesdata = (network_edge_t**) node->edges[0].data;
    for(i=0; i<node->edges[0].size; i++)
    {
        assert(edgesdata[i]->nodes[0]);
        node->val += edgesdata[i]->nodes[0]->val * edgesdata[i]->weight;
    }

    node->val = network_run_relu(node->val);
}

void network_run_runlayer(network_layer_t* layer)
{
    int i;

    assert(layer);

    network_node_t *nodesdata;

    nodesdata = (network_node_t*) layer->nodes.data;
    for(i=0; i<layer->nodes.size; i++)
        network_run_runnode(&nodesdata[i]);
}

void network_run(network_network_t* network)
{
    int i;

    network_layer_t *layersdata;

    assert(network);
    assert(network->layers.size >= 2);

    layersdata = (network_layer_t*) network->layers.data;
    for(i=1; i<network->layers.size; i++)
        network_run_runlayer(&layersdata[i]);
}
