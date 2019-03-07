//
//  control_type.h
//
//  Created by Matthew Fonken on 2/10/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef control_types_h
#define control_types_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
    
#ifndef ZDIV
#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) ((Y==0)?(X==0?0:ZDIV_LNUM):X/Y)
#endif
    
#define IN_RANGE(A,X,Y) ( ( A > X ) && ( A < Y ) )
#define SET_MAX(A,B)    ( A = MAX ( A, B ) )
    
#define MAX_THRESH 255
    
#define TARGET_STATE 2
    
    //#define NUM_STATES              10
#define NUM_OBSERVATION_SYMBOLS 2//5 // Should be max number of clusters in GMM
#define MAX_OBSERVATIONS        (1 << 5) // Length of history
#define MAX_OBSERVATION_MASK    (MAX_OBSERVATIONS-1)
    
#define MAX_DISTANCE 10000.f
#define MIN_TOTAL_MIXTURE_PROBABILITY 1e-15f
#define MAX_CLUSTERS 100
#define MAX_ERROR 20
#define INITIAL_VARIANCE 1
#define INV_INITIAL_VARIANCE (1/INITIAL_VARIANCE)
#define MIN_MAHALANOBIS_DISTANCE_SQ 1.386f
#define MAX_MAHALANOBIS_SQ_FOR_UPDATE 1000.f
#define SMALL_VALUE_ERROR_OFFSET 1e-4f
#define VALID_CLUSTER_STD_DEV 2.
#define MIN_CLUSTER_SCORE 1e-3f
#define FALLBACK_MAX_ERROR 1e-2f

#define MIN(A,B) (A<B?A:B)
#define MAX(A,B) (A>B?A:B)
    
#define ALPHA 0.5
#define BETA 4.
    
#define MAX_LABELS 10
#define LABEL_MOVING_AVERAGE_MAX_HISTORY 10
#define NULL_LABEL 0xff
#define MIN_LABEL_CONTRIBUTION 0.1
    
#define BOUNDARY_START(X)   !!(X<0)
#define BOUNDARY_END(X)     !!(X>0)
    
    typedef struct
    {
        double a,b,c,d;
    } mat2x2;
    
    typedef struct
    {
        double a,b;
    } vec2;
    
    static void vec2SubVec2(vec2 * A, vec2 * B, vec2 * C)
    {
        C->a = A->a - B->a;
        C->b = A->b - B->b;
    }
    static void vec2AddVec2(vec2 * A, vec2 * B, vec2 * C)
    {
        C->a = A->a + B->a;
        C->b = A->b + B->b;
    }
    static void scalarMulVec2(double A, vec2 * B, vec2 * C)
    {
        C->a = A * B->a;
        C->b = A * B->b;
    }
    static double getMat2x2Determinant( mat2x2 * mat )
    {
        return ( ( mat->a * mat->d ) - ( mat->c * mat->b ) );
    }
    static void getMat2x2Inverse( mat2x2 * mat, mat2x2 * res )
    {
        double det = getMat2x2Determinant( mat );
        if( det )
        {
            res->a = mat->a / det;
            res->b = mat->b / det;
            res->c = mat->c / det;
            res->d = mat->d / det;
        }
        else
            memset( res, ZDIV_LNUM, sizeof(mat2x2) );
    }
    static double * GetMat2x2ValuePointerByIndex(mat2x2 * mat, uint8_t a, uint8_t b)
    {
        uint8_t offset = a + ( b << 1 );
        return (double *)(mat+offset);
    }
    
    static void mat2x2DotVec2(mat2x2 * mat, vec2 * vec, vec2 * res)
    {
        res->a = mat->a * vec->a + mat->b * vec->b;
        res->a = mat->c * vec->a + mat->d * vec->b;
    }
    static void mat2x2AddMat2x2(mat2x2 * A, mat2x2 * B, mat2x2 * C)
    {
        C->a = A->a + B->a;
        C->b = A->b + B->b;
        C->c = A->c + B->c;
        C->d = A->d + B->d;
    }
    static void mat2x2SubMat2x2(mat2x2 * A, mat2x2 * B, mat2x2 * C)
    {
        C->a = A->a - B->a;
        C->b = A->b - B->b;
        C->c = A->c - B->c;
        C->d = A->d - B->d;
    }
    static void scalarMulMat2x2( double A, mat2x2 * B, mat2x2 * C )
    {
        C->a = A * B->a;
        C->b = A * B->b;
        C->c = A * B->c;
        C->d = A * B->d;
    }
    static void mat2x2MulMat2x2(mat2x2 * A, mat2x2 * B, mat2x2 * res)
    {
        res->a = A->a * B->a + A->b * B->c;
        res->b = A->a * B->b + A->b * B->d;
        res->c = A->c * B->a + A->d * B->c;
        res->d = A->c * B->b + A->d * B->d;
    }
    
    static void vec2DotVec2(vec2 * A, vec2 * B, double * res)
    {
        *res = A->a * B->a + A->b * B->b;
    }
    
    static void GetMat2x2LLT( mat2x2 * mat, mat2x2 * llt )
    {
        mat2x2 L = { 0., 0., 0., 0. };
        L.a = sqrt( mat->a );
        L.c = ZDIV( mat->c, L.a );
        L.d = sqrt( mat->d - L.c );
        mat2x2 LT = { L.a, L.c, L.b, L.d };
        mat2x2MulMat2x2( &L, &LT, llt );
    }
    
    static double CalculateMahalanobisDistanceSquared(mat2x2 * inv_covariance, vec2 * delta)
    {
        vec2 inv_covariance_delta;
        mat2x2DotVec2(inv_covariance, delta, &inv_covariance_delta);
        double mahalanobis_distance_squared;
        vec2DotVec2(delta, &inv_covariance_delta, &mahalanobis_distance_squared);
        return mahalanobis_distance_squared;
    }

    typedef enum
    {
        UNKNOWN_STATE = 0,
        STABLE_NONE,
        UNSTABLE_NONE,
        STABLE_SINGLE,
        UNSTABLE_SINGLE,
        STABLE_DOUBLE,
        UNSTABLE_DOUBLE,
        STABLE_MANY,
        UNSTABLE_MANY,
        NUM_STATES
    } state_t;
    
#define NUM_STATE_GROUPS ((uint8_t)NUM_STATES/2)
    
    /* Stability tracking for selec tions */
    typedef struct
    {
        double primary;
        double secondary;
        double alternate;
        double overall;
    } stability_t;
    
    /* FSM state tree with fsm base */
    typedef struct
    {
        double map[NUM_STATES][NUM_STATES];
        uint8_t length;
    } fsm_map_t;
    
    
#define UNCALCULABILTY_BOUND_FOR_SAFE_EXPONENT 30.
#define SAFE_EXPONENT_MAX_VALUE 2.35385266837019985408e17f
    
    static float safe_exp(double x)
    {
        if (x < -UNCALCULABILTY_BOUND_FOR_SAFE_EXPONENT)
            return 0.0f;
        else if (x > UNCALCULABILTY_BOUND_FOR_SAFE_EXPONENT)
            return SAFE_EXPONENT_MAX_VALUE;
        else
            return exp(x);
    }
    
    typedef struct
    {
        double
        mean,
        std_dev;
    } gaussian1d_t;
    
    typedef struct
    {
        vec2 mean;
        mat2x2 covariance;
    } gaussian2d_t;
    
    static gaussian1d_t getGaussian1dFrom2dY( gaussian2d_t * a )
    {
        return (gaussian1d_t){ a->mean.b, a->covariance.d };
    }
    
    static void mulGaussian1d( gaussian1d_t * a, gaussian1d_t * b, gaussian1d_t * c )
    {
        double cov_y_a_var = a->std_dev * a->std_dev, cov_y_b_var = b->std_dev * b->std_dev;
        double k = cov_y_a_var / ( cov_y_a_var + cov_y_b_var), mean_diff = b->mean - a->mean;
        c->mean = a->mean + k * mean_diff;
        c->std_dev = a->std_dev * ( 1 - k );
    }
    static void divGaussian1d( gaussian1d_t * a, gaussian1d_t * b, gaussian1d_t * c )
    {
        double cov_y_a_var = a->std_dev * a->std_dev, cov_y_b_var = b->std_dev * b->std_dev;
        double k = cov_y_a_var / ( cov_y_a_var - cov_y_b_var), mean_diff = a->mean - b->mean;
        c->mean = -a->mean + k * mean_diff;
        c->std_dev = a->std_dev * ( 1 + k );
    }
    
    static void copyGaussian1d( gaussian1d_t * a, gaussian1d_t * b )
    {
        b->mean = a->mean;
        b->std_dev = a->std_dev;
    }
    
    typedef struct
    {
        double a, b;
        uint8_t label;
    } observation_t;
    
    typedef struct
    {
        double value;
        int8_t label;
    } cluster_boundary_t;
    
    typedef struct
    {
        uint8_t length;
        cluster_boundary_t list[MAX_CLUSTERS*2];
    } cluster_boundary_list_t;
    
    typedef struct
    {
        double lower_boundary;
        vec2 true_center;
    } band_t;
    
    typedef struct
    {
        uint8_t length;
        band_t band[NUM_STATE_GROUPS];
    } band_list_t;
    
    typedef struct
    {
        double average[MAX_LABELS];
        uint8_t count[MAX_LABELS];
        uint8_t num_valid;
    } label_manager_t;
    
    static double GetMeanXAtGaussianYOffset( gaussian2d_t * gaussian, double y_offset )
    { /* See - https://courses.cs.washington.edu/courses/cse590b/02wi/eig2x2.cpp
       * and http://www.math.harvard.edu/archive/21b_fall_04/exhibits/2dmatrices/index.html
       */
        /* Find maximum eigen value */
        double a = gaussian->covariance.a, b = gaussian->covariance.b, d = gaussian->covariance.d,
        a_minus_d = a - d,
        radius = sqrt( a_minus_d * a_minus_d + 4. * b * b ),
        lambda = a_minus_d + radius;
        return y_offset * ZDIV( -2 * b, a - lambda );
    }
    
    static vec2 WeightedIncreaseMean( vec2 * new_val, gaussian2d_t * gaussian, double weight )
    {
        vec2 delta_mean, weighted_mean;
        vec2SubVec2( new_val, &gaussian->mean, &delta_mean );
        scalarMulVec2( weight, &delta_mean, &weighted_mean );
        vec2AddVec2( &weighted_mean, &gaussian->mean, &gaussian->mean );
        return delta_mean;
    }
    
    static void UpdateCovarianceWithWeight( vec2 * A, vec2 * B, gaussian2d_t * gaussian, double weight )
    {
        double delta_mean_a_a = A->a * B->a,
        delta_mean_a_b = A->a * B->b,
        delta_mean_b_b = A->b * B->b;
        if( delta_mean_a_a == 0. ) delta_mean_a_a += SMALL_VALUE_ERROR_OFFSET;
        if( delta_mean_b_b == 0. ) delta_mean_b_b += SMALL_VALUE_ERROR_OFFSET;
        mat2x2 covariance_delta_factor, unweighted_covariance_factor =
        { delta_mean_a_a, delta_mean_a_b, delta_mean_a_b, delta_mean_b_b };
        
        printf("m_covf: <%.2f %.2f> [%.2f %.2f | %.2f %.2f]\n", A->a, A->b, unweighted_covariance_factor.a, unweighted_covariance_factor.b, unweighted_covariance_factor.c, unweighted_covariance_factor.d);
        
        mat2x2SubMat2x2( &unweighted_covariance_factor, &gaussian->covariance, &covariance_delta_factor );
        scalarMulMat2x2( weight, &covariance_delta_factor, &covariance_delta_factor );
        mat2x2AddMat2x2( &gaussian->covariance, &covariance_delta_factor, &gaussian->covariance );
    }
    
    static bool ReportLabel( label_manager_t * labels, uint8_t new_label )
    {
        if( new_label > MAX_LABELS ) return false;
        labels->count[new_label]++;
        double sum = 0;
        for( uint8_t i = 0; i < MAX_LABELS; i++ )
            sum += (double)labels->count[i];
        for( uint8_t i = 0; i < MAX_LABELS; i++ )
            labels->average[i] = (double)labels->count[i] / sum;
        return true;
    }
    
    static void GetTopTwoLabels( label_manager_t * labels, uint8_t * first, uint8_t * second )
    {
        *first = 0; *second = 1;
        double first_value = labels->average[0], second_value = labels->average[1], current = 0.;
        for( uint8_t i = 2; i < MAX_LABELS; i++ )
        {
            current = labels->average[i];
            if( current > first_value )
            {
                second_value = first_value;
                *second = *first;
                first_value = current;
                *first = i;
            }
            else if ( current > second_value )
            {
                second_value = current;
                *second = i;
            }
        }
    }
    
    static uint8_t GetNumLabels( label_manager_t * labels )
    {
        uint8_t num = 0;
        for( uint8_t i = 0; i < MAX_LABELS; i++ )
        {
            if( labels->average[i] > MIN_LABEL_CONTRIBUTION )
                num++;
        }
        return num;
    }
    
    typedef struct
    {
        uint8_t
        data[MAX_OBSERVATIONS],
        next, first, last, curr, prev;
    } observation_buffer_t;
    
    static uint8_t addToObservationBuffer( observation_buffer_t * buffer, uint8_t v )
    {
        buffer->prev = buffer->curr;
        buffer->curr = v;
        buffer->last = buffer->next;
        buffer->data[buffer->next++] = v;
        buffer->next &= MAX_OBSERVATION_MASK;
        if(buffer->next == buffer->first)
            buffer->first = ( buffer->next + 1 ) & MAX_OBSERVATION_MASK;
        return buffer->last;
    }
    
#ifdef __cplusplus
}
#endif

#endif /* control_types_h */
