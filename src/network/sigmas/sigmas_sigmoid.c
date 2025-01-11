#include <network/sigmas/sigmas.h>

#include <assert/assert.h>
#include <math.h>

float sigmas_sigmoid(network_layer_t* layer, float x)
{
    assert(layer);

    return 1.0 / (1.0 + expf(-x));
}