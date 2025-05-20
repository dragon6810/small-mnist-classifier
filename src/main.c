#include <stdio.h>
#include <std/assert/assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <network/network.h>
#include <std/timer/timer.h>
#include <std/random/random.h>
#include <network/sigmas/sigmas.h>

#define MAIN_MNISTINPUTLEN  784
#define MAIN_MNISTOUTPUTLEN 10
#define MAIN_MNISTNDIGITS   60000
#define MAIN_NEPOCHS        8
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

typedef struct main_mnistdigit_s
{
    float values[MAIN_MNISTINPUTLEN];
    int label;
} main_mnistdigit_t;

LIST_TYPE(main_mnistdigit_t, list_main_mnistdigit)

network_network_t network;
list_main_mnistdigit_t digits;

static void main_runtests(void)
{
    const int learniters = 100000;

    int i;

    timer_begin();

    network_initialize(&network);

    network_layer_t layer;
    network_layer_t *layersdata;
    network_edge_t *edgesdata;
    network_edge_t **interedgesdata;
    network_node_t *inputdata, *outputdata;
    vector_t wantvec;

    /*
     * 2 - 2
    */
    network_layerinitialize(&layer, 2, NULL, NULL);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, 2, sigmas_none, sigmas_noneslope);
    network_addlayer(&network, &layer);

    network_genedges(&network);

    // Initialize all weights to 1
    edgesdata = (network_edge_t*) network.edges.data;
    for(i=0; i<network.edges.size; i++)
        edgesdata[i].weight = 1;

    // Initialize all biases to 0
    layersdata = (network_layer_t*) network.layers.data;
    outputdata = (network_node_t*) layersdata[1].nodes.data;
    for(i=0; i<layersdata[1].nodes.size; i++)
        outputdata[i].bias = 0;

    inputdata = (network_node_t*) layersdata[0].nodes.data;
    inputdata[0].val = 1;
    inputdata[1].val = 1;
    network_run(&network);
    assert(outputdata[0].val == 2.0);
    assert(outputdata[1].val == 2.0);

    vector_alloc(&wantvec, 2);

    wantvec.data[0] = 1;
    wantvec.data[1] = 0;
    network_backprop(&network, wantvec, 1);

    printf("negative gradients:\n");
    interedgesdata = (network_edge_t**) outputdata[0].edges[0].data;
    printf("bias 0: %f.\n", outputdata[0].wantnudge);
    printf("weight 0, 0: %f.\n", interedgesdata[0]->wantnudge);
    printf("weight 1, 0: %f.\n", interedgesdata[1]->wantnudge);
    interedgesdata = (network_edge_t**) outputdata[1].edges[0].data;
    printf("bias 1: %f.\n", outputdata[1].wantnudge);
    printf("weight 0, 1: %f.\n", interedgesdata[0]->wantnudge);
    printf("weight 1, 1: %f.\n", interedgesdata[1]->wantnudge);

    interedgesdata = (network_edge_t**) outputdata[0].edges[0].data;
    assert(interedgesdata[0]->wantnudge == 2.0);
    assert(interedgesdata[1]->wantnudge == 2.0);
    interedgesdata[0]->wantnudge = interedgesdata[1]->wantnudge = 0;
    interedgesdata = (network_edge_t**) outputdata[1].edges[0].data;
    assert(interedgesdata[0]->wantnudge == 4.0);
    assert(interedgesdata[1]->wantnudge == 4.0);
    interedgesdata[0]->wantnudge = interedgesdata[1]->wantnudge = 0;
    assert(outputdata[0].wantnudge == 2.0);
    assert(outputdata[1].wantnudge == 4.0);
    outputdata[0].wantnudge = outputdata[1].wantnudge = 0;

    for(i=0; i<learniters; i++)
    {
        network_run(&network);
        network_backprop(&network, wantvec, 1);
        network_learn(&network);
    }

    network_run(&network);
    printf("after learning:\n");
    printf("%f.\n%f.\n", outputdata[0].val, outputdata[1].val);
    assert(fabs(outputdata[0].val - 1.0) < 0.1);
    assert(fabs(outputdata[1].val - 0.0) < 0.1);

    network_free(&network);
    network_initialize(&network);

    /*
     * 2 - 2
    */
    network_layerinitialize(&layer, 2, NULL, NULL);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, 4, sigmas_sigmoid, sigmas_sigmoidslope);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, 4, sigmas_sigmoid, sigmas_sigmoidslope);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, 2, sigmas_sigmoid, sigmas_sigmoidslope);
    network_addlayer(&network, &layer);

    network_genedges(&network);

    layersdata = (network_layer_t*) network.layers.data;
    outputdata = (network_node_t*) layersdata[2].nodes.data;
    inputdata = (network_node_t*) layersdata[0].nodes.data;
    inputdata[0].val = 1;
    inputdata[1].val = 1;
    for(i=0; i<learniters; i++)
    {
        network_run(&network);
        network_backprop(&network, wantvec, 1);
        network_learn(&network);
    }

    network_run(&network);
    printf("after learning:\n");
    printf("%f.\n%f.\n", outputdata[0].val, outputdata[1].val);
    assert(fabs(outputdata[0].val - 1.0) < 0.1);
    assert(fabs(outputdata[1].val - 0.0) < 0.1);

    vector_free(&wantvec);
    network_free(&network);

    assert(fabs(sigmas_sigmoid(&layersdata[1], -2.0) - 0.1192) < 0.01);
    assert(fabs(sigmas_sigmoid(&layersdata[1],  0.0) - 0.5000) < 0.01);
    assert(fabs(sigmas_sigmoid(&layersdata[1],  2.0) - 0.8808) < 0.01);

    assert(fabs(sigmas_sigmoidslope(&layersdata[1], -2.0) - 0.1050) < 0.01);
    assert(fabs(sigmas_sigmoidslope(&layersdata[1],  0.0) - 0.2500) < 0.01);
    assert(fabs(sigmas_sigmoidslope(&layersdata[1],  2.0) - 0.1050) < 0.01);

    timer_end();
    printf("all tests passed in %fms.\n", timer_elapsedms);
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

    network_layerinitialize(&layer, 16, sigmas_sigmoid, sigmas_sigmoidslope);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, 16, sigmas_sigmoid, sigmas_sigmoidslope);
    network_addlayer(&network, &layer);

    network_layerinitialize(&layer, MAIN_MNISTOUTPUTLEN, sigmas_sigmoid, sigmas_sigmoidslope);
    network_addlayer(&network, &layer);
}

static void main_intromsg(void)
{
    printf("================================\n");
    printf("     Small MNIST Classifier     \n");
    printf("================================\n");
}

static void main_usagemsg(void)
{
    printf("Usage:");
    printf(" small-mnist-classifier");
    printf(" [-ht]");
    printf("\n");
    printf("-h: prints usage and returns.\n");
    printf("-t: runs test instead of core program.\n");
}

int main(int argc, char** argv)
{
    int e, i, j, k;

    vector_t vwanted;
    network_layer_t *input;
    network_layer_t *output;

    random_seed();

    main_intromsg();

    for(i=1; i<argc; i++)
    {
        if(argv[i][0] == '-')
        {
            for(j=1; j<strlen(argv[i]); j++)
            {
                switch(argv[i][j])
                {
                case 'h':
                    main_usagemsg();
                    return 0;
                case 't':
                    main_runtests();
                    return 0;
                default:
                    fprintf(stderr, "unknown option '%c'.\n", argv[i][j]);
                    main_usagemsg();
                    return 1;
                }
            }
        }
    }

    timer_begin();
    network_initialize(&network);
    main_makelayers();
    network_genedges(&network);
    timer_end();
    printf("network initialized in %fms.\n", timer_elapsedms);

    timer_begin();
    LIST_INITIALIZE(digits);
    LIST_RESIZE(digits, MAIN_MNISTNDIGITS);
    main_loadmnist();
    main_loadmnistlabels();
    timer_end();
    printf("mnist training set loaded in %fms.\n", timer_elapsedms);

    timer_begin();
    input = (network_layer_t*) network.layers.data;
    for(e=0; e<MAIN_NEPOCHS; e++)
    {
        for(i=0; i<MAIN_NBATCHES; i++)
        {
            printf("batch  %d:\n", i);

            for(j=i*MAIN_BATCHSIZE; j<(i+1)*MAIN_BATCHSIZE; j++)
            {
                vector_alloc(&vwanted, MAIN_MNISTOUTPUTLEN);
                vwanted.data[digits.data[j].label] = 1.0;

                for(k=0; k<MAIN_MNISTINPUTLEN; k++)
                    input->nodes.data[k].val = digits.data[j].values[k];

                network_run(&network);
                network_backprop(&network, vwanted, MAIN_BATCHSIZE);

                vector_free(&vwanted);

                output = &network.layers.data[network.layers.size-1];
                printf("    expected %hhu:\n", digits.data[j].label);
                for(k=0; k<MAIN_MNISTOUTPUTLEN; k++)
                    printf("        %d: %f.\n", k, output->nodes.data[k].val);
            }

            network_learn(&network);
        }

        LIST_SHUFFLE(digits, digits);
    }

    network_free(&network);

    timer_end();
    printf("network ran in %fms:\n", timer_elapsedms);
    
    return 0;
}
