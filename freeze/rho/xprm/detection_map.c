//
//  detection_map.c
//  pcrxprm
//
//  Created by Matthew Fonken on 2/5/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#include "detection_map.h"

void InitializeDetectionMap( detection_map_t * map, uint16_t len )
{
    memset( map, 0, sizeof(detection_map_t) );
    map->length = len;
}

uint8_t GetDetectionMapFirstIndex( detection_map_t * map )
{
    if( map->fill < ( map->length - 1 ) )
        return 0;
    else
        return ( map->index + 1 ) % ( map->length );
}

void IncrementDetectionMapIndex( detection_map_t * map )
{
    map->index++;
    map->index &= ( DETECTION_BUFFER_MASK );
    if( ++map->fill > DETECTION_BUFFER_SIZE )
        map->fill = DETECTION_BUFFER_MASK;
}

void AddBlobToPredictionMap( detection_map_t * map, uint8_t thresh, uint8_t density, uint8_t tracking_id )
{
    detection_element_t e = { thresh, density, tracking_id };
    map->buffer[map->index] = e;
    DetectionMapFunctions.Increment( map );
}

void AddBlobSetToPredictionMap( detection_map_t * map, prediction_pair_t * pp, uint8_t thresh )
{
    uint8_t p = 0, i, io;

    prediction_t * pair[2] = { &pp->x, &pp->y };
    for( ; p < 2; p++ )
    {
        for( i = 0; i < pair[p]->NumBlobs; i++ )
        {
            io = pair[p]->BlobsOrder[i];
            blob_t * b = &pair[p]->Blobs[io];
            double A = (double)(b->den<<8), B = (double)MAX_DENSITY;
            uint8_t byteDensity = (uint8_t)ZDIV( A, B );
            DetectionMapFunctions.Add( map, thresh, byteDensity, b->tracking_id);
        }
    }
    map->first = DetectionMapFunctions.FirstIndex( map );
}


