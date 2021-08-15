/*
 * app.h
 *
 *  Created on: Jul 7, 2021
 *      Author: Matthew
 */

#ifndef APP_H_
#define APP_H_

#ifdef __OV9712__
#include "OV9712/OV9712.h"
#endif
#ifdef __RHO__
#include "../Rho/rho_master.h"
#else
#include "../UniSM/system_master.h"
#include "../UniLog/unilog.h"
#include "../App/states.h"
#endif

#endif /* APP_H_ */
