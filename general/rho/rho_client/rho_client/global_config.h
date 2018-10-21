//
//  global_config.h
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef global_config_h
#define global_config_h

/* Camera Configuration */
#define CAMERA_WIDTH        1280
#define CAMERA_HEIGHT       800

/* Algorithm Configuration */
#define TARGET_COVERAGE     (0.004)

/* Algorithm Parameters */
#define Y_DEL               0xffff

/* Buffers */
#define CAPTURE_BUFFER_SIZE CAMERA_WIDTH
#define THRESH_BUFFER_SIZE  ((CAMERA_WIDTH * CAMERA_HEIGHT) * TARGET_COVERAGE)

#define BACKGROUND_PERIOD   10 // Frames



#endif /* global_config_h */
