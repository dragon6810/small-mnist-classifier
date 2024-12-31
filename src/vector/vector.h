#ifndef _vector_h
#define _vector_h

/*
 * ================================
 *        TYPES DEFENITIONS
 * ================================
*/

typedef struct vector_s vector_t;

struct vector_s
{
    unsigned long int len;
    float* data;
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

void vector_alloc(vector_t* vec, unsigned long int len);
void vector_free(vector_t* vec);

#endif