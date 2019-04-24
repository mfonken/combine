//
//  psm.c
//  hmmtest
//
//  Created by Matthew Fonken on 2/12/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#include "psm.h"

void InitializePSM( psm_t * model )
{
    GMMFunctions.Model.Initialize( &model->gmm );
    HMMFunctions.Initialize( &model->hmm );
    KumaraswamyFunctions.Initialize( &model->kumaraswamy, NUM_STATES + 1 );
    model->state_bands.length = NUM_STATE_GROUPS;
    
#ifdef __PSM__
    double default_bands_intervals[] = STATE_KUMARASWAMY_INTERVALS;
    double prev_boundary = 0., vertical_center = 0;
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
    {
        band_t * band = &model->state_bands.band[i];
        band->lower_boundary = prev_boundary;
        band->upper_boundary = default_bands_intervals[i];
        vertical_center = ( ( (double)( band->upper_boundary + band->lower_boundary ) ) * THRESH_RANGE ) / 2;
        band->true_center = (vec2){ 0, vertical_center };
    }
#else
    memset( model->state_bands.band, 0, sizeof(model->state_bands.band) );
#endif
}

void ReportObservationsPSM( psm_t * model, observation_list_t * observation_list )
{
    /* Cycle through and add observations to gaussian mixture model */
    vec2 value = { 0 };
    for( uint8_t i = 0; i < observation_list->length && i < MAX_OBSERVATIONS; i++ )
        GMMFunctions.Model.AddValue( &model->gmm, &observation_list->observations[i], &value );
    model->current_observation = observation_list->length;
    if( observation_list->length > 0 )
        model->previous_thresh = observation_list->observations[0].b;
    // Analyse value
}

void UpdatePSM( psm_t * model, observation_list_t * observation_list, double nu )
{
#ifdef __PSM__
    /* Report tracking observations & update state band knowledge  */
    if( observation_list->length > 0 )
        PSMFunctions.ReportObservations( model, observation_list );
    PSMFunctions.DiscoverStateBands( model, &model->state_bands );
    
    /* Calculate current observation and update observation matrix */
    model->observation_state = PSMFunctions.GetCurrentBand( model, &model->state_bands );
    model->hmm.T = AddToObservationBuffer( &model->hmm.O, model->current_observation );// model->observation_state );
    ///TODO: Compare model->observation_state with hmm recommendation
    
    /* Update state path prediction to best cluster */
    HMMFunctions.UpdateObservationMatrix( &model->hmm );
    
    /* Update state bands */
    floating_t state_bands[NUM_STATE_GROUPS] = { 0. },
    observation_set[NUM_STATE_GROUPS];
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
        observation_set[i] = model->hmm.B.expected[i][model->current_observation];
    KumaraswamyFunctions.GetVector( &model->kumaraswamy, nu, state_bands, observation_set, NUM_STATE_GROUPS );
    
    /* Update states/transition matrix */
    FSMFunctions.Sys.Update( &model->hmm.A, state_bands );
    
    model->best_state = PSMFunctions.FindMostLikelyHiddenState( model, TARGET_STATE, &model->best_confidence );
    PSMFunctions.UpdateBestCluster( model, &model->state_bands );
    
    /* Generate proposals to complete update */
    PSMFunctions.GenerateProposals( model );
#else
//    double bands[NUM_STATE_GROUPS] = { 0 };
//    KumaraswamyFunctions.GetVector( &model->kumaraswamy, nu, bands, &model->state_bands );
#endif
}

void UpdateStateBandPSM( band_list_t * band_list, uint8_t i, int8_t c, gaussian2d_t * band_gaussian )
{
    if( i > band_list->length ) return;
    if( c == 0 )
    { /* If no gaussian for band, zero state info */
        if( !i )
        {
            band_list->band[i] = (band_t){ HEIGHT, HEIGHT,  0., (vec2){ 0., 0. } };
        }
        else
        {
            memcpy( &band_list->band[i], &band_list->band[i-1], sizeof(band_t) );
        }
    }
    else if( c == -1 )
    {
        double boundary = band_list->band[i-1].upper_boundary;
        band_list->band[i].lower_boundary = boundary;
        band_list->band[i].upper_boundary = boundary;
        band_list->band[i].true_center = (vec2){ band_list->band[i-1].true_center.a, boundary };
        band_list->band[i].variance = band_list->band[i-1].variance;
    }
    else
    { /* Otherwise set using cumulated band gaussian */
        double radius = band_gaussian->covariance.d * VALID_CLUSTER_STD_DEV * 5;
        band_list->band[i].lower_boundary = band_gaussian->mean.b + radius;
        band_list->band[i].upper_boundary = band_gaussian->mean.b - radius;
        band_list->band[i].true_center = band_gaussian->mean;
        band_list->band[i].variance = band_gaussian->covariance.d;
        if(i)
        {
            band_list->band[i-1].upper_boundary = band_list->band[i].lower_boundary;
        }
    }
}

void DiscoverStateBandsPSM( psm_t * model, band_list_t * band_list )
{
#ifdef SPOOF_STATE_BANDS
    band_list->length = NUM_STATE_GROUPS;
    double prev = 0, curr, center;
    double spoof_bands[] = { 0.25, 0.5, 0.75, 1. };
    double spoof_deviation = 40.;
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
    {
        curr = spoof_bands[i] * HEIGHT;
        center = ( curr + prev ) / 2;
        band_list->band[NUM_STATE_GROUPS - 1 - i] = (band_t){ curr, prev, spoof_deviation, (vec2){ (1 - spoof_bands[i]) * WIDTH, center } };
        prev = curr;
    }
    return;
#else
    uint32_t processed_clusters = { 0 };
    for( uint8_t i = 0; i < model->gmm.num_clusters; i++ )
        GMMFunctions.Cluster.UpdateLimits( model->gmm.cluster[i] );
    
    gaussian2d_t band_gaussian = { 0 };
    band_list->band[0].lower_boundary = HEIGHT;
    
    int8_t current_band_id = 0, num_clusters_in_band = 0, num_to_process = model->gmm.num_clusters;
    uint32_t running_label_vector = 0;
    while(num_to_process-- > 0)
    {
        /* Get next min cluster */
        uint8_t min_id = 0;
        double min_boundary = 0, check_boundary;
        for(uint32_t i = 0, m = 1; i < model->gmm.num_clusters; i++, m <<= 1 )
        {
            if( processed_clusters & m ) continue;
            check_boundary = model->gmm.cluster[i]->max_y;
            if( check_boundary > min_boundary )
            {
                min_boundary = check_boundary;
                min_id = i;
            }
        }
        
        /* Check if new cluster has new label(s) */
        uint32_t current_label_vector = GetValidLabels( &model->gmm.cluster[min_id]->labels );
        uint32_t check = current_label_vector & ~running_label_vector;
        if( check )
        { /* If new labels in cluster */
            uint32_t new_label_vector = running_label_vector | current_label_vector;
            /* If new update skipped bands, if any */
            current_band_id = CountSet(new_label_vector);
            for( uint8_t i = CountSet(running_label_vector); i < current_band_id; i++ )
            {
                PSMFunctions.UpdateStateBand( band_list, i, num_clusters_in_band, &band_gaussian );
                num_clusters_in_band = -1;
            }
            band_gaussian = model->gmm.cluster[min_id]->gaussian_in;
            num_clusters_in_band = 1;
            running_label_vector = new_label_vector;
        }
        else
        { /* Otherwise cumulate current gaussian into band gaussian */
            mulGaussian2d( &band_gaussian, &model->gmm.cluster[min_id]->gaussian_in, &band_gaussian );
            num_clusters_in_band++;
        }
        
        if( !num_to_process )
            PSMFunctions.UpdateStateBand( band_list, current_band_id, num_clusters_in_band, &band_gaussian );

        processed_clusters |= 1 << min_id;
    }
    for( uint8_t i = current_band_id+1; i < band_list->length; i++ )
        PSMFunctions.UpdateStateBand( band_list, i, -1, NULL );
    band_list->band[band_list->length-1].upper_boundary = 0;
    
    if(band_list->length > 0)
    {
        LOG_PSM(DEBUG_2, "State bands: \n");
        for( uint8_t i = 0; i < band_list->length; i++ )
            LOG_PSM(DEBUG_2, " %d: (%.3f %.3f)  C<%.3f %.3f>[%.3f]\n", i, band_list->band[i].lower_boundary, band_list->band[i].upper_boundary, band_list->band[i].true_center.a, band_list->band[i].true_center.b, band_list->band[i].variance);
        LOG_PSM(DEBUG_2, "\n");
    }
#endif
}

uint8_t FindMostLikelyHiddenStatePSM( psm_t * model, uint8_t observation_state, double * confidence )
{
    uint8_t best_observation_id = 0;
    double best_observation_weight = 0.;
    
    /* Determine target observation band */
    for( uint8_t i = 0; i < NUM_OBSERVATION_SYMBOLS; i++ )
    {
        double check = model->hmm.B.expected[TARGET_STATE][i];
        if( check > best_observation_weight )
        {
            best_observation_id = i;
            best_observation_weight = check;
        }
    }
    if( confidence != NULL) *confidence = best_observation_weight;
    return best_observation_id;
}

void UpdateBestClusterPSM( psm_t * model, band_list_t * band_list )
{
    double
        lower_bound = band_list->band[model->best_state].lower_boundary,
        upper_bound = ( model->best_state + 1 >= NUM_STATE_GROUPS ) ?
            MAX_THRESH : band_list->band[model->best_state + 1].lower_boundary,
        best_cluster_weight = 0.;
    int8_t best_cluster_id = -1;
    for( uint8_t i = 0; i < model->gmm.num_clusters; i++ )
    {
        gaussian_mixture_cluster_t * cluster = model->gmm.cluster[i];
        if( IN_RANGE( cluster->gaussian_out.mean.b, lower_bound, upper_bound ) )
        {
            GMMFunctions.Cluster.Weigh( cluster );
            if( cluster->weight > best_cluster_weight )
            {
                best_cluster_weight = cluster->weight;
                best_cluster_id = i;
            }
        }
    }
    model->best_cluster_id = best_cluster_id;
    model->best_cluster_weight = best_cluster_weight;
}

uint8_t GetCurrentBandPSM( psm_t * model, band_list_t * band_list )
{
    uint8_t state = 0;
    for( ; state < band_list->length-1; state++ )
    {
        /* If next boundary is above thresh, band is in current thresh */
        if( model->previous_thresh > band_list->band[state+1].lower_boundary )
            break;
    }
    return state;
}

void GenerateProposalsPSM( psm_t * model )
{
    if( !model->gmm.num_clusters
       || model->best_cluster_id >= model->gmm.num_clusters
       || model->best_cluster_id < 0 ) return;
    
    /* Update predictions */
    vec2 * proposed_center = &model->state_bands.band[model->best_state].true_center;
    model->proposed_nu = proposed_center->a;
    model->proposed_thresh = proposed_center->b;
    
    /* Update primary & secondary to be reconstructed */
    gaussian_mixture_cluster_t * cluster = model->gmm.cluster[model->best_cluster_id];
    model->proposed_primary_id = cluster->primary_id;
    model->proposed_primary_id = cluster->secondary_id;
    
    /* Update current state */
    model->current_state = PSMFunctions.FindMostLikelyHiddenState( model, model->hmm.A.state, NULL );
}
