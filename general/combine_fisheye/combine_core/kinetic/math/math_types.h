/***************************************************************************************************
 * \file   kinetic_types.h
 * \brief  Types for kinetic math and filtering
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef math_types_h
#define math_types_h

#ifdef __cplusplus
extern "C" {
#endif
    
//#include "quaternion.h"
//#include "matrix.h"
//#include "vector.h"
    
/***************************************************************************************************
 Public MACROS
 ***************************************************************************************************/
    
#define RAD_TO_DEG (180 / M_PI)
#define DEG_TO_RAD (M_PI / 180)
    
#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) Y==0?ZDIV_LNUM:X/Y
#define SIGN(X)   X>=0?1:-1
#define RASIN(X)  X>-1&&X<1?asin(X):SIGN(X)*M_PI/2

#ifdef __cplusplus
}
#endif
    
#endif /* KINETIC_KINETIC_TYPES_H_ */
