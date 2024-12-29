#ifndef _network_h
#define _network_h

#define NETWORK_MAX_LAYERS 16
#define NETWORK_MAX_EDGES 8192
#define NETWORK_MAX_LAYER_NODES 2048

/*
 * Each network is 819264 bytes, I should make these dynamic some time.
*/

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
    network_edge_t *edges[2]; // 0 is before, 1 is after
};

struct network_edge_s
{
    float weight;
    network_node_t *nodes[2]; // 0 is before, 1 is after
};

struct network_layer_s
{
    int nnodes;
    network_node_t nodes[NETWORK_MAX_LAYER_NODES];
};

struct network_network_s
{
    int nlayers;
    network_layer_t layers[NETWORK_MAX_LAYERS];
    network_edge_t edges[NETWORK_MAX_EDGES];
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

#endif
