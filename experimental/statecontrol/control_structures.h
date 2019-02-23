//
//  control_structures.h
//  hmmtest
//
//  Created by Matthew Fonken on 2/11/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef control_structures_h
#define control_structures_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include "kumaraswamy.h"

/* System self-diagnostic state control type */
typedef struct
{
    state_t         state;
    state_t         prev;
    state_t         next;
    uint8_t         selection_index;
    stability_t     stability;
    fsm_map_t       probabilities;
} fsm_system_t;
    
typedef struct
{
    gaussian2d_t
        gaussian_in,
        gaussian_out;
    double
        mahalanobis_sq,
        log_gaussian_norm_factor,
        probability_condition_input,
        score, weight;
    mat2x2
        inv_covariance_in,
        llt_in;
    label_manager_t labels;
    double max_y, min_y;
    uint8_t primary_id, secondary_id;
} gaussian_mixture_cluster_t;

typedef struct
{
    gaussian_mixture_cluster_t
        cluster[MAX_CLUSTERS];
    uint8_t
        num_clusters,
        selected_cluster;
    double
        initial_variance;
    vec2
        min_in,
        max_in,
        max_out,
        min_out;
} gaussian_mixture_model_t;

typedef struct
{
    double map[NUM_OBSERVATION_SYMBOLS][NUM_STATES];
    uint8_t num_observation_symbols;
} observation_matrix_t;

typedef struct
{
    double                  p[NUM_STATES];       // Initial probabilities
    fsm_system_t            A;                   // Transition matrix
    observation_matrix_t    B;                   // Observation matrix
    observation_buffer_t    O;                   // Observation sequence
    double                  K[NUM_STATES];       // Kumaraswamy boundaries
    
    uint8_t
        T, // Number of observations
        N, // Number of states
        M; // Number of observation symbols
    uint8_t
        best_state;
    double
        best_confidence;
} hidden_markov_model_t;

typedef struct
{ /* Predictive State Model */
    gaussian_mixture_model_t gmm;
    hidden_markov_model_t hmm;
    kumaraswamy_t kumaraswamy;
    band_list_t state_bands;
    double
        previous_thresh,
        proposed_thresh,
        proposed_nu,
        proposed_primary_id,
        proposed_secondary_id;
} psm_t;
    
#ifdef __cplusplus
}
#endif

#endif /* control_structures_h */
