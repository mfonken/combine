//
//  rho_structures.cpp
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2022 Marbl. All rights reserved.
//

#include "rho_structures.hpp"
#include "rho_utility.h"

Prediction::Prediction()
{
    primary   = KalmanFilter(0., PREDICTION_LIFESPAN, PREDICTION_VALUE_UNCERTAINTY, PREDICTION_BIAS_UNCERTAINTY, PREDICTION_SENSOR_UNCERTAINTY);
    secondary = KalmanFilter(0., PREDICTION_LIFESPAN, PREDICTION_VALUE_UNCERTAINTY, PREDICTION_BIAS_UNCERTAINTY, PREDICTION_SENSOR_UNCERTAINTY);
    primary_new = 0.;
    secondary_new = 0.;
    probabilities = { 0. };
}

rho_system_t RhoSystem =
{
    { /* VARIABLES */
        { /* Utility */
            { /* Density map pair */
                { /* Dy */
                    FOREGROUND_DENSITY_MAP_Y,
                    BACKGROUND_DENSITY_MAP_Y
                },
                { /* Dx */
                    FOREGROUND_DENSITY_MAP_X,
                    BACKGROUND_DENSITY_MAP_X
                }
            },
            CAPTURE_WIDTH,
            CAPTURE_HEIGHT,
            CAPTURE_SUB_SAMPLE,
            DEFAULT_THRESH
        },
        { 0 }, /* Addresses */
        { /* Buffers */
            NULL,
            NULL
        },
//        0 /* Flags */
    },
    { /* FUNCTIONS */
        0
//        { /* Perform */
//            .Initialize         = RhoSystem_Initialize,
//            .CaptureRowCallback = CaptureRowCallback,
//            .FrameCapture       = RhoSystem_ProcessFrameCapture,
//            .CoreProcess        = RhoSystem_PerformProcess,
//            .ConnectToInterface = RhoSystem_ConnectToInterface,
//            .TransmitPacket     = RhoSystem_TransmitPacket,
//            .Activate           = RhoSystem_Activate,
//            .Deactivate         = DeactivateRhoSystem,
//
//            .CaptureRow            = RhoCapture_CaptureRow,
//            .ProcessSection        = RhoCapture_ProcessFrameSection
//        },
//        { 0 }, /* Platform */
//        { /* Memory */
//            .Zero               = ZeroRhoSystemMemory
//        }
    }
};
