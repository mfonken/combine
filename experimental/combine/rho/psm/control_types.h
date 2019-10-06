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

#ifdef __RHO__
#include "rho_config.h"
#endif
    
#include "kalman.h"
#include "pid.h"
#include "matvec.h"

#ifndef ZDIV
#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) ((Y==0)?(X==0?0:ZDIV_LNUM):X/Y)
#endif

#ifndef MIN
#define MIN(A,B) (A<B?A:B)
#endif

#ifndef MAX
#define MAX(A,B) (A>B?A:B)
#endif

#define IN_RANGE(A,X,Y) ( ( A > X ) && ( A < Y ) )
#define SET_MAX(A,B)    ( A = MAX ( A, B ) )

#define MAX_THRESH 255
#define MIN_THRESH 0
#define THRESH_RANGE (MAX_THRESH - MIN_THRESH)

#define MAX_SINGLE_CONFIDENCE 1

#define TARGET_STATE TARGET_POPULATED

      typedef enum
    {
        UNKNOWN_STATE = -1,
//        NONE_POPULATED,
        UNDER_POPULATED,
        TARGET_POPULATED,
        OVER_POPULATED,
        CHAOTIC,
        NUM_STATES
    } state_t;
#define NUM_STATE_GROUPS NUM_STATES

    /* Stability tracking for selec tions */
    typedef struct
    {
        kalman_filter_t
            state,
            system;
    } stability_t;

#ifdef __PSM__
    //#define NUM_STATES              10
//#define NUM_OBSERVATION_SYMBOLS 2//5 // Should be max number of clusters in GMM
#ifndef MAX_OBSERVATIONS
#define MAX_OBSERVATIONS        (1 << 7) // Length of history
#endif
#define MAX_OBSERVATION_MASK    ((MAX_OBSERVATIONS)-1)
#define HMM_UPDATE_DELTA 1.0

#define MAX_STD_DEVS_TO_BE_INCLUDED_IN_BAND_CALC 3
#define MIN_STD_DEV_SPAN_TO_REJECT_FOR_BAND_CALC 8
#define MIN_VARIANCE_SPAN_TO_REJECT_FOR_BAND_CALC ( MIN_STD_DEV_SPAN_TO_REJECT_FOR_BAND_CALC * MIN_STD_DEV_SPAN_TO_REJECT_FOR_BAND_CALC )

#define MAX_CLUSTER_LIFETIME 10. // sec
#define MAX_DISTANCE 1000.f
#define MIN_TOTAL_MIXTURE_PROBABILITY 1e-15f
#define MAX_CLUSTERS 25
#define MAX_ERROR 0.2
#define INITIAL_VARIANCE 50.
#define INV_INITIAL_VARIANCE (1./INITIAL_VARIANCE)
#define MAX_MAHALANOBIS_SQ 9//25
#define MAX_MAHALANOBIS_SQ_FOR_UPDATE MAX_MAHALANOBIS_SQ
#define VALID_CLUSTER_STD_DEV 0.25
#define MIN_CLUSTER_SCORE 0.5
#define FALLBACK_MAX_ERROR 1e-2f
#define ALPHA 0.0001
#define BETA 2
    
#define SMALL_VALUE_ERROR_OFFSET 1e-4f

#define MAX_LABELS 10
#define LABEL_MOVING_AVERAGE_MAX_HISTORY 10
#define NULL_LABEL 0xff
#define MIN_LABEL_CONTRIBUTION 0.15

#define BOUNDARY_START(X)   !!(X<0)
#define BOUNDARY_END(X)     !!(X>0)
#endif
    
#define HMM_GAUSSIAN_EMISSIONS
#ifdef USE_2D_OBSERVATIONS
#define HMM_2D_EMISSIONS
#endif
    
#ifdef HMM_2D_EMISSIONS
    typedef vec2         hmm_observation_t;
    typedef gaussian2d_t emission_t;
#else
    typedef double       hmm_observation_t;
    typedef gaussian1d_t emission_t;
#endif
    
    typedef struct
    {
        index_t density;
        uint8_t thresh;
        uint8_t label;
    } observation_t;
    
    typedef struct
    {
        observation_t observations[MAX_OBSERVATIONS];
        uint8_t length;
    } observation_list_t;
    
//    typedef enum
//    {
//        UNKNOWN_SYMBOL = -1,
//        ZERO_SYMBOL,
//        ONE_SYMBOL,
//        TWO_SYMBOL,
//        THREE_SYMBOL,
//        MANY_SYMBOL
//    } observation_symbol_t;
    
    /* FSM state tree with fsm base */
    typedef struct
    {
        double map[NUM_STATES][NUM_STATES];
        uint8_t length;
    } fsm_map_t;

    typedef struct
    {
        double
        lower_boundary,
        upper_boundary,
        variance;
        vec2
        true_center;
    } band_t;
    
    typedef struct
    {
        uint8_t length;
        band_t band[NUM_STATES];
    } band_list_t;

#ifdef __PSM__
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
        double average[MAX_LABELS];
        uint8_t count[MAX_LABELS];
        uint8_t num_valid;
    } label_manager_t;

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
    
    static uint32_t GetValidLabels( label_manager_t * labels )
    {
#if MAX_LABELS > 32
#warning "Valid label mask is max 32 bit wide."
#endif
        uint32_t valid_vector = 0;
        for( uint8_t i = 0; i < MAX_LABELS; i++ )
        {
            valid_vector |= ( labels->average[i] > MIN_LABEL_CONTRIBUTION ) << i;
        }
        return valid_vector;
    }
    static uint8_t CountSet( uint32_t v )
    {
        uint8_t c = 0;
        for( uint8_t i = 0, m = 1; i < 32; i++, c += !!(v & m), m <<= 1 );
        return c;
    }
    static uint32_t GetNumberOfValidLabels( label_manager_t * labels )
    {
        return CountSet( GetValidLabels( labels ) );
    }

    typedef struct
    {
        uint8_t length;
        hmm_observation_t data[MAX_OBSERVATIONS];
        struct { uint8_t next, first, last; } index;
        struct { hmm_observation_t curr, prev; } value;
    } observation_buffer_t;
    
    static uint8_t PushToObservationBuffer( observation_buffer_t * buffer, hmm_observation_t v )
    {
        buffer->value.prev = buffer->value.curr;
        buffer->value.curr = v;
        
        buffer->data[buffer->index.next] = buffer->value.curr;
        buffer->index.next = ( ( buffer->index.next + 1 ) & MAX_OBSERVATION_MASK );
        
        if( buffer->index.next == buffer->index.first )
        { // Update first if looped (filled)
            buffer->index.last = buffer->index.next;
            buffer->index.first = ( ( buffer->index.next + 1 ) & MAX_OBSERVATION_MASK );
        }
        else
        {
            buffer->length++;
        }
        return buffer->index.last;
    }
    static hmm_observation_t PullFromObservationBuffer( observation_buffer_t * buffer )
    {
        if( buffer->index.next == buffer->index.first ) return
#ifdef HMM_2D_EMISSIONS
            (vec2){ -1, -1 };
#else
        -1;
#endif
        buffer->index.first = ( ( buffer->index.first + 1 ) & MAX_OBSERVATION_MASK );
        buffer->length--;
        return buffer->data[buffer->index.first];
    }
    static uint8_t GetLengthObservationBuffer( observation_buffer_t * buffer )
    {
        uint8_t l = buffer->index.next - buffer->index.first;
        if( !l ) l = MAX_OBSERVATIONS;
        buffer->length = l;
        return l;
    }
    static hmm_observation_t GetIndexObservationBuffer( observation_buffer_t * buffer, uint8_t i )
    {
        uint8_t l = buffer->length;// GetLengthObservationBuffer( buffer );
        if( i > l ) return
#ifdef HMM_2D_EMISSIONS
            (vec2){ -1, -1 };
#else
        -1;
#endif
        uint8_t io = ( ( buffer->index.first + i ) & MAX_OBSERVATION_MASK );
        return buffer->data[io];
    }
    
    static floating_t NumStdDevsFromYMean( gaussian2d_t * gaussian, floating_t check_value )
    {
        floating_t diff = fabs( gaussian->mean.b - check_value );
        floating_t num_std_devs = ZDIV( diff, sqrt( gaussian->covariance.d ) );
        return num_std_devs;
    }
    
    static floating_t MeanXAtGaussianYOffset( gaussian2d_t * gaussian, floating_t y_offset )
    { /* See - https://courses.cs.washington.edu/courses/cse590b/02wi/eig2x2.cpp
       * and http://www.math.harvard.edu/archive/21b_fall_04/exhibits/2dmatrices/index.html
       */
        /* Find maximum eigen value */
        floating_t a = gaussian->covariance.a, b = gaussian->covariance.b, d = gaussian->covariance.d,
        a_minus_d = a - d,
        radius = sqrt( a_minus_d * a_minus_d + 4. * b * b ),
        lambda = a_minus_d + radius;
        return y_offset * ZDIV( -2 * b, a - lambda );
    }
#endif
    static floating_t WeightedAverage( floating_t a, floating_t b, floating_t w )
   {
       return ( ( a * w ) + ( b * ( 1 - w ) ) );
   }

#ifdef __cplusplus
}
#endif

#endif /* control_types_h */
