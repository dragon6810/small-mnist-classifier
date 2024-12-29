#include <stdio.h>
#include <assert.h>

#include <network/network.h>
#include <timer/timer.h>
#include <random/random.h>

#define MAIN_MNISTINPUTLEN  784
#define MAIN_MNISTOUTPUTLEN 10
#define MAIN_MNISTNDIGITS   60000

#if defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && (__BYTE_ORDER == __BIG_ENDIAN)
    #define HOST_IS_BIG_ENDIAN 1
#else
    #define HOST_IS_BIG_ENDIAN 0
#endif

#define SWAP32(x) ((unsigned int)((((x) & 0x000000FF) << 24) | \
                              (((x) & 0x0000FF00) << 8)  | \
                              (((x) & 0x00FF0000) >> 8)  | \
                              (((x) & 0xFF000000) >> 24)))

#define BIG_TO_HOST32(x) (HOST_IS_BIG_ENDIAN ? (x) : SWAP32(x))

network_network_t network;

static unsigned char main_mnistexpect(int index)
{
    int i;

    FILE* ptr;
    unsigned char datatype, ndimensions;
    unsigned int dimsize;
    unsigned char curval;
    network_layer_t *input;

    ptr = fopen("mnist/train-labels-idx1-ubyte", "rb");
    assert(ptr);

    fseek(ptr, 2, SEEK_SET);
    fread(&datatype, 1, 1, ptr);
    fread(&ndimensions, 1, 1, ptr);

    assert(datatype == 0x08);
    assert(ndimensions == 1);

    fread(&dimsize, sizeof(int), 1, ptr);
    dimsize = BIG_TO_HOST32(dimsize);

    assert(dimsize == MAIN_MNISTNDIGITS);
    assert(index < dimsize);

    fseek(ptr, index, SEEK_CUR);
    fread(&curval, 1, 1, ptr);

    fclose(ptr);

    return curval;
}

static void main_loadmnist(int index)
{
    int i;

    FILE* ptr;
    unsigned char datatype, ndimensions;
    unsigned int dimsizes[3];
    unsigned char curval;
    network_layer_t *input;

    ptr = fopen("mnist/train-images-idx3-ubyte", "rb");
    assert(ptr);

    fseek(ptr, 2, SEEK_SET);
    fread(&datatype, 1, 1, ptr);
    fread(&ndimensions, 1, 1, ptr);

    assert(datatype == 0x08);
    assert(ndimensions == 3);

    for(i=0; i<ndimensions; i++)
    {
        fread(&dimsizes[i], sizeof(int), 1, ptr);
        dimsizes[i] = BIG_TO_HOST32(dimsizes[i]);
    }

    assert(dimsizes[0] == MAIN_MNISTNDIGITS);
    assert(dimsizes[1] == 28);
    assert(dimsizes[2] == 28);
    assert(index < dimsizes[0]);

    input = (network_layer_t*) network.layers.data;
    fseek(ptr, index*28*28, SEEK_CUR);
    for(i=0; i<28*28; i++)
    {
        fread(&curval, 1, 1, ptr);
        ((network_node_t*)input->nodes.data)[i].val = (float) curval / 255.0;
    }

    fclose(ptr);
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

static void main_intromsg(void)
{
    printf("================================\n");
    printf("     Small MNIST Classifier     \n");
    printf("================================\n");
}

int main(int argc, char** argv)
{
    int i, j;

    int wanted;
    network_layer_t *output;

    random_seed();

    main_intromsg();

    timer_begin();
    network_initialize(&network);
    main_makelayers();
    network_genedges(&network);
    timer_end();
    printf("network initialized in %fms.\n", timer_elapsedms);

    timer_begin();
    for(i=0; i<MAIN_MNISTNDIGITS; i++)
    {
        printf("training iteration %d:\n", i);
        
        main_loadmnist(i);
        wanted = main_mnistexpect(i);
        network_run(&network);
        network_learn(&network, wanted);

        output = &((network_layer_t*)network.layers.data)[network.layers.size-1];
        printf("    expected %hhu.\n", wanted);
        for(j=0; j<MAIN_MNISTOUTPUTLEN; j++)
            printf("    %d: %f.\n", j, ((network_node_t*)output->nodes.data)[j].val);
    
        timer_end();
    }
    printf("network ran in %fms:\n", timer_elapsedms);
    
    return 0;
}
