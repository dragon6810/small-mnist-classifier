#include <network/sigmas/sigmas.h>

#include <assert/assert.h>

float sigmas_softmaxslope(network_layer_t* layer, float x)
{
    float softmax;

    assert(layer);

    softmax = sigmas_softmax(layer, x);
    return softmax * (1 - softmax);
}