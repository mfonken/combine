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

#define PACKET_ADDRESS_INITIALIZER(r)           \
{                                               \
(address_t)&r.y.TrackingFilters[0].value,         /* px */ \
(address_t)&r.x.TrackingFilters[0].value,         /* py */ \
(address_t)&r.y.TrackingFilters[1].value,       /* sx */ \
(address_t)&r.x.TrackingFilters[1].value,       /* sy */ \
(address_t)&r.Probabilities.P[1],   /* pp */ \
(address_t)&r.Probabilities.P[2], /* ap */ \
(address_t)&r.Probabilities.P[3]  /* ap */ \
}

#endif /* rho_structure_h */
