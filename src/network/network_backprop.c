#include <network/network.h>

#include <assert.h>
#include <math.h>

float divforavg;

void network_backprop_node(network_layer_t* layer, network_node_t* node)
{
    int i;

    network_edge_t **edgesdata;
    float slope, bslope;
    float plainactivation;

    assert(layer);
    assert(node);

    // Do bias
    bslope = divforavg;
    bslope *= layer->sigmaslope(layer, plainactivation + node->bias);
    bslope *= node->inboundbslope;

    // Do weights
    edgesdata = (network_edge_t**) node->edges[0].data;
    plainactivation = 0;
    for(i=0; i<node->edges->size; i++)
    {
        plainactivation += edgesdata[i]->weight * edgesdata[i]->nodes[0]->val;

        slope = divforavg;
        slope *= edgesdata[i]->nodes[0]->val;
        slope *= layer->sigmaslope(layer, edgesdata[i]->weight * edgesdata[i]->nodes[0]->val + node->bias);
        slope *= node->inboundwslope;

        edgesdata[i]->nodes[0]->inboundwslope += slope;
        edgesdata[i]->nodes[0]->inboundbslope += bslope;

        edgesdata[i]->wantnudge += -slope;
    }

    node->wantnudge = -slope;
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

    divforavg = 1.0 / (float) batchsize;

    layersdata = (network_layer_t*) network->layers.data;
    curlayer = &layersdata[network->layers.size-1];

    assert(want.len == curlayer->nodes.size);

    nodesdata = (network_node_t*) curlayer->nodes.data;
    for(i=0; i<want.len; i++)
        nodesdata[i].inboundwslope = nodesdata[i].inboundbslope = 2.0 * (nodesdata[i].val - want.data[i]);

    for(i=network->layers.size-1; i>0; i--)
        network_backprop_layer(curlayer);
}