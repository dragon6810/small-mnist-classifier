#include <network/network.h>

#include <assert/assert.h>
#include <math.h>

float divforavg;

void network_backprop_cleanup(network_network_t* network)
{
    int i, j;

    network_layer_t *layersdata;
    network_node_t *nodesdata;

    layersdata = (network_layer_t*) network->layers.data;

    for(i=0; i<network->layers.size; i++)
    {
        nodesdata = (network_node_t*) layersdata[i].nodes.data;
        for(j=0; j<layersdata[i].nodes.size; j++)
            nodesdata[j].inboundslope = 0.0;
    }
}

void network_backprop_node(network_layer_t* layer, network_node_t* node)
{
    int i, j;

    network_edge_t **edgesdata;
    float wslope, bslope, z;

    assert(layer);
    assert(node);

    // calculate z
    z = node->bias;
    edgesdata = (network_edge_t**) node->edges[0].data;
    for(i=0; i<node->edges->size; i++)
        z += edgesdata[i]->nodes[0]->val * edgesdata[i]->weight;
    
    // calculate bias slope
    bslope = layer->sigmaslope(layer, z) * node->inboundslope;

    // a with respect to z
    wslope = layer->sigmaslope(layer, z) * node->inboundslope;

    // do weights slopes
    for(i=0; i<node->edges->size; i++)
    {   
        edgesdata[i]->nodes[0]->inboundslope += wslope * edgesdata[i]->weight;

        // z with respect to w
        edgesdata[i]->wantnudge += -(edgesdata[i]->nodes[0]->val * wslope) * divforavg;
    }

    node->wantnudge += -bslope * divforavg;
}

void network_backprop_layer(network_layer_t* layer)
{
    int i;

    network_node_t *nodesdata;

    assert(layer);

    nodesdata = (network_node_t*) layer->nodes.data;
    for(i=0; i<layer->nodes.size; i++)
        network_backprop_node(layer, &nodesdata[i]);
}

void network_backprop(network_network_t* network, vector_t want, unsigned long int batchsize)
{
    int i;

    network_node_t *nodesdata;
    network_layer_t *layersdata;
    network_layer_t *curlayer;

    assert(network);
    assert(want.data);

    network_backprop_cleanup(network);

    divforavg = 1.0 / (float) batchsize;

    layersdata = (network_layer_t*) network->layers.data;
    curlayer = &layersdata[network->layers.size-1];

    assert(want.len == curlayer->nodes.size);

    nodesdata = (network_node_t*) curlayer->nodes.data;
    for(i=0; i<want.len; i++)
    {
        // cost with respect to a
        nodesdata[i].inboundslope = 2.0 * (nodesdata[i].val - want.data[i]);
    }

    for(i=network->layers.size-1; i>0; i--)
        network_backprop_layer(curlayer);
}
