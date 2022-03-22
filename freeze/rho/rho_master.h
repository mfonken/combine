//
//  rho_master.h
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2019 Marbl. All rights reserved.
//

#ifndef rho_master_h
#define rho_master_h

#include USE_INTERRUPT_MODEL 

#if defined __linux || defined __APPLE__
#include "rho_core.h"
#else
#include "rho_client.h"
#endif

#endif /* rho_master_h */
