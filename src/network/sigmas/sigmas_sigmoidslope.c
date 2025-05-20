#include <network/sigmas/sigmas.h>

#include <std/assert/assert.h>
#include <math.h>

float sigmas_sigmoidslope(network_layer_t* layer, float x)
{
    float sigmoid;

    assert(layer);

    sigmoid = sigmas_sigmoid(layer, x);
    return sigmoid * (1.0 - sigmoid);
}