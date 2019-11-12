/***********************************************************************************************//**
* \file   vector.h
* \brief  Vector Math Header for QMath
***************************************************************************************************
*      Author: Matthew Fonken
**************************************************************************************************/

#ifndef vector_h
#define vector_h

#ifdef __cplusplus
extern "C" {
#endif

#include "qmath_types.h"
    
//#include "matrix.h"
//#include "math_types.h"
    
#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) Y==0?ZDIV_LNUM:X/Y

    typedef struct kvector kvector;
    struct kvector
    {
        floating_t  (*len2)( vec3_t * v );
        floating_t  (*len3)( vec3_t * v );
        void        (*nrm3)( vec3_t * v );
        void        (*sub3)( vec3_t * x, vec3_t * y, vec3_t * z );
        floating_t  (*dot3)( vec3_t * u, vec3_t * v );
        floating_t  (*ang2)( vec3_t * u, vec3_t * v );
        floating_t  (*ang3)( vec3_t * u, vec3_t * v );
        void        (*mul3)( floating_t m,   vec3_t * v, vec3_t * r );
        void      (*cross3)( vec3_t * u, vec3_t * v, vec3_t * r );
        void        (*add3)( vec3_t * u, vec3_t * v, vec3_t * w );
        void       (*add33)( vec3_t * u, vec3_t * v, vec3_t * w, vec3_t * r);
        void        (*rot2)( vec3_t* a, floating_t theta);
    };
    
    extern const kvector Vector;
    
#ifdef __cplusplus
}
#endif

#endif /* matrix_h */
