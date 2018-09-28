//
//  rho_master.h
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_master_h
#define rho_master_h

#define LOG(...) printf(__VA_ARGS__)

#ifdef RHO_DEBUG
#define LOG_RHO(...) LOG("Rho: " __VA_ARGS__)
#else
#define LOG_RHO(...)
#endif

#ifdef STATEM_DEBUG
#define LOG_STATEM(...) LOG("State Machine: " __VA_ARGS__)
#else
#define LOG_STATEM(...)
#endif

#include "rho_kalman.h"
#include "rho_types.h"
#include "state_machine_utility.h"
#include "rho_utility.h"
#include "rho_client.h"

#endif /* rho_master_h */
