#include <network/network.h>

#include <assert.h>
#include <math.h>

#include <vector/vector.h>

void network_run_sigmalayer(network_layer_t* layer)
{
    int i;

    vector_t vec;
    network_node_t *nodesdata;

    assert(layer);

    vector_alloc(&vec, layer->nodes.size);
    nodesdata = (network_node_t*) layer->nodes.data;

    for(i=0; i<vec.len; i++)
        vec.data[i] = layer->sigma(layer, nodesdata[i].val);

    for(i=0; i<vec.len; i++)
        nodesdata[i].val = vec.data[i];
    
    vector_free(&vec);
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
}

void network_run_runlayer(network_layer_t* layer)
{
    int i;

    assert(layer);

    network_node_t *nodesdata;

    nodesdata = (network_node_t*) layer->nodes.data;
    for(i=0; i<layer->nodes.size; i++)
        network_run_runnode(&nodesdata[i]);

    network_run_sigmalayer(layer);
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
