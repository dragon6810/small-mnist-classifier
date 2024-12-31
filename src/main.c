#include <stdio.h>
#include <assert.h>

#include <network/network.h>
#include <timer/timer.h>
#include <random/random.h>
#include <network/sigmas/sigmas.h>

#define MAIN_MNISTINPUTLEN  784
#define MAIN_MNISTOUTPUTLEN 10
#define MAIN_MNISTNDIGITS   60000
#define MAIN_NEPOCHS        4
#define MAIN_NBATCHES       500
#define MAIN_BATCHSIZE      (MAIN_MNISTNDIGITS / MAIN_NBATCHES)

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
list_t digits;

typedef struct main_mnistdigit_s
{
    float values[MAIN_MNISTINPUTLEN];
    int label;
} main_mnistdigit_t;

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

static void main_loadmnistlabels()
{
    int i;

    FILE* ptr;
    unsigned char datatype, ndimensions;
    unsigned int dimsize;
    unsigned char curval;
    main_mnistdigit_t *digitsdata;

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
    
    digitsdata = (main_mnistdigit_t*) digits.data;
    for(i=0; i<dimsize; i++)
    {
        fread(&curval, 1, 1, ptr);
        digitsdata[i].label = curval;
    }

    fclose(ptr);
}

static void main_loadmnist()
{
    int d, i;

    FILE* ptr;
    unsigned char datatype, ndimensions;
    unsigned int dimsizes[3];
    unsigned char curval;
    main_mnistdigit_t *digitsdata;

    assert(digits.size == MAIN_MNISTNDIGITS);

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

    digitsdata = (main_mnistdigit_t*) digits.data;
    for(d=0; d<dimsizes[0]; d++)
    {
        for(i=0; i<28*28; i++)
        {
            fread(&curval, 1, 1, ptr);
            digitsdata[d].values[i] = (float) curval / 255.0;
        }
    }

    fclose(ptr);
}

static void main_makelayers(void)
{
    network_layer_t layer;

    /*
     * 784 - 16 - 16 - 10
    */
    network_layerinitialize(&layer, MAIN_MNISTINPUTLEN, NULL, NULL);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, 16, sigmas_relu, sigmas_reluslope);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, 16, sigmas_relu, sigmas_reluslope);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, MAIN_MNISTOUTPUTLEN, sigmas_softmax, sigmas_softmaxslope);
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
    int e, i, j, k;

    int wanted;
    vector_t vwanted;
    main_mnistdigit_t *digitsdata;
    network_layer_t *input;
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
    list_initialize(&digits, sizeof(main_mnistdigit_t));
    list_resize(&digits, MAIN_MNISTNDIGITS);
    main_loadmnist();
    main_loadmnistlabels();
    timer_end();
    printf("mnist training set loaded in %fms.\n", timer_elapsedms);

    timer_begin();
    digitsdata = (main_mnistdigit_t*) digits.data;
    input = (network_layer_t*) network.layers.data;
    for(e=0; e<MAIN_NEPOCHS; e++)
    {
        for(i=0; i<MAIN_NBATCHES; i++)
        {
            printf("batch  %d:\n", i);

            for(j=i*MAIN_BATCHSIZE; j<(i+1)*MAIN_BATCHSIZE; j++)
            {
                vector_alloc(&vwanted, MAIN_MNISTOUTPUTLEN);
                vwanted.data[digitsdata[j].label] = 1.0;

                for(k=0; k<MAIN_MNISTINPUTLEN; k++)
                    ((network_node_t*)input->nodes.data)[k].val = digitsdata[j].values[k];

                network_run(&network);
                network_backprop(&network, vwanted, MAIN_BATCHSIZE);
                network_cleanupbackprop(&network);

                vector_free(&vwanted);

                output = &((network_layer_t*)network.layers.data)[network.layers.size-1];
                printf("    expected %hhu:\n", digitsdata[j].label);
                for(k=0; k<MAIN_MNISTOUTPUTLEN; k++)
                    printf("        %d: %f.\n", k, ((network_node_t*)output->nodes.data)[k].val);
            }

            network_learn(&network);
        }

        list_shuffle(&digits, &digits);
    }

    timer_end();
    printf("network ran in %fms:\n", timer_elapsedms);
    
    return 0;
}
