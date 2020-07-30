//
//  port_master.h
//  app
//
//  Created by Matthew Fonken on 7/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef port_master_h
#define port_master_h

/* PAPI */
#if __PAPI__ == EMLIB
#include "emlib_interface.h"
#else
#error "No peripheral API defined."
#endif

/* OS */
#if __OS__ == MICRIUM
#include "micrium_interface.h"
#else
#error "No OS defined."
#endif

#endif /* port_master_h */
