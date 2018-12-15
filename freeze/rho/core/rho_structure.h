//
//  rho_structure.h
//  rho_client
//
//  Created by Matthew Fonken on 12/12/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_structure_h
#define rho_structure_h

#define DUAL_FILTER_CYCLE(X) \
byte_t X, X##_; \
for( X = 1, X##_ = 2; X##_ > 0; X--, X##_-- )

#define BOUNDED_CYCLE_DUAL(A,B,C,D,E,F,G) \
index_t C; \
for(x = A; x > B; --C, D = E[x], F = G[x] )

#endif /* rho_structure_h */
