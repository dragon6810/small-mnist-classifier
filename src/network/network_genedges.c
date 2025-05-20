#include <network/network.h>

#include <std/assert/assert.h>
#include <math.h>

#include <std/random/random.h>

void network_genedges(network_network_t* network)
{
    int i, j, k, e;
    network_layer_t *layer, *nextlayer;
    network_node_t *node, *nextnode;

    network_layer_t *layersdata;
    network_edge_t edge, *newedge;
    int nedges;
    float max;

    assert(network);
    assert(network->layers.size >= 2);

    layersdata = (network_layer_t*) network->layers.data;
    for(i=nedges=0; i<network->layers.size-1; i++)
        nedges += layersdata[i].nodes.size * layersdata[i+1].nodes.size;

    LIST_RESIZE(network->edges, nedges);
    for(i=e=0; i<network->layers.size-1; i++)
    {
        layer = &layersdata[i];
        nextlayer = &layersdata[i+1];
        
        for(j=0; j<layer->nodes.size; j++)
        {
            node = &layer->nodes.data[j];
            for(k=0; k<nextlayer->nodes.size; k++, e++)
            {
                assert(e < nedges);

                nextnode = &nextlayer->nodes.data[k];

                network_edgeinitialize(&edge);
                edge.nodes[0] = node;
                edge.nodes[1] = nextnode;
                max = sqrtf(6.0 / (float) layer->nodes.size);
                edge.weight = random_float(-max, max);

                network->edges.data[e] = edge;
                newedge = &network->edges.data[e];

                LIST_PUSH(node->edges[1], newedge);
                LIST_PUSH(nextnode->edges[0], newedge);
            }
        }
    }
}
