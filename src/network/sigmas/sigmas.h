#ifndef _sigmas_h
#define _sigmas_h

#include <network/network.h>

/*
 * ================================
 *        TYPES DEFENITIONS
 * ================================
*/

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

float sigmas_relu(network_layer_t* layer, float x);
float sigmas_reluslope(network_layer_t* layer, float x);

float sigmas_softmax(network_layer_t* layer, float x);
float sigmas_softmaxslope(network_layer_t* layer, float x);

float sigmas_sigmoid(network_layer_t* layer, float x);
float sigmas_sigmoidslope(network_layer_t* layer, float x);

#endif