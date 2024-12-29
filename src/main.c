#include <stdio.h>

#include <network/network.h>
#include <timer/timer.h>

#define MAIN_MNISTINPUTLEN 784
#define MAIN_MNISTOUTPUTLEN 10

network_network_t network;

static void main_intromsg(void)
{
    printf("================================\n");
    printf("     Small MNIST Classifier     \n");
    printf("================================\n");
}

static void main_makelayers(void)
{
    network_layer_t layer;

    /*
     * 784 - 16 - 16 - 10
    */
    network_layerinitialize(&layer, MAIN_MNISTINPUTLEN);
    network_addlayer(&network, &layer);
    network_layerinitialize(&layer, 16);
    network_addlayer(&network, &layer);
    network_layerinitialize(&layer, 16);
    network_addlayer(&network, &layer);
    network_layerinitialize(&layer, MAIN_MNISTOUTPUTLEN);
    network_addlayer(&network, &layer);
}

int main(int argc, char** argv)
{
    main_intromsg();

    timer_begin();
    network_initialize(&network);
    main_makelayers();
    timer_end();
    printf("network initialized in %fms.\n", timer_elapsedms);

    return 0;
}
