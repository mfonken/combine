//
//  statistics.c
//  combine
//
//  Created by Matthew Fonken on 8/10/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#include "statistics.h"

void cma( double new_val, double *avg, int num )
{
    *avg+=(new_val-*avg)/(double)(num+1);
}
