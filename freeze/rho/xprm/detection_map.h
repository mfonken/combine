//
//  detection_map.h
//  pcrxprm
//
//  Created by Matthew Fonken on 2/5/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef detection_map_h
#define detection_map_h

#include "rho_types.h"

void InitializeDetectionMap( detection_map_t *, uint16_t );
uint8_t GetDetectionMapFirstIndex( detection_map_t * );
void IncrementDetectionMapIndex( detection_map_t * );
void AddBlobToPredictionMap( detection_map_t *, uint8_t, uint8_t, uint8_t );
void AddBlobSetToPredictionMap( detection_map_t *, prediction_pair_t *, uint8_t );

typedef struct
{
    void (*Init)( detection_map_t *, uint16_t );
    uint8_t (*FirstIndex)( detection_map_t * );
    void (*Increment)( detection_map_t * );
    void (*Add)( detection_map_t *, uint8_t, uint8_t, uint8_t );
    void (*AddSet)( detection_map_t *, prediction_pair_t *, uint8_t );
} detection_map_function;

static detection_map_function DetectionMapFunctions =
{
    .Init = InitializeDetectionMap,
    .FirstIndex = GetDetectionMapFirstIndex,
    .Increment = IncrementDetectionMapIndex,
    .Add = AddBlobToPredictionMap,
    .AddSet = AddBlobSetToPredictionMap
};

#endif /* detection_map_h */
