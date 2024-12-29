#include <network/network.h>

#include <assert.h>

void network_genedges(network_network_t* network)
{
    int i, j, k, e;
    network_layer_t *layer, *nextlayer;
    network_node_t *node, *nextnode;

    network_node_t *nodesdata, *nextnodesdata;
    network_layer_t *layersdata;
    network_edge_t edge, *newedge;
    int nedges;

    assert(network);
    assert(network->layers.size >= 2);

    layersdata = (network_layer_t*) network->layers.data;
    for(i=nedges=0; i<network->layers.size-1; i++)
        nedges += layersdata[i].nodes.size * layersdata[i+1].nodes.size;

    list_resize(&network->edges, nedges);
    for(i=e=0; i<network->layers.size-1; i++)
    {
        layer = &layersdata[i];
        nextlayer = &layersdata[i+1];

        nodesdata = (network_node_t*) layer->nodes.data;
        nextnodesdata = (network_node_t*) nextlayer->nodes.data;
        for(j=0; j<layer->nodes.size; j++)
        {
            node = &nodesdata[j];
            for(k=0; k<nextlayer->nodes.size; k++, e++)
            {
                assert(e < nedges);

                nextnode = &nextnodesdata[k];

                network_edgeinitialize(&edge);
                edge.nodes[0] = node;
                edge.nodes[1] = nextnode;
                ((network_edge_t*) network->edges.data)[e] = edge;
                newedge = ((network_edge_t*) network->edges.data) + e;

                list_push(&node->edges[1], &newedge);
                list_push(&nextnode->edges[0], &newedge);
            }
        }
    }
}
