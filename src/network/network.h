#ifndef _network_h
#define _network_h

#include <std/list/list.h>
#include <vector/vector.h>

/*
 * ================================
 *        TYPES DEFENITIONS
 * ================================
*/

typedef struct network_node_s network_node_t;
typedef struct network_edge_s network_edge_t;
typedef struct network_layer_s network_layer_t;
typedef struct network_network_s network_network_t;

LIST_TYPE(network_node_t, list_network_node)
LIST_TYPE(network_edge_t, list_network_edge)
LIST_TYPE(network_edge_t*, list_network_edge_p)
LIST_TYPE(network_layer_t, list_network_layer)

struct network_node_s
{
    float val;
    list_network_edge_p_t edges[2]; // 0 is incoming, 1 is outgoing
    float bias;      // bias

    // backprop stuff
    float inboundslope;
    float wantnudge;
};

struct network_edge_s
{
    float weight;
    network_node_t *nodes[2]; // 0 is before, 1 is after

    // backprop stuff
    float wantnudge;
};

struct network_layer_s
{
    list_network_node_t nodes;
    float (*sigma)(network_layer_t*, float);
    float (*sigmaslope)(network_layer_t*, float);
};

struct network_network_s
{
    list_network_layer_t layers;
    list_network_edge_t edges;
};

/*
 * ================================
 *      VARIABLE DEFENITIONS
 * ================================
*/

/*
 * ================================
 *  EXTERNAL ROUTINES DEFENITIONS
 * ================================
*/

void network_nodeinitialize(network_node_t* node);

void network_edgeinitialize(network_edge_t* edge);

void network_layerinitialize(network_layer_t* layer, int nnodes, float (*sigma)(network_layer_t*, float), float (*sigmaslope)(network_layer_t*, float));

void network_initialize(network_network_t* network);
void network_addlayer(network_network_t* network, network_layer_t* layer);
void network_genedges(network_network_t* network);
void network_run(network_network_t* network);
void network_backprop(network_network_t* network, vector_t want, unsigned long int batchsize);
void network_learn(network_network_t* network);
void network_free(network_network_t* network);

#endif
