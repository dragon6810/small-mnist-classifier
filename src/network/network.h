#ifndef _network_h
#define _network_h

#include <list/list.h>

/*
 * ================================
 *        TYPES DEFENITIONS
 * ================================
*/

typedef struct network_node_s network_node_t;
typedef struct network_edge_s network_edge_t;
typedef struct network_layer_s network_layer_t;
typedef struct network_network_s network_network_t;

struct network_node_s
{
    float val;
    list_t edges[2]; // list of network_edge_t*; 0 is incoming, 1 is outgoing
    float bias;      // incoming bias
} __attribute__((packed));

struct network_edge_s
{
    float weight;
    network_node_t *nodes[2]; // 0 is before, 1 is after
} __attribute__((packed));

struct network_layer_s
{
    list_t nodes; // list of network_node_t
} __attribute__((packed));

struct network_network_s
{
    list_t layers; // list of network_layer_t
    list_t edges;  // list of network_edge_t
} __attribute__((packed));

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

void network_layerinitialize(network_layer_t* layer, int nnodes);

void network_initialize(network_network_t* network);
void network_addlayer(network_network_t* network, network_layer_t* layer);
void network_genedges(network_network_t* network);
void network_run(network_network_t* network);
void network_learn(network_network_t* network, unsigned long int want);

#endif
