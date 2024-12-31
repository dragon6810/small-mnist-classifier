#include <network/sigmas/sigmas.h>

float sigmas_relu(network_layer_t* layer, float x)
{
    if(x < 0)
        return 0;

    return x;
}